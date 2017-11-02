#ifndef LOOKANDFEELCUSTOM_H_INCLUDED
#define LOOKANDFEELCUSTOM_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

class LookAndFeelCustom   : public LookAndFeel_V3
{
    
public:
    
    LookAndFeelCustom();
    ~LookAndFeelCustom();
    
    // ==================================================
    
    void drawButtonBackground (Graphics&, Button&, const Colour& backgroundColour,
                               bool isMouseOverButton, bool isButtonDown) override;
    Font getTextButtonFont (TextButton&, int buttonHeight) override;
    void drawButtonText (Graphics&, TextButton&, bool isMouseOverButton, bool isButtonDown) override;
    
    // ==================================================
    
    void drawTreeviewPlusMinusBox (Graphics&, const Rectangle<float>& area,
                                   Colour backgroundColour, bool isOpen, bool isMouseOver) override;
    
    // ==================================================
    
    void drawPropertyPanelSectionHeader (Graphics&, const String& name, bool isOpen, int width, int height) override;
    void drawPropertyComponentBackground (Graphics&, int width, int height, PropertyComponent&) override;
    void drawPropertyComponentLabel (Graphics&, int width, int height, PropertyComponent&) override;
    Rectangle<int> getPropertyComponentContentPosition (PropertyComponent&) override;
    
private:
    
};

#endif  // LOOKANDFEELCUSTOM_H_INCLUDED
