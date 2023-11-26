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

    synth.clearVoices();

    for (int i = 0; i < 1; i++) {
        synth.addVoice(new SynthVoice());
    }

    synth.clearSounds();
    synth.addSound(new SynthSound());
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
    ignoreUnused(samplesPerBlock);
    synth.setCurrentPlaybackSampleRate(sampleRate);

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
    
    buffer.clear();

    for (int i = 0; i < synth.getNumVoices(); i++) {
        if ((voice = dynamic_cast<SynthVoice*>(synth.getVoice(i)))) {
            
        }
    }

    synth.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());

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
