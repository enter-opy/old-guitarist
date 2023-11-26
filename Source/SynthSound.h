/*
  ==============================================================================

    SynthSound.h
    Created: 10 Nov 2023 7:51:17pm
    Author:  vivek

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"

class SynthSound : public SynthesiserSound {
public:
    bool appliesToNote(int midiNoteNumber) {
        return true;
    }

    bool appliesToChannel(int midiChannel) {
        return true;
    }
};