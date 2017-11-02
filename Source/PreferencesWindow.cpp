#include "../JuceLibraryCode/JuceHeader.h"
#include "PreferencesWindow.h"

PreferencesWindow::PreferencesWindow() : DocumentWindow("Preferences",
                                                        Colours::lightgrey,
                                                        DocumentWindow::allButtons)
{
    setAddress (DEFAULT_TARGET_IP);
    setPort (DEFAULT_PORT);
    
    centreWithSize (500, 200);
    
    Array<juce::PropertyComponent *> props;
    
    props.add ( new PreferencesTextProperty( "address", this ) );
    props.add ( new PreferencesTextProperty( "port", this ) );
    
    propertyPanel.addProperties (props);
    
    propertyPanel.setBounds(0, 0, getWidth(), getHeight());

    setUsingNativeTitleBar (true);
    setContentOwned (&propertyPanel, true);
    
    setResizable(false, false);
}

PreferencesWindow::~PreferencesWindow()
{
    propertyPanel.clear();
}

void PreferencesWindow::changeListenerCallback (juce::ChangeBroadcaster *source)
{
    sendChangeMessage();
}