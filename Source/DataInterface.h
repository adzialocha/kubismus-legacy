#ifndef DATAINTERFACE_H_INCLUDED
#define DATAINTERFACE_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

class DataInterface
{
    
public:
    
    // DataInterface::Package
    
    struct Package
    {
        String address;
        int channel;
        float value;
        bool dirty;
    };
    
    // DataInterface
    
    DataInterface()
    {
        connectionStatus = false;
    }
    
    ~DataInterface()
    {
        if (connectionStatus)
            disconnect();
    }
    
    bool connect()
    {
        return connect (DEFAULT_TARGET_IP, DEFAULT_PORT);
    }
    
    bool connect(const String &target)
    {
        return connect (target, DEFAULT_PORT);
    }
    
    bool connect(const String &target, const int targetPortNumber)
    {
        connectionStatus = sender.connect (target, targetPortNumber);
        return connectionStatus;
    }
    
    bool disconnect()
    {
        connectionStatus = ! sender.disconnect();
        return ! connectionStatus;
    }
    
    bool isConnected()
    {
        return connectionStatus;
    }
    
    bool send(Array<Package> data)
    {
        if (! isConnected())
            return false;
        
        if (data.size() == 0)
            return false;
        
        OSCBundle bundle;
        
        try
        {
            for (unsigned i = 0; i < data.size(); i++)
            {
                if (data[i].address.isNotEmpty() && data[i].dirty) {
                    bundle.addElement ( OSCMessage ( data[i].address, data[i].value ) );
//                    std::cout << data[i].address << " " << data[i].value << data[i].dirty << "\n";
                }
            }
            
            if (! bundle.isEmpty())
                return sender.send (bundle);
            else
                return false;
        }
        catch (const OSCException e)
        {
            return false;
        }
    }
    
private:
    
    bool connectionStatus;
    
    const String DEFAULT_TARGET_IP = "0.0.0.0";
    const int DEFAULT_PORT = 9001;
    
    OSCSender sender;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DataInterface)

};

#endif  // DATAINTERFACE_H_INCLUDED
