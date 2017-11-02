#ifndef CONNECTIONTOGGLEBUTTONCOMPONENT_H_INCLUDED
#define CONNECTIONTOGGLEBUTTONCOMPONENT_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

class ConnectionToggleButtonComponent : public TextButton
{

public:
    
    ConnectionToggleButtonComponent (const String &connected, const String &disconnected) : TextButton(connected)
    {
        setClickingTogglesState (true);
        
        connectedStr = connected;
        disconnectedStr = disconnected;
    }
    
    ~ConnectionToggleButtonComponent ()
    {
        
    }
    
    void paintButton (Graphics &g, bool isMouseOverButton, bool isButtonDown) override
    {
        String text;
        
        text = getToggleState() ? connectedStr : disconnectedStr;
        
        if (getToggleState())
            g.fillAll (Colours::limegreen.withAlpha(0.5f).withBrightness(isMouseOverButton ? 0.8f : 1.0f));
        else
            g.fillAll (Colours::black.withAlpha(isMouseOverButton ? 0.4f : 0.2f));

        Font font (jmin (10.0f, getHeight() * 0.6f));

        g.setFont (font);
        g.setColour (Colours::white);
        
        const int yIndent = jmin (4, proportionOfHeight (0.3f));

        const int fontHeight = roundToInt (font.getHeight() * 0.6f);
        const int leftIndent  = jmin (fontHeight, 2 + 0 / (isConnectedOnLeft() ? 4 : 2));
        const int rightIndent = jmin (fontHeight, 2 + 0 / (isConnectedOnRight() ? 4 : 2));
        const int textWidth = getWidth() - leftIndent - rightIndent;
        
        if (textWidth > 0)
            g.drawFittedText (text.toUpperCase(),
                              leftIndent, yIndent, textWidth, getHeight() - yIndent * 2,
                              Justification::centred, 2);
    }
                              
private:
    
    String connectedStr, disconnectedStr;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ConnectionToggleButtonComponent)
    
};

#endif  // CONNECTIONTOGGLEBUTTONCOMPONENT_H_INCLUDED
