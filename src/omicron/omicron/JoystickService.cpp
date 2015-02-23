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

/*
// Previous button mappings (for ps3->Xbox controller)
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

	// PS Button
	if(b.button == 8) curButtonState |= Event::SpecialButton3;

	if (b.state == 1) {
		evt->reset(Event::Down, Service::Controller, 0);
		evt->setFlags(curButtonState);
	} else if (b.state == 0) {
		evt->reset(Event::Up, Service::Controller, 0);
		prevButtonState = prevButtonState - curButtonState;
		evt->setFlags(prevButtonState);
	}

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
*/

/* Button Mappings for Ps3 Controllers
 * Button#	PSbutton	Event
 * 0		SELECT		SpecialButton2
 * 1		L3			Button9
 * 2		R3			Button10
 * 3		START		SpecialButton1
 * 4		UP			Up
 * 5		RIGHT		Right
 * 6		DOWN		Down
 * 7		LEFT		Left
 * 8		L2			Button7
 * 9		R2			Button8
 * 10		L1			Button5
 * 11		R1			Button6
 * 12		Triangle	Button4
 * 13		Circle		Button2
 * 14		Cross		Button1
 * 15		Square		Button3
 * 16		PS			SpecialButton3
 *
*/

// New button mappings (directly from a ps3 controller/ wand controller)
///////////////////////////////////////////////////////////////////////////////
void JoystickService::generateButtonEvent(vrpn_BUTTONCB b)
{
	mysInstance->lockEvents();
	Event* evt = mysInstance->writeHead();

	uint curButtonState = 0;

	if(b.button == 0) curButtonState |= Event::SpecialButton2;
	if(b.button == 1) curButtonState |= Event::Button9;
// 	if(b.button == 2) curButtonState |= Event::Button10;
	if(b.button == 3) curButtonState |= Event::SpecialButton1;
	if(b.button == 4) curButtonState |= Event::ButtonUp;
	if(b.button == 5) curButtonState |= Event::ButtonRight;
	if(b.button == 6) curButtonState |= Event::ButtonDown;
	if(b.button == 7) curButtonState |= Event::ButtonLeft;
	if(b.button == 8) curButtonState |= Event::Button7;
	if(b.button == 9) curButtonState |= Event::Button8;
	if(b.button == 10) curButtonState |= Event::Button5;
	if(b.button == 11) curButtonState |= Event::Button6;
	if(b.button == 12) curButtonState |= Event::Button4;
	if(b.button == 13) curButtonState |= Event::Button2;
	if(b.button == 14) curButtonState |= Event::Button1;
	if(b.button == 15) curButtonState |= Event::Button3;
	if(b.button == 16) curButtonState |= Event::SpecialButton3;

	if (b.state == 1) {
		evt->reset(Event::Down, Service::Controller, 0);
		evt->setFlags(curButtonState);
	} else if (b.state == 0) {
		evt->reset(Event::Up, Service::Controller, 0);
		prevButtonState = prevButtonState - curButtonState;
		evt->setFlags(prevButtonState);
	}

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

/*
// Previous axes mapping (PS3 Controller -> Xbox controller)
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
*/

/* Axis Mapping for Ps3 Controllers
 * Axis#	PSAxis
 * 0		L stick (-L,  +R)
 * 1		L stick (-U,  +D)
 * 2		R stick (-L,  +R)
 * 3		R stick (-U,  +D)
 * 4-7		None
 * 8		Up Button press
 * 9		Right Button press
 * 10		Down Button press
 * 11		Left Button press
 * 12		L2 Button press
 * 13		R2 Button press
 * 14		L1 Button press
 * 15		R1 Button press
 * 16		Triangle
 * 17		Circle
 * 18		Cross
 * 19		Square
 * 20-23	None
 * 24		acc. (-R, +L)
 * 25		acc. (-F, +B)
 * 26		acc. (-D, +U)
 *
*/

// New axes mapping (Ps3/Move Controller)
///////////////////////////////////////////////////////////////////////////////
void JoystickService::generateUpdateEvent(vrpn_ANALOGCB a)
{
	mysInstance->lockEvents();
	Event* evt = mysInstance->writeHead();

	uint curButtonState = 0;

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
 	evt->setExtraDataFloat(4, 0.5 * (a.channel[12] + 1)); // R2 (0, +pressed)
 	evt->setExtraDataFloat(5, 0.5 * (a.channel[13] + 1)); // L2 (0, +pressed)

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

 	printf("button %i state %i\n", b.button, b.state);
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
