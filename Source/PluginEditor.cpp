/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
OldguitaristAudioProcessorEditor::OldguitaristAudioProcessorEditor (OldguitaristAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (340, 500);
}

OldguitaristAudioProcessorEditor::~OldguitaristAudioProcessorEditor()
{
}

//==============================================================================
void OldguitaristAudioProcessorEditor::paint (juce::Graphics& g)
{
    background = ImageCache::getFromMemory(BinaryData::Old_guitarist_chicago_jpg, BinaryData::Old_guitarist_chicago_jpgSize);
    g.drawImageWithin(background, 0, 0, getWidth(), getHeight(), RectanglePlacement::stretchToFit);
}

void OldguitaristAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}
