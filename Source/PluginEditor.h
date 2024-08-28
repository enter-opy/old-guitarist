/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class OldguitaristAudioProcessorEditor  : public juce::AudioProcessorEditor,
    public Timer
{
public:
    OldguitaristAudioProcessorEditor (OldguitaristAudioProcessor&);
    ~OldguitaristAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

    void timerCallback(void) override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    OldguitaristAudioProcessor& audioProcessor;

    Image background;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OldguitaristAudioProcessorEditor)
};
