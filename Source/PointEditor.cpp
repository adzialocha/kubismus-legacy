#include "PointEditor.h"

PointEditor::PointEditor()
{
    selectedPoint = -1;
    
    addKeyListener(this);
    setWantsKeyboardFocus(true);
    
    paintTransport = false;
    paintTransportExternal = false;
}

PointEditor::~PointEditor()
{
    points.clear();
    
    removeKeyListener(this);
}

// ==============================================================

void PointEditor::paint (Graphics& g)
{
    point next;
    int i;
    
    g.setColour(Colours::white);
    
    for (i = 0; i < points.size(); ++i)
    {
        if (i + 1 < points.size())
        {
            next = points[i + 1];
            
            g.setColour(Colours::white);
            
            g.drawLine(
                       points[i].x,
                       points[i].y,
                       next.x,
                       next.y
                       );
        }
    }
    
    if (points.size() > 0)
    {
        const float dashLengths[] = { 5, 5 };
        
        if (points.front().x > 0)
        {
            g.drawDashedLine(
                             Line<float>(
                                         0,
                                         points.front().y,
                                         points.front().x,
                                         points.front().y
                                         ),
                             &dashLengths[0], 2, 1.0, 0
                             );
        }
        
        if (points.back().x < getWidth())
        {
            g.drawDashedLine(
                             Line<float>(
                                         getWidth(),
                                         points.back().y,
                                         points.back().x,
                                         points.back().y
                                         ),
                             &dashLengths[0], 2, 1.0, 0
                             );
        }
    }
    
    for (i = 0; i < points.size(); ++i)
    {
        if (selectedPoint == i)
            g.setColour(Colours::red);
        else
            g.setColour(Colours::white);
        
        g.fillEllipse(points[i].x - (POINT_SIZE * 0.5f), points[i].y - (POINT_SIZE * 0.5f), POINT_SIZE, POINT_SIZE);
    }
    
    if (paintTransport && paintTransportExternal)
    {
        g.setColour(Colours::yellow);
        
        g.drawLine(line);
        g.drawLine(intersection);
        
        Point<float> p = line.getIntersection(intersection);
        
        g.setColour(Colours::red);
        
        g.fillEllipse(p.x - (POINT_SIZE * 0.5f), p.y - (POINT_SIZE * 0.5f), POINT_SIZE, POINT_SIZE);
    }
}

// ==============================================================

void PointEditor::sortPoints()
{
    for (unsigned i = 0; i < points.size(); i++)
        for (unsigned j = 0; j < points.size(); j++)
            if (points.at(j).x > points.at(i).x)
                std::swap(points.at(j), points.at(i));
}

int PointEditor::getVerticalPoint(int x)
{
    for (unsigned i = 0; i < points.size(); i++)
    {
        if (points.at(i).x == x)
            return i;
    }
    
    return -1;
}

int PointEditor::getSelectedPoint(int x, int y)
{
    for (unsigned i = 0; i < points.size(); i++)
    {
        if (x >= points.at(i).x - (POINT_SIZE * 0.5f) && x <= points.at(i).x + (POINT_SIZE * 0.5f) &&
            y >= points.at(i).y - (POINT_SIZE * 0.5f) && y <= points.at(i).y + (POINT_SIZE * 0.5f))
            return i;
    }
    
    return -1;
}

void PointEditor::mouseDown (const MouseEvent& e)
{
    int verticalPoint = getVerticalPoint(e.x);
    
    if (verticalPoint > -1)
    {
        points[verticalPoint].y = e.y;
        selectedPoint = verticalPoint;
    }
    else
    {
        selectedPoint = getSelectedPoint(e.x, e.y);
        
        if (selectedPoint == -1)
        {
            addPoint (e.x, e.y);
            sortPoints();
            
            sendChangeMessage();
        }
    }
    
    repaint();
}

void PointEditor::mouseDrag (const MouseEvent& e)
{
    if (selectedPoint > -1 && e.getDistanceFromDragStart() > 0)
    {
        int x, y;
        
        x = e.x;
        y = e.y;
        
        // y constraint
        
        if (y <= 0)
            y = 0;
        else if (y >= getBounds().getHeight())
            y = getBounds().getHeight();
        
        // x constraint
        
        if (x <= 0)
            x = 0;
        else if (x >= getBounds().getWidth())
            x = getBounds().getWidth();
        
        if (points.size() > 2)
        {
            int prevX, nextX;
            
            if (selectedPoint == 0)
                prevX = -1;
            else
                prevX = points.at(selectedPoint - 1).x;
            
            if (selectedPoint == points.size() - 1)
                nextX = getBounds().getWidth() + 1;
            else
                nextX = points.at(selectedPoint + 1).x;
            
            if (e.x <= prevX)
                x = prevX + 1;
            else if (e.x >= nextX)
                x = nextX - 1;
        }
        
        sendChangeMessage();
        
        points[selectedPoint].x = x;
        points[selectedPoint].y = y;
        
        repaint();
    }
}

void PointEditor::mouseUp (const MouseEvent& e)
{
    if (selectedPoint > -1)
    {
        if (points.size() == 2)
            sortPoints();
        
        repaint();
    }
}

bool PointEditor::keyPressed (const KeyPress &k, Component *c)
{
    if (k.getKeyCode() == k.backspaceKey)
    {
        if (selectedPoint > -1)
        {
            points.erase(points.begin() + selectedPoint);
            
            if (points.size() > selectedPoint - 1 && selectedPoint - 1 > -1)
                selectedPoint = selectedPoint - 1;
            else if (points.size() < selectedPoint)
                selectedPoint = -1;
            
            sendChangeMessage();
            
            repaint();
            
            return true;
        }
    }
    
    return false;
}

// ==============================================================

const float PointEditor::getValueAt(const float position)
{
    point first, second;
    int firstPointIndex;
    float result;
    
    firstPointIndex = -1;
    
    paintTransport = false;
    
    // get next point
    
    for (unsigned i = 0; i < points.size(); i++)
    {
        if (points.at(i).x <= position)
            firstPointIndex++;
        else
            break;
    }
    
    intersection = Line<float>(position, 0, position, getHeight());
    
    // before first point or after last point, or no points
    
    if (firstPointIndex == -1)
    {
        result = 0;
        
        if ( points.size() > 1 )
            result = points.front().y;
    }
    else
    {
        paintTransport = true;
        
        // only one point
        
        if ( points.size() == 1 )
            result = points.front().y;
        
        // last point
        
        if ( firstPointIndex == points.size() - 1)
        {
            result = points.back().y;
            
            if (position > points.back().x)
                paintTransport = false;
        }
        else
        {
            // any other point
            
            first = points.at(firstPointIndex);
            
            if ( points.size() - 1 >= firstPointIndex + 1)
            {
                second = points.at(firstPointIndex + 1);
                
                line = Line<float>(first.x, first.y, second.x, second.y);
                
                result = line.getIntersection(intersection).getY();
            }
        }
    }
    
    repaint();
    
    return result;
}