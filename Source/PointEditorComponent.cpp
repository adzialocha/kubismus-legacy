#include "PointEditorComponent.h"

PointEditorComponent::PointEditorComponent()
{
}

PointEditorComponent::~PointEditorComponent()
{
}

// ==============================================================

void PointEditorComponent::paint (Graphics& g)
{
    g.fillAll( Colour::fromRGBA( 0, 0, 0, 200 ) );
    
    PointEditor::paint(g);
}

// ==============================================================

void PointEditorComponent::getEditableProperties (Array<juce::PropertyComponent *> &props)
{
    EditorComponent::getEditableProperties (props);
}

// ==============================================================

ValueTree PointEditorComponent::getForDocument ()
{
    ValueTree data = EditorComponent::getForDocument();
    
    vector<PointEditor::point> points = getPoints();
    
    for (unsigned i = 0; i < points.size(); i++)
    {
        ValueTree point ("POINT");
        
        point.setProperty ("x", points[i].x, nullptr);
        point.setProperty ("y", points[i].y, nullptr);
        
        data.addChild (point, i, nullptr);
    }
    
    return data;
}

void PointEditorComponent::loadFromDocument (ValueTree data)
{
    for (unsigned i = 0; i < data.getNumChildren(); i++)
        addPoint (data.getChild(i).getProperty("x"), data.getChild(i).getProperty("y"));
    
    sortPoints();
    
    repaint();
}

// ==============================================================

const float PointEditorComponent::getData (const float position)
{
    float result;
    result = getValueAt (position);
    
    repaint();
    
    return 1.0 - ( (float) result / (float) getHeight() );
}

void PointEditorComponent::transportEnter()
{
    setPaintTransport (true);
}

void PointEditorComponent::transportExit()
{
    setPaintTransport (false);
}