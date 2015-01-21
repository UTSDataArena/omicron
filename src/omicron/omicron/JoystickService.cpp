#include "omicron/JoystickService.h"
#include "omicron/StringUtils.h"

using namespace omicron;


///////////////////////////////////////////////////////////////////////////////
void JoystickService::setup(Setting& settings)
{
	name = "Joylin0";
    if(settings.exists("name"))
    {
        name = (const char*)settings["name"];
    }

    if(settings.exists("serverIP"))
    {
        serverIP = (const char*)settings["serverIP"];
    }

    if(settings.exists("serverPort"))
    {
        serverPort = Config::getIntValue("serverPort",settings, 3891);
    }

    if(settings.exists("joystickPort"))
    {
        joystickPort = (const char*)settings["joystickPort"];
    }


	myUpdateInterval = 0.0f;
	if(settings.exists("updateInterval"))
	{
		myUpdateInterval = settings["updateInterval"];
	}

    setPollPriority(Service::PollFirst);
}

///////////////////////////////////////////////////////////////////////////////
JoystickService* JoystickService::mysInstance = NULL;


///////////////////////////////////////////////////////////////////////////////
void JoystickService::generateButtonEvent(vrpn_BUTTONCB b)
{
	mysInstance->lockEvents();
	Event* evt = mysInstance->writeHead();

	uint curButtonState = 0;

	// Triangle Button
	if(b.button == 0) curButtonState |= Event::Button1;
	// Circle Button
	if(b.button == 1) curButtonState |= Event::Button2;
	// Cross Button
	if(b.button == 2) curButtonState |= Event::Button3;
	// Square Button
	if(b.button == 3) curButtonState |= Event::Button4;
	// Left Shoulder Button (L1)
	if(b.button == 4) curButtonState |= Event::Button5;
	// Left Analog Pad Pressed (L3)
	if(b.button == 5) curButtonState |= Event::Button6;

	if (b.state == 1) {
		evt->reset(Event::Down, Service::Controller, 0);
	} else if (b.state == 0) {
		evt->reset(Event::Up, Service::Controller, 0);
	}

	evt->setFlags(curButtonState);
 	prevButtonState = curButtonState;

	evt->setExtraDataType(Event::ExtraDataFloatArray);

	evt->setExtraDataFloat(0, 0);  // Left analog (-left, +right)
	evt->setExtraDataFloat(1, 0);  // Left analog (-up, +down)
 	evt->setExtraDataFloat(2, 0); // Right analog (-left, +right)
 	evt->setExtraDataFloat(3, 0); // Right analog (-up, +down)
 	evt->setExtraDataFloat(4, 0); // Trigger 2 (-unpressed, +pressed)
 	evt->setExtraDataFloat(5, 0); // Trigger 1 (-unpressed, +pressed)

	mysInstance->unlockEvents();

}

///////////////////////////////////////////////////////////////////////////////
void JoystickService::generateUpdateEvent(vrpn_ANALOGCB a)
{
	mysInstance->lockEvents();
	Event* evt = mysInstance->writeHead();

	uint curButtonState = 0;

	if (a.channel[5] > 0) curButtonState |= Event::Button7;

	// d-pad (-left, +right)
	if (a.channel[6] < 0) curButtonState |= Event::ButtonLeft;
	if (a.channel[6] > 0)  curButtonState |= Event::ButtonRight;
	// d-pad (-up, +down)
	if (a.channel[7] < 0) curButtonState |= Event::ButtonUp;
	if (a.channel[7] > 0)  curButtonState |= Event::ButtonDown;

	if (curButtonState != prevButtonState) {
		if (curButtonState > prevButtonState) {
			evt->reset(Event::Down, Service::Controller, 0);
			evt->setFlags(curButtonState);

		} else {
			evt->reset(Event::Up, Service::Controller, 0);
			prevButtonState = prevButtonState - curButtonState;
			evt->setFlags(prevButtonState);

		}
	} else {
		evt->reset(Event::Update, Service::Controller, 0);
			evt->setFlags(curButtonState);
	}

	prevButtonState = curButtonState;

	evt->setExtraDataType(Event::ExtraDataFloatArray);

	evt->setExtraDataFloat(0, a.channel[0]);  // Left analog (-left, +right)
	evt->setExtraDataFloat(1, a.channel[1]);  // Left analog (-up, +down)
 	evt->setExtraDataFloat(2, a.channel[2]); // Right analog (-left, +right)
 	evt->setExtraDataFloat(3, a.channel[3]); // Right analog (-up, +down)
 	evt->setExtraDataFloat(4, 0.5 * (a.channel[4] + 1)); // R2 (0, +pressed)
 	evt->setExtraDataFloat(5, 0.5 * (a.channel[5] + 1)); // L2 (0, +pressed)

	mysInstance->unlockEvents();

}

///////////////////////////////////////////////////////////////////////////////
// void VRPN_CALLBACK handle_tracker(void *userData, const vrpn_TRACKERCB t)
// {
//     VRPNStruct* vs = ((VRPNStruct*)userData);
//     VRPNService* vrpnService = vs->vrnpService;
//
//     vrpnService->generateEvent(t, vs->object_id, vs->userId, vs->jointId);
// }
void VRPN_CALLBACK js_button_press(void *userData, const vrpn_BUTTONCB b)
{

	JoystickService* js =  (JoystickService*) userData;

// 	printf("button %i state %i\n", b.button, b.state);
	js->generateButtonEvent(b);
}

void VRPN_CALLBACK js_channel_change(void *userData, const vrpn_ANALOGCB a)
{
	JoystickService* js =  (JoystickService*) userData;

    vrpn_int32 i;

//     fprintf(stderr, "Analog:\n         %5.2f", a.channel[0]);
//     for (i = 1; i < a.num_channel; i++) {
//         fprintf(stderr, ", %5.2f", a.channel[i]);
//     }
//     fprintf(stderr, " (%d chans)\n", a.num_channel);
	js->generateUpdateEvent(a);
}


///////////////////////////////////////////////////////////////////////////////
void JoystickService::initialize()
{
    printf("JoystickService: Initialize\n");
    mysInstance = this;

	char command[255];

	char server[128];

	sprintf(server, "%d", serverPort);

	sprintf(command, "%s@%s:%d", name, serverIP, serverPort);

	ofmsg("connecting to %1%..", %command);

// 	vrpnJoystick = new Joystick(command, connection, strdup(joystickPort));
	buttons = new vrpn_Button_Remote(command);
	channels = new vrpn_Analog_Remote(command);

	buttons->register_change_handler(mysInstance, js_button_press);
	channels->register_change_handler(mysInstance, js_channel_change);

	prevButtonState = 0;
}

///////////////////////////////////////////////////////////////////////////////
void JoystickService::poll()
{
    static float lastt;
    float curt = (float)((double)clock() / CLOCKS_PER_SEC);
    if(curt - lastt > myUpdateInterval)
    {
// 		omsg("Polling...");
   		buttons->mainloop();
 		channels->mainloop();
        lastt = curt;
    }
}

///////////////////////////////////////////////////////////////////////////////
void JoystickService::dispose()
{
	delete buttons;
	delete channels;
    mysInstance = NULL;
}
