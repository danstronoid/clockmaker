/*
  ==============================================================================

    Style.h
    Created: 19 Jul 2020 12:25:53pm
    Author:  Daniel Schwartz

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

namespace dingus_dsp
{
    //===================================================================================
    // a collection of colors to be used
    struct ColorPalette
    {
        juce::Colour windowBackground{ juce::Colours::black };
        juce::Colour widgetBackground{ juce::Colours::darkgrey };
        juce::Colour menuBackground{ juce::Colours::darkgrey };
        juce::Colour outline{ juce::Colours::lightgrey };
        juce::Colour defaultText{ juce::Colours::ghostwhite };
        juce::Colour defaultFill{ juce::Colours::lightgrey };
        juce::Colour highlightedText{ juce::Colours::black };
        juce::Colour highlightedFill{ juce::Colours::skyblue };
        juce::Colour menuText{ juce::Colours::ghostwhite };
    };

    //===================================================================================
    class Style : public juce::LookAndFeel_V4
    {
    public:
        Style();
        ~Style();

        //===================================================================================
            // Tabbed Component
        int getTabButtonBestWidth(juce::TabBarButton& button, int tabDepth) override;
        void drawTabButton(juce::TabBarButton& button, juce::Graphics& g, bool isMouseOver, bool isMouseDown) override;
        void drawTabAreaBehindFrontButton(juce::TabbedButtonBar& bar, juce::Graphics& g, const int w, const int h) override;

        //===================================================================================
            // Sliders
        void drawLinearSlider(juce::Graphics& g, int x, int y, int width, int height,
            float sliderPos, float /*minSliderPos*/, float /*maxSliderPos*/, const juce::Slider::SliderStyle /*style*/, juce::Slider& slider) override;
        void drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos,
            const float rotaryStartAngle, const float rotaryEndAngle, juce::Slider& slider) override;
        juce::Slider::SliderLayout getSliderLayout(juce::Slider& slider) override;

        //===================================================================================
            // ComboBox
        void drawComboBox(juce::Graphics& g, int width, int height, bool,
            int, int, int, int, juce::ComboBox& box) override;

        //===================================================================================
            // Button

        void drawToggleButton(juce::Graphics& g, juce::ToggleButton& button,
            bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override;

        void drawTickBox(juce::Graphics& g, juce::Component& component,
            float x, float y, float w, float h,
            const bool ticked,
            const bool isEnabled,
            const bool shouldDrawButtonAsHighlighted,
            const bool shouldDrawButtonAsDown) override;

        //===================================================================================

    private:

        // constants
        ColorPalette palette;
        ColourScheme defaultScheme;
        const int componentPadding{ 10 };
        const float cornerSize{ 10.0f };

        // slider constants
        const float trackWidth = 6.0f;
        const float thumbWidth = 18.0f;
        const float thumbHeight = 6.0f;

    };
    //===================================================================================
}

