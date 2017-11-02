#ifndef OSCPROPERTIES_H_INCLUDED
#define OSCPROPERTIES_H_INCLUDED

class OSCProperties
{
    
public:
    
    OSCProperties()
    {
        oscAddress = DEFAULT_OSC_ADDRESS;
        oscChannel = DEFAULT_OSC_CHANNEL;
        oscMinValue = DEFAULT_OSC_MIN_VALUE;
        oscMaxValue = DEFAULT_OSC_MAX_VALUE;
    };
    
    virtual ~OSCProperties() {};
    
    const int getOSCChannel() { return oscChannel; }
    void setOSCChannel(const int channel) { oscChannel = channel; }
    
    const String& getOSCAddress() { return oscAddress; }
    void setOSCAddress(const String& address) { oscAddress = address; }
    
    const float getOSCMinValue() { return oscMinValue; }
    void setOSCMinValue(const float val) { oscMinValue = val; }
    
    const float getOSCMaxValue() { return oscMaxValue; }
    void setOSCMaxValue(const float val) { oscMaxValue = val; }
    
protected:
    
    const int DEFAULT_OSC_CHANNEL = 1;
    const String DEFAULT_OSC_ADDRESS = "/kubismus";
    const float DEFAULT_OSC_MIN_VALUE = 0.0f;
    const float DEFAULT_OSC_MAX_VALUE = 1.0f;
    
    int oscChannel;
    String oscAddress;
    
    float oscMinValue;
    float oscMaxValue;
    
};

#endif  // OSCPROPERTIES_H_INCLUDED
