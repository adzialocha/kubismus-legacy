#ifndef HORIZONTALCOMPONENTBOUNDSCONSTRAINER_H_INCLUDED
#define HORIZONTALCOMPONENTBOUNDSCONSTRAINER_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

class HorizontalComponentBoundsConstrainer : public ComponentBoundsConstrainer
{
    
public:
    
    HorizontalComponentBoundsConstrainer();
    ~HorizontalComponentBoundsConstrainer();
    
    void checkBounds (Rectangle<int>& bounds,
                      const Rectangle<int>& previousBounds,
                      const Rectangle<int>& limits,
                      bool isStretchingTop,
                      bool isStretchingLeft,
                      bool isStretchingBottom,
                      bool isStretchingRight) override;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (HorizontalComponentBoundsConstrainer)
    
};


#endif  // HORIZONTALCOMPONENTBOUNDSCONSTRAINER_H_INCLUDED
