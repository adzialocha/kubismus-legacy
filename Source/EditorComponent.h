#ifndef EDITORCOMPONENT_H_INCLUDED
#define EDITORCOMPONENT_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "EditableProperties.h"

class EditorComponent : public Component,
                        public EditableProperties
{
    
public:
    
    // EditorComponent::Type
    
    enum Type
    {
        unset,
        pointEditor,
        randomEditor,
        binaryStretchEditor,
        stepsizeEditor
    };
    
    virtual EditorComponent::Type getType () { return type; }
    
    static EditorComponent::Type findTypeFromDocument (const int num) { return static_cast<Type>(num); }
    
    // EditorComponent
    
    EditorComponent() {};
    virtual ~EditorComponent() {};
    
    // EditableProperties

    void getEditableProperties(Array<PropertyComponent*>& props) override;
    
    // transport
    
    virtual const float getData(const float position);
    
    virtual void transportEnter ();
    virtual void transportExit ();
    
    // project
    
    virtual ValueTree getForDocument ();
    virtual void loadFromDocument (ValueTree data);
    
private:
    
    const EditorComponent::Type type = EditorComponent::Type::unset;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (EditorComponent)
    
};

#endif  // EDITORCOMPONENT_H_INCLUDED
