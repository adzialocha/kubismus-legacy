#ifndef TIMELINECOMPONENT_H_INCLUDED
#define TIMELINECOMPONENT_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

class TimelineComponent : public Component,
                          public ChangeBroadcaster
{
public:
    
    TimelineComponent();
    ~TimelineComponent();

    void paint (Graphics&) override;
    void resized() override;
    
    const int getMousePosition()
    {
        return mousePosition;
    }
    
    bool wasCommandKeyPressed;
    
    void setResolution (const int minuteInPixels)
    {
        minInPixels = minuteInPixels;
        
        repaint();
    }
    
    void mouseDown (const MouseEvent& e) override;
    
    void setViewportArea (const Rectangle< int > &newVisibleArea);
    
private:
    
    const int RULER_HEIGHT = 25;
    const int GRID_SIZE_SECONDS = 10;
    const int MINUTE_IN_PIXELS_DEFAULT = 1000;
    
    int minInPixels;
    int startPos;
    int mousePosition;
    
    int roundUp(int numToRound, int multiple)
    {
        if (multiple == 0)
            return numToRound;
        
        int remainder = numToRound % multiple;
        
        if (remainder == 0)
            return numToRound;
        
        return numToRound + multiple - remainder;
    }
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TimelineComponent)
};


#endif  // TIMELINECOMPONENT_H_INCLUDED
