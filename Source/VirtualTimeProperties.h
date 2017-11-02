#ifndef VIRTUALTIMEPROPERTIES_H_INCLUDED
#define VIRTUALTIMEPROPERTIES_H_INCLUDED

class VirtualTimeProperties
{
    
public:
    
    VirtualTimeProperties()
    {
        virtualTime = DEFAULT_TIME;
    };
    
    virtual ~VirtualTimeProperties() {};
    
    const double getVirtualTime() { return virtualTime; }
    void setVirtualTime(const double time) { virtualTime = time; }
    
protected:
    
    const double DEFAULT_TIME = 1.0;
    
    double virtualTime;
    
};

#endif  // VIRTUALTIMEPROPERTIES_H_INCLUDED
