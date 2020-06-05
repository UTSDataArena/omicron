#ifndef __JOYSTICK__SERVICE_H__
#define __JOYSTICK__SERVICE_H__

#include "omicron/osystem.h"
#include "omicron/ServiceManager.h"

#include <stdlib.h>
#include <stdio.h>
#include "omicron/vrpn/vrpn_Analog.h"
#include "omicron/vrpn/vrpn_Button.h"
#include "omicron/vrpn/vrpn_Analog_Output.h"

namespace omicron
{
///////////////////////////////////////////////////////////////////////////////
class OMICRON_API JoystickService: public Service
{
public:
    // Allocator function
    static JoystickService* New() { return new JoystickService(); }

    void setup(Setting& settings);
    virtual void initialize();
    virtual void poll();
    virtual void dispose();

    //! Sets the data update interval, in seconds. This is the interval at which this service will generate events
    //! If set to zero, the service will generate events as fast as possible.
    void setUpdateInterval(float value);
    //! @see setUpdateInterval
    float getUpdateInterval();

	void generateButtonEvent_ps3(vrpn_BUTTONCB b);
	void generateUpdateEvent_ps3(vrpn_ANALOGCB a);
	void generateButtonEvent_ps4(vrpn_BUTTONCB b);
	void generateUpdateEvent_ps4(vrpn_ANALOGCB a);
	void generateButtonEvent_xbox(vrpn_BUTTONCB b);
	void generateUpdateEvent_xbox(vrpn_ANALOGCB a);

	String joystickType;

	double channel[vrpn_CHANNEL_MAX];

private:
    static JoystickService* mysInstance;


// 	Joystick* vrpnJoystick;
//     vrpn_Connection* connection;
// 	vrpn_BUTTONCB *prevBCB;
// 	vrpn_ANALOGCB *prevACB;
	vrpn_Button_Remote *buttons;
	vrpn_Analog_Remote *channels;
	float myUpdateInterval;
	String serverIP;
	String name;
	int serverPort;

	uint prevButtonState;
};

///////////////////////////////////////////////////////////////////////////////
inline void JoystickService::setUpdateInterval(float value)
{ myUpdateInterval = value; }

///////////////////////////////////////////////////////////////////////////////
inline float JoystickService::getUpdateInterval()
{ return myUpdateInterval; }
}; // namespace omicron

#endif
