#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class DiscLookAndFeel  : public juce::LookAndFeel_V4
{
public:
    DiscLookAndFeel();
    ~DiscLookAndFeel() override;
    
    // Initialise custom look and feel for rotary slider (disc for position slider)
    void drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos,
                          const float rotaryStartAngle, const float rotaryEndAngle, juce::Slider& slider) override;
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DiscLookAndFeel);
};
