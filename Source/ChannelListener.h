#ifndef CHANNELLISTENER_H_INCLUDED
#define CHANNELLISTENER_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

class ChannelComponent;
class ChannelElementComponent;

class ChannelListener
{
    
public:
    
    virtual ~ChannelListener()  {}
    
    virtual void channelSelected ( ChannelComponent &selectedChannel );
    virtual void channelElementSelected ( ChannelComponent &selectedChannel, ChannelElementComponent &selectedElement );
    virtual void channelElementRemoved ( ChannelComponent &selectedChannel );
    
};

#endif  // CHANNELLISTENER_H_INCLUDED
