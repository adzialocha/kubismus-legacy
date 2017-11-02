#ifndef VIEWPORTLISTENER_H_INCLUDED
#define VIEWPORTLISTENER_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

class ViewportListener
{
    
public:
    
    virtual ~ViewportListener()  {}

    virtual void visibleAreaChanged (const Rectangle<int> &newVisibleArea);
  
};

#endif  // VIEWPORTLISTENER_H_INCLUDED
