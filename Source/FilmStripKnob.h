/*
  ==============================================================================

    FilmStripKnob.h
    Created: 4 May 2022 11:57:10am
    Author:  proje

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <string>

//==============================================================================
/*
*/
class FilmStripKnob  : public juce::Slider
{
public:
    FilmStripKnob();
    FilmStripKnob(juce::String filename, juce::Image fullStrip, int numFrames, bool horizontal, int width, int height);
    ~FilmStripKnob() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    void resize(int width, int height);


    //Getter & Setters
    void setFilename(juce::String filename);
    juce::String getFilename();

    void setImage(juce::Image image);
    juce::Image getImage();

    int getNumFrames();

    bool isHorizontal();

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
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FilmStripKnob)
    
    juce::String filename;
    juce::Image fullStrip;
    int numFrames;
    bool horizontal;
    int width;
    int height;
    int imageWidth;
    int imageHeight;
};
