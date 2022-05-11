/*
  ==============================================================================

    ImageButton.cpp
    Created: 10 May 2022 3:21:55pm
    Author:  proje

  ==============================================================================
*/

#include <JuceHeader.h>
#include "ImageButton.h"

ImageButton::ImageButton() {}

ImageButton::ImageButton(juce::String filename, juce::Image fullStrip, int width, int height)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    this->filename = filename;
    this->fullStrip = fullStrip;
    this->width = width;
    this->height = height;

    //the width and height of the image is calculated from the given image strip and might be different than the bounds of the component.
    if (!this->fullStrip.isNull()) {
        this->imageHeight = fullStrip.getHeight() / 2; // always 2 statess
        this->imageWidth = fullStrip.getWidth();
    }
}

ImageButton::~ImageButton()
{
}

void ImageButton::paint(juce::Graphics& g)
{
    if (!fullStrip.isNull()) {
        int x = (this->width - this->imageWidth) / 2;
        int y = (this->height - this->imageHeight) / 2;
        if (getToggleState()) {
            g.drawImage(this->fullStrip, 0, 0, getWidth(), getHeight(), 0, 0, this->imageWidth, this->imageHeight);
        }
        else {
            g.drawImage(this->fullStrip, 0, 0, getWidth(), getHeight(), 0, 20, this->imageWidth, this->imageHeight);
        }
    }
}

void ImageButton::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

}

void ImageButton::resize(int width, int height) {
    this->imageWidth = width;
    this->imageHeight = height;
    this->fullStrip = fullStrip.rescaled(this->imageWidth, this->imageHeight * 2, juce::Graphics::highResamplingQuality);
}

void ImageButton::setFilename(juce::String filename) {
    this->filename = filename;
}
juce::String ImageButton::getFilename() {
    return this->filename;
}

void ImageButton::setImage(juce::Image image) {
    this->fullStrip = image;
}

juce::Image ImageButton::getImage() {
    return this->fullStrip;
}

void ImageButton::setWidth(int width) {
    this->width = width;
}

int ImageButton::getWidth() {
    return this->width;
}

void ImageButton::setHeight(int height) {
    this->height = height;
}

int ImageButton::getHeight() {
    return this->height;
}

void ImageButton::setImageWidth(int imageWidth) {
    this->imageWidth = imageWidth;
}

int ImageButton::getImageWidth() {
    return this->imageWidth;
}

void ImageButton::setImageHeight(int imageHeight) {
    this->imageHeight = imageHeight;
}

int ImageButton::getImageHeight() {
    return this->imageHeight;
}

juce::String ImageButton::toString() {
    return "ImageButton [filename = " + this->filename + " width = " + std::to_string(this->width) + " height = " + std::to_string(this->height) + " imageWidth = " + std::to_string(this->imageWidth) + " imageHeight = " + std::to_string(this->imageHeight);
}
