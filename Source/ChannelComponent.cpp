#include "ChannelComponent.h"

ChannelComponent::ChannelComponent()
{
    isDragOver = false;
    isSelected = false;
    
    selectedElement = nullptr;
    currentPlayedElement = nullptr;
}

ChannelComponent::~ChannelComponent()
{
    elements.clear();
    
    removeAllChildren();
}

// channel listeners handling

void ChannelComponent::addChannelListener (ChannelListener* const newListener)
{
    channelListeners.add (newListener);
}

void ChannelComponent::removeChannelListener (ChannelListener* const listenerToRemove)
{
    channelListeners.remove (listenerToRemove);
}

// paint and layout

void ChannelComponent::paint (Graphics& g)
{
    if (isSelected)
    {
        g.fillAll ( Colour::fromRGBA(255, 255, 255, 50) );
    }
    
    if (isDragOver)
    {
        g.setColour ( Colour::fromRGBA(255, 255, 255, 100) );
        g.fillRect (dragGhost);
    }
    
    g.setColour ( Colours::white );
    g.drawLine ( 0, getHeight(), getWidth(), getHeight() );
}

void ChannelComponent::resized()
{
    setBounds(getLocalBounds());
}

// properties

void ChannelComponent::getEditableProperties(Array<juce::PropertyComponent *> &props)
{
    props.add ( new ChannelElementProperties ( "x", selectedElement ) );
    props.add ( new ChannelElementProperties ( "width", selectedElement ) );
}

// project

ValueTree ChannelComponent::getForDocument ()
{
    ValueTree data ("CHANNEL");
    
    for (unsigned i = 0; i < elements.size(); i++)
        data.addChild (elements[i]->getForDocument(), 0, nullptr);
    
    return data;
}

void ChannelComponent::loadFromDocument (ValueTree data)
{
    for (unsigned i = 0; i < data.getNumChildren(); i++)
    {
        if (data.getChild(i).hasType("ELEMENT"))
        {
            ValueTree elem = data.getChild(i);
            
            addChannelElement (
                           EditorComponent::findTypeFromDocument((int) elem.getProperty("type")),
                           (int) elem.getProperty("x"),
                           (int) elem.getProperty("width") );
            
            elements[i]->loadFromDocument (data.getChild(i));
        }
        
    }
}

// selection state

void ChannelComponent::setSelectionState (bool state)
{
    isSelected = state;
    repaint();
}

void ChannelComponent::addChannelElement ( EditorComponent::Type type, const int pos, const int width )
{
    ChannelElementComponent* newElement;
    
    newElement = new ChannelElementComponent();
    newElement->setEditor(type);
    newElement->addElementListener(this);
    
    elements.add(newElement);
    
    addAndMakeVisible(newElement);
    
    newElement->setBounds(pos, 0, width, getHeight());
}

void ChannelComponent::removeChannelElement(ChannelElementComponent &element)
{
    element.removeElementListener(this);
    
    if (currentPlayedElement == &element)
        currentPlayedElement = nullptr;
    
    elements.removeObject (&element, true);
    
    // inform about this removal
    
    channelListeners.call( &ChannelListener::channelElementRemoved, *this );
}

bool ChannelComponent::isInterestedInDragSource(const SourceDetails& dragSourceDetails)
{
    ChannelElementComponent* original = dynamic_cast<ChannelElementComponent*> (dragSourceDetails.sourceComponent.get());
    
    return original->getParentComponent() != this;
}

void ChannelComponent::itemDragEnter (const SourceDetails& dragSourceDetails)
{
    isDragOver = true;
    updateDragGhost (dragSourceDetails);
    repaint();
}

void ChannelComponent::itemDragExit (const SourceDetails&)
{
    isDragOver = false;
    repaint();
}

void ChannelComponent::itemDragMove (const SourceDetails& dragSourceDetails)
{
    updateDragGhost (dragSourceDetails);

    if (selectedElement != nullptr)
        selectedElement->sendChangeMessage();
    
    repaint();
}

void ChannelComponent::itemDropped (const SourceDetails& dragSourceDetails)
{
    // get dropped element
    
    ChannelElementComponent* newElement = dynamic_cast<ChannelElementComponent*> (dragSourceDetails.sourceComponent.get());
    
    // inform channel of the dropped element
    
    selectedElement = newElement;
    
    newElement->broadcastDrop();

    // add element to this channel
    
    newElement->addElementListener(this);
    elements.add(newElement);
    addAndMakeVisible(newElement);
  
    newElement->setBounds( dragSourceDetails.localPosition.getX(), 0, newElement->getWidth(), getHeight() );
    
    // select this element
    
    channelListeners.call( &ChannelListener::channelElementSelected, *this, *newElement );
    
    // repaint

    isDragOver = false;
    repaint();
}

void ChannelComponent::elementDropped (ChannelElementComponent &element)
{
    element.removeElementListener (this);
    
    elements.removeObject (&element, false);
}

void ChannelComponent::elementSelected (ChannelElementComponent &element)
{
    selectedElement = &element;
    channelListeners.call( &ChannelListener::channelElementSelected, *this, element );
}

void ChannelComponent::mouseDown (const MouseEvent& e)
{
    channelListeners.call( &ChannelListener::channelSelected, *this );
}

// transport

DataInterface::Package ChannelComponent::getData (const int from, const int to, const float position)
{
    ChannelElementComponent* current;
    
    current = nullptr;
    
    for (unsigned i = 0; i < elements.size(); i++)
    {
        if (elements[i]->getX() <= position && elements[i]->getX() + elements[i]->getWidth() >= position)
        {
            current = elements[i];
            break;
        }
    }
    
    if (current != currentPlayedElement)
    {
        if (currentPlayedElement != nullptr && currentPlayedElement->editor != nullptr)
            currentPlayedElement->editor->transportExit();

        currentPlayedElement = current;
        
        if (current != nullptr)
            current->editor->transportEnter();
    }
    
    if (current != nullptr)
        return current->getData(from, to, position);

    return DataInterface::Package();
}
