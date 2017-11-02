#ifndef ELEMENTTIMEPROPERTIES_H_INCLUDED
#define ELEMENTTIMEPROPERTIES_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

class ElementTimeProperties  : public SliderPropertyComponent
{
    
public:
    
    ElementTimeProperties (const String& _name, VirtualTimeProperties* elem)
                            : SliderPropertyComponent (_name, 0.1, 1.0, 0.01, 1.0),
                              element(elem)
    {
    }
    
    ~ElementTimeProperties() {}
    
    void setValue (double newValue) override
    {
        element->setVirtualTime ( newValue );
        Component* comp = dynamic_cast<Component*> (element);
        comp->repaint();
    }
    
    double getValue() const override
    {
        return element->getVirtualTime();
    }
    
private:
    
    VirtualTimeProperties* element;
    
};

#endif  // ELEMENTTIMEPROPERTIES_H_INCLUDED
