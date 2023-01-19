/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
//Flute_Plugin_TryAudioProcessorEditor::Flute_Plugin_TryAudioProcessorEditor (Flute_Plugin_TryAudioProcessor& p)
//    : AudioProcessorEditor (&p), audioProcessor (p), keyboardComponent (keyboardState, juce::MidiKeyboardComponent::horizontalKeyboard),
//startTime (juce::Time::getMillisecondCounterHiRes() * 0.001)

Flute_Plugin_TryAudioProcessorEditor::Flute_Plugin_TryAudioProcessorEditor (Flute_Plugin_TryAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
//     setWantsKeyboardFocus(true);
//     addKeyListener(this);
    
//     auto list = juce::MidiInput::getAvailableDevices();
//
//    addAndMakeVisible (keyboardComponent);
//    keyboardState.addListener (this);


//    addAndMakeVisible (infoLabel);
//    juce::String infoText;
//    infoLabel.setColour (juce::Label::backgroundColourId, juce::Colours::darkblue);

    // SLIDERS
//    addAndMakeVisible(frequencySlider);
//    frequencySlider.setRange(0.5, 0.99);          // [1]
//    frequencySlider.setTextValueSuffix(" [-]");     // [2]
//    frequencySlider.addListener(this);             // [3]
//
//    addAndMakeVisible(frequencyLabel);
//    frequencyLabel.setText("ipress", juce::dontSendNotification);
//    frequencyLabel.attachToComponent(&frequencySlider, true); // [4]
//
//    frequencySlider.setValue(audioProcessor.getIpress()); // [5]

    addAndMakeVisible(frequencySlider);
    frequencySlider.setTextValueSuffix(" [-]");     // [2]
    addAndMakeVisible(frequencyLabel);
    frequencyLabel.setText("Breath", juce::dontSendNotification);
    frequencyLabel.attachToComponent(&frequencySlider, true); // [4]
    
    freqSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,"IPRESS",frequencySlider);

    
    addAndMakeVisible(biquadSlider);
    biquadSlider.setTextValueSuffix(" [Hz]");     // [2]
    addAndMakeVisible(biquadLabel);
    biquadLabel.setText("Frequency", juce::dontSendNotification);
    biquadLabel.attachToComponent(&biquadSlider, true); // [4]
    biquadSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,"F_LPF",biquadSlider);
    
    
    addAndMakeVisible(vibratoSlider);
    vibratoSlider.setTextValueSuffix(" [-]");     // [2]
    addAndMakeVisible(vibratoLabel);
    vibratoLabel.setText("Vibrato", juce::dontSendNotification);
    vibratoLabel.attachToComponent(&vibratoSlider, true); // [4]
    vibratoSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,"VIBRATO",vibratoSlider);
    
    
    addAndMakeVisible(freq_vibratoSlider);
    freq_vibratoSlider.setTextValueSuffix(" [Hz]");     // [2]
    addAndMakeVisible(freq_vibratoLabel);
    freq_vibratoLabel.setText("Rate", juce::dontSendNotification);
    freq_vibratoLabel.attachToComponent(&freq_vibratoSlider, true); // [4]
    freq_vibratoSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,"F_VIBRATO",freq_vibratoSlider);
    
    addAndMakeVisible(durSlider);
    durSlider.setTextValueSuffix(" [s]");     // [2]
    addAndMakeVisible(durLabel);
    durLabel.setText("Release", juce::dontSendNotification);
    durLabel.attachToComponent(&durSlider, true); // [4]
    
    durSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,"RELEASE_TIME",durSlider);
    
    

//    addAndMakeVisible(pitchSlider);
//    pitchSlider.setRange(-12.0, 12.0);          // [1]
//    pitchSlider.setTextValueSuffix(" [note]");     // [2]
//    pitchSlider.addListener(this);             // [3]
//    pitchSlider.setValue(audioProcessor.getPitchBend()); // [5]
//
//    addAndMakeVisible(pitchLabel);
//    pitchLabel.setText("pitch", juce::dontSendNotification);
//    pitchLabel.attachToComponent(&pitchSlider, true); // [4]
    
    
    addAndMakeVisible(pitchSlider);
    pitchSlider.setTextValueSuffix(" [note]");     // [2]
    addAndMakeVisible(pitchLabel);
    pitchLabel.setText("Pitch", juce::dontSendNotification);
    pitchLabel.attachToComponent(&pitchSlider, true); // [4]
    
    pitchSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,"PITCH",pitchSlider);
    
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 300);
}

Flute_Plugin_TryAudioProcessorEditor::~Flute_Plugin_TryAudioProcessorEditor()
{
}

//==============================================================================
void Flute_Plugin_TryAudioProcessorEditor::paint (juce::Graphics& g)
{
//    // (Our component is opaque, so we must completely fill the background with a solid colour)
//    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
//
//    g.setColour (juce::Colours::white);
//    g.setFont (15.0f);
//    g.drawFittedText ("Hello World!", getLocalBounds(), juce::Justification::centred, 1);
}

void Flute_Plugin_TryAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    
    auto sliderLeft = 120;
    
//    infoLabel.setBounds (10, 110, getWidth() - 20, getHeight() - 120); // this is for live debugging in the plugin..

//    auto area = getLocalBounds();
    
    frequencySlider.setBounds(sliderLeft, 20, getWidth() - sliderLeft - 10, 20);
    biquadSlider.setBounds(sliderLeft, 50, getWidth() - sliderLeft - 10, 20);
    vibratoSlider.setBounds(sliderLeft, 80, getWidth() - sliderLeft - 10, 20);
    freq_vibratoSlider.setBounds(sliderLeft, 110, getWidth() - sliderLeft - 10, 20);
    durSlider.setBounds(sliderLeft, 140, getWidth() - sliderLeft - 10, 20);
    pitchSlider.setBounds(sliderLeft, 170, getWidth() - sliderLeft - 10, 20);

