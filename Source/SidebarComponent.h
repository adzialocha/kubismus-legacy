#ifndef SIDEBARCOMPONENT_H_INCLUDED
#define SIDEBARCOMPONENT_H_INCLUDED


#include "../JuceLibraryCode/JuceHeader.h"

#include "ChannelComponent.h"
#include "ViewportListener.h"
#include "ConnectionToggleButtonComponent.h"
#include "CollapseButtonComponent.h"

class SidebarComponent    : public Component,
                            public ActionBroadcaster,
                            private Button::Listener
{
public:

    SidebarComponent();
    ~SidebarComponent();
    
    void paint (Graphics&) override;
    void resized() override;
    
    void setInsertPosition(const int pos)
    {
        insertPosition = pos;
    }
    
    bool isCollapsed();
    
    void setConnectionState( bool status );
    
    void showChannel( ChannelComponent* channel );
    void showElement( ChannelComponent* channel, ChannelElementComponent* element );
    
    void reset();
    
private:
    
    int insertPosition;
    
    bool collapsed;
    
    ConnectionToggleButtonComponent connectButton;
    CollapseButtonComponent collapseButton;
    
    TextButton addChannelButton, addElementButton, removeElementButton;

    void buttonClicked (Button* button) override;
    
    ChannelComponent* selectedChannel;
    
    ChannelComponent* selectedElementChannel;
    ChannelElementComponent* selectedElement;
    
    PropertyPanel propertyPanel;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SidebarComponent)
};

#endif  // SIDEBARCOMPONENT_H_INCLUDED
