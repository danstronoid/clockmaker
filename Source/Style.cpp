/*
  ==============================================================================

    Style.cpp
    Created: 19 Jul 2020 12:25:53pm
    Author:  Daniel Schwartz

  ==============================================================================
*/

#include "Style.h"

using namespace dingus_dsp;
using namespace juce;

//===================================================================================
Style::Style() :
    defaultScheme({ palette.windowBackground, palette.widgetBackground, palette.menuBackground,
        palette.outline, palette.defaultText, palette.defaultFill,
        palette.highlightedText, palette.highlightedFill, palette.menuText })
{
    setColourScheme(defaultScheme);
}
Style::~Style() {}

//===================================================================================
// Tabbed Component

// change tab button width
int Style::getTabButtonBestWidth(TabBarButton& button, int /*tabDepth*/) 
{
    return button.getTabbedButtonBar().getWidth() / button.getTabbedButtonBar().getNumTabs();
}

// modified tab button drawing to remove gradient
void Style::drawTabButton(TabBarButton& button, Graphics& g, bool isMouseOver, bool isMouseDown) 
{
    const Rectangle<int> activeArea(button.getActiveArea().reduced(4));

    const TabbedButtonBar::Orientation o = button.getTabbedButtonBar().getOrientation();

    const Colour bkg = findColour(TextButton::ColourIds::buttonColourId);

    if (button.getToggleState())
        g.setColour(bkg);
    else
        g.setColour(bkg.darker(0.2f));

    g.fillRoundedRectangle(activeArea.toFloat(), cornerSize);

    const float alpha = button.isEnabled() ? ((isMouseOver || isMouseDown) ? 1.0f : 0.8f) : 0.3f;

    Colour col(bkg.contrasting().withMultipliedAlpha(alpha));

    if (TabbedButtonBar* bar = button.findParentComponentOfClass<TabbedButtonBar>())
    {
        TabbedButtonBar::ColourIds colID = button.isFrontTab() ? TabbedButtonBar::frontTextColourId
            : TabbedButtonBar::tabTextColourId;

        if (bar->isColourSpecified(colID))
            col = bar->findColour(colID);
        else if (isColourSpecified(colID))
            col = findColour(colID);
    }

    const Rectangle<float> area(button.getTextArea().toFloat());

    float length = area.getWidth();
    float depth = area.getHeight();

    if (button.getTabbedButtonBar().isVertical())
        std::swap(length, depth);

    TextLayout textLayout;
    createTabTextLayout(button, length, depth, col, textLayout);

    AffineTransform t;

    switch (o)
    {
    case TabbedButtonBar::TabsAtLeft:   t = t.rotated(MathConstants<float>::pi * -0.5f).translated(area.getX(), area.getBottom()); break;
    case TabbedButtonBar::TabsAtRight:  t = t.rotated(MathConstants<float>::pi * 0.5f).translated(area.getRight(), area.getY()); break;
    case TabbedButtonBar::TabsAtTop:
    case TabbedButtonBar::TabsAtBottom: t = t.translated(area.getX(), area.getY()); break;
    default:                            jassertfalse; break;
    }

    g.addTransform(t);
    textLayout.draw(g, Rectangle<float>(length, depth));
}

// necessary to override, removes unwanted features
void Style::drawTabAreaBehindFrontButton(TabbedButtonBar& /*bar*/, Graphics& /*g*/, const int /*w*/, const int /*h*/) {}

//===================================================================================
// Sliders

