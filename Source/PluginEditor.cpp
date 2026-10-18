#include "PluginEditor.h"

RockverbAudioProcessorEditor::RockverbAudioProcessorEditor (RockverbAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
    setSize (620, 330);
    title.setText ("ROCKVERB", juce::dontSendNotification);
    title.setJustificationType (juce::Justification::centred);
    title.setFont (juce::Font (28.0f, juce::Font::bold));
    title.setColour (juce::Label::textColourId, juce::Colour (0xffd8c8ff));
    addAndMakeVisible (title);

    juce::Slider* sliders[] = { &mix, &decay, &size, &damping, &width, &predelay };
    const char* names[] = { "MIX", "DECAY", "SIZE", "DAMPING", "WIDTH", "PRE-DELAY" };
    for (int i = 0; i < 6; ++i)
    {
        sliders[i]->setSliderStyle (juce::Slider::RotaryHorizontalVerticalDrag);
        sliders[i]->setTextBoxStyle (juce::Slider::TextBoxBelow, false, 72, 20);
        sliders[i]->setColour (juce::Slider::rotarySliderFillColourId, juce::Colour (0xffa77bff));
        sliders[i]->setColour (juce::Slider::thumbColourId, juce::Colour (0xff65d9ff));
        addAndMakeVisible (*sliders[i]);
        labels[i].setText (names[i], juce::dontSendNotification);
        labels[i].setJustificationType (juce::Justification::centred);
        labels[i].setColour (juce::Label::textColourId, juce::Colours::white);
        addAndMakeVisible (labels[i]);
    }
    auto& state = processor.parameters;
    mixAttach = std::make_unique<Attachment>(state, "mix", mix);
    decayAttach = std::make_unique<Attachment>(state, "decay", decay);
    sizeAttach = std::make_unique<Attachment>(state, "size", size);
    dampingAttach = std::make_unique<Attachment>(state, "damping", damping);
    widthAttach = std::make_unique<Attachment>(state, "width", width);
    predelayAttach = std::make_unique<Attachment>(state, "predelay", predelay);
}

void RockverbAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colour (0xff100d1c));
    juce::ColourGradient glow (juce::Colour (0xff39245c), 20.0f, 20.0f,
                               juce::Colour (0xff10101d), 600.0f, 320.0f, false);
    g.setGradientFill (glow);
    g.fillRoundedRectangle (8.0f, 8.0f, 604.0f, 314.0f, 18.0f);
    g.setColour (juce::Colour (0xff6b4d99));
    g.drawRoundedRectangle (8.0f, 8.0f, 604.0f, 314.0f, 18.0f, 1.5f);
}

void RockverbAudioProcessorEditor::resized()
{
    title.setBounds (20, 18, 580, 48);
    juce::Slider* sliders[] = { &mix, &decay, &size, &damping, &width, &predelay };
    for (int i = 0; i < 6; ++i)
    {
        int x = 24 + i * 98;
        labels[i].setBounds (x, 75, 90, 22);
        sliders[i]->setBounds (x, 98, 90, 180);
    }
}
