/*
  ==============================================================================

    ImageButton.h
    Created: 10 May 2022 3:21:55pm
    Author:  proje

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class ImageButton  : public juce::ImageButton
{
public:
    ImageButton();
    ImageButton(juce::String filename, juce::Image fullStrip, int width, int height);
    ~ImageButton() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    void resize(int width, int height);

    void setFilename(juce::String filename);
    juce::String getFilename();

    void setImage(juce::Image image);
    juce::Image getImage();

    void setWidth(int width);
    int getWidth();

    void setHeight(int height);
    int getHeight();

    void setImageWidth(int imageWidth);
    int getImageWidth();

    void setImageHeight(int imageHeight);
    int getImageHeight();

    juce::String toString();

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ImageButton)

    juce::String filename;
    juce::Image fullStrip;
    int width;
    int height;
    int imageWidth;
    int imageHeight;
};