// Draws a linear vertical or horizontal sliders.  This does not implement multi point sliders.
void Style::drawLinearSlider(Graphics& g, int x, int y, int width, int height,
    float sliderPos, float /*minSliderPos*/, float /*maxSliderPos*/, const Slider::SliderStyle /*style*/, Slider& slider) 
{
    Point<float> startPoint(slider.isHorizontal() ? x : x + width * 0.5f,
        slider.isHorizontal() ? y + height * 0.5f : height + y);

    Point<float> endPoint(slider.isHorizontal() ? width + x : startPoint.x,
        slider.isHorizontal() ? startPoint.y : y);

    Point<float> thumbPoint(slider.isHorizontal() ? sliderPos : (x + width * 0.5f),
        slider.isHorizontal() ? (y + height * 0.5f) : sliderPos);

    Path backgroundTrack;
    backgroundTrack.startNewSubPath(startPoint);
    backgroundTrack.lineTo(endPoint);
    g.setColour(slider.findColour(Slider::backgroundColourId));
    g.strokePath(backgroundTrack, { trackWidth, PathStrokeType::curved, PathStrokeType::square });

    Path valueTrack;
    valueTrack.startNewSubPath(startPoint);
    valueTrack.lineTo(thumbPoint);
    g.setColour(slider.findColour(Slider::trackColourId));
    g.strokePath(valueTrack, { trackWidth, PathStrokeType::curved, PathStrokeType::square });

    g.setColour(slider.findColour(Slider::thumbColourId));
    if (slider.isHorizontal())
        g.fillRoundedRectangle(Rectangle<float>(thumbHeight, thumbWidth).withCentre(thumbPoint), 0.0f);
    else
        g.fillRoundedRectangle(Rectangle<float>(thumbWidth, thumbHeight).withCentre(thumbPoint), 0.0f);
}


void Style::drawRotarySlider(Graphics& g, int x, int y, int width, int height, float sliderPos,
    const float rotaryStartAngle, const float rotaryEndAngle, Slider& slider) 
{
    Rectangle<float> bounds = Rectangle<int>(x, y, width, height).toFloat().reduced(10);
    float radius = jmin(bounds.getWidth(), bounds.getHeight()) / 2.0f;
    float angle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);
    float arcRadius = radius - trackWidth * 0.5f;

    Path backgroundArc;
    backgroundArc.addCentredArc(bounds.getCentreX(), bounds.getCentreY(), arcRadius, arcRadius,
        0.0f, rotaryStartAngle, rotaryEndAngle, true);

    g.setColour(slider.findColour(Slider::rotarySliderOutlineColourId));
    g.strokePath(backgroundArc, PathStrokeType(trackWidth, PathStrokeType::curved, PathStrokeType::square));


    Path valueArc;
    valueArc.addCentredArc(bounds.getCentreX(), bounds.getCentreY(), arcRadius, arcRadius,
        0.0f, rotaryStartAngle, angle, true);

    g.setColour(slider.findColour(Slider::rotarySliderFillColourId));
    g.strokePath(valueArc, PathStrokeType(trackWidth, PathStrokeType::curved, PathStrokeType::square));

    Path thumb;
    thumb.addRectangle(-thumbHeight * 0.5f, -arcRadius - thumbWidth * 0.5f, thumbHeight, thumbWidth);
    thumb.applyTransform(AffineTransform::rotation(angle).translated(bounds.getCentreX(), bounds.getCentreY()));

    g.setColour(slider.findColour(Slider::thumbColourId));
    g.fillPath(thumb);
}

// This implementation provides custom textbox locations
Slider::SliderLayout Style::getSliderLayout(Slider& slider) 
{
    Slider::SliderLayout layout;

    Rectangle<int> bounds = slider.getLocalBounds();
    layout.sliderBounds = bounds;

    int thumbPadding = (int)thumbHeight * 2;
    if (slider.isHorizontal())
        layout.sliderBounds.reduce(thumbPadding, 0);
    else if (slider.isVertical())
        layout.sliderBounds.reduce(0, thumbPadding);

    if (slider.getTextBoxPosition() != Slider::NoTextBox)
    {
        int textBoxWidth = slider.getTextBoxWidth();
        int textBoxHeight = 20;

        layout.textBoxBounds.setWidth(textBoxWidth);
        layout.textBoxBounds.setHeight(textBoxHeight);

        // put the text box in the center of the rotary, or put it below
        if (slider.isRotary())
        {
            if (slider.getTextBoxPosition() == Slider::TextBoxBelow)
            {
                layout.textBoxBounds.setX((bounds.getWidth() - textBoxWidth) / 2);
                layout.textBoxBounds.setY(bounds.getHeight() - textBoxHeight);
                layout.sliderBounds.removeFromBottom(textBoxHeight);
            }
            else
            {
                layout.textBoxBounds.setX(bounds.getCentreX() - textBoxWidth / 2);
                layout.textBoxBounds.setY(bounds.getCentreY() - textBoxHeight / 2);
            }
        }
        // put the text box to the left of a horizontal slider
        else if (slider.isHorizontal())
        {
            layout.textBoxBounds.setX(0);
            layout.textBoxBounds.setY((bounds.getHeight() - textBoxHeight) / 2);
            layout.sliderBounds.removeFromLeft(textBoxWidth);
        }
        // put the text box below anything else
        else
        {
            layout.textBoxBounds.setX((bounds.getWidth() - textBoxWidth) / 2);
            layout.textBoxBounds.setY(bounds.getHeight() - textBoxHeight);
            layout.sliderBounds.removeFromBottom(textBoxHeight);
        }
    }

    return layout;
}

