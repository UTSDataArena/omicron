#ifndef __SPACE_NAVIGATOR_SERVICE_H__
#define __SPACE_NAVIGATOR_SERVICE_H__

#include "omicron/osystem.h"
#include "omicron/ServiceManager.h"

#include <stdlib.h>
#include <stdio.h>
// #include "/da/sw/vrpn/install/include/vrpn_Tracker.h" // Remember to set Linker directories and Input/Dependencies to vrpn.lib
// #include "/da/sw/vrpn/install/include/vrpn_Joylin.h" // Remember to set Linker directories and Input/Dependencies to vrpn.lib
// #include "omicron/VRPNDeviceLinux.h"
// TODO: need to add include path for this
#include "/da/sw/vrpn/install/include/vrpn_Analog.h"
#include "/da/sw/vrpn/install/include/vrpn_Button.h"
#include "/da/sw/vrpn/install/include/vrpn_Analog_Output.h"

namespace omicron
{
///////////////////////////////////////////////////////////////////////////////
class OMICRON_API SpaceNavigatorService: public Service
{
public:
    // Allocator function
    static SpaceNavigatorService* New() { return new SpaceNavigatorService(); }

public:
    void setup(Setting& settings);
    virtual void initialize();
    virtual void poll();
    virtual void dispose();

    //! Sets the data update interval, in seconds. This is the interval at which this service will generate events
    //! If set to zero, the service will generate events as fast as possible.
    void setUpdateInterval(float value);
    //! @see setUpdateInterval
    float getUpdateInterval();

	void generateButtonEvent(vrpn_BUTTONCB b);
	void generateUpdateEvent(vrpn_ANALOGCB a);

private:
    static SpaceNavigatorService* mysInstance;


// 	Joystick* vrpnJoystick;
//     vrpn_Connection* connection;
	vrpn_Button_Remote *buttons;
	vrpn_Analog_Remote *channels;
	float myUpdateInterval;
	const char* serverIP;
	const char* joystickPort;
	const char* name;
	int serverPort;

};

///////////////////////////////////////////////////////////////////////////////
inline void SpaceNavigatorService::setUpdateInterval(float value)
{ myUpdateInterval = value; }

///////////////////////////////////////////////////////////////////////////////
inline float SpaceNavigatorService::getUpdateInterval()
{ return myUpdateInterval; }
}; // namespace omicron

#endif
