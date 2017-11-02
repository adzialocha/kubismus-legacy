#ifndef CHANNELCOMPONENT_H_INCLUDED
#define CHANNELCOMPONENT_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

#include "ChannelElementComponent.h"
#include "ChannelListener.h"
#include "EditableProperties.h"
#include "ChannelElementProperties.h"
#include "DataInterface.h"

class ChannelComponent    : public Component,
                            public DragAndDropTarget,
                            public EditableProperties,
                            private ElementListener
{
    
public:
    
    ChannelComponent();
    ~ChannelComponent();

    void paint (Graphics&) override;
    void resized() override;
    
    void addChannelListener (ChannelListener* newListener);
    void removeChannelListener (ChannelListener* listenerToRemove);
    
    void addChannelElement(EditorComponent::Type type, const int pos, const int width);
    void removeChannelElement( ChannelElementComponent &element );
    
    // ElementListener
    
    void elementDropped ( ChannelElementComponent &element ) override;
    void elementSelected ( ChannelElementComponent &element ) override;
    
    // DragAndDropTarget
    
    bool isInterestedInDragSource (const SourceDetails&) override;
    void itemDragEnter (const SourceDetails&) override;
    void itemDragMove (const SourceDetails&) override;
    void itemDragExit (const SourceDetails&) override;
    void itemDropped (const SourceDetails& dragSourceDetails) override;
    
    // EditableProperties
    
    void getEditableProperties(Array<PropertyComponent*>& props) override;
    
    // selection state
    
    void setSelectionState (bool state);
    bool getSelectionState () { return isSelected; }
    
    ChannelElementComponent* selectedElement;
    
    // transport
    
    DataInterface::Package getData (const int from, const int to, const float position);
    
    // project
    
    ValueTree getForDocument ();
    void loadFromDocument (ValueTree data);
    
private:
    
    ChannelElementComponent* currentPlayedElement;
    
    ListenerList<ChannelListener> channelListeners;

    OwnedArray<ChannelElementComponent> elements;
    
    Rectangle<int> dragGhost;
    
    void updateDragGhost (const SourceDetails& dragSourceDetails)
    {
        ChannelElementComponent* original = dynamic_cast<ChannelElementComponent*> (dragSourceDetails.sourceComponent.get());
        
        dragGhost.setPosition(dragSourceDetails.localPosition.getX(), 0);
        dragGhost.setSize(original->getWidth(), original->getHeight());
    }
    
    bool isDragOver;
    bool isSelected;
    
    void mouseDown (const MouseEvent&) override;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ChannelComponent)
    
};

#endif