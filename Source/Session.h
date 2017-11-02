#ifndef SESSION_H_INCLUDED
#define SESSION_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

class Session    : public ChangeBroadcaster,
                   private HighResolutionTimer
{
public:
    
    Session()
    {
        position = 0;
        precisionPosition = 0.0;
        resolution = MINUTE_IN_PIXELS_DEFAULT;
    };
    
    ~Session() {};
    
    void setResolution (const int res)
    {
        resolution = res;
    }
    
    void start ()
    {
        startTimer (TIMER_INTERVAL_MS);
    }
    
    void stop ()
    {
        stopTimer ();
    }
    
    bool isRunning ()
    {
        return isTimerRunning ();
    }
    
    const int getPosition()
    {
        return position;
    }
    
    const float getPrecisePosition ()
    {
        return precisionPosition;
    }
    
    void setPosition (const int pos)
    {
        precisionPosition = pos;
        position = pos;
    }
    
private:
    
    const int TIMER_INTERVAL_MS = 10;
    const int MINUTE_IN_PIXELS_DEFAULT = 1000;
    
    float precisionPosition;
    
    int position;
    int resolution;
    
    void hiResTimerCallback () override
    {
        precisionPosition = precisionPosition + ( ( (float) resolution / 60.0 ) / ( 1000.0 / (float) TIMER_INTERVAL_MS ) );
        position = round (precisionPosition);

        sendChangeMessage();
    }
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Session)
    
};

#endif  // SESSION_H_INCLUDED
