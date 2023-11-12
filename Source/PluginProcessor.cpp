/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
OldguitaristAudioProcessor::OldguitaristAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ),
    treeState(*this, nullptr, "PARAMETER", {

        })
#endif
{
    treeState.state = ValueTree("savedParams");
}

OldguitaristAudioProcessor::~OldguitaristAudioProcessor()
{
}

//==============================================================================
const juce::String OldguitaristAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool OldguitaristAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool OldguitaristAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool OldguitaristAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double OldguitaristAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int OldguitaristAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int OldguitaristAudioProcessor::getCurrentProgram()
{
    return 0;
}

void OldguitaristAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String OldguitaristAudioProcessor::getProgramName (int index)
{
    return {};
}

void OldguitaristAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void OldguitaristAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    this->sampleRate = sampleRate;

    length = c / (2 * frequency);
    gamma = 0.00005;
    stiffness = 0.00005;

    dx = length / (N - 1);
    dt = 1.0 / sampleRate;

    spec.maximumBlockSize = samplesPerBlock;
    spec.sampleRate = sampleRate;
    spec.numChannels = getMainBusNumOutputChannels();

    convolution.reset();
    convolution.loadImpulseResponse(BinaryData::impulse_response_wav, BinaryData::impulse_response_wavSize, dsp::Convolution::Stereo::yes, dsp::Convolution::Trim::no, 0);
    convolution.prepare(spec);
}

void OldguitaristAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool OldguitaristAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void OldguitaristAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (const auto midiMessage : midiMessages)
    {
        const auto midiEvent = midiMessage.getMessage();

        if (midiEvent.isNoteOn())
        {
            frequency = MidiMessage::getMidiNoteInHertz(midiEvent.getNoteNumber()) / 2.0;
            length = c / (2 * frequency);
            dx = length / (N - 1);

            copy(begin(initialState), end(initialState), begin(previousState));
            copy(begin(initialState), end(initialState), begin(currentState));
        }
    }
    
    buffer.clear();

    for (int sample = 0; sample < buffer.getNumSamples(); sample++) {
        if (!midiMessages.isEmpty() && sample == midiMessages.getFirstEventTime()) {
            copy(begin(initialState), end(initialState), begin(previousState));
            copy(begin(initialState), end(initialState), begin(currentState));
        }

        fill(std::begin(nextState), std::end(nextState), 0);

        for (int i = 2; i < N - 2; i++) {
            float t0 = 1.0 / (1.0 / (c * c * dt * dt) + gamma / (2 * dt));
            float t1 = 1.0 / (dx * dx) * (currentState[i - 1] - 2 * currentState[i] + currentState[i + 1]);
            float t2 = 1.0 / (c * c * dt * dt) * (previousState[i] - 2 * currentState[i]);
            float t3 = gamma / (2 * dt) * previousState[i];
            float t4 = (stiffness * stiffness) / pow(dx, 4) * (currentState[i - 2] - 4 * currentState[i - 1] + 6 * currentState[i] - 4 * currentState[i + 1] + currentState[i + 2]);

            nextState[i] = t0 * (t1 - t2 + t3 - t4);
        }

        amplitude = 0.0;
        for (int n = 0; n < 10; n++) {
            for (int i = 0; i < N; i++) {
                amplitude += currentState[i] * sin(n * float_Pi * i / N);
            }
        }
        
        amplitude /= N;


        for (int channel = 0; channel < buffer.getNumChannels(); channel++) {
            buffer.addSample(channel, sample, amplitude);
        }

        copy(std::begin(currentState), end(currentState), begin(previousState));
        copy(std::begin(nextState), end(nextState), begin(currentState));
    }

    dsp::AudioBlock<float> block {buffer};

    convolution.process(dsp::ProcessContextReplacing<float>(block));
}

//==============================================================================
bool OldguitaristAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* OldguitaristAudioProcessor::createEditor()
{
    return new OldguitaristAudioProcessorEditor (*this);
}

//==============================================================================
void OldguitaristAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
}

void OldguitaristAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new OldguitaristAudioProcessor();
}
