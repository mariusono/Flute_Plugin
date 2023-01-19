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
class Flute_Plugin_TryAudioProcessorEditor  : public juce::AudioProcessorEditor,
                                              public Slider::Listener
//                                              public juce::KeyListener,
//public juce::MidiKeyboardStateListener
{
public:
    Flute_Plugin_TryAudioProcessorEditor (Flute_Plugin_TryAudioProcessor&);
    ~Flute_Plugin_TryAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    
    void sliderValueChanged (Slider* slider) override;
    
//    virtual bool keyPressed(const KeyPress& key, Component* originatingComponent) override;
//    virtual bool keyStateChanged(bool isKeyDown, Component* originatingComponent) override;
//
//    virtual void handleNoteOn (MidiKeyboardState* source,
//                               int midiChannel, int midiNoteNumber, float velocity) override;
//    virtual void handleNoteOff (MidiKeyboardState* source,
//                               int midiChannel, int midiNoteNumber, float velocity) override;
    
private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    Flute_Plugin_TryAudioProcessor& audioProcessor;
    
//    juce::MidiKeyboardState keyboardState;
//    juce::MidiKeyboardComponent keyboardComponent;
//    double startTime;
    
//    juce::ComboBox midiInputList;
//    juce::Label midiInputListLabel;

    
    // Text label for debug
    juce::Label infoLabel;

    
    // SLIDER STUFF

    Slider frequencySlider;
    Label frequencyLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> freqSliderAttachment; // alwats define below the Slider and Label instantiation

    Slider biquadSlider;
    Label biquadLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> biquadSliderAttachment; // alwats define below the Slider and Label instantiation

    Slider vibratoSlider;
    Label vibratoLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> vibratoSliderAttachment; // alwats define below the Slider and Label instantiation

    Slider freq_vibratoSlider;
    Label freq_vibratoLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> freq_vibratoSliderAttachment; // alwats define below the Slider and Label instantiation

    Slider durSlider;
    Label durLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> durSliderAttachment; // alwats define below the Slider and Label instantiation

    Slider pitchSlider;
    Label pitchLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> pitchSliderAttachment; // alwats define below the Slider and Label instantiation
    

    std::vector<char> keys = { 'A', 'W', 'S', 'E', 'D', 'F', 'T', 'G', 'Y', 'H', 'U', 'J', 'K' };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Flute_Plugin_TryAudioProcessorEditor)
};
