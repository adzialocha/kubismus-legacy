#include "SidebarComponent.h"

SidebarComponent::SidebarComponent() :  connectButton("Disconnect", "Connect"),
                                        collapseButton("Collapse"),
                                        addChannelButton("Add Channel"),
                                        addElementButton("Add Element"),
                                        removeElementButton("Remove Element")
{
    collapsed = false;
    
    collapseButton.addListener(this);
    addChannelButton.addListener(this);
    addElementButton.addListener(this);
    removeElementButton.addListener(this);
    connectButton.addListener(this);

    collapseButton.setWantsKeyboardFocus(false);
    addChannelButton.setWantsKeyboardFocus(false);
    removeElementButton.setWantsKeyboardFocus(false);
    addElementButton.setWantsKeyboardFocus(false);
    
    addAndMakeVisible(collapseButton);
    addAndMakeVisible(addChannelButton);
    addAndMakeVisible(addElementButton);
    addAndMakeVisible(removeElementButton);
    addAndMakeVisible(connectButton);

    selectedChannel = nullptr;
    selectedElement = nullptr;
    selectedElementChannel = nullptr;
    
    addElementButton.setVisible(false);
    removeElementButton.setVisible(false);
    
    addAndMakeVisible (propertyPanel);
    
    setConnectionState (false);
    
    insertPosition = 0;
}

SidebarComponent::~SidebarComponent()
{
    collapseButton.removeListener(this);
    addChannelButton.removeListener(this);
    addElementButton.removeListener(this);
    removeElementButton.removeListener(this);
    connectButton.removeListener(this);
    
    removeAllChildren();
}

void SidebarComponent::paint (Graphics& g)
{
    g.fillAll (Colours::darkgrey);
}

void SidebarComponent::resized()
{
    int x;
    
    x = collapsed ? -getWidth() : 0;
    
    collapseButton.setBounds(0, 0, getWidth(), 25);
    
    addChannelButton.setBounds(x, 25, getWidth(), 25);
    addElementButton.setBounds(x, 50, getWidth(), 25);
    removeElementButton.setBounds(x, 75, getWidth(), 25);
    
    propertyPanel.setBounds (x, 100, getWidth(), getHeight() - 125);
    
    connectButton.setBounds(x, getHeight() - 25, getWidth(), 25);
}

bool SidebarComponent::isCollapsed()
{
    return collapsed;
}

void SidebarComponent::showChannel(ChannelComponent* channel)
{
    selectedChannel = channel;
    
    addElementButton.setVisible(true);
}

void SidebarComponent::showElement(ChannelComponent* channel, ChannelElementComponent* element)
{
    selectedElement = element;
    selectedElementChannel = channel;
    
    removeElementButton.setVisible(true);
    
    propertyPanel.clear();
    
    Array <PropertyComponent*> channelProps;
    
    selectedElementChannel->getEditableProperties(channelProps);
    
    propertyPanel.addSection ("Position", channelProps);
    
    Array <PropertyComponent*> elementProps;
    
    element->getEditableProperties(elementProps);
    propertyPanel.addSection ("Element", elementProps);
    
    Array <PropertyComponent*> editorProps;
    
    element->editor->getEditableProperties(editorProps);
    
    if (editorProps.size() > 0)
        propertyPanel.addSection ("Editor", editorProps);
}

void SidebarComponent::setConnectionState(bool status)
{
    connectButton.setToggleState (status, NotificationType::sendNotification);
}

void SidebarComponent::buttonClicked (Button* button)
{
    if (button == &collapseButton)
    {
        collapsed = ! collapsed;
        
        sendActionMessage("collapsedClicked");
    }
    else if (button == &addChannelButton)
    {
        sendActionMessage("addChannelClicked");
    }
    else if (button == &connectButton)
    {
        if (connectButton.getToggleState())
            sendActionMessage("connectClicked");
        else
            sendActionMessage("disconnectClicked");
    }
    else if (button == &addElementButton)
    {
        if ( selectedChannel != nullptr )
        {
            PopupMenu m;
            
            m.addItem (1, "Point Element");
            m.addItem (2, "Random Element");
            m.addItem (3, "Binary Stretch Element");
            m.addItem (4, "Step Element");
            
            const int r = m.show();
            
            if (r == 1)
               selectedChannel->addChannelElement( EditorComponent::Type::pointEditor, insertPosition, 100 );
            else if (r == 2)
                selectedChannel->addChannelElement( EditorComponent::Type::randomEditor, insertPosition, 100 );
            else if (r == 3)
                selectedChannel->addChannelElement( EditorComponent::Type::binaryStretchEditor, insertPosition, 100 );
            else if (r == 4)
                selectedChannel->addChannelElement( EditorComponent::Type::stepsizeEditor, insertPosition, 100 );
        }
    }
    else if (button == &removeElementButton)
    {
        if ( selectedElementChannel != nullptr && selectedElement != nullptr )
        {
            propertyPanel.clear();
            
            selectedElementChannel->removeChannelElement(*selectedElement);
            
            removeElementButton.setVisible(false);
            selectedElement = nullptr;
            selectedElementChannel = nullptr;
        }
    }
}

void SidebarComponent::reset ()
{
    propertyPanel.clear();
    
    addElementButton.setVisible (false);
    removeElementButton.setVisible (false);
    
    insertPosition = 0;
    
    setConnectionState (false);
    sendActionMessage ("disconnectClicked");
    
    selectedChannel = nullptr;
    selectedElement = nullptr;
    selectedElementChannel = nullptr;
}