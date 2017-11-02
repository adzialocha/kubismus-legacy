#include "LookAndFeelCustom.h"

LookAndFeelCustom::LookAndFeelCustom()
{
    setColour (TextButton::textColourOnId, Colours::white.darker());
    setColour (TextButton::textColourOffId, Colours::white);
}

LookAndFeelCustom::~LookAndFeelCustom() {}

// ==================================================

void LookAndFeelCustom::drawButtonBackground (Graphics& g, Button& button, const Colour& backgroundColour,
                                           bool isMouseOverButton, bool isButtonDown)
{
    g.fillAll ( Colour::fromRGBA(0, 0, 0, isButtonDown || isMouseOverButton ? 80 : 50) );
}

Font LookAndFeelCustom::getTextButtonFont (TextButton&, int buttonHeight)
{
    return Font (jmin (10.0f, buttonHeight * 0.6f));
}

void LookAndFeelCustom::drawButtonText (Graphics& g, TextButton& button, bool isMouseOverButton, bool isButtonDown)
{
    Font font (getTextButtonFont (button, button.getHeight()));
    g.setFont (font);
    g.setColour (button.findColour (button.getToggleState() || isButtonDown ? TextButton::textColourOnId
                                    : TextButton::textColourOffId)
                 .withMultipliedAlpha (button.isEnabled() ? 1.0f : 0.5f));
    
    const int yIndent = jmin (4, button.proportionOfHeight (0.3f));
    const int cornerSize = jmin (button.getHeight(), button.getWidth()) / 2;
    
    const int fontHeight = roundToInt (font.getHeight() * 0.6f);
    const int leftIndent  = jmin (fontHeight, 2 + cornerSize / (button.isConnectedOnLeft() ? 4 : 2));
    const int rightIndent = jmin (fontHeight, 2 + cornerSize / (button.isConnectedOnRight() ? 4 : 2));
    const int textWidth = button.getWidth() - leftIndent - rightIndent;
    
    if (textWidth > 0)
        g.drawFittedText (button.getButtonText().toUpperCase(),
                          leftIndent, yIndent, textWidth, button.getHeight() - yIndent * 2,
                          Justification::centred, 2);
}

// ==================================================

void LookAndFeelCustom::drawTreeviewPlusMinusBox (Graphics& g, const Rectangle<float>& area,
                                               Colour backgroundColour, bool isOpen, bool isMouseOver)
{
    Path p;
    p.addTriangle (0.0f, 0.0f, 1.0f, isOpen ? 0.0f : 0.5f, isOpen ? 0.5f : 0.0f, 1.0f);
    g.setColour (backgroundColour);
    g.fillPath (p, p.getTransformToScaleToFit (area.reduced (2, area.getHeight() / 3), true));
}

// ==================================================

void LookAndFeelCustom::drawPropertyPanelSectionHeader (Graphics& g, const String& name,
                                                     bool isOpen, int width, int height)
{
    const float buttonSize = height * 0.75f;
    const float buttonIndent = (height - buttonSize) * 0.5f;
    
    drawTreeviewPlusMinusBox (g, Rectangle<float> (buttonIndent, buttonIndent, buttonSize, buttonSize), Colours::white, isOpen, false);
    
    const int textX = (int) (buttonIndent * 2.0f + buttonSize + 2.0f);
    
    g.setColour (Colours::white);
    g.setFont (Font (10.0f));
    g.drawText (name.toUpperCase(), textX, 0, width - textX - 4, height, Justification::centredLeft, true);
}

void LookAndFeelCustom::drawPropertyComponentBackground (Graphics& g, int width, int height, PropertyComponent& component)
{
    g.setColour (component.findColour (PropertyComponent::backgroundColourId));
    g.fillRect (0, 0, width, height - 1);
}

void LookAndFeelCustom::drawPropertyComponentLabel (Graphics& g, int, int height, PropertyComponent& component)
{
    g.setColour (component.findColour (PropertyComponent::labelTextColourId)
                 .withMultipliedAlpha (component.isEnabled() ? 1.0f : 0.6f));
    
    g.setFont (10.0f);
    
    const Rectangle<int> r (getPropertyComponentContentPosition (component));
    
    g.drawFittedText (component.getName().toUpperCase(),
                      3, r.getY(), r.getX() - 5, r.getHeight(),
                      Justification::centredLeft, 2);
}

Rectangle<int> LookAndFeelCustom::getPropertyComponentContentPosition (PropertyComponent& component)
{
    const int textW = jmin (200, component.getWidth() / 3);
    return Rectangle<int> (textW, 1, component.getWidth() - textW - 1, component.getHeight() - 3);
}