//===================================================================================
// ComboBox

void Style::drawComboBox(Graphics& g, int width, int height, bool,
    int, int, int, int, ComboBox& box)
{
    Rectangle<int> boxBounds(0, 0, width, height);

    g.setColour(box.findColour(ComboBox::backgroundColourId));
    g.fillRect(boxBounds.toFloat());

    /*g.setColour(box.findColour(ComboBox::outlineColourId));
    g.drawRect(boxBounds.toFloat().reduced(0.5f, 0.5f), 1.0f);*/

    Rectangle<int> arrowZone(width - 30, 0, 20, height);
    Path path;
    path.startNewSubPath(arrowZone.getX() + 3.0f, arrowZone.getCentreY() - 2.0f);
    path.lineTo(static_cast<float> (arrowZone.getCentreX()), arrowZone.getCentreY() + 3.0f);
    path.lineTo(arrowZone.getRight() - 3.0f, arrowZone.getCentreY() - 2.0f);

    g.setColour(box.findColour(ComboBox::arrowColourId));
    g.strokePath(path, PathStrokeType(2.0f));
}

//===================================================================================
// Buttons

// This override is taken directly from the LookAndFeel_V4 implementation so that a custom tickbox can be used
void Style::drawToggleButton(Graphics& g, ToggleButton& button,
    bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown)
{
    auto fontSize = jmin(15.0f, button.getHeight() * 0.75f);
    auto tickWidth = fontSize * 1.1f;

    drawTickBox(g, button, 4.0f, (button.getHeight() - tickWidth) * 0.5f,
        tickWidth, tickWidth,
        button.getToggleState(),
        button.isEnabled(),
        shouldDrawButtonAsHighlighted,
        shouldDrawButtonAsDown);

    g.setColour(button.findColour(ToggleButton::textColourId));
    g.setFont(fontSize);

    if (!button.isEnabled())
        g.setOpacity(0.5f);

    g.drawFittedText(button.getButtonText(),
        button.getLocalBounds().withTrimmedLeft(roundToInt(tickWidth) + 10)
        .withTrimmedRight(2),
        Justification::centredLeft, 10);
}

// this is taken from LookAndFeel_V4 but modified for a non-rounded box
void Style::drawTickBox(Graphics& g, Component& component,
    float x, float y, float w, float h,
    const bool ticked,
    const bool isEnabled,
    const bool shouldDrawButtonAsHighlighted,
    const bool shouldDrawButtonAsDown)
{
    ignoreUnused(isEnabled, shouldDrawButtonAsHighlighted, shouldDrawButtonAsDown);

    Rectangle<float> tickBounds(x, y, w, h);

    g.setColour(component.findColour(ToggleButton::tickDisabledColourId));
    g.drawRect(tickBounds, 2.0f);

    if (ticked)
    {
        g.setColour(component.findColour(ToggleButton::tickColourId));
        auto tick = getTickShape(0.75f);
        g.fillPath(tick, tick.getTransformToScaleToFit(tickBounds.reduced(4, 5).toFloat(), false));
    }
}

//===================================================================================