#ifndef CHANNELELEMENTPROPERTIES_H_INCLUDED
#define CHANNELELEMENTPROPERTIES_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

#include "ChannelElementProperties.h"

class ChannelElementProperties  : public TextPropertyComponent,
                                  private ChangeListener
{
    
public:
    
    ChannelElementProperties (const String& _name, ChannelElementComponent* elem)
                                : TextPropertyComponent (_name, 16, false),
                                  name(_name),
                                  element(elem)
    {
        element->addChangeListener (this);
    }
    
    ~ChannelElementProperties()
    {
        element->removeChangeListener (this);
    }
    
    void setText (const String& newText) override
    {
        if (name == "x")
            return element->setTopLeftPosition ( newText.getIntValue () , 0 );
        else if (name == "width")
            return element->setSize ( newText.getIntValue (), element->getHeight() );
    }
    
    String getText() const override
    {
        if (name == "x")
            return std::to_string ( element->getX() );
        else if (name == "width")
            return std::to_string ( element->getWidth() );
        else
            return "";
    }
    
    void changeListenerCallback (ChangeBroadcaster* source) override
    {
        refresh();
    }
    
private:
    
    const String name;
    
    ChannelElementComponent* element;
    
};

#endif  // CHANNELELEMENTPROPERTIES_H_INCLUDED
