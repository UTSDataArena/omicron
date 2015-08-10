#include "omicron/SpaceNavigatorService.h"
#include "omicron/StringUtils.h"

using namespace omicron;


///////////////////////////////////////////////////////////////////////////////
void SpaceNavigatorService::setup(Setting& settings)
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
SpaceNavigatorService* SpaceNavigatorService::mysInstance = NULL;


///////////////////////////////////////////////////////////////////////////////
void SpaceNavigatorService::generateButtonEvent(vrpn_BUTTONCB b)
{
	mysInstance->lockEvents();
	Event* evt = mysInstance->writeHead();

	uint curButtonState = 0;

	// Left
	if(b.button == 0) curButtonState |= Event::Button1;
	// Right
	if(b.button == 1) curButtonState |= Event::Button2;

	if (b.state == 1) {
		evt->reset(Event::Down, Service::Controller, 1);
	} else if (b.state == 0) {
		evt->reset(Event::Up, Service::Controller, 1);
	}

	evt->setFlags(curButtonState);

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
void SpaceNavigatorService::generateUpdateEvent(vrpn_ANALOGCB a)
{
	mysInstance->lockEvents();
	Event* evt = mysInstance->writeHead();

	uint curButtonState = 0;

	evt->reset(Event::Update, Service::Controller, 1);

	evt->setFlags(curButtonState);

	evt->setExtraDataType(Event::ExtraDataFloatArray);

	// SpaceNavigator Axes..
	evt->setExtraDataFloat(0, a.channel[0]);  // (-left, +right)
	evt->setExtraDataFloat(1, a.channel[1]);  // (-forward, +back)
 	evt->setExtraDataFloat(2, a.channel[2]); // (-up, +down)
 	evt->setExtraDataFloat(3, a.channel[3]); // pitch (-forward, +back)
 	evt->setExtraDataFloat(4, a.channel[4]); // roll (-right, +left)
 	evt->setExtraDataFloat(5, a.channel[5]); // yaw (-left, +right)

	mysInstance->unlockEvents();
}
/*
///////////////////////////////////////////////////////////////////////////////
void SpaceNavigatorService::generateEvent(SpaceNavigatorService* js)
{
	vrpn_ANALOGCB a =

	mysInstance->lockEvents();
	Event* evt = mysInstance->writeHead();

	uint curButtonState = 0;
// 	evt->reset(Event::Update, Service::Mocap, id, getServiceId(), userId);
// 	evt->setPosition(t.pos[0], t.pos[1], t.pos[2]);
// 	evt->setOrientation(t.quat[3], t.quat[0], t.quat[1], t.quat[2]);

// 	// Triangle Button
// 	if(b.button == 0) curButtonState |= Event::Button1;
// 	// Circle Button
// 	if(b.button == 1) curButtonState |= Event::Button2;
// 	// Cross Button
// 	if(b.button == 2) curButtonState |= Event::Button3;
// 	// Square Button
// 	if(b.button == 3) curButtonState |= Event::Button4;
// 	// Left Shoulder Button (L1)
// 	if(b.button == 4) curButtonState |= Event::Button5;
// 	// Left Analog Pad Pressed (L3)
// 	if(b.button == 5) curButtonState |= Event::Button6;
//
// 	if (b.state == 1) {
// 		evt->reset(Event::Down, Service::Controller, 1);
// 	} else if (b.state == 0) {
// 		evt->reset(Event::Up, Service::Controller, 1);
// 	}

	evt->reset(Event::Update, Service::Controller, 1);

	evt->setFlags(curButtonState);

	evt->setExtraDataType(Event::ExtraDataFloatArray);

	evt->setExtraDataFloat(0, a.channel[0]);  // Left analog (-left, +right)
	evt->setExtraDataFloat(1, a.channel[1]);  // Left analog (-up, +down)
 	evt->setExtraDataFloat(2, a.channel[2]); // Right analog (-left, +right)
 	evt->setExtraDataFloat(3, a.channel[3]); // Right analog (-up, +down)
 	evt->setExtraDataFloat(4, a.channel[4]); // Trigger 2 (-unpressed, +pressed)
 	evt->setExtraDataFloat(5, a.channel[5]); // Trigger 1 (-unpressed, +pressed)

	mysInstance->unlockEvents();
}
*/

///////////////////////////////////////////////////////////////////////////////
// void VRPN_CALLBACK handle_tracker(void *userData, const vrpn_TRACKERCB t)
// {
//     VRPNStruct* vs = ((VRPNStruct*)userData);
//     VRPNService* vrpnService = vs->vrnpService;
//
//     vrpnService->generateEvent(t, vs->object_id, vs->userId, vs->jointId);
// }
void VRPN_CALLBACK sns_button_press(void *userData, const vrpn_BUTTONCB b)
{

	SpaceNavigatorService* js =  (SpaceNavigatorService*) userData;

	printf("button %i stat %i\n", b.button, b.state);
	js->generateButtonEvent(b);
}

void VRPN_CALLBACK sns_channel_change(void *userData, const vrpn_ANALOGCB a)
{
	SpaceNavigatorService* js =  (SpaceNavigatorService*) userData;

    vrpn_int32 i;
//     const char *name = (const char *)userData;

//     fprintf(stderr, "Analog:\n         %5.2f", a.channel[0]);
//     for (i = 1; i < a.num_channel; i++) {
//         fprintf(stderr, ", %5.2f", a.channel[i]);
//     }
//     fprintf(stderr, " (%d chans)\n", a.num_channel);

	js->generateUpdateEvent(a);
}


///////////////////////////////////////////////////////////////////////////////
void SpaceNavigatorService::initialize()
{
    printf("SpaceNavigatorService: Initialize\n");
    mysInstance = this;

	char command[255];


//     ofmsg("SpaceNavigatorService: Creating VRPNServer %1%", %name);
//     ofmsg("              Port: %1%", %serverPort);
//     ofmsg("              Controller Port: %1%", %joystickPort);

//  	connection = vrpn_create_server_connection(serverPort);

	char server[128];

	sprintf(server, "%d", serverPort);

	sprintf(command, "%s@%s:%d", name, serverIP, serverPort);

// 	connection = vrpn_get_connection_by_name(server);

	ofmsg("connecting to %1%..", %command);

// 	vrpnJoystick = new Joystick(command, connection, strdup(joystickPort));
	buttons = new vrpn_Button_Remote(command);
	channels = new vrpn_Analog_Remote(command);

	buttons->register_change_handler(mysInstance, sns_button_press);
	channels->register_change_handler(mysInstance, sns_channel_change);
}

///////////////////////////////////////////////////////////////////////////////
void SpaceNavigatorService::poll()
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
void SpaceNavigatorService::dispose()
{
	delete buttons;
	delete channels;
    mysInstance = NULL;
}
