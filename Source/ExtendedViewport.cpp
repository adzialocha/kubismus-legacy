#include "ExtendedViewport.h"

ExtendedViewport::ExtendedViewport()
{
}

ExtendedViewport::~ExtendedViewport()
{
}

void ExtendedViewport::addViewportListener (ViewportListener* const newListener)
{
    viewportListeners.add (newListener);
}

void ExtendedViewport::removeViewportListener (ViewportListener* const listenerToRemove)
{
    viewportListeners.remove (listenerToRemove);
}

void ExtendedViewport::visibleAreaChanged (const Rectangle<int> &newVisibleArea)
{
    viewportListeners.call(&ViewportListener::visibleAreaChanged, newVisibleArea);
}