#ifndef ELEMENTPROPERTIES_H_INCLUDED
#define ELEMENTPROPERTIES_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

class ElementProperties  : public TextPropertyComponent
{
    
public:
    
    ElementProperties (const String& _name, Component* elem)
                        : TextPropertyComponent (_name, 64, false),
                          name(_name),
                          element(elem)
    {
    }
    
    ~ElementProperties() {}
    
    void setText (const String& newText) override
    {
        element->setName ( newText );
        element->repaint();
    }
    
    String getText() const override
    {
        if (name == "name")
            return element->getName ();
        else
            return "";
    }
    
private:
    
    const String name;
    
    Component* element;
    
};

#endif  // ELEMENTPROPERTIES_H_INCLUDED
