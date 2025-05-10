#include <JuceHeader.h>
#include "DiscLookAndFeel.h"

//==============================================================================
DiscLookAndFeel::DiscLookAndFeel()
{
}

DiscLookAndFeel::~DiscLookAndFeel()
{
}

void DiscLookAndFeel::drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height,
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

    // Draw the background of the rotary slider with a dark colour
    g.setColour (Colour(20,20,20));
    g.fillEllipse (rx, ry, rw, rw);
    
    // Draw a lighter outline around the rotary slider
    g.setColour (Colour(150,150,150));
    g.drawEllipse (rx, ry, rw, rw, 10.0f);

    // Set colour for additional rings on rotary slider
    g.setColour (juce::Colours::whitesmoke);
    
    // Draw concentric circles based on diminishing scale
    for (int i = 3; i < 5; ++i)
    {
        float scale = (i / 2.0f);
        rx = centreX - radius / scale;
        ry = centreY - radius / scale;
        rw = (radius * 2) / scale;
        g.drawEllipse (rx, ry, rw, rw, 1.0f);
    }
        
    // Create the pointer of the rotary slider
    juce::Path p;
    auto pointerLength = radius;
    auto pointerThickness = 2.0f;
    p.addRectangle (-pointerThickness * 0.5f, -radius, pointerThickness, pointerLength);
    
    // Apply rotation based on slider angle
    p.applyTransform(juce::AffineTransform::rotation(angle).translated(centreX, centreY));
        
    // Set colour of pointer to white
    g.setColour (juce::Colours::whitesmoke);
    g.fillPath (p);
    
    // Draw small circle in centre of rotary slider for depth with bright pink colour
    auto smallrx = centreX - radius/4.0f;
    auto smallry = centreY - radius/4.0f;
    auto smallrw = radius/2;
    g.setColour (juce::Colours::mediumvioletred);
    g.fillEllipse (smallrx, smallry, smallrw, smallrw);
}
