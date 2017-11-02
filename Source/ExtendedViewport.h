#ifndef EXTENDEDVIEWPORT_H_INCLUDED
#define EXTENDEDVIEWPORT_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

#include "ViewportListener.h"

class ExtendedViewport    : public Viewport
{
public:
    
    ExtendedViewport();
    ~ExtendedViewport();
    
    void addViewportListener (ViewportListener* newListener);
    void removeViewportListener (ViewportListener* listenerToRemove);
    
    void visibleAreaChanged (const Rectangle< int > &newVisibleArea) override;

private:
    
    ListenerList<ViewportListener> viewportListeners;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ExtendedViewport)
    
};


#endif  // EXTENDEDVIEWPORT_H_INCLUDED
