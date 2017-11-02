#include "ScannerManagerComponent.h"

ScannerManagerComponent::ScannerManagerComponent()
{
    startPos = 0;
    nextCueIndex = -1;

    fromPosition = 0;
    toPosition = -1;

    setInterceptsMouseClicks (false, true);
    setWantsKeyboardFocus(true);
    
    scanner.addChangeListener (this);
    
    addAndMakeVisible (scanner);
    
    addKeyListener (this);
}

ScannerManagerComponent::~ScannerManagerComponent()
{
    for (unsigned i = 0; i < cues.size(); i++)
        cues[i]->removeChangeListener (this);
    
    scanner.removeChangeListener (this);
    
    cues.clear ();
    
    removeAllChildren ();
    removeKeyListener (this);
}

void ScannerManagerComponent::resized()
{
    scanner.setSize (6, getHeight());
    
    for (unsigned i = 0; i < cues.size(); i++)
        cues[i]->setSize (6, getHeight());
}

void ScannerManagerComponent::setViewportArea (const Rectangle< int > &newVisibleArea)
{
    if (startPos != newVisibleArea.getX())
    {
        startPos = newVisibleArea.getX();

        scanner.setStartPosition (startPos);
        scanner.setTopLeftPosition (scanner.getPosition() - startPos, 0);
        
        for (unsigned i = 0; i < cues.size(); i++)
        {
            cues[i]->setStartPosition (startPos);
            cues[i]->setTopLeftPosition (cues[i]->getPosition() - startPos, 0);
        }
        
        repaint();
    }
}

void ScannerManagerComponent::setNewPosition (const int pos, bool createNewCue, bool silent, bool withoutStart)
{
    if (createNewCue)
    {
        addCue (pos);
    }
    else
    {
        scanner.setPosition (pos, withoutStart);
        scanner.setTopLeftPosition (scanner.getPosition() - startPos, 0);
        
        if (! silent) {
            findNextCue ();
            sendChangeMessage();
        }
        else
        {
            checkCues ();
        }
    }
    
    repaint();
}

bool ScannerManagerComponent::keyPressed (const KeyPress &k, Component* originatingComponent)
{
    if (k.getKeyCode() == k.backspaceKey)
    {
        for (unsigned i = 0; i < cues.size(); i++)
        {
            if (cues[i]->isSelected())
            {
                removeCue (i);
                repaint ();
                
                return true;
            }
        }
        
        return false;
    }

    return false;
}

void ScannerManagerComponent::changeListenerCallback (ChangeBroadcaster *source)
{
    if (source == &scanner)
    {
        sendChangeMessage ();
    }
    else
    {
        sortCues ();
    }
    
    findNextCue ();
}

// ============================================================

ValueTree ScannerManagerComponent::getForDocument()
{
    ValueTree data ("SCANNERS");
    
    for (unsigned i = 0; i < cues.size(); i++)
    {
        ValueTree cue ("CUE");
        cue.setProperty ("x", cues[i]->getPosition(), nullptr);
        
        data.addChild (cue, i, nullptr);
    }
    
    return data;
}

void ScannerManagerComponent::loadFromDocument (ValueTree data)
{
    for (unsigned i = 0; i < data.getNumChildren(); i++)
        if (data.getChild(i).hasType("CUE"))
            addCue (data.getChild(i).getProperty("x"));
}