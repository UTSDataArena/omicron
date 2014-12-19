// vrpn_Microsoft_Controller_Raw.C: VRPN driver for Microsoft Controller Raw devices

#include <stdio.h>                      // for fprintf, stderr, NULL
#include <string.h>                     // for memset
#include <math.h>                       // for sqrt and fabs

#include "omicron/VRPNDeviceLinux.h"

// VRPN_SUPPRESS_EMPTY_OBJECT_WARNING()

// #if defined(VRPN_USE_HID)

static const double POLL_INTERVAL = 1e+6 / 30.0;		// If we have not heard, ask.

#define MS_GAMEPAD_LEFT_THUMB_DEADZONE 7849
#define MS_GAMEPAD_RIGHT_THUMB_DEADZONE 8689
#define MS_GAMEPAD_TRIGGER_THRESHOLD 30

using namespace omicron;

//////////////////////////////////////////////////////////////////////////
// Xbox 360
//////////////////////////////////////////////////////////////////////////
vrpn_MyMicrosoft_Controller_Raw_Xbox_360::vrpn_MyMicrosoft_Controller_Raw_Xbox_360(const char *name, vrpn_Connection *c, vrpn_uint16 vendorId /*=MICROSOFT_VENDOR*/, vrpn_uint16 productId /*=XBOX_360*/)
: vrpn_Microsoft_Controller_Raw_Xbox_360(name, c, vendorId, productId)
{
}

// void vrpn_Microsoft_Controller_Raw_Xbox_360::mainloop()
// {
// 	update();
// 	server_mainloop();
// 	struct timeval current_time;
// 	vrpn_gettimeofday(&current_time, NULL);
// 	if (vrpn_TimevalDuration(current_time, _timestamp) > POLL_INTERVAL ) {
// 		_timestamp = current_time;
// 		report_changes();
//
// 		vrpn_Analog::server_mainloop();
// 		vrpn_Button::server_mainloop();
// 		if (vrpn_Dial::num_dials > 0)
// 		{
// 			vrpn_Dial::server_mainloop();
// 		}
// 	}
// }

void vrpn_MyMicrosoft_Controller_Raw_Xbox_360::myUpdate(const Event* evt)
{
	update();
	server_mainloop();
}

