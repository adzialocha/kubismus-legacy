#ifndef POINTEDITOR_H_INCLUDED
#define POINTEDITOR_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

#include <vector>
#include "EditorComponent.h"

using std::vector;

class PointEditor    : public EditorComponent,
                       public ChangeBroadcaster,
                       private KeyListener
{
    
public:
    
    struct point {
        int   x;
        int   y;
    };
    
    PointEditor();
    ~PointEditor();
    
    void paint (Graphics&) override;
    
    void setPaintTransport (bool state)
    {
        paintTransportExternal = state;
        repaint();
    }

    const float getValueAt(const float position);
    
    vector<point> getPoints ()
    {
        return points;
    };
    
    void addPoint (const int x, const int y)
    {
        point newPoint;
        
        newPoint.x = x;
        newPoint.y = y;
        
        points.push_back(newPoint);
    }
    
    void sortPoints();
    
protected:
    
    Line<float> line;
    Line<float> intersection;
    
    bool paintTransport;
    bool paintTransportExternal;

    int POINT_SIZE = 8;
    
    vector<point> points;
    
    int selectedPoint;
    
    int getVerticalPoint(int);
    int getSelectedPoint(int, int);
    
    void mouseDown (const MouseEvent&) override;
    void mouseDrag (const MouseEvent&) override;
    void mouseUp (const MouseEvent&) override;
    
    bool keyPressed (const KeyPress&, Component*) override;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PointEditor)
    
};

#endif  // POINTEDITOR_H_INCLUDED
