#include "TimelineComponent.h"

TimelineComponent::TimelineComponent()
{
    startPos = 0;
    minInPixels = MINUTE_IN_PIXELS_DEFAULT;
}

TimelineComponent::~TimelineComponent()
{
}

void TimelineComponent::paint (Graphics& g)
{
    // prepare
    
    g.setColour( Colours::black );
    g.fillRect( 0, 0, getWidth(), RULER_HEIGHT );
    
    g.setFont ( 10.0f );
    
    // paint grid and labels
    
    int x, min, sec, grid;
    float position;
    
    String label;
    
    grid = floor( (float) GRID_SIZE_SECONDS * ( (float) minInPixels / 60.0f ) );
    position = ( (float) ( startPos - ( startPos % grid ) ) ) / (float) minInPixels;
    min = floor( position );
    sec = roundUp( floor( ( ( position - (float) min ) * 60.0f ) ), GRID_SIZE_SECONDS );
    
    if (sec == 60)
    {
        min = min + 1;
        sec = 0;
    }
    
    for (unsigned i = 0; i < getWidth() + grid; i = i + grid)
    {
        x = i - ( startPos % grid );
        
        label = min < 10 ? "0" + std::to_string ( min ) + ":" : std::to_string ( min ) + ":" ;
        label = sec < 10 ? label + "0" + std::to_string( sec ) : label + std::to_string( sec );

        g.setColour (Colours::white);
        
        if (x > 0)
            g.drawText( label, x - ( grid / 2 ), 0, grid, RULER_HEIGHT, Justification::centred );

        if (sec == 0 && min > 0)
            g.setColour (Colours::white);
        else
            g.setColour (Colours::lightgrey);
        
        if (x > 0)
            g.drawLine( x, RULER_HEIGHT + 1, x, getHeight() );
        
        sec = sec + GRID_SIZE_SECONDS;
        
        if (sec >= 60)
        {
            min = min + 1;
            sec = 0;
        }
    }
}

void TimelineComponent::resized()
{
    
}

void TimelineComponent::mouseDown(const juce::MouseEvent &e)
{
    mousePosition = e.getPosition().x;
    wasCommandKeyPressed = e.source.getCurrentModifiers().isCommandDown();
    
    sendChangeMessage();
}

void TimelineComponent::setViewportArea (const Rectangle< int > &newVisibleArea)
{
    if (startPos != newVisibleArea.getX())
    {
        startPos = newVisibleArea.getX();
        repaint();
    }
}
