#ifndef ELEMENTOSCHANDLER_H_INCLUDED
#define ELEMENTOSCHANDLER_H_INCLUDED

#include "OSCProperties.h"

class ElementOSCProperties  : public TextPropertyComponent
{

public:
    
    ElementOSCProperties (const String& _name, OSCProperties* elem)
                              : TextPropertyComponent (_name, 16, false),
                                name(_name),
                                target(elem)
    {
    }
    
    ~ElementOSCProperties() {};
    
    String getText() const override
    {
        if (name == "oscChannel")
            return std::to_string(target->getOSCChannel());
        else if (name == "oscAddress")
            return target->getOSCAddress();
        else if (name == "oscMin")
            return std::to_string(target->getOSCMinValue());
        else if (name == "oscMax")
            return std::to_string(target->getOSCMaxValue());
        
        return "";
    }
    
    void setText (const String& newText) override
    {
        if (name == "oscChannel")
            target->setOSCChannel(newText.getIntValue());
        else if (name == "oscAddress")
            target->setOSCAddress(newText);
        else if (name == "oscMin")
            target->setOSCMinValue(newText.getFloatValue());
        else if (name == "oscMax")
            target->setOSCMaxValue(newText.getFloatValue());
    }
    
private:
    
    const String name;
    
    OSCProperties* target;
    
};

#endif  // ELEMENTOSCHANDLER_H_INCLUDED
