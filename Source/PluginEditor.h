#pragma once
#include <JuceHeader.h>
#include "PluginProcessor.h"

class RockverbAudioProcessorEditor : public juce::AudioProcessorEditor
{
public:
    explicit RockverbAudioProcessorEditor (RockverbAudioProcessor&);
    ~RockverbAudioProcessorEditor() override = default;
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    RockverbAudioProcessor& processor;
    juce::Label title;
    juce::Slider mix, decay, size, damping, width, predelay;
    juce::Label labels[6];
    using Attachment = juce::AudioProcessorValueTreeState::SliderAttachment;
    std::unique_ptr<Attachment> mixAttach, decayAttach, sizeAttach, dampingAttach, widthAttach, predelayAttach;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (RockverbAudioProcessorEditor)
};
