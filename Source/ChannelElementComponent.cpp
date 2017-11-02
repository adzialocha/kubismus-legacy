#include "ChannelElementComponent.h"

ChannelElementComponent::ChannelElementComponent()
{
    setName (DEFAULT_TITLE);
    
    editor = nullptr;
    
    lastValue = 0.0f;
    
    isSelected = false;
    hasStartedDragging = false;
    
    resizableEdgeConstrainer.setMinimumWidth (5);
    
    addAndMakeVisible ( resizableEdge = new ResizableEdgeComponent ( this, &resizableEdgeConstrainer,
                                                                    ResizableEdgeComponent::rightEdge ) );
}

ChannelElementComponent::~ChannelElementComponent()
{
    removeAllChildren();
}

void ChannelElementComponent::paint (Graphics& g)
{
    g.setColour(Colours::darkgrey);
    g.fillRect ( 0, 0, getWidth(), HEADER_HEIGHT );
    
    if (isSelected)
    {
        g.setColour ( Colour::fromRGBA( 255, 255, 255, 50 ) );
        g.fillRect ( 0, 0, getWidth(), HEADER_HEIGHT );
    }
    
    g.setColour( Colours::white );
    g.drawText( getName(), 5, 0, getWidth() - 10, HEADER_HEIGHT, Justification::centredLeft );
    
    if (getVirtualTime() < 1.0)
    {
        g.setColour(Colours::aqua);
        g.fillRect ( 0, 0, getWidth(), 2 );
        
        g.setColour(Colours::cadetblue);
        g.fillRect ( 0, 0, getWidth() * getVirtualTime(), 2 );
    }
}

void ChannelElementComponent::resized ()
{
    resizableEdgeConstrainer.setMinimumOnscreenAmounts (getHeight(), getWidth(), getHeight(), getWidth());
    resizableEdgeConstrainer.setMaximumHeight (getHeight());
    
    resizableEdge->setBounds( getWidth() - HEADER_HEIGHT, 0, HEADER_HEIGHT, HEADER_HEIGHT );
    
    if (editor != nullptr)
        editor->setBounds( 0, HEADER_HEIGHT, getWidth(), getHeight() - HEADER_HEIGHT );
    
    sendChangeMessage();
}

// properties

void ChannelElementComponent::getEditableProperties (Array<juce::PropertyComponent *> &props)
{
    props.add ( new ElementProperties ( "name", this ) );
    
    props.add ( new ElementOSCProperties ( "oscAddress", this ) );
    // props.add ( new ElementOSCProperties ( "oscChannel", this ) );
    props.add ( new ElementOSCProperties ( "oscMin", this ) );
    props.add ( new ElementOSCProperties ( "oscMax", this ) );
    
    props.add (new ElementTimeProperties ( "time", this ) );
}

// project

ValueTree ChannelElementComponent::getForDocument ()
{
    ValueTree data ("ELEMENT");
    
    data.setProperty ("x", getX(), nullptr);
    data.setProperty ("width", getWidth(), nullptr);
 
    data.setProperty ("name", getName(), nullptr);
    data.setProperty ("time", getVirtualTime(), nullptr);
    
    data.setProperty ("oscAddress", getOSCAddress(), nullptr);
    data.setProperty ("oscMin", getOSCMinValue(), nullptr);
    data.setProperty ("oscMax", getOSCMaxValue(), nullptr);
    
    data.setProperty ("type", editor->getType(), nullptr);
    
    data.addChild (editor->getForDocument(), 0, nullptr);
    
    return data;
}

void ChannelElementComponent::loadFromDocument (ValueTree data)
{
    setName (data.getProperty("name"));
    setVirtualTime (data.getProperty("time"));
    
    setOSCAddress (data.getProperty("oscAddress"));
    setOSCMinValue (data.getProperty("oscMin"));
    setOSCMaxValue (data.getProperty("oscMax"));
    
    if (data.getNumChildren() > 0 && data.getChild(0).hasType("EDITOR"))
        editor->loadFromDocument (data.getChild(0));
}

// channel listeners handling

void ChannelElementComponent::addElementListener (ElementListener* const newListener)
{
    elementListeners.add (newListener);
}

void ChannelElementComponent::removeElementListener (ElementListener* const listenerToRemove)
{
    elementListeners.remove (listenerToRemove);
}

void ChannelElementComponent::broadcastDrop ()
{
    elementListeners.call ( &ElementListener::elementDropped, *this );
}

void ChannelElementComponent::setEditor( EditorComponent::Type type )
{
    if (editor != nullptr)
        return;
    
    if (type == EditorComponent::Type::pointEditor)
        editor = new PointEditorComponent();
    else if (type == EditorComponent::Type::randomEditor)
        editor = new RandomEditorComponent();
    else if (type == EditorComponent::Type::binaryStretchEditor)
        editor = new BinaryStretchComponent();
    else if (type == EditorComponent::Type::stepsizeEditor)
        editor = new StepEditor();
    
    addAndMakeVisible(editor);
}

void ChannelElementComponent::mouseDown (const MouseEvent& e)
{
    elementListeners.call ( &ElementListener::elementSelected, *this );
    
    dragger.startDraggingComponent (this, e);
}

void ChannelElementComponent::mouseDrag (const MouseEvent& e)
{
    const int NEEDED_DISTANCE_FOR_DRAGDROP = 50;
    
    if (! hasStartedDragging)
    {
        if ( abs( e.getDistanceFromDragStartY() ) > NEEDED_DISTANCE_FOR_DRAGDROP )
        {
            DragAndDropContainer* dragContainer = DragAndDropContainer::findParentDragContainerFor(this);
            
            dragContainer->startDragging( "", this );

            hasStartedDragging = true;
        }
        else
        {
            dragger.dragComponent (this, e, &resizableEdgeConstrainer);
        }
        
        sendChangeMessage();
    }
}

void ChannelElementComponent::mouseUp (const MouseEvent& e)
{
    hasStartedDragging = false;
}

void ChannelElementComponent::setSelectionState (bool state)
{
    isSelected = state;
    repaint();
}

// transport

DataInterface::Package ChannelElementComponent::getData (const int from, const int to, const float position)
{
    int virtualPosition;
    
    if (getVirtualTime() < 1.0)
    {
        int virtualFrom, virtualTo, timeframe;
        double virtualWidth, virtualPercentage;
        
        if (from <= getX())
            virtualFrom = 0;
        else
            virtualFrom = from - getX();
        
        if (to == -1 || to >= getX() + getWidth())
            virtualTo = getWidth();
        else
            virtualTo = to - getX();
        
        timeframe = virtualTo - virtualFrom;
        
        virtualWidth = (double) timeframe * getVirtualTime();
        
        if (virtualWidth < 1)
            virtualWidth = 1;
        
        virtualPercentage = (double) ((int) (position - getX()) % (int) virtualWidth) / virtualWidth;
        virtualPosition = virtualFrom + ( (float) timeframe * virtualPercentage );
     
//        std::cout << position << " " << virtualFrom << " - " << virtualTo << " // " << virtualPosition << " // " << virtualWidth << " (" << virtualPercentage << ")" << std::endl;
    }
    else
    {
        virtualPosition = position - getX();
    }
    
    DataInterface::Package package;
    
    package.address = getOSCAddress();
    package.channel = getOSCChannel();
    package.value = getOSCMinValue() + (editor->getData(virtualPosition) * ( getOSCMaxValue() - getOSCMinValue() ) );
    package.dirty = lastValue != package.value;
    
    lastValue = package.value;

    return package;
}