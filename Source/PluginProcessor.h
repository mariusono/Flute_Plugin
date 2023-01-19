/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "flute.h"

//==============================================================================
/**
*/
class Flute_Plugin_TryAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    Flute_Plugin_TryAudioProcessor();
    ~Flute_Plugin_TryAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    
    
    //==============================================================================
    void setIpress(double val) { ipress = val; }
    void setF_Biquad(double val) { f_biquad = val;}
    void setVibrato(double val) { vibrato = val; }
    void setFreq_vibrato(double val) { freq_vibrato = val; }
    void setRelease_time(double val) { release_time = val; }
    void setPitchBend(double val) { pitchBend_val = val; }

    double getIpress() {return ipress;}
    double getF_biquad() {return f_biquad;}
    double getVibrato() {return vibrato;}
    double getFreq_vibrato() {return freq_vibrato;}
    double getRelease_time() {return release_time;}
    double getPitchBend() {return pitchBend_val;}

    void setOctave(int val) { octave = val; }
    int getOctave() { return octave; }
    
    void activateFlute(int val) {fluteNotes[val]->activate();}

    std::vector<bool> keyDownVec; // make it global so it's accessible outside the class ? 
    std::vector<std::unique_ptr<flute>> fluteNotes;
    
    MidiMessage msg;
    int sampleNumber;

    juce::AudioProcessorValueTreeState apvts;
    
private:
    //==============================================================================
    
    juce::AudioProcessorValueTreeState::ParameterLayout createParameters();
    
    bool keyDown = false;

    bool graphicsToggle = true;

    double fs;

    double freq_chosen;

    double scl, pch, ipress, ibreath, ifeedback1, ifeedback2, dur, gate, vibrato, freq_vibrato, f_biquad, Q_biquad;
    
    double pitchBend_val;

//    double release_time;
    double release_time;
//    auto release_time;


    float globalAmp = 0.07 ;
    double globalCurrentSample;

    std::vector<double> preOutputVec;

    int numFlutes = 128;
    int octave = 24;
        

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Flute_Plugin_TryAudioProcessor)
};
