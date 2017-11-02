#include "StepEditor.h"

StepEditor::StepEditor()
{
    stepsize = 0;

    setStepSize (DEFAULT_STEPSIZE);
}

StepEditor::~StepEditor()
{
    tiles.clear();
    
    removeAllChildren();
}

// ==============================================================

void StepEditor::paint(juce::Graphics &g)
{

}

void StepEditor::resized()
{
    int matrixSize, tileWidth, tileHeight, tileYPos;
    
    tileYPos = 0;
    
    matrixSize = (int) ceil ( sqrt (stepsize) );
    
    tileWidth = ceil((float) getWidth() / matrixSize);
    tileHeight = ceil((float) getHeight() / matrixSize);
    
    for (unsigned i = 0; i < tiles.size(); i++)
    {
        if (i > 0 && i % matrixSize == 0)
            tileYPos = tileYPos + tileHeight;
        
        tiles[i]->setBounds(
            (i % matrixSize) * tileWidth,
            tileYPos,
            tileWidth,
            tileHeight
        );
    }
}

// ==============================================================

void StepEditor::getEditableProperties(Array<juce::PropertyComponent *> &props)
{
    EditorComponent::getEditableProperties (props);
    
    props.add ( new StepSizeComponent ( "Size", this ) );
}

// ==============================================================

ValueTree StepEditor::getForDocument ()
{
    ValueTree data = EditorComponent::getForDocument();
    
    data.setProperty ("size", getStepSize(), nullptr);
    
    for (unsigned i = 0; i < tiles.size(); i++)
        data.addChild (tiles[i]->getValueTree(), i, nullptr);
    
    return data;
}

void StepEditor::loadFromDocument (ValueTree data)
{
    setStepSize (data.getProperty("size"));
    
    for (unsigned i = 0; i < data.getNumChildren(); i++)
    {
        if (data.getChild(i).hasType("TILE"))
        {
            tiles[i]->setActive (data.getChild(i).getProperty("active"));
            tiles[i]->setValue (data.getChild(i).getProperty("value"));
        }
    }
    
    repaint();
}

// ==============================================================

const float StepEditor::getData(const float position)
{
    int tileIndex;
    
    tileIndex = floor( tiles.size() * (position / (float) getWidth()) );
    
    if (tiles.size() > tileIndex)
    {
        for (unsigned i = 0; i < tiles.size(); i++)
            tiles[i]->setPlayingState (false);
        
        tiles[tileIndex]->setPlayingState (true);
        
        return tiles[tileIndex]->getValue ();
    }
    
    return 0;
}

void StepEditor::transportExit()
{
    for (unsigned i = 0; i < tiles.size(); i++)
        tiles[i]->setPlayingState (false);
}