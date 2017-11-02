#include "BinaryStretchComponent.h"

BinaryStretchComponent::BinaryStretchComponent()
{
    distance = NEXT_SIGNAL_MAX;
    
    isActivated = false;
    isPaintingActivity = false;
    
    addChangeListener (this);
    setPaintTransport (false);
}

BinaryStretchComponent::~BinaryStretchComponent()
{
    data.clear();
    
    removeChangeListener (this);
}

// ==============================================================

void BinaryStretchComponent::paint(juce::Graphics &g)
{
    g.fillAll (Colour::fromRGBA( 0, 0, 0, 200 ));
    
    if (isPaintingActivity && isActivated)
        g.fillAll (Colours::gold.withAlpha(0.25f));
    
    g.setColour (Colours::aliceblue);
    
    for (unsigned i = 0; i < data.size(); i++)
        if (data[i] == 1)
            g.drawLine(i, 0, i, getHeight());
    
    PointEditor::paint(g);
}

void BinaryStretchComponent::resized ()
{
    calculateData();
    repaint();
}

// ==============================================================

void BinaryStretchComponent::getEditableProperties(Array<juce::PropertyComponent *> &props)
{
    EditorComponent::getEditableProperties (props);
    
    props.add ( new DistancePropertyComponent ( "Distance", this ) );
}

// ==============================================================

ValueTree BinaryStretchComponent::getForDocument ()
{
    ValueTree data = EditorComponent::getForDocument();
    
    data.setProperty ("distance", getMaxDistance(), nullptr);
    
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

void BinaryStretchComponent::loadFromDocument (ValueTree data)
{
    for (unsigned i = 0; i < data.getNumChildren(); i++)
        addPoint (data.getChild(i).getProperty("x"), data.getChild(i).getProperty("y"));
    
    setMaxDistance (data.getProperty("distance"));
    
    sortPoints();
    
    repaint();
}

// ==============================================================

const float BinaryStretchComponent::getData(const float position)
{
    int rounded = round (position);
    
    if (data.size() > rounded)
    {
        isActivated = data[round(position)] == 1;
        return data[round(position)];
    }
    else
    {
        return 0;
    }
}

void BinaryStretchComponent::transportEnter()
{
    isPaintingActivity = true;
}

void BinaryStretchComponent::transportExit ()
{
    isPaintingActivity = false;
}
