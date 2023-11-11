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

#define N 101

class SynthVoice :public SynthesiserVoice {
public:
    void prepareToPlay() {

    }

    bool canPlaySound(SynthesiserSound* sound) override {
        return dynamic_cast<SynthSound*>(sound) != nullptr;
    }

    void startNote(int midiNoteNumber, float velocity, SynthesiserSound* sound, int currentPitchWheelPosition) override {
        frequency = MidiMessage::getMidiNoteInHertz(midiNoteNumber);
    }

    void stopNote(float velocity, bool allowTailOff) override {

    }

    void pitchWheelMoved(int newPitchWheelValue) override {

    }

    void controllerMoved(int controllerNumber, int newControllerValue) override {

    }

    void renderNextBlock(AudioBuffer<float>& outputBuffer, int startSample, int numSamples) override {
        //DBG("new buffer");
        
        for (int sample = startSample; sample < numSamples; sample++) {

            std::fill(std::begin(nextState), std::end(nextState), 0);

            for (int i = 2; i < N - 2; i++) {
                float t0 = 1.0 / (1.0 / (c * c * dt * dt) + gamma / (2 * dt));
                float t1 = 1.0 / (dx * dx) * (currentState[i - 1] - 2 * currentState[i] + currentState[i + 1]);
                float t2 = 1.0 / (c * c * dt * dt) * (previousState[i] - 2 * currentState[i]);
                float t3 = gamma / (2 * dt) * previousState[i];
                float t4 = (stiffness * stiffness) / pow(dx, 4) * (currentState[i - 2] - 4 * currentState[i - 1] + 6 * currentState[i] - 4 * currentState[i + 1] + currentState[i + 2]);

                nextState[i] = t0 * (t1 - t2 + t3 - t4);
            }
            
            std::fill(std::begin(magnitudes), std::end(magnitudes), 0);
            for (int n = 0; n < 10; n++) {
                sum = 0.0;
                for (int i = 0; i < N; i++) {
                    sum += currentState[i] * sin(n * float_Pi * i / N);
                }
                magnitudes[n] = sum;
            }

            sound = 0.0;
            for (int n = 0; n < 10; n++) {
                sound += magnitudes[n] / N;
            }
            DBG("new");
            DBG(sound);

            for (int channel = 0; channel < outputBuffer.getNumChannels(); channel++) {
                outputBuffer.addSample(channel, sample, sound);
            }

            std::copy(std::begin(currentState), std::end(currentState), std::begin(previousState));
            std::copy(std::begin(nextState), std::end(nextState), std::begin(currentState));
        }
    }

private:
    float sum;

    float sound;

    float sampleRate = getSampleRate();

    float length = 0.7;
    float gamma = 0.00005;
    float stiffness = 0.00005;

    float dx = length / (N - 1);
    float dt = 1.0 / sampleRate;

    float s[10][N];

    const float initialState[N] = {
        0.0, 0.01449275, 0.02898551, 0.04347826, 0.05797101, 0.07246377, 0.08695652, 0.10144928, 0.11594203, 0.13043478,
        0.14492754, 0.15942029, 0.17391304, 0.1884058, 0.20289855, 0.2173913, 0.23188406, 0.24637681, 0.26086957, 0.27536232,
        0.28985507, 0.30434783, 0.31884058, 0.33333333, 0.34782609, 0.36231884, 0.37681159, 0.39130435, 0.4057971, 0.42028986,
        0.43478261, 0.44927536, 0.46376812, 0.47826087, 0.49275362, 0.50724638, 0.52173913, 0.53623188, 0.55072464, 0.56521739,
        0.57971014, 0.5942029, 0.60869565, 0.62318841, 0.63768116, 0.65217391, 0.66666667, 0.68115942, 0.69565217, 0.71014493,
        0.72463768, 0.73913043, 0.75362319, 0.76811594, 0.7826087, 0.79710145, 0.8115942, 0.82608696, 0.84057971, 0.85507246,
        0.86956522, 0.88405797, 0.89855072, 0.91304348, 0.92753623, 0.94202899, 0.95652174, 0.97101449, 0.98550725, 1.0,
        1.0, 0.96666667, 0.93333333, 0.9, 0.86666667, 0.83333333, 0.8, 0.76666667, 0.73333333, 0.7,
        0.66666667, 0.63333333, 0.6, 0.56666667, 0.53333333, 0.5, 0.46666667, 0.43333333, 0.4, 0.36666667,
        0.33333333, 0.3, 0.26666667, 0.23333333, 0.2, 0.16666667, 0.13333333, 0.1, 0.06666667, 0.03333333,
        0.0
    };

