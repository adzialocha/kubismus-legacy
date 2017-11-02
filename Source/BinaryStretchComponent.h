#ifndef BINARYSTRETCHCOMPONENT_H_INCLUDED
#define BINARYSTRETCHCOMPONENT_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

#include "PointEditor.h"

class BinaryStretchComponent    : public PointEditor,
                                  private ChangeListener
{
    
public:
    
    BinaryStretchComponent();
    ~BinaryStretchComponent();

    void paint (Graphics&) override;
    void resized () override;
    
    // distance property
    
    void setMaxDistance (const int dist)
    {
        distance = dist;
        
        calculateData();
    }
    
    int getMaxDistance ()
    {
        return distance;
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
    
    class DistancePropertyComponent : public SliderPropertyComponent
    {
        
    public:
        
        DistancePropertyComponent (const String& _name, BinaryStretchComponent* elem)
                                    : SliderPropertyComponent (_name, 2, 200, 1, 0.5),
                                    element(elem)
        {
            
        }
        
        ~DistancePropertyComponent() {}
        
        void setValue (double newValue) override
        {
            element->setMaxDistance ( (int) newValue );
            
            element->repaint();
        }
        
        double getValue() const override
        {
            return (double) element->getMaxDistance();
        }
        
    private:
        
        BinaryStretchComponent* element;
        
    };

    // =============================================
    
    int distance;
    
    bool isActivated, isPaintingActivity;
    
    const int NEXT_SIGNAL_MAX = 50;
    
    Array<int> data;
    
    void calculateData ()
    {
        int nextSignalTreshold;
        int nextCounter;
        
        int result;
        float val;
        
        nextCounter = 0;
        
        data.clear();

        for (unsigned i = 0; i < getWidth(); i++)
        {
            val = 1.0 - ( (float) getValueAt(i) / (float) getHeight() );
            nextSignalTreshold = val * distance;
            
            if (nextSignalTreshold < 2)
                nextSignalTreshold = 2;
            
            result = 0;
            
            if (nextSignalTreshold <= nextCounter)
            {
                nextCounter = 0;
                result = 1;
            }

            nextCounter++;
            
            data.add (result);
        }
    }
    
    void changeListenerCallback (ChangeBroadcaster *source) override
    {
        calculateData();
    }
    
    const EditorComponent::Type type = EditorComponent::Type::binaryStretchEditor;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BinaryStretchComponent)
    
};

#endif  // BINARYSTRETCHCOMPONENT_H_INCLUDED
