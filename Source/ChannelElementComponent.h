#ifndef CHANNELELEMENTCOMPONENT_H_INCLUDED
#define CHANNELELEMENTCOMPONENT_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

#include "EditorComponent.h"

#include "PointEditorComponent.h"
#include "RandomEditorComponent.h"
#include "BinaryStretchComponent.h"
#include "StepEditor.h"

#include "ElementListener.h"
#include "HorizontalComponentBoundsConstrainer.h"
#include "EditableProperties.h"
#include "DataInterface.h"
#include "ElementProperties.h"
#include "ElementOSCProperties.h"
#include "OSCProperties.h"
#include "VirtualTimeProperties.h"
#include "ElementTimeProperties.h"

class ChannelElementComponent    : public Component,
                                   public ChangeBroadcaster,
                                   public EditableProperties,
                                   public OSCProperties,
                                   public VirtualTimeProperties
{
    
public:
    
    ChannelElementComponent();
    ~ChannelElementComponent();
    
    void paint (Graphics&) override;
    void resized() override;
    
    void addElementListener (ElementListener* newListener);
    void removeElementListener (ElementListener* listenerToRemove);
    
    void broadcastDrop ();
    
    void setEditor (EditorComponent::Type e);
    ScopedPointer<EditorComponent> editor;
    
    // transport
    
    DataInterface::Package getData (const int from, const int to, const float position);

    // selection
    
    void setSelectionState (bool state);
    bool getSelectionState () { return isSelected; }
    
    // project
    
    ValueTree getForDocument ();
    void loadFromDocument (ValueTree data);
    
    // EditableProperties
    
    void getEditableProperties(Array<juce::PropertyComponent *> &props) override;
    
private:
    
    const int HEADER_HEIGHT = 25;
    const String DEFAULT_TITLE = "UNTITLED";
    
    float lastValue;
    
    ListenerList<ElementListener> elementListeners;
    
    ComponentDragger dragger;
    bool hasStartedDragging;
    
    ScopedPointer<ResizableEdgeComponent> resizableEdge;
    HorizontalComponentBoundsConstrainer resizableEdgeConstrainer;
    
    void mouseDown (const MouseEvent&) override;
    void mouseDrag (const MouseEvent&) override;
    void mouseUp (const MouseEvent&) override;
    
    bool isSelected;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ChannelElementComponent)
    
};

#endif  // CHANNELELEMENTCOMPONENT_H_INCLUDED
