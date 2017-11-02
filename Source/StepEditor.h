#ifndef STEPEDITOR_H_INCLUDED
#define STEPEDITOR_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

#include "EditorComponent.h"

class StepEditor    : public EditorComponent
{
    
public:
    
    StepEditor();
    ~StepEditor();
    
    void paint (Graphics&) override;
    void resized() override;
    
    // stepsize property
    
    void setStepSize (const int size)
    {
        int prevSize;
        
        prevSize = tiles.size();
        
        stepsize = size;
        
        if (stepsize > tiles.size())
        {
            for (unsigned i = 0; i < (stepsize - prevSize); i++ )
            {
                StepTileComponent* tile;
                tile = new StepTileComponent();
                
                addAndMakeVisible(tile);

                tiles.add(tile);
            }
        }
        else if (stepsize < tiles.size())
        {
            for (unsigned i = 0; i < (prevSize - stepsize); i++ )
            {
                removeChildComponent(tiles.getLast());
                
                tiles.removeLast();
            }
        }
        
        resized();
    }
    
    int getStepSize ()
    {
        return stepsize;
    }
    
    // EditorComponent
    
    EditorComponent::Type getType () override { return type; };
    
    // EditableProperties
    
    void getEditableProperties(Array<PropertyComponent*>& props) override;
    
    // transport
    
    const float getData(const float position) override;
    
    void transportExit () override;
    
    // project
    
    ValueTree getForDocument() override;
    void loadFromDocument (ValueTree data) override;
    
private:
    
    // ======================================================
    
    class StepSizeComponent : public SliderPropertyComponent
    {
        
    public:
        
        StepSizeComponent (const String& _name, StepEditor* elem)
                                    : SliderPropertyComponent (_name, 2, 256, 1, 0.75),
                                    element(elem)
        {
        }
        
        ~StepSizeComponent() {};
        
        void setValue (double newValue) override
        {
            element->setStepSize ( (int) newValue );
        }
        
        double getValue() const override
        {
            return (double) element->getStepSize();
        }
        
    private:
        
        StepEditor* element;
        
    };
    
    // ======================================================
    
    class StepTileComponent : public Component
    {
        
    public:
        
        StepTileComponent()
        {
            isActive = false;
            isDragging = false;
            isPlaying = false;
            
            value = DEFAULT_VALUE;
        };
        
        ~StepTileComponent() {};
        
        void paint (Graphics &g) override
        {
            g.fillAll (Colours::darkgrey.withAlpha(0.75f));
            
            g.setColour (Colour::fromRGB(50, 50, 50).withAlpha(0.75f));
            
            if (isActive)
                g.setColour (Colours::aquamarine.withAlpha(0.75f));
                
            if (isPlaying)
            {
                if (isActive)
                    g.setColour (Colours::gold.withAlpha(0.75f));
                else
                    g.setColour (Colours::gold.darker().withAlpha(0.25f));
            }
                
            if (isMouseButtonDown())
                g.setColour(Colours::white.withAlpha(0.75f));
            
            g.fillRect(0, round((1.0 - value) * getHeight()), getWidth(), getHeight());
                
            g.setColour (Colours::darkgrey);
            
            g.drawLine(getWidth(), 0, getWidth(), getHeight());
            g.drawLine(0, 0, getWidth(), 0);
        }
        
        void setPlayingState (bool state)
        {
            isPlaying = state;
            
            repaint();
        }
        
        ValueTree getValueTree ()
        {
            ValueTree data ("TILE");
            
            data.setProperty ("active", isActive, nullptr);
            data.setProperty ("value", value, nullptr);
            
            return data;
        }
        
        float getValue ()
        {
            return isActive? value : 0.0;
        }
        
        void setValue (const float val) { value = val; repaint(); }
        
        void setActive (bool state) { isActive = state; repaint(); }
        
    private:
        
        const float DEFAULT_VALUE = 1.0;
        
        bool isActive, isDragging, isPlaying;
        
        float value;
        
        void mouseEnter (const MouseEvent& e) override
        {
            setMouseCursor (MouseCursor::PointingHandCursor);
        }
        
        void mouseDown (const MouseEvent& e) override
        {
            repaint();
        }
        
        void mouseDrag (const MouseEvent& e) override
        {
            if (e.getDistanceFromDragStart() > 0)
            {
                setMouseCursor (MouseCursor::DraggingHandCursor);
                
                isDragging = true;
                
                value = 1.0 - ( (float) e.getPosition().y / (float) getHeight() );
                
                if (value > 1.0)
                    value = 1.0;
                else if (value < 0.0)
                    value = 0.0;
                
                repaint();
            }
        }
        
        void mouseUp (const MouseEvent& e) override
        {
            if (! isDragging)
                isActive = ! isActive;
            
            isDragging = false;
            
            if (isMouseOver())
                setMouseCursor (MouseCursor::PointingHandCursor);
            else
                setMouseCursor (MouseCursor::NormalCursor);
            
            repaint();
        }
        
        void mouseExit (const MouseEvent&) override
        {
            if (! isDragging)
                setMouseCursor (MouseCursor::NormalCursor);
        }
        
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (StepTileComponent)
        
    };
    
    // ======================================================
    
    const int DEFAULT_STEPSIZE = 8;

    const EditorComponent::Type type = EditorComponent::Type::stepsizeEditor;
    
    OwnedArray<StepTileComponent> tiles;
    
    int stepsize;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (StepEditor)
    
};

#endif  // STEPEDITOR_H_INCLUDED
