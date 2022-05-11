/*
  ==============================================================================

    FilmStripKnob.cpp
    Created: 4 May 2022 11:57:10am
    Author:  proje

  ==============================================================================
*/

#include <JuceHeader.h>
#include "FilmStripKnob.h"

//==============================================================================
FilmStripKnob::FilmStripKnob() {}

FilmStripKnob::FilmStripKnob(juce::String filename, juce::Image fullStrip, int numFrames, bool horizontal, int width, int height)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    this->filename = filename;
    this->fullStrip = fullStrip;
    this->numFrames = numFrames;
    this->horizontal = horizontal;
    this->width = width;
    this->height = height;

    //the width and height of the image is calculated from the given image strip and might be different than the bounds of the component.
    if (!this->fullStrip.isNull()) {
        setTextBoxStyle(NoTextBox, 0, 0, 0);
        if (horizontal) {
            this->imageHeight = fullStrip.getHeight();
            this->imageWidth = fullStrip.getWidth() / numFrames;
        }
        else {
            this->imageHeight = fullStrip.getHeight() / numFrames;
            this->imageWidth = fullStrip.getWidth();
        }
    }
}

FilmStripKnob::~FilmStripKnob()
{
}

void FilmStripKnob::paint (juce::Graphics& g)
{
    if (!fullStrip.isNull()) {
        int value = (getValue() - getMinimum()) / (getMaximum() - getMinimum()) * (numFrames - 1);
        // we calculate the x and y position such that the image is placed in the center of the component
        int x = (this->width - this->imageWidth) / 2;
        int y = (this->height - this->imageHeight) / 2;
        if (horizontal) {
            g.drawImage(this->fullStrip, x, y, getWidth(), getHeight(),value * this->imageWidth, 0, this->imageWidth, this->imageHeight);
        }
        else {
            g.drawImage(this->fullStrip, x, y, getWidth(), getHeight(), 0, value * this->imageHeight, this->imageWidth, this->imageHeight);
        }
    }
}

void FilmStripKnob::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

}

void FilmStripKnob::resize(int width, int height) {
    this->imageWidth = width;
    this->imageHeight = height;
    this->fullStrip = fullStrip.rescaled(this->imageWidth, this->imageHeight * this->numFrames, juce::Graphics::highResamplingQuality);
}

void FilmStripKnob::setFilename(juce::String filename) {
    this->filename = filename;
}
juce::String FilmStripKnob::getFilename() {
    return this->filename;
}

void FilmStripKnob::setImage(juce::Image image) {
    this->fullStrip = image;
}

juce::Image FilmStripKnob::getImage() {
    return this->fullStrip;
}

int FilmStripKnob::getNumFrames() {
    return this->numFrames;
}

bool FilmStripKnob::isHorizontal() {
    return this->horizontal;
}

void FilmStripKnob::setWidth(int width) {
    this->width = width;
}

int FilmStripKnob::getWidth() {
    return this->width;
}

void FilmStripKnob::setHeight(int height) {
    this->height = height;
}

int FilmStripKnob::getHeight() {
    return this->height;
}

void FilmStripKnob::setImageWidth(int imageWidth) {
    this->imageWidth = imageWidth;
}

int FilmStripKnob::getImageWidth() {
    return this->imageWidth;
}

void FilmStripKnob::setImageHeight(int imageHeight) {
    this->imageHeight = imageHeight;
}

int FilmStripKnob::getImageHeight() {
    return this->imageHeight;
}

juce::String FilmStripKnob::toString() {
    return "FilmStripKnob [filename = " + this->filename + " numFrames = " + std::to_string(this->numFrames) + " horizontal = " + std::to_string(this->horizontal) + " width = " + std::to_string(this->width) + " height = " + std::to_string(this->height) + " imageWidth = " + std::to_string(this->imageWidth) + " imageHeight = " + std::to_string(this->imageHeight);
}


