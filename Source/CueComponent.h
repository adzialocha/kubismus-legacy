#ifndef CUECOMPONENT_H_INCLUDED
#define CUECOMPONENT_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

class CueComponent : public ScannerComponent
{
    
public:
    
    CueComponent()
    {
        blockedState = false;
    };
    
    ~CueComponent() {};
    
    void paint(juce::Graphics &g) override
    {
        if (isMouseOver)
        {
            g.setColour (Colours::white);
        }
        else
        {
            g.setColour (Colours::firebrick);
        }
        
        if (blockedState)
        {
            const float dashLengths[] = { 30, 40 };
            g.drawDashedLine(Line<float>( 0, 0, 0, getHeight() ), &dashLengths[0], 2, 1.0, 0);
        }
        else
        {
            const float dashLengths[] = { 5, 5 };
            g.drawDashedLine(Line<float>( 0, 0, 0, getHeight() ), &dashLengths[0], 2, 1.0, 0);
        }
    }
    
    void setBlockedState(bool state)
    {
        blockedState = state;
    }
    
    int getBlockedState()
    {
        return blockedState;
    }
    
private:
    
    bool blockedState;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CueComponent)
    
};

#endif  // CUECOMPONENT_H_INCLUDED
