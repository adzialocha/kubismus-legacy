#include "HorizontalComponentBoundsConstrainer.h"

HorizontalComponentBoundsConstrainer::HorizontalComponentBoundsConstrainer()
{

}

HorizontalComponentBoundsConstrainer::~HorizontalComponentBoundsConstrainer()
{

}

void HorizontalComponentBoundsConstrainer::checkBounds (Rectangle<int>& bounds,
                                              const Rectangle<int>& old,
                                              const Rectangle<int>& limits,
                                              const bool isStretchingTop,
                                              const bool isStretchingLeft,
                                              const bool isStretchingBottom,
                                              const bool isStretchingRight)
{
    
    const int limit = old.getY();
    
    ComponentBoundsConstrainer::checkBounds(bounds,
                                            old,
                                            limits,
                                            isStretchingTop,
                                            isStretchingLeft,
                                            isStretchingBottom,
                                            isStretchingRight);
    
    bounds.setY ( limit );
}