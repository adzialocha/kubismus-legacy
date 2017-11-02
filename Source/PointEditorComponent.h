#ifndef POINTEDITORCOMPONENT_H_INCLUDED
#define POINTEDITORCOMPONENT_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

#include "PointEditor.h"

class PointEditorComponent    : public PointEditor
{
    
public:
    
    PointEditorComponent();
    ~PointEditorComponent();

    void paint (Graphics&) override;
    
    // EditorComponent
    
    EditorComponent::Type getType () override { return type; };
    
    // EditableProperties
    
    void getEditableProperties(Array<PropertyComponent*>& props) override;
    
    // transport
    
    const float getData(const float position) override;
    
    void transportEnter () override;
    void transportExit () override;
    
    // project
    
    ValueTree getForDocument() override;
    void loadFromDocument (ValueTree data) override;

private:
    
    const EditorComponent::Type type = EditorComponent::Type::pointEditor;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PointEditorComponent)
    
};

#endif