#ifndef PREFERENCESWINDOW_H_INCLUDED
#define PREFERENCESWINDOW_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"


class PreferencesWindow    : public DocumentWindow,
                             public ChangeBroadcaster,
                             public ChangeListener
{
    
public:
    
    PreferencesWindow();
    ~PreferencesWindow();

    void closeButtonPressed() override
    {
        setVisible (false);
    }
    
    // preferences
    
    void setAddress (const String newAddress) {
        address = newAddress;
    }
    
    const String getAddress () {
        return address;
    }
    
    void setPort (const unsigned newPort) {
        port = newPort;
    }
    
    const unsigned getPort () {
        return port;
    }
    
    // ChangeListener
    
    void changeListenerCallback ( ChangeBroadcaster *source ) override;

private:
    
    class PreferencesTextProperty  : public TextPropertyComponent,
                                     public ChangeBroadcaster
    {
        
    public:
        
        PreferencesTextProperty (const String& _name, PreferencesWindow* _window)
                                        : TextPropertyComponent (_name, 64, false),
                                          name(_name),
                                          window(_window)
        {
            addChangeListener (window);
        }
        
        ~PreferencesTextProperty()
        {
            removeChangeListener (window);
        }
        
        void setText (const String& newText) override
        {
            if (name == "address")
                return window->setAddress ( newText );
            else if (name == "port")
                return window->setPort ( newText.getIntValue() );
            
            sendChangeMessage();
                
            window->repaint();
        }
        
        String getText() const override
        {
            if (name == "address")
                return window->getAddress ();
            else if (name == "port")
                return std::to_string ( window->getPort () );
            else
                return "";
        }
        
    private:
        
        const String name;
        
        PreferencesWindow* window;
        
    };
    
    const String DEFAULT_TARGET_IP = "0.0.0.0";
    const int DEFAULT_PORT = 9001;
    
    String address;
    unsigned port;
    
    PropertyPanel propertyPanel;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PreferencesWindow)
    
};

#endif  // PREFERENCESWINDOW_H_INCLUDED
