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
    setSize (1000, 400);
    startTimerHz(10);
}

OldguitaristAudioProcessorEditor::~OldguitaristAudioProcessorEditor()
{
}

//==============================================================================
void OldguitaristAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.setColour(Colours::black);
    g.fillAll();
    //background = ImageCache::getFromMemory(BinaryData::Old_guitarist_chicago_jpg, BinaryData::Old_guitarist_chicago_jpgSize);
    //g.drawImageWithin(background, 0, 0, getWidth(), getHeight(), RectanglePlacement::stretchToFit);

    g.setColour(Colours::white);

    for (int i = 0; i < audioProcessor.voice->numNodes - 1; i++) {
        float dx = getWidth() / (audioProcessor.voice->numNodes - 1);
        float dy = getHeight() / 2;

        //DBG(audioProcessor.voice->currentState[i]);

        g.drawLine(i * dx, -audioProcessor.voice->currentState[i] + getHeight() / 2, (i + 1) * dx, -audioProcessor.voice->currentState[i + 1] * dy + getHeight() / 2);
    }

}

void OldguitaristAudioProcessorEditor::timerCallback() {
    repaint();
}

void OldguitaristAudioProcessorEditor::resized()
{
}
