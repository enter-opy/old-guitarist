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

class SynthVoice :public SynthesiserVoice {
public:

    bool canPlaySound(SynthesiserSound* sound) override {
        return dynamic_cast<SynthSound*>(sound) != nullptr;
    }

    void startNote(int midiNoteNumber, float velocity, SynthesiserSound* sound, int currentPitchWheelPosition) override {
        float frequency = MidiMessage::getMidiNoteInHertz(midiNoteNumber);

        float dyLeft = yElongation / xPluck;
        float dyRight = yElongation / (length - xPluck);


        if (frequency < 220.0) {
            c = 2 * frequency * length;

            for (int i = 0; i < numNodes; i++) {

                float x = i * dx;

                if (x < xPluck) {
                    currentState[i] = dyLeft * x;
                }
                else {
                    currentState[i] = dyRight * (length - x);
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
        
        /*for (int sample = startSample; sample < numSamples; sample++) {

            fill(std::begin(nextState), std::end(nextState), 0);

            for (int i = 2; i < numNodes - 2; i++) {
                float t0 = 1.0 / (1.0 / (c * c * dt * dt) + gamma / (2 * dt));
                float t1 = 1.0 / (dx * dx) * (currentState[i - 1] - 2 * currentState[i] + currentState[i + 1]);
                float t2 = 1.0 / (c * c * dt * dt) * (previousState[i] - 2 * currentState[i]);
                float t3 = gamma / (2 * dt) * previousState[i];
                float t4 = (stiffness * stiffness) / pow(dx, 4) * (currentState[i - 2] - 4 * currentState[i - 1] + 6 * currentState[i] - 4 * currentState[i + 1] + currentState[i + 2]);

                nextState[i] = t0 * (t1 - t2 + t3 - t4);
            }

            float amplitude = currentState[(int)numNodes / 2];

            for (int channel = 0; channel < outputBuffer.getNumChannels(); channel++) {
                outputBuffer.addSample(channel, sample, amplitude);
            }

            copy(std::begin(currentState), end(currentState), begin(previousState));
            copy(std::begin(nextState), end(nextState), begin(currentState));
        }*/
    }

    static const int numNodes = 101;
    float previousState[numNodes] = { 0 };
    float currentState[numNodes] = { 0 };
    float nextState[numNodes] = { 0 };

private:

    float sampleRate = getSampleRate();

    float length = 0.6477;
    float gamma = 0.0001;
    float stiffness = 0.0005;

    float dt = 1.0 / sampleRate;
    float dx = length / (numNodes - 1);

    float c = 308;

    float yElongation = 0.5;
    float xPluck = length / 4;
    
};