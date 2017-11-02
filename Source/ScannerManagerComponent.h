#ifndef SCANNERMANAGERCOMPONENT_H_INCLUDED
#define SCANNERMANAGERCOMPONENT_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

#include "ScannerComponent.h"
#include "CueComponent.h"

class ScannerManagerComponent : public Component,
                                public ChangeBroadcaster,
                                private KeyListener,
                                private ChangeListener
{
    
public:
    
    ScannerManagerComponent();
    ~ScannerManagerComponent();

    void resized() override;
    
    void setViewportArea (const Rectangle< int > &newVisibleArea);
    void setNewPosition (const int pos, bool createNewCue, bool silent, bool withoutStart);
    
    void reset ()
    {
        for (unsigned i = 0; i < cues.size(); i++)
            removeCue(i);
    }
    
    void ignoreNextCue ()
    {
        bool rememberState;
        
        findNextCue();
        
        if (nextCueIndex != -1)
            rememberState = cues[nextCueIndex]->getBlockedState();
        
        for (unsigned i = 0; i < cues.size(); i++)
            cues[i]->setBlockedState(false);
        
        if (nextCueIndex != -1)
            cues[nextCueIndex]->setBlockedState(! rememberState);
        
        repaint();
    }
    
    const int getScannerPosition()
    {
        return scanner.getPosition();
    }
    
    const int getFrom()
    {
        return fromPosition;
    }
    
    const int getTo()
    {
        return toPosition;
    }
    
    // project
    
    ValueTree getForDocument ();
    void loadFromDocument (ValueTree data);
    
private:
    
    int startPos;
    
    int fromPosition, toPosition;

    int nextCueIndex;

    ScannerComponent scanner;
    OwnedArray<CueComponent> cues;
    
    void addCue(const int pos)
    {
        CueComponent* newCue;
        
        newCue = new CueComponent();
        newCue->setStartPosition (startPos);
        newCue->setPosition (pos, false);
        newCue->addChangeListener (this);
        
        addAndMakeVisible(newCue);
        
        newCue->setBounds (newCue->getPosition() - startPos, 0, 6, getHeight());
        
        cues.add(newCue);
        sortCues();
        
        findNextCue();
    }
    
    void removeCue (const int pos)
    {
        cues[pos]->removeChangeListener (this);
        removeChildComponent (cues[pos]);
        cues.remove (pos);
        
        findNextCue();
    }
    
    void sortCues ()
    {
        for (unsigned i = 0; i < cues.size(); i++)
            for (unsigned j = 0; j < cues.size(); j++)
                if (cues[j]->getPosition() > cues[i]->getPosition())
                    cues.swap(j, i);
    }
    
    void checkCues ()
    {
        for (unsigned i = 0; i < cues.size(); i++)
        {
            if (nextCueIndex > -1 && cues[nextCueIndex]->getPosition() < scanner.getPosition())
            {
                if (cues[nextCueIndex]->getBlockedState())
                {
                    cues[nextCueIndex]->setBlockedState (false);
                    
                    findNextCue ();
                }
                else
                {
                    scanner.setPosition (fromPosition, true);

                    sendChangeMessage ();
                }
            }
        }
    }
    
    void findNextCue()
    {
        fromPosition = 0;
        toPosition = -1;
        nextCueIndex = -1;
        
        for (unsigned i = 0; i < cues.size(); i++)
        {
            if (cues[i]->getPosition() < scanner.getPosition())
                nextCueIndex++;
        }
        
        if (nextCueIndex > -1) // somewhere between cues
            nextCueIndex++;
        else if (cues.size() > 0) // at the beginning
            nextCueIndex = 0;
        
        // at the end
        
        if (nextCueIndex == cues.size())
            nextCueIndex = -1;
        
        // calculate positions
        
        if (nextCueIndex > 0)
            fromPosition = cues[nextCueIndex - 1]->getPosition();
        
        if (nextCueIndex > -1)
            toPosition = cues[nextCueIndex]->getPosition();
    }
    
    // KeyListener
    
    bool keyPressed ( const KeyPress& key, Component* originatingComponent ) override;
    
    // ChangeListener
    
    void changeListenerCallback (ChangeBroadcaster *source) override;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ScannerManagerComponent)
    
};

#endif  // SCANNERMANAGERCOMPONENT_H_INCLUDED
