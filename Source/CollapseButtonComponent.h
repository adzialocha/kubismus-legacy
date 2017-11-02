#ifndef COLLAPSEBUTTONCOMPONENT_H_INCLUDED
#define COLLAPSEBUTTONCOMPONENT_H_INCLUDED

class CollapseButtonComponent : public Button
{
    
public:
    
    CollapseButtonComponent (const String &title) : Button(title)
    {
        setClickingTogglesState (true);
    }
    
    ~CollapseButtonComponent ()
    {
        
    }
    
    void paintButton (Graphics &g, bool isMouseOverButton, bool isButtonDown) override
    {
        g.fillAll (Colour::fromRGB(40, 40, 40));
        
        const float buttonSize = getHeight() * 0.75f;
        const float buttonIndent = (getHeight() - buttonSize) * 0.5f;
        
        Rectangle<float> area (buttonIndent, buttonIndent, buttonSize, buttonSize);
                                  
        Path p;
        
        p.addTriangle (0.0f, 0.0f, 1.0f, getToggleState() ? 0.0f : 0.5f, getToggleState() ? 0.5f : 0.0f, 1.0f);
        
        if (isMouseOverButton)
            g.setColour (Colours::white.darker());
        else
            g.setColour (Colours::white);
            
        g.fillPath (p, p.getTransformToScaleToFit (area.reduced (2, area.getHeight() / 3), true));
    }
    
private:
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CollapseButtonComponent)
    
};

#endif  // COLLAPSEBUTTONCOMPONENT_H_INCLUDED