//    keyboardComponent.setBounds (area.removeFromTop (80).reduced(8));
//    keyboardComponent.setBounds (10, 170, getWidth() - 10, 70);
}

void Flute_Plugin_TryAudioProcessorEditor::sliderValueChanged(Slider* slider)
{

    if (slider == &frequencySlider)
    {
        audioProcessor.setIpress(frequencySlider.getValue());
        DBG(String(frequencySlider.getValue()));
        
//        infoLabel.setText (String(frequencySlider.getValue()), juce::dontSendNotification);
        
//        infoLabel.setText (audioProcessor.msg.getDescription(), juce::dontSendNotification);

//        ipress = frequencySlider.getValue();
    }
    else if (slider == &biquadSlider)
    {
        audioProcessor.setF_Biquad(biquadSlider.getValue());
//        f_biquad = biquadSlider.getValue();
    }
    else if (slider == &vibratoSlider)
    {
        audioProcessor.setVibrato(vibratoSlider.getValue());
//        vibrato = vibratoSlider.getValue();
    }
    else if (slider == &freq_vibratoSlider)
    {
        audioProcessor.setFreq_vibrato(freq_vibratoSlider.getValue());
//        freq_vibrato = freq_vibratoSlider.getValue();
    }
    else if (slider == &durSlider)
    {
        audioProcessor.setRelease_time(durSlider.getValue());
//        release_time = durSlider.getValue();
    }
    else if (slider == &pitchSlider)
    {
        audioProcessor.setPitchBend(pitchSlider.getValue());
    }
}
//
//void Flute_Plugin_TryAudioProcessorEditor::handleNoteOn (MidiKeyboardState* source,
//                           int midiChannel, int midiNoteNumber, float velocity)
//{
//    auto m = juce::MidiMessage::noteOn (midiChannel, midiNoteNumber, velocity);
//    m.setTimeStamp (juce::Time::getMillisecondCounterHiRes() * 0.001);
//
//    int idx = m.getNoteNumber();
////    DBG(String(idx));
//    audioProcessor.fluteNotes[idx]->activate();
//    if (!audioProcessor.keyDownVec[idx])
//    {
//        audioProcessor.fluteNotes[idx]->re_init_Flute_filterStates();
//        audioProcessor.fluteNotes[idx]->flag_stop_off();
//        audioProcessor.keyDownVec[idx] = true;
//    }
//
//}
//
//void Flute_Plugin_TryAudioProcessorEditor::handleNoteOff (MidiKeyboardState* source,
//                           int midiChannel, int midiNoteNumber, float velocity)
//{
//    auto m = juce::MidiMessage::noteOff (midiChannel, midiNoteNumber, velocity);
//    m.setTimeStamp (juce::Time::getMillisecondCounterHiRes() * 0.001);
//
////    DBG(String(m.getNoteNumber()));
//
//    int idx = m.getNoteNumber();
//
//    if (audioProcessor.keyDownVec[idx])
//    {
//        audioProcessor.fluteNotes[idx]->flag_stop_on();
//        audioProcessor.keyDownVec[idx] = false;
//        //Logger::getCurrentLogger()->outputDebugString("key" + String(k) + "released ");
//    }
//
//}
//
//
//bool Flute_Plugin_TryAudioProcessorEditor::keyStateChanged(bool isKeyDown, Component* originatingComponent)
//{
//    for (int i = 0; i < 13; i++)
//    {
//        char k = keys[i];
//        int idx = i + audioProcessor.getOctave();
//        if (KeyPress::isKeyCurrentlyDown(k))
//        {
//            audioProcessor.fluteNotes[idx]->activate();
////            audioProcessor.activateFlute(idx);
//
//
//            if (!audioProcessor.keyDownVec[idx])
//            {
//                audioProcessor.fluteNotes[idx]->re_init_Flute_filterStates();
//                audioProcessor.fluteNotes[idx]->flag_stop_off();
//
////                double dur_check = audioProcessor.fluteNotes[idx]->get_dur();
//                //Logger::getCurrentLogger()->outputDebugString("dur_check: (" + String(dur_check) + ")");
//                //Logger::getCurrentLogger()->outputDebugString("key" + String(k) + "pressed once");
//                audioProcessor.keyDownVec[idx] = true;
//            }
//        }
//        else
//        {
//            if (audioProcessor.keyDownVec[idx])
//            {
//                audioProcessor.fluteNotes[idx]->flag_stop_on();
//                audioProcessor.keyDownVec[idx] = false;
//                //Logger::getCurrentLogger()->outputDebugString("key" + String(k) + "released ");
//            }
//        }
//    }
//    return false;
//}
//
//
//
//bool Flute_Plugin_TryAudioProcessorEditor::keyPressed(const juce::KeyPress& key, juce::Component* originatingComponent)
//{
//    switch (key.getKeyCode())
//    {
//    case 'Z':
////        octave = 0;
//            audioProcessor.setOctave(0);
//        break;
//    case 'X':
////        octave = 12;
//        audioProcessor.setOctave(12);
//        break;
//    case 'C':
//        audioProcessor.setOctave(24);
////        octave = 24;
//        break;
//    case 'V':
//        audioProcessor.setOctave(36);
////        octave = 36;
//        break;
//    case 'B':
////        octave = 48;
//        audioProcessor.setOctave(48);
//        break;
//    }
//    return true;
//}
//
//
//
