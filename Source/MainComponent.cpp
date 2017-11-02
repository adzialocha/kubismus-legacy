#include "MainComponent.h"

MainContentComponent::MainContentComponent () :
                                            interface(),
                                            preferences()
{
    setSize (800, 500);
    
    viewport.setViewedComponent (&channelView, true);
    viewport.setScrollBarThickness (6);
    
    channelView.setChannelListener (this);
    viewport.addViewportListener (this);
    sidebar.addActionListener (this);
    timeline.addChangeListener (this);
    session.addChangeListener (this);
    manager.addChangeListener (this);
    preferences.addChangeListener (this);
    
    addKeyListener (this);
    
    addAndMakeVisible (timeline);
    addAndMakeVisible (viewport);
    addAndMakeVisible (sidebar);
    addAndMakeVisible (manager);
    
    isTransportActive = false;
    scannerStart = 0;
    
    session.setResolution (MINUTE_IN_PIXELS);
    timeline.setResolution (MINUTE_IN_PIXELS);
}

MainContentComponent::~MainContentComponent ()
{
    if (isTransportActive)
        session.stop ();
    
    sidebar.removeActionListener (this);
    viewport.removeViewportListener (this);
    timeline.removeChangeListener (this);
    manager.removeChangeListener (this);
    preferences.removeChangeListener (this);
    
    PopupMenu::dismissAllActiveMenus ();
    
    removeKeyListener (this);
    
    removeAllChildren ();
}

void MainContentComponent::paint (Graphics& g)
{
    g.fillAll ( Colours::grey );
}

void MainContentComponent::resized ()
{
    const int sidebarWidth = sidebar.isCollapsed() ? SIDEBAR_WIDTH_COLLAPSED : SIDEBAR_WIDTH;

    Rectangle<int> area (getLocalBounds());
    
    viewport.setBounds( sidebarWidth, TIMELINE_HEIGHT, getWidth() - sidebarWidth, getHeight() - TIMELINE_HEIGHT );
    sidebar.setBounds( 0, 0, sidebarWidth, getHeight() );

    timeline.setBounds( sidebarWidth, 0, getWidth(), getHeight() );
    manager.setBounds( sidebarWidth, 0, getWidth(), getHeight() );
    
    #ifndef JUCE_MAC
        menu.setBounds (area.removeFromTop (LookAndFeel::getDefaultLookAndFeel().getDefaultMenuBarHeight()));
    #endif
}

// ActionListener (Sidebar)

void MainContentComponent::actionListenerCallback (const String& message)
{
    if (message == "collapsedClicked")
    {
        resized();
    }
    else if (message == "addChannelClicked")
    {
        channelView.addChannel();
    }
    else if (message == "connectClicked")
    {
        connectToServer();
    }
    else if (message == "disconnectClicked")
    {
        interface.disconnect();
        sidebar.setConnectionState(false);
    }
}

// ViewportListener

void MainContentComponent::visibleAreaChanged (const Rectangle<int> &newVisibleArea)
{
    timeline.setViewportArea( newVisibleArea );
    manager.setViewportArea( newVisibleArea );
}

// ChannelListener

void MainContentComponent::channelSelected (ChannelComponent &selectedChannel )
{
    sidebar.showChannel(&selectedChannel);
}

void MainContentComponent::channelElementSelected (ChannelComponent &selectedChannel, ChannelElementComponent &selectedElement)
{
    sidebar.showElement(&selectedChannel, &selectedElement);
}

// ChangeListener (Timeline & Session & Manager & Preferences)

void MainContentComponent::changeListenerCallback (ChangeBroadcaster *source)
{
    if (source == &timeline)
    {
        manager.setNewPosition ( timeline.getMousePosition(), timeline.wasCommandKeyPressed, false, false );
    }
    else if (source == &session)
    {
        manager.setNewPosition ( session.getPosition(), false, true, true );
        
        if (interface.isConnected())
            interface.send (channelView.getData(manager.getFrom(), manager.getTo(), session.getPrecisePosition()));
    }
    else if (source == &manager)
    {
        session.setPosition ( manager.getScannerPosition() );
        scannerStart = manager.getScannerPosition();
        
        if (interface.isConnected())
            interface.send (channelView.getData(manager.getFrom(), manager.getTo(), session.getPrecisePosition()));
    }
    else if (source == &preferences)
    {
        if (interface.isConnected())
        {
            interface.disconnect();
            connectToServer();
        }
        
    }
    
    sidebar.setInsertPosition (manager.getScannerPosition());
}

// KeyListener

bool MainContentComponent::keyPressed (const juce::KeyPress &key, juce::Component *originatingComponent)
{
    if (key.getKeyCode() == KeyPress::spaceKey)
    {
        isTransportActive = ! isTransportActive;
        
        if (isTransportActive)
        {
            session.setPosition ( scannerStart );
            
            session.start();
        }
        else
        {
            session.stop();
        }
        
        return true;
    }
    else if (key.getKeyCode() == KeyPress::returnKey)
    {
        manager.ignoreNextCue ();
        return true;
    }
    
    return false;
}
