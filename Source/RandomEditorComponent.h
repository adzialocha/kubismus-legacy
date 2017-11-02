#ifndef RANDOMEDITORCOMPONENT_H_INCLUDED
#define RANDOMEDITORCOMPONENT_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

#include "EditorComponent.h"

class RandomEditorComponent    : public EditorComponent
{
    
public:
    
    struct BufferData
    {
        float position;
        float value;
    };
    
    RandomEditorComponent();
    ~RandomEditorComponent();
    
    void paint (Graphics&) override;
    
    // randomness property
    
    void setRandomness (const double rand)
    {
        randomness = rand;
    }
    
    double getRandomness ()
    {
        return randomness;
    }
    
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
    
    class RandomnessPropertyComponent : public SliderPropertyComponent
    {
        
    public:
        
        RandomnessPropertyComponent (const String& _name, RandomEditorComponent* elem)
                                        : SliderPropertyComponent (_name, 0.01, 1.0, 0.01, 0.5),
                                          element(elem)
        {
        }
        
        ~RandomnessPropertyComponent() {}
        
        void setValue (double newValue) override
        {
            element->setRandomness ( newValue );
        }
        
        double getValue() const override
        {
            return element->getRandomness();
        }
        
    private:
        
        RandomEditorComponent* element;
        
    };
    
    const int BUFFER_SIZE = 512;
    
    const EditorComponent::Type type = EditorComponent::Type::randomEditor;
    
    bool isInTransport;
    
    double randomness;
    
    float currentValue;
    float currentPosition;
    
    Array<BufferData> buffer;
    
    Random randomizer;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (RandomEditorComponent)
    
};

#endif  // RANDOMEDITORCOMPONENT_H_INCLUDED
