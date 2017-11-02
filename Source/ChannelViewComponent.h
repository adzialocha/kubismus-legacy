#ifndef CHANNELVIEWCOMPONENT_H_INCLUDED
#define CHANNELVIEWCOMPONENT_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

#include "ChannelComponent.h"
#include "ChannelListener.h"
#include "DataInterface.h"

class ChannelViewComponent    : public Component,
                                private ChannelListener
{
    
public:
    
    ChannelViewComponent();
    ~ChannelViewComponent();
    
    void addChannel();

    void paint (Graphics&) override;
    void resized() override;
    
    void setChannelListener (ChannelListener* const listener);
    
    // project
    
    void reset ();
    void loadFromDocument (ValueTree data);
    ValueTree getForDocument ();
    
    // transport
    
    Array<DataInterface::Package> getData(const int from, const int to, const float pos);
    
    // ChannelListener
    
    void channelSelected ( ChannelComponent &selectedChannel ) override;
    void channelElementSelected ( ChannelComponent &selectedChannel, ChannelElementComponent &selectedElement ) override;
    void channelElementRemoved ( ChannelComponent &selectedChannel ) override;
    
private:

    const int DEFAULT_CHANNEL_HEIGHT = 200;
    const int DEFAULT_CHANNEL_WIDTH = 25000;
    
    int channelHeight;
    
    ChannelListener* channelListener;
    
    ChannelComponent* currentSelectedChannel;
    ChannelElementComponent* currentSelectedElement;
 
    OwnedArray<ChannelComponent> channels;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ChannelViewComponent)
    
};

#endif