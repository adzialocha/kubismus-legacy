#ifndef EDITABLEPROPERTIES_H_INCLUDED
#define EDITABLEPROPERTIES_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

class EditableProperties
{
    
public:
    
    EditableProperties() {};
    
    virtual ~EditableProperties() {};
    
    virtual void getEditableProperties(Array<PropertyComponent*>& props) {};
    
};

#endif  // EDITABLEPROPERTIES_H_INCLUDED
