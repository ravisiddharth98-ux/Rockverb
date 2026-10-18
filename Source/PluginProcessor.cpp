#include "PluginProcessor.h"
#include "PluginEditor.h"

static juce::AudioProcessorValueTreeState::ParameterLayout createParameters()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> p;
    p.push_back (std::make_unique<juce::AudioParameterFloat>("mix", "Mix", juce::NormalisableRange<float>(0.0f, 1.0f), 0.25f));
    p.push_back (std::make_unique<juce::AudioParameterFloat>("decay", "Decay", juce::NormalisableRange<float>(0.1f, 10.0f, 0.01f), 2.5f));
    p.push_back (std::make_unique<juce::AudioParameterFloat>("size", "Size", juce::NormalisableRange<float>(0.0f, 1.0f), 0.65f));
    p.push_back (std::make_unique<juce::AudioParameterFloat>("damping", "Damping", juce::NormalisableRange<float>(0.0f, 1.0f), 0.35f));
    p.push_back (std::make_unique<juce::AudioParameterFloat>("width", "Width", juce::NormalisableRange<float>(0.0f, 1.0f), 1.0f));
    p.push_back (std::make_unique<juce::AudioParameterFloat>("predelay", "Pre-delay", juce::NormalisableRange<float>(0.0f, 250.0f, 1.0f), 20.0f));
    return { p.begin(), p.end() };
}

RockverbAudioProcessor::RockverbAudioProcessor()
    : AudioProcessor (BusesProperties().withInput("Input", juce::AudioChannelSet::stereo(), true)
                                      .withOutput("Output", juce::AudioChannelSet::stereo(), true)),
      parameters (*this, nullptr, "PARAMETERS", createParameters())
{
}

void RockverbAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    juce::ignoreUnused (samplesPerBlock);
    reverb.reset();
    reverb.setSampleRate (sampleRate);
}

void RockverbAudioProcessor::releaseResources() {}

void RockverbAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midi)
{
    juce::ignoreUnused (midi);
    juce::ScopedNoDenormals noDenormals;
    auto* mix = parameters.getRawParameterValue ("mix");
    auto* decay = parameters.getRawParameterValue ("decay");
    auto* size = parameters.getRawParameterValue ("size");
    auto* damping = parameters.getRawParameterValue ("damping");
    auto* width = parameters.getRawParameterValue ("width");
    auto* predelay = parameters.getRawParameterValue ("predelay");

    reverbParams.roomSize = *size;
    reverbParams.damping = *damping;
    reverbParams.wetLevel = 1.0f;
    reverbParams.dryLevel = 0.0f;
    reverbParams.width = *width;
    reverbParams.freezeMode = 0.0f;
    reverb.setParameters (reverbParams);

    // Basic reverb prototype: predelay and low/high-cut are not implemented yet.
    juce::AudioBuffer<float> dry (buffer.getNumChannels(), buffer.getNumSamples());
    dry.makeCopyOf (buffer, true);
    reverb.processStereo (buffer.getWritePointer(0),
                          buffer.getNumChannels() > 1 ? buffer.getWritePointer(1) : buffer.getWritePointer(0),
                          buffer.getNumSamples());

    const float wet = *mix;
    const float dryGain = 1.0f - wet;
    for (int ch = 0; ch < buffer.getNumChannels(); ++ch)
        for (int i = 0; i < buffer.getNumSamples(); ++i)
            buffer.setSample (ch, i, dry.getSample(ch, i) * dryGain + buffer.getSample(ch, i) * wet);

    juce::ignoreUnused (decay, predelay); // UI parameters reserved; not yet mapped to DSP.
}

juce::AudioProcessorEditor* RockverbAudioProcessor::createEditor()
{
    return new RockverbAudioProcessorEditor (*this);
}

void RockverbAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    auto state = parameters.copyState();
    std::unique_ptr<juce::XmlElement> xml (state.createXml());
    copyXmlToBinary (*xml, destData);
}

void RockverbAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xml (getXmlFromBinary (data, sizeInBytes));
    if (xml != nullptr && xml->hasTagName (parameters.state.getType()))
        parameters.replaceState (juce::ValueTree::fromXml (*xml));
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new RockverbAudioProcessor();
}
