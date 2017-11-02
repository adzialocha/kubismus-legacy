#ifndef SCANNERCOMPONENT_H_INCLUDED
#define SCANNERCOMPONENT_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

#include "HorizontalComponentBoundsConstrainer.h"

class ScannerComponent : public Component,
                         public ChangeBroadcaster
{
    
public:
    
    ScannerComponent()
    {
        position = 0;
        startPos = 0;
        
        isMouseOver = false;
    };
    
    ~ScannerComponent() {};
    
    const int getPosition()
    {
        return position;
    }
    
    bool isSelected()
    {
        return isMouseOver;
    }
    
    void setPosition(const int pos, bool withoutStart)
    {
        position = withoutStart ? pos : startPos + pos;
    }
    
    void setStartPosition(const int pos)
    {
        startPos = pos;
    }
    
    void paint(juce::Graphics &g) override
    {
        if (isMouseOver)
        {
            g.setColour (Colours::silver);
        }
        else
        {
            g.setColour (Colours::gold.withAlpha(.5f));
        }
        
        g.drawLine( 0, 0, 0, getHeight() );
    }
    
    void resized() override
    {
        resizableEdgeConstrainer.setMinimumOnscreenAmounts (getHeight(), getWidth(), getHeight(), getWidth());
        resizableEdgeConstrainer.setMaximumHeight (getHeight());
    }
    
    void mouseDrag (const MouseEvent& e) override
    {
        int x;
        
        x = e.getEventRelativeTo(getParentComponent()).getPosition().x;
        
        if (x > 0) {
            position = startPos + x;
        }
        
        dragger.dragComponent (this, e, &resizableEdgeConstrainer);
        
        sendChangeMessage();
    }
    
    void mouseEnter (const MouseEvent& e) override
    {
        setMouseCursor (MouseCursor::DraggingHandCursor);
        
        isMouseOver = true;
        
        repaint();
    }
    
    void mouseExit (const MouseEvent& e) override
    {
        setMouseCursor (MouseCursor::NormalCursor);
        
        isMouseOver = false;

        repaint();
    }
    
protected:
    
    bool isMouseOver;
    
private:
    
    int position;
    int startPos;

    ComponentDragger dragger;
    
    HorizontalComponentBoundsConstrainer resizableEdgeConstrainer;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ScannerComponent)
    
};

#endif  // SCANNERCOMPONENT_H_INCLUDED
