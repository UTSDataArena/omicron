#ifndef VRPNDEVICELINUX_H
#define VRPNDEVICELINUX_H

#include <omicron.h>
#include "omicron/osystem.h"
#include "omicron/ServiceManager.h"

// for vrpn_HidAcceptor (ptr only), etc
// #include "vrpn_Microsoft_Controller_Raw.h"
// linux-based joystick
#include "vrpn_Joylin.h"

namespace omicron {
/*
class vrpn_MyMicrosoft_Controller_Raw_Xbox_360 : public vrpn_Microsoft_Controller_Raw_Xbox_360
{
public:
	vrpn_MyMicrosoft_Controller_Raw_Xbox_360(const char *name, vrpn_Connection *c = 0, vrpn_uint16 vendorId = MICROSOFT_VENDOR, vrpn_uint16 productId = XBOX_360);
	virtual ~vrpn_MyMicrosoft_Controller_Raw_Xbox_360(void) {};

 	void myUpdate(const Event*);
};
*/
class Joystick : public vrpn_Joylin
{
public:
	Joystick(char *name, vrpn_Connection *c, char *portname);
	virtual ~Joystick(void) {};

 	void update(const Event*);

//  	void mainloop(void);

// 	inline char* getButtons() { return (char *)&buttons; }
// 	inline vrpn_float64* getChannels() { return (vrpn_float64*) &channel; }
};

};
#endif // VRPNDEVICELINUX_H
