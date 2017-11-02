#ifndef ELEMENTLISTENER_H_INCLUDED
#define ELEMENTLISTENER_H_INCLUDED

class ChannelElementComponent;

class ElementListener
{
    
public:
    
    virtual ~ElementListener()  {}
    
    virtual void elementDropped ( ChannelElementComponent &element );
    virtual void elementSelected ( ChannelElementComponent &element );
    
};

#endif  // ELEMENTLISTENER_H_INCLUDED