    float previousState[N] = {
        0.0, 0.01449275, 0.02898551, 0.04347826, 0.05797101, 0.07246377, 0.08695652, 0.10144928, 0.11594203, 0.13043478,
        0.14492754, 0.15942029, 0.17391304, 0.1884058, 0.20289855, 0.2173913, 0.23188406, 0.24637681, 0.26086957, 0.27536232,
        0.28985507, 0.30434783, 0.31884058, 0.33333333, 0.34782609, 0.36231884, 0.37681159, 0.39130435, 0.4057971, 0.42028986,
        0.43478261, 0.44927536, 0.46376812, 0.47826087, 0.49275362, 0.50724638, 0.52173913, 0.53623188, 0.55072464, 0.56521739,
        0.57971014, 0.5942029, 0.60869565, 0.62318841, 0.63768116, 0.65217391, 0.66666667, 0.68115942, 0.69565217, 0.71014493,
        0.72463768, 0.73913043, 0.75362319, 0.76811594, 0.7826087, 0.79710145, 0.8115942, 0.82608696, 0.84057971, 0.85507246,
        0.86956522, 0.88405797, 0.89855072, 0.91304348, 0.92753623, 0.94202899, 0.95652174, 0.97101449, 0.98550725, 1.0,
        1.0, 0.96666667, 0.93333333, 0.9, 0.86666667, 0.83333333, 0.8, 0.76666667, 0.73333333, 0.7,
        0.66666667, 0.63333333, 0.6, 0.56666667, 0.53333333, 0.5, 0.46666667, 0.43333333, 0.4, 0.36666667,
        0.33333333, 0.3, 0.26666667, 0.23333333, 0.2, 0.16666667, 0.13333333, 0.1, 0.06666667, 0.03333333,
        0.0
    };
    float currentState[N] = {
        0.0, 0.01449275, 0.02898551, 0.04347826, 0.05797101, 0.07246377, 0.08695652, 0.10144928, 0.11594203, 0.13043478,
        0.14492754, 0.15942029, 0.17391304, 0.1884058, 0.20289855, 0.2173913, 0.23188406, 0.24637681, 0.26086957, 0.27536232,
        0.28985507, 0.30434783, 0.31884058, 0.33333333, 0.34782609, 0.36231884, 0.37681159, 0.39130435, 0.4057971, 0.42028986,
        0.43478261, 0.44927536, 0.46376812, 0.47826087, 0.49275362, 0.50724638, 0.52173913, 0.53623188, 0.55072464, 0.56521739,
        0.57971014, 0.5942029, 0.60869565, 0.62318841, 0.63768116, 0.65217391, 0.66666667, 0.68115942, 0.69565217, 0.71014493,
        0.72463768, 0.73913043, 0.75362319, 0.76811594, 0.7826087, 0.79710145, 0.8115942, 0.82608696, 0.84057971, 0.85507246,
        0.86956522, 0.88405797, 0.89855072, 0.91304348, 0.92753623, 0.94202899, 0.95652174, 0.97101449, 0.98550725, 1.0,
        1.0, 0.96666667, 0.93333333, 0.9, 0.86666667, 0.83333333, 0.8, 0.76666667, 0.73333333, 0.7,
        0.66666667, 0.63333333, 0.6, 0.56666667, 0.53333333, 0.5, 0.46666667, 0.43333333, 0.4, 0.36666667,
        0.33333333, 0.3, 0.26666667, 0.23333333, 0.2, 0.16666667, 0.13333333, 0.1, 0.06666667, 0.03333333,
        0.0
    };

    float nextState[N];

    float magnitudes[10];

    float frequency = 220.0;
    float c = 2 * frequency * length;
};