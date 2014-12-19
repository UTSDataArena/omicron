#ifndef VRPNDEVICELINUX_H
#define VRPNDEVICELINUX_H


#include <omicron.h>

#include "/da/sw/vrpn/install/include/vrpn_Microsoft_Controller_Raw.h"        // for vrpn_HidAcceptor (ptr only), etc

namespace omicron {

class vrpn_MyMicrosoft_Controller_Raw_Xbox_360 : public vrpn_Microsoft_Controller_Raw_Xbox_360
{
public:
	vrpn_MyMicrosoft_Controller_Raw_Xbox_360(const char *name, vrpn_Connection *c = 0, vrpn_uint16 vendorId = MICROSOFT_VENDOR, vrpn_uint16 productId = XBOX_360);
	virtual ~vrpn_MyMicrosoft_Controller_Raw_Xbox_360(void) {};

 	void myUpdate(const Event*);
};

};
#endif // VRPNDEVICELINUX_H