/******************************************************************************
 * THE OMICRON SDK
 *-----------------------------------------------------------------------------
 * Copyright 2010-2014		Electronic Visualization Laboratory, 
 *							University of Illinois at Chicago
 * Authors:										
 *  Alessandro Febretti		febret@gmail.com
 * Contributors:
 *  Victor Mateevitsi		mvictoras@gmail.com
 *-----------------------------------------------------------------------------
 * Copyright (c) 2010-2013, Electronic Visualization Laboratory,  
 * University of Illinois at Chicago
 * All rights reserved.
 * Redistribution and use in source and binary forms, with or without modification, 
 * are permitted provided that the following conditions are met:
 * 
 * Redistributions of source code must retain the above copyright notice, this 
 * list of conditions and the following disclaimer. Redistributions in binary 
 * form must reproduce the above copyright notice, this list of conditions and 
 * the following disclaimer in the documentation and/or other materials provided 
 * with the distribution. 
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO THE 
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE 
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE 
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL 
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE  GOODS OR 
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER 
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, 
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *-----------------------------------------------------------------------------
 * What's in this file:
 *	The Event class, a generic container of input event data.
 ******************************************************************************/
#ifndef __EVENT_H__
#define __EVENT_H__

#include "osystem.h"
#include "Service.h"
#include <string>

// This makes omicronConnectorClient.h only define the EventBase and EventData classes.
#define OMICRON_CONNECTOR_LEAN_AND_MEAN
#include "connector/omicronConnectorClient.h"
// undefine OMICRON_CONNECTOR_LEAN_AND_MEAN, so eventual includes of omicronConnectorClient.h
// following this one will include the full header.
#undef OMICRON_CONNECTOR_LEAN_AND_MEAN

// Used for timestamp
#include <sys/timeb.h>

#define FLOAT_PTR(x) *((float*)&x)
#define INT_PTR(x) *((int*)&x)

namespace omicron
{
    ///////////////////////////////////////////////////////////////////////////
    //! Events are generated by Service instances. 
    class Event: public ReferenceType, public EventBase
    {
    // Friend the class that implements methods used for event serialization by equalizer.
    friend class EventUtils; 
    friend class Service; 
    public:
        static const int ExtraDataSize = 1024;
        static const int MaxExtraDataItems = 32;
        static Event::Flags parseButtonName(const String& name);
        static int parseJointName(const String& name);

    public:
        Event();

        void copyFrom(const Event& e);
        //! Serializes this event to a streamable event data packet. Returns the
        //! size of data to stream.
        size_t serialize(omicronConnector::EventData* ed) const;
        //! Deserializes from an event data packet
        void deserialize(const omicronConnector::EventData* ed);

        void reset(Type type, Service::ServiceType serviceType, uint sourceId = 0, unsigned short serviceId = 0, unsigned short userId = 0);
        //! Only resets the event source id, keeping the rest of the event data intact.
        //! Useful when dynamically re-routing events (i.e. to transparently re-associate 
        //! head tracking sources to applications)
        void resetSourceId(uint newSourceId);
        //! Only resets the event type, keeping the rest of the event data intact.
        void resetType(Type type);

        //! id of the source of this event. Input services associate unique ids to each of their event sources.
        unsigned int getSourceId() const;

        //! Type of the service that generated this event.
        Service::ServiceType getServiceType() const;

        //! Used to mark events that have been processed.
        void setProcessed() const;
        bool isProcessed() const;

        // ! Used to mark events that are sent to a single endpoint instead of 
        //! being broadcast.
        void setExclusive() const;
        bool isExclusive() const;

        //! Get the event position 
        const Vector3f& getPosition() const;
        void setServiceType(Service::ServiceType type);

        //! Gets the unique id of the event service that generated this event.
        //! This value is part of the device tag.
        unsigned int getServiceId() const;
        //! Numeric id of the user associated to this event. This value is part 
        //! of the device tag.
        unsigned int getUserId() const;

        //! Gets the event device tag. The device tag identifies the service and
        //! the user associated with this event. The lower two bytes contain
        //! the service id, while the top two bytes contain the user id. Both
        //! can be accessed using the getServiceId and getUserId utility
        //! functions.
        unsigned int getDeviceTag() const;

        //! The event type.
        Type getType() const;

        //! Gets the event timestamp. The timestamp is updated everytime the Event::reset is called.
        unsigned int getTimestamp() const;

        //! Set to true if this event has been processed already.
        //float getPosition(int component) const;
        void setPosition(const Vector3f& value);
        void setPosition(float x, float y, float z);
        void setPosition(float x, float y);

        //! Rotation.
        const Quaternion& getOrientation() const;
        void setOrientation(const Quaternion& value);
        void setOrientation(float w, float x, float y, float z);

        
        //! Event flags.
        bool isFlagSet(uint flag) const;
        void setFlags(uint flags) const;
        void clearFlags() const;
        uint getFlags() const;

        //! Utility method to check is the event is a key down event.
        bool isKeyDown(int key) const;
        //! Utility method to check is the event is a key up event.
        bool isKeyUp(int key) const;
        //! Utility method to check if the event is a button down event.
        bool isButtonDown(Flags button) const;
        bool isButtonUp(Flags button) const;
        //! Utility method to read an axis value from the event extra data.
        float getAxis(int axisId) const;

        //! Utility method to check if an event is from a specific source service and source id.
        bool isFrom(Service* svc, int sourceId) const;
        //! Utility method to check if an event is from a specific service type and source id.
        bool isFrom(Service::ServiceType type, int sourceId) const;
        //! Utility method that gets a character stored in this event.
        //! @returns true if the event has been generated by a keyboard service
        //! and contains an ASCII character.
        bool getChar(char* c) const;

        //! Extra data
        void setExtraDataType(ExtraDataType type);
        ExtraDataType getExtraDataType() const;
        float getExtraDataFloat(int index) const;
        void setExtraDataFloat(int index, float value);
        int getExtraDataInt(int index) const;
        void setExtraDataInt(int index, int value);
        Vector3f getExtraDataVector3(int index) const;
        void setExtraDataVector3(int index, const Vector3f value);
        const char* getExtraDataString() const;
        void setExtraDataString(const String& value);
        void resetExtraData();
        bool isExtraDataNull(int pointId) const;
        void setExtraData(ExtraDataType type, unsigned int items, int mask, void* data);
        uint getExtraDataMask() const { return myExtraDataValidMask; }

        //! Returns the size in bytes of the event extra data.
        int getExtraDataSize() const;

        //! Returns the number of elements stored in the extra data section of the Event.
        int getExtraDataItems() const;

        //! Returns the raw etra data buffer.
        void* getExtraDataBuffer() const;

    private:
        unsigned int mySourceId;
        enum Service::ServiceType myServiceType;
        unsigned int myDeviceTag;
        enum Type myType;

        Vector3f myPosition;
        Quaternion myOrientation;
        int myTimestamp;

        mutable unsigned int myFlags;

        ExtraDataType myExtraDataType;
        int myExtraDataItems;
        int myExtraDataValidMask;
        char myExtraData[ExtraDataSize];
    };

    ///////////////////////////////////////////////////////////////////////////
    inline
    size_t Event::serialize(omicronConnector::EventData* ed) const
    {
        // Serialize event.
        ed->timestamp = getTimestamp();
        ed->sourceId = getSourceId();
        ed->deviceTag = getDeviceTag();
        ed->serviceType = getServiceType();
        ed->type = getType();
        ed->flags = getFlags();
        ed->posx = getPosition().x();
        ed->posy = getPosition().y();
        ed->posz = getPosition().z();
        ed->orx = getOrientation().x();
        ed->ory = getOrientation().y();
        ed->orz = getOrientation().z();
        ed->orw = getOrientation().w();
        ed->extraDataType = getExtraDataType();
        ed->extraDataItems = getExtraDataItems();
        ed->extraDataMask = getExtraDataMask();
        memcpy(ed->extraData, getExtraDataBuffer(), getExtraDataSize());

        // Message size = total event data size - number of unused extra data bytes
        size_t freextrabytes = omicronConnector::EventData::ExtraDataSize - getExtraDataSize();
        size_t msgsize = sizeof(*ed) - freextrabytes;

        return msgsize;
    }

    ///////////////////////////////////////////////////////////////////////////
    inline 
    void Event::deserialize(const omicronConnector::EventData* ed)
    {
        reset((Event::Type)ed->type,
            (Service::ServiceType)ed->serviceType,
            ed->sourceId,
            (ed->deviceTag & DTServiceIdMask) >> DTServiceIdOffset,
            (ed->deviceTag & DTUserIdMask) >> DTUserIdOffset);
        setPosition(ed->posx, ed->posy, ed->posz);
        setOrientation(ed->orw, ed->orx, ed->ory, ed->orz);
        setFlags(ed->flags);
        setExtraData((Event::ExtraDataType)ed->extraDataType, ed->extraDataItems, ed->extraDataMask, (void*)ed->extraData);
    }

    ///////////////////////////////////////////////////////////////////////////
    inline 
    Event::Flags Event::parseButtonName(const String& name)
    {
        if(name == "Button1") return Button1;
        if(name == "Button2") return Button2;
        if(name == "Button3") return Button3;
        if(name == "Button4") return Button4;
        if(name == "Button5") return Button5;
        if(name == "Button6") return Button6;
        if(name == "Button7") return Button7;
        if(name == "Button8") return Button8;
        if(name == "Button9") return Button9;
        if(name == "ButtonLeft") return ButtonLeft;
        if(name == "ButtonRight") return ButtonRight;
        if(name == "ButtonUp") return ButtonUp;
        if(name == "ButtonDown") return ButtonDown;
        if(name == "Ctrl") return Ctrl;
        if(name == "Alt") return Alt;
        if(name == "Shift") return Shift;
        if(name == "SpecialButton1") return SpecialButton1;
        if(name == "SpecialButton2") return SpecialButton2;
        if(name == "SpecialButton3") return SpecialButton3;
        return (Event::Flags)0;
    }

    ///////////////////////////////////////////////////////////////////////////
    inline 
    int Event::parseJointName(const String& name)
    {
        if(name == "head") return OMICRON_SKEL_HEAD;
        if(name == "torso") return OMICRON_SKEL_TORSO;
        if(name == "leftHand") return OMICRON_SKEL_LEFT_HAND;
        if(name == "rightHand") return OMICRON_SKEL_RIGHT_HAND;
        if(name == "leftFoot") return OMICRON_SKEL_LEFT_FOOT;
        if(name == "rightFoot") return OMICRON_SKEL_RIGHT_FOOT;
        return -1;
    }

    ///////////////////////////////////////////////////////////////////////////
    inline Event::Event():
        myFlags(0),
        myExtraDataType(ExtraDataNull),
        myExtraDataValidMask(0)
    {}

    ///////////////////////////////////////////////////////////////////////////
    inline void Event::reset(Type type, Service::ServiceType serviceType, uint sourceId, unsigned short serviceId, unsigned short userId)
    {
        myType = type;
        mySourceId = sourceId;
        myServiceType = serviceType;
        myFlags = 0;
        myExtraDataItems = 0;
        myExtraDataValidMask = 0;
        myExtraDataType = ExtraDataNull;
        if(serviceId != 0) myDeviceTag = (serviceId << DTServiceIdOffset);
        myDeviceTag |= (userId << DTUserIdOffset);

        timeb tb;
        ftime( &tb );
        int curTime = tb.millitm + (tb.time & 0xfffff) * 1000; // Millisecond timer
        myTimestamp = curTime;
    }

    ///////////////////////////////////////////////////////////////////////////
    inline 
    void Event::resetSourceId(uint newSourceId)
    {
        mySourceId = newSourceId;
    }

    ///////////////////////////////////////////////////////////////////////////
    inline 
    void Event::resetType(Type type)
    {
        myType = type;
    }

    ///////////////////////////////////////////////////////////////////////////
    inline void Event::copyFrom(const Event& e)
    { memcpy(this, &e, sizeof(Event)); }

    ///////////////////////////////////////////////////////////////////////////
    inline unsigned int Event::getTimestamp() const
    { return myTimestamp; }

    ///////////////////////////////////////////////////////////////////////////
    inline unsigned int Event::getSourceId() const
    { return mySourceId; }

    ///////////////////////////////////////////////////////////////////////////
    inline Service::ServiceType Event::getServiceType() const
    { return myServiceType;	}

    ///////////////////////////////////////////////////////////////////////////
    inline void Event::setServiceType(Service::ServiceType value)
    { myServiceType = value;	}

    ///////////////////////////////////////////////////////////////////////////
    inline unsigned int Event::getServiceId() const
    { return (myDeviceTag & DTServiceIdMask) >> DTServiceIdOffset; }

    ///////////////////////////////////////////////////////////////////////////
    inline unsigned int Event::getUserId() const
    { return (myDeviceTag & DTUserIdMask) >> DTUserIdOffset; }

    ///////////////////////////////////////////////////////////////////////////
    inline unsigned int Event::getDeviceTag() const
    { return myDeviceTag; }

    ///////////////////////////////////////////////////////////////////////////
    inline Event::Type Event::getType() const
    { return myType; }

    ///////////////////////////////////////////////////////////////////////////
    inline void Event::setProcessed() const
    { myFlags |= Processed; }

    ///////////////////////////////////////////////////////////////////////////
    inline bool Event::isProcessed() const
    { return ((myFlags & Processed) == Processed); }

    ///////////////////////////////////////////////////////////////////////////
    inline void Event::setExclusive() const
    { myFlags |= Exclusive; }

    ///////////////////////////////////////////////////////////////////////////
    inline bool Event::isExclusive() const
    { return ((myFlags & Exclusive) == Exclusive); }

    ///////////////////////////////////////////////////////////////////////////
    inline const Vector3f& Event::getPosition() const
    { return myPosition; }

    ///////////////////////////////////////////////////////////////////////////
    //inline float Event::getPosition(int component) const
    //{ return myPosition[component]; }

    ///////////////////////////////////////////////////////////////////////////
    inline void Event::setPosition(const Vector3f& v)
    {
        myPosition[0] = v[0];
        myPosition[1] = v[1];
        myPosition[2] = v[2];
    }

    ///////////////////////////////////////////////////////////////////////////
    inline void Event::setPosition(float x, float y, float z)
    {
        myPosition[0] = x;
        myPosition[1] = y;
        myPosition[2] = z;
    }

    ///////////////////////////////////////////////////////////////////////////
    inline void Event::setPosition(float x, float y)
    {
        myPosition[0] = x;
        myPosition[1] = y;
        myPosition[2] = 0;
    }

    ///////////////////////////////////////////////////////////////////////////
    inline const Quaternion& Event::getOrientation() const
    { return myOrientation; }

    ///////////////////////////////////////////////////////////////////////////
    inline void Event::setOrientation(const Quaternion& value)
    { myOrientation = value; }

    ///////////////////////////////////////////////////////////////////////////
    inline void Event::setOrientation(float w, float x, float y, float z)
    { myOrientation = Quaternion(w, x, y, z); }

    ///////////////////////////////////////////////////////////////////////////
    inline bool Event::isFlagSet(uint flag) const
    { return (myFlags & flag) == flag; }

    ///////////////////////////////////////////////////////////////////////////
    inline void Event::setFlags(uint flags) const
    { myFlags |= flags; }

    ///////////////////////////////////////////////////////////////////////////
    inline void Event::clearFlags() const
    { myFlags = 0; }

    ///////////////////////////////////////////////////////////////////////////
    inline uint Event::getFlags() const
    { return myFlags; }

    ///////////////////////////////////////////////////////////////////////////
    inline bool Event::isKeyDown(int key) const
    {
        return (mySourceId == key && myType == Event::Down);
    }

    ///////////////////////////////////////////////////////////////////////////
    inline bool Event::isKeyUp(int key) const
    {
        return ((char)mySourceId == key && myType == Event::Up);
    }

    ///////////////////////////////////////////////////////////////////////////
    inline bool Event::isButtonDown(Flags button) const
    {
        return (myType == Down && ((myFlags & button) == button));
    }

    ///////////////////////////////////////////////////////////////////////////
    inline bool Event::isButtonUp(Flags button) const
    {
        return (myType == Up && ((myFlags & button) == button));
    }

    ///////////////////////////////////////////////////////////////////////////
    inline bool Event::isFrom(Service* svc, int sourceId) const
    {
        return (getServiceId() == svc->getServiceId() && mySourceId == sourceId);
    }

    ///////////////////////////////////////////////////////////////////////////
    inline bool Event::isFrom(Service::ServiceType type, int sourceId) const
    {
        return (myServiceType == type && mySourceId == sourceId);
    }

    ///////////////////////////////////////////////////////////////////////////
    inline float Event::getAxis(int axisId) const
    {
        if(myExtraDataType == ExtraDataFloatArray && myExtraDataItems > axisId)
        {
            return getExtraDataFloat(axisId);
        }
        return 0;
    }

    ///////////////////////////////////////////////////////////////////////////
    inline void Event::setExtraDataType(Event::ExtraDataType type)
    { myExtraDataType = type; }

    ///////////////////////////////////////////////////////////////////////////
    inline Event::ExtraDataType Event::getExtraDataType() const
    { return myExtraDataType; }

    ///////////////////////////////////////////////////////////////////////////
    inline float Event::getExtraDataFloat(int index) const
    {
        oassert(myExtraDataType == ExtraDataFloatArray);
        oassert(!isExtraDataNull(index));
        return FLOAT_PTR(myExtraData[index * 4]);
    }

    ///////////////////////////////////////////////////////////////////////////
    inline void Event::setExtraDataFloat(int index, float value)
    {
        oassert(myExtraDataType == ExtraDataFloatArray);
        oassert(index < MaxExtraDataItems);
        if(index >= myExtraDataItems) myExtraDataItems = index + 1;
        // Mark this entry bit as valid in the extra data validity mask
        myExtraDataValidMask |= (1 << index);
        FLOAT_PTR(myExtraData[index * 4]) = value;
    }

    ///////////////////////////////////////////////////////////////////////////
    inline int Event::getExtraDataInt(int index) const
    {
        oassert(myExtraDataType == ExtraDataIntArray);
        oassert(!isExtraDataNull(index));
        return INT_PTR(myExtraData[index * 4]);
    }

    ///////////////////////////////////////////////////////////////////////////
    inline void Event::setExtraDataInt(int index, int value)
    {
        oassert(myExtraDataType == ExtraDataIntArray);
        oassert(index < MaxExtraDataItems);
        if(index >= myExtraDataItems) myExtraDataItems = index + 1;
        // Mark this entry bit as valid in the extra data validity mask
        myExtraDataValidMask |= (1 << index);
        INT_PTR(myExtraData[index * 4]) = value;
    }

    ///////////////////////////////////////////////////////////////////////////
    inline Vector3f Event::getExtraDataVector3(int index) const
    {
        oassert(myExtraDataType == ExtraDataVector3Array);
        oassert(!isExtraDataNull(index));
        int offset = index * 3 * 4;
        float x = FLOAT_PTR(myExtraData[offset]);
        float y = FLOAT_PTR(myExtraData[offset + 4]);
        float z = FLOAT_PTR(myExtraData[offset + 8]);
        return Vector3f(x, y, z);
    }

    ///////////////////////////////////////////////////////////////////////////
    inline void Event::setExtraDataVector3(int index, const Vector3f value)
    {
        oassert(myExtraDataType == ExtraDataVector3Array);
        oassert(index < MaxExtraDataItems);
        if(index >= myExtraDataItems) myExtraDataItems = index + 1;
        // Mark this entry bit as valid in the extra data validity mask
        myExtraDataValidMask |= (1 << index);
        int offset = index * 3 * 4;
        FLOAT_PTR(myExtraData[offset]) = value[0];
        FLOAT_PTR(myExtraData[offset + 4]) = value[1];
        FLOAT_PTR(myExtraData[offset + 8]) = value[2];
    }

    ///////////////////////////////////////////////////////////////////////////
    inline const char* Event::getExtraDataString() const
    {
        oassert(myExtraDataType == ExtraDataString);
        return (const char*)myExtraData;
    }

    ///////////////////////////////////////////////////////////////////////////
    inline void* Event::getExtraDataBuffer() const
    {
        return (void*)myExtraData;
    }

    ///////////////////////////////////////////////////////////////////////////
    inline void Event::setExtraData(Event::ExtraDataType type, unsigned int items, int mask, void* data)
    {
        myExtraDataType = type;
        myExtraDataItems = items;
        myExtraDataValidMask = mask;
        memcpy(myExtraData, data, getExtraDataSize());
    }

    ///////////////////////////////////////////////////////////////////////////
    inline void Event::setExtraDataString(const String& value)
    {
        oassert(myExtraDataType == ExtraDataString);
        strcpy((char*)myExtraData, value.c_str());
        myExtraDataItems = (int)value.size();
        myExtraData[myExtraDataItems] = '\0';
    }

    ///////////////////////////////////////////////////////////////////////////
    inline void Event::resetExtraData()
    { 
        myExtraDataValidMask = 0; 
        myExtraDataType = ExtraDataNull;
        myExtraDataItems = 0;
    }

    ///////////////////////////////////////////////////////////////////////////
    inline bool Event::isExtraDataNull(int index) const
    { return !((myExtraDataValidMask & (1 << index)) == (1 << index)); }

    ///////////////////////////////////////////////////////////////////////////
    inline int Event::getExtraDataItems() const
    { return myExtraDataItems; }

    ///////////////////////////////////////////////////////////////////////////
    inline int Event::getExtraDataSize() const
    {
        switch(myExtraDataType)
        {
        case ExtraDataNull:
            return 0;
        case ExtraDataFloatArray:
        case ExtraDataIntArray:
            return myExtraDataItems * 4;
            break;
        case ExtraDataVector3Array:
            return myExtraDataItems * 4 * 3;
        case ExtraDataString:
            return myExtraDataItems;
        default:
            oerror("Event::getExtraDataSize: unknown extra data type");
        }

        // Default: return data lengthp
        return myExtraDataItems;
    }

    ///////////////////////////////////////////////////////////////////////////
    inline bool Event::getChar(char* c) const
    {
        if(myServiceType == Service::Keyboard && myType == Down)
        {
            if(getExtraDataType() == ExtraDataString)
            {
                *c = getExtraDataString()[0];
                return true;
            }
        }
        return false;
    }
}; // namespace omicron

#undef FLOAT_PTR
#undef INT_PTR

#endif
