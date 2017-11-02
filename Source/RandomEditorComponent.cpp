#include "RandomEditorComponent.h"

RandomEditorComponent::RandomEditorComponent() : randomizer()
{
    isInTransport = false;
    currentValue = 0.0;
    randomness = 1.0;
}

RandomEditorComponent::~RandomEditorComponent()
{
    buffer.clear();
}

// ==============================================================

void RandomEditorComponent::paint(juce::Graphics &g)
{
    g.fillAll( Colour::fromRGBA( 25, 25, 25, 200 ) );
    
    for (unsigned i = 0; i < buffer.size(); i++)
    {
        if (isInTransport)
            g.setColour(Colour(Colours::gold).withBrightness(buffer[i].value));
        else
            g.setColour(Colour(Colours::aqua).withBrightness(buffer[i].value));
        
        g.drawLine(Line<float>(buffer[i].position, getHeight(), buffer[i].position, ( 1.0 - buffer[i].value ) * getHeight()));
    }
}

// ==============================================================

void RandomEditorComponent::getEditableProperties(Array<juce::PropertyComponent *> &props)
{
    EditorComponent::getEditableProperties (props);
    
    props.add ( new RandomnessPropertyComponent ( "Randomness", this ) );
}

// ==============================================================

ValueTree RandomEditorComponent::getForDocument ()
{
    ValueTree data = EditorComponent::getForDocument();
    
    data.setProperty ("randomness", getRandomness(), nullptr);
    
    return data;
}

void RandomEditorComponent::loadFromDocument (ValueTree data)
{
    setRandomness (data.getProperty("randomness"));
    
    repaint();
}

// ==============================================================

const float RandomEditorComponent::getData(const float position)
{
    isInTransport = true;
    
    if (randomizer.nextDouble() <= getRandomness())
    {
        currentValue = randomizer.nextFloat();
    }
    
    currentPosition = position;
    
    if (buffer.size() > BUFFER_SIZE)
        buffer.clear();
    
    BufferData data;
    
    data.position = currentPosition;
    data.value = currentValue;
    
    buffer.add(data);
    
    repaint();
    
    return currentValue;
}

void RandomEditorComponent::transportEnter()
{
    buffer.clear();
    
    repaint();
}

void RandomEditorComponent::transportExit()
{
    isInTransport = false;
    
    repaint();
}