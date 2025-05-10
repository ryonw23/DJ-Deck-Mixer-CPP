#include <JuceHeader.h>
#include "CustomLookAndFeel.h"

//==============================================================================
CustomLookAndFeel::CustomLookAndFeel()
{
}

CustomLookAndFeel::~CustomLookAndFeel()
{
}

void CustomLookAndFeel::drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height,
                          float sliderPos, const float rotaryStartAngle,
                          const float rotaryEndAngle, juce::Slider& slider)
{
    // Calculate key dimensions and positioning
    auto radius = (float) juce::jmin (width / 2, height / 2) - 22.0f;
    auto centreX = (float) x + (float) width  * 0.5f;
    auto centreY = (float) y + (float) height * 0.5f;
    
    auto rx = centreX - radius;
    auto ry = centreY - radius;
    auto rw = radius * 2.0f;
    auto angle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);

    // Draw outer rotary background with slightly lighter colour than main background colour
    g.setColour (Colour(10,10,70));
    g.fillEllipse (rx, ry, rw, rw);

    // Draw outline for the rotary control with bright pink for contrast
    g.setColour (juce::Colours::mediumvioletred);
    g.drawEllipse (rx, ry, rw, rw, 2.0f);
    
    // Draw inner ellipse for depth with grey colour
    auto smallrx = centreX - radius * 1.25f/2.0f;
    auto smallry = centreY - radius * 1.25f/2.0f;
    auto smallrw = radius * 1.25f;
    g.setColour (Colour(100,100,100));
    g.fillEllipse (smallrx, smallry, smallrw, smallrw);
    
    // Draw evenly spaced notches around the control
    int numNotches = 6;  // Number of tick marks
    float notchLength = 6.0f; // Length of each notch
    
    float halfPI = juce::MathConstants<float>::halfPi;
    
    for (int i = 0; i < numNotches; ++i)
    {
        // Calculate angle for each notch
        float notchAngle = rotaryStartAngle - juce::MathConstants<float>::pi *5.0f/180.0f + (i / (float)(numNotches - 1))
        * ((rotaryEndAngle + juce::MathConstants<float>::pi *5.0f/180.0f) - (rotaryStartAngle - juce::MathConstants<float>::pi *5.0f/180.0f)) - halfPI;
        
        // Calculate notch start and end points
        float notchX1 = centreX + (radius + 4.0f) * std::cos(notchAngle);
        float notchY1 = centreY + (radius + 4.0f) * std::sin(notchAngle);
        float notchX2 = centreX + (radius + notchLength + 4.0f) * std::cos(notchAngle);
        float notchY2 = centreY + (radius + notchLength + 4.0f) * std::sin(notchAngle);

        // Draw the notches in white
        g.setColour(juce::Colours::ghostwhite);
        g.drawLine(notchX1, notchY1, notchX2, notchY2, 2.0f);
    }
    
    // Draw the pointer to show the slider position
    g.setColour (Colour(100,100,100));
    juce::Path p;
    
    // Calculate coordinates for triangular pointer
    float triX1 = centreX + (radius * 0.9f) * std::cos(angle - halfPI);
    float triY1 = centreY + (radius * 0.9f) * std::sin(angle - halfPI);
    float triX2 = centreX + (radius * 0.3f) * std::cos(angle + juce::MathConstants<float>::pi *60.0f/180.0f - halfPI);
    float triY2 = centreY + (radius * 0.3f) * std::sin(angle + juce::MathConstants<float>::pi *60.0f/180.0f - halfPI);
    float triX3 = centreX + (radius * 0.3f) * std::cos(angle - juce::MathConstants<float>::pi *60.0f/180.0f - halfPI);
    float triY3 = centreY + (radius * 0.3f) * std::sin(angle - juce::MathConstants<float>::pi *60.0f/180.0f - halfPI);
    
    // Add and draw triangular pointer
    p.addTriangle(triX1, triY1, triX2, triY2, triX3, triY3);
    g.fillPath (p);
}
