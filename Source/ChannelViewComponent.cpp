#include "ChannelViewComponent.h"

ChannelViewComponent::ChannelViewComponent ()
{
    channelHeight = DEFAULT_CHANNEL_HEIGHT;
    channelListener = nullptr;
    currentSelectedElement = nullptr;
    currentSelectedChannel = nullptr;
}

ChannelViewComponent::~ChannelViewComponent ()
{
    for (unsigned i = 0; i < channels.size(); i++)
    {
        channels[i]->removeChannelListener(this);
        
        if (channelListener != nullptr)
            channels[i]->removeChannelListener( channelListener );
    }
    
    channels.clear();
    
    channelListener = nullptr;
    
    removeAllChildren();
}

void ChannelViewComponent::reset ()
{
    for (unsigned i = 0; i < channels.size(); i++)
    {
        channels[i]->removeChannelListener(this);
        
        if (channelListener != nullptr)
            channels[i]->removeChannelListener( channelListener );
    }
    
    channels.clear();

    currentSelectedElement = nullptr;
    currentSelectedChannel = nullptr;
    
    setSize(0, 0);

    repaint();
}

ValueTree ChannelViewComponent::getForDocument()
{
    ValueTree data ("CHANNELS");
    
    for (unsigned i = 0; i < channels.size(); i++)
        data.addChild (channels[i]->getForDocument(), i, nullptr);

    return data;
}

void ChannelViewComponent::loadFromDocument (ValueTree data)
{
    for (unsigned i = 0; i < data.getNumChildren(); i++)
    {
        if (data.getChild(i).hasType("CHANNEL"))
        {
            addChannel();
            
            channels[i]->loadFromDocument (data.getChild(i));
        }
        
    }
}

void ChannelViewComponent::setChannelListener (ChannelListener* const listener)
{
    channelListener = listener;
}

void ChannelViewComponent::channelSelected (ChannelComponent &channel )
{
    if (currentSelectedChannel != nullptr)
        currentSelectedChannel->setSelectionState(false);
    
    channel.setSelectionState(true);
    currentSelectedChannel = &channel;
}

void ChannelViewComponent::channelElementSelected(ChannelComponent &channel, ChannelElementComponent &element)
{
    if (currentSelectedElement != nullptr)
        currentSelectedElement->setSelectionState(false);
    
    element.setSelectionState(true);
    currentSelectedElement = &element;
    
    channelSelected(channel);
}

void ChannelViewComponent::channelElementRemoved(ChannelComponent &selectedChannel)
{
    currentSelectedElement = nullptr;
}

void ChannelViewComponent::addChannel ()
{
    // add new channel
    
    ChannelComponent* newChannel;
    newChannel = new ChannelComponent();
    
    if (channelListener != nullptr)
        newChannel->addChannelListener( channelListener );
    
    newChannel->addChannelListener( this );
    
    channels.add(newChannel);
    
    addAndMakeVisible(newChannel);
    
    // update layout
    
    setSize( DEFAULT_CHANNEL_WIDTH, ( channels.size() * channelHeight ) );
    
    resized();
}

void ChannelViewComponent::paint (Graphics& g)
{
    g.fillAll( Colour::fromRGBA(0, 0, 0, 25) );
}

void ChannelViewComponent::resized ()
{
    for (unsigned i = 0; i < channels.size(); i++)
        channels[i]->setBounds(0, i * channelHeight, DEFAULT_CHANNEL_WIDTH, channelHeight);
}

// transport

Array<DataInterface::Package> ChannelViewComponent::getData (const int from, const int to, const float position)
{
    Array<DataInterface::Package> data;
    
    for (unsigned i = 0; i < channels.size(); i++)
        data.add (channels[i]->getData(from, to, position));

    return data;
}