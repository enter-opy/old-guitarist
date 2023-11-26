/*
  ==============================================================================

    SynthVoice.h
    Created: 10 Nov 2023 7:51:29pm
    Author:  vivek

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"
#include "SynthSound.h"

using namespace std;

#define N 101

class SynthVoice :public SynthesiserVoice {
public:

    bool canPlaySound(SynthesiserSound* sound) override {
        return dynamic_cast<SynthSound*>(sound) != nullptr;
    }

    void startNote(int midiNoteNumber, float velocity, SynthesiserSound* sound, int currentPitchWheelPosition) override {
        float frequency = MidiMessage::getMidiNoteInHertz(midiNoteNumber);

        if (frequency < 220.0) {
            c = 2 * frequency * length;

            for (int i = 0; i < N; i++) {
                currentState[i] = i * dx;

                if (currentState[i] > 1.0) {
                    currentState[i] = 2.0 - currentState[i];
                }
            }

            copy(std::begin(currentState), end(currentState), begin(previousState));
        }
    }

    void stopNote(float velocity, bool allowTailOff) override {
        clearCurrentNote();
    }

    void pitchWheelMoved(int newPitchWheelValue) override {

    }

    void controllerMoved(int controllerNumber, int newControllerValue) override {

    }

    void renderNextBlock(AudioBuffer<float>& outputBuffer, int startSample, int numSamples) override {
        
        for (int sample = startSample; sample < numSamples; sample++) {

            fill(std::begin(nextState), std::end(nextState), 0);

            for (int i = 2; i < N - 2; i++) {
                float t0 = 1.0 / (1.0 / (c * c * dt * dt) + gamma / (2 * dt));
                float t1 = 1.0 / (dx * dx) * (currentState[i - 1] - 2 * currentState[i] + currentState[i + 1]);
                float t2 = 1.0 / (c * c * dt * dt) * (previousState[i] - 2 * currentState[i]);
                float t3 = gamma / (2 * dt) * previousState[i];
                float t4 = (stiffness * stiffness) / pow(dx, 4) * (currentState[i - 2] - 4 * currentState[i - 1] + 6 * currentState[i] - 4 * currentState[i + 1] + currentState[i + 2]);

                nextState[i] = t0 * (t1 - t2 + t3 - t4);
            }

            float amplitude = 0.0;
            for (int n = 0; n < 10; n++) {
                for (int i = 0; i < N; i++) {
                    amplitude += currentState[i] * sin(n * float_Pi * i / N);
                }
            }

            amplitude /= N;

            //DBG(amplitude);

            for (int channel = 0; channel < outputBuffer.getNumChannels(); channel++) {
                outputBuffer.addSample(channel, sample, amplitude);
            }

            copy(std::begin(currentState), end(currentState), begin(previousState));
            copy(std::begin(nextState), end(nextState), begin(currentState));
        }
    }

private:
    float sampleRate = getSampleRate();

    float length = 0.6477;
    float gamma = 0.0001;
    float stiffness = 0.0005;

    float dt = 1.0 / sampleRate;
    float dx = length / (N - 1);

    float c = 308;

    float previousState[N] = { 0 };
    float currentState[N] = { 0 };

    float nextState[N];
};