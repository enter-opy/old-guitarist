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
}
