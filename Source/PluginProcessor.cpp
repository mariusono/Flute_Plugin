/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================
Flute_Plugin_TryAudioProcessor::Flute_Plugin_TryAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ), apvts(*this, nullptr, "Parameters", createParameters())
#endif
{
}

Flute_Plugin_TryAudioProcessor::~Flute_Plugin_TryAudioProcessor()
{
}

//==============================================================================
const juce::String Flute_Plugin_TryAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool Flute_Plugin_TryAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool Flute_Plugin_TryAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool Flute_Plugin_TryAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double Flute_Plugin_TryAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int Flute_Plugin_TryAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int Flute_Plugin_TryAudioProcessor::getCurrentProgram()
{
    return 0;
}

void Flute_Plugin_TryAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String Flute_Plugin_TryAudioProcessor::getProgramName (int index)
{
    return {};
}

void Flute_Plugin_TryAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void Flute_Plugin_TryAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
//    setWantsKeyboardFocus(true);
//    addKeyListener(this);
    
//    getTopLevelComponent()->addKeyListener(this);
//    getTopLevelComponent()->setWantsKeyboardFocus(true);
    
//    // Use this method as the place to do any pre-playback
//    // initialisation that you need..
    
    DBG("Sample Rate is " + String(sampleRate));
    
    freq_chosen = 220.0;

    fs = sampleRate;
    globalCurrentSample = 0;

//    ipress = 0.9;
    auto* selection_ipress = dynamic_cast<AudioParameterFloat*> (apvts.getParameter ("IPRESS"));
    ipress = selection_ipress->get();
    
    ibreath = 0.008;
    ifeedback1 = 0.4;
    ifeedback2 = 0.4;
    dur = 0.1;
    
    
//    release_time = 0.1;
//    auto paramVal = apvts.getRawParameterValue("RELEASE_TIME");
//    paramVal->load();
//
    
    auto* selection = dynamic_cast<AudioParameterFloat*> (apvts.getParameter ("RELEASE_TIME"));
    release_time = selection->get();
    
//    release_time = static_cast<double> (paramVal);
    
    gate = 1.0;
//    vibrato = 0.02;
    auto* selection_vibrato = dynamic_cast<AudioParameterFloat*> (apvts.getParameter ("VIBRATO"));
    vibrato = selection_vibrato->get();
    
    freq_vibrato = 5.0;
    auto* selection_freq_vibrato = dynamic_cast<AudioParameterFloat*> (apvts.getParameter ("F_VIBRATO"));
    freq_vibrato = selection_freq_vibrato->get();
    
    f_biquad = 3000;
    auto* selection_f_biquad= dynamic_cast<AudioParameterFloat*> (apvts.getParameter ("F_LPF"));
    f_biquad = selection_f_biquad->get();
    
    Q_biquad = 0.707;
    
//    pitchBend_val = 0; // goes between -12 and 12 (1 octave down and 1 octave up)
    auto* selection_pitchBend_val = dynamic_cast<AudioParameterFloat*> (apvts.getParameter ("PITCH"));
    pitchBend_val = selection_pitchBend_val->get();
    
    
//    Hertz = 440.0 * pow(2.0, (midi_note - 69)/12);
    
    int test = 0;
    for (int i = 0; i < numFlutes; ++i)
    {
        double freq_chosen = 440.0 * pow(2.0, ((float) i - 69.0)/12.0);
        fluteNotes.push_back(std::make_unique<flute>(freq_chosen, fs));
//        fluteNotes.push_back(std::make_unique<flute>(220.0 * pow(2, i / 12.0), fs));
    }

    preOutputVec.resize(numFlutes, 0);
    keyDownVec.resize(numFlutes, false);
}

void Flute_Plugin_TryAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool Flute_Plugin_TryAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void Flute_Plugin_TryAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    
    for (const auto meta : midiMessages)
    {
        // All this stuff needs to be in the midi message loop in order to react to all possible midi messages !
        msg = meta.getMessage();
        
        int idx = msg.getNoteNumber();

        if (msg.isNoteOn())
        {
            fluteNotes[idx]->activate();

            if (!keyDownVec[idx])
            {
                fluteNotes[idx]->re_init_Flute_filterStates();
                fluteNotes[idx]->flag_stop_off();
                keyDownVec[idx] = true;
            }

        }else if(msg.isNoteOff())
        {
            if (keyDownVec[idx])
            {
                fluteNotes[idx]->flag_stop_on();
                keyDownVec[idx] = false;
                //Logger::getCurrentLogger()->outputDebugString("key" + String(k) + "released ");
            }
        }
    }
    
    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
    for (int channel = 0; channel < totalNumOutputChannels; ++channel)
    {

        auto* output = buffer.getWritePointer (channel);

        // updating global model parameters every buffersize.. for efficiency. may cause some clicks
        for (int iFlute = 0; iFlute < numFlutes; ++iFlute)
        {
            auto* selection_ipress = dynamic_cast<AudioParameterFloat*> (apvts.getParameter ("IPRESS"));
            ipress = selection_ipress->get();
            
            auto* selection = dynamic_cast<AudioParameterFloat*> (apvts.getParameter ("RELEASE_TIME")); // this feels dubious to get the float this way : /
            release_time = selection->get();
            
            auto* selection_vibrato = dynamic_cast<AudioParameterFloat*> (apvts.getParameter ("VIBRATO"));
            vibrato = selection_vibrato->get();
            
            auto* selection_freq_vibrato = dynamic_cast<AudioParameterFloat*> (apvts.getParameter ("F_VIBRATO"));
            freq_vibrato = selection_freq_vibrato->get();
            
            auto* selection_f_biquad= dynamic_cast<AudioParameterFloat*> (apvts.getParameter ("F_LPF"));
            f_biquad = selection_f_biquad->get();

            auto* selection_pitchBend_val = dynamic_cast<AudioParameterFloat*> (apvts.getParameter ("PITCH"));
            pitchBend_val = selection_pitchBend_val->get();
            
            
            fluteNotes[iFlute]->set_ipress(ipress);

            fluteNotes[iFlute]->set_biquad(f_biquad, Q_biquad);
            fluteNotes[iFlute]->update_biquad(f_biquad, Q_biquad, fs);

            fluteNotes[iFlute]->set_vibrato(vibrato);
            fluteNotes[iFlute]->set_freq_vibrato_smoo(freq_vibrato);

            
            fluteNotes[iFlute]->set_release_time(release_time);
            fluteNotes[iFlute]->set_pitchBend(pitchBend_val);
        }
        
        Logger::getCurrentLogger()->outputDebugString("freq_vibrato:" + String(fluteNotes[0]->get_freqVibrato()));
        
        
        if (channel == 0)
        {
            for (int i = 0; i < buffer.getNumSamples(); ++i)
            {
                float preOutput = 0;
                
//                auto* selection_freq_vibrato = dynamic_cast<AudioParameterFloat*> (apvts.getParameter ("F_VIBRATO"));
//                freq_vibrato = selection_freq_vibrato->get();

                for (int iFlute = 0; iFlute < numFlutes; ++iFlute)
                {
                    if (fluteNotes[iFlute]->isActive())
                    {
//                        fluteNotes[iFlute]->set_freq_vibrato_smoo(freq_vibrato);

                        preOutputVec[iFlute] = fluteNotes[iFlute]->process(fs);
                        fluteNotes[iFlute]->updateStates(preOutputVec[iFlute]);

                        preOutput = preOutput + preOutputVec[iFlute];
                    }
                }
                if (abs((preOutput)*globalAmp) > 1)
                {
                    Logger::getCurrentLogger()->outputDebugString("Output is too loud!");
                }
                else {
                    output[i] = (preOutput)*globalAmp;
                }
                ++globalCurrentSample;
            }
        } else {
            // Copy the left channel to the right channel
            for (int i = 0; i < buffer.getNumSamples(); ++i)
                output[i] = buffer.getWritePointer (channel-1)[i];
        }
    }
}





//==============================================================================
bool Flute_Plugin_TryAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* Flute_Plugin_TryAudioProcessor::createEditor()
{
    return new Flute_Plugin_TryAudioProcessorEditor (*this);
}

//==============================================================================
void Flute_Plugin_TryAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void Flute_Plugin_TryAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new Flute_Plugin_TryAudioProcessor();
}


juce::AudioProcessorValueTreeState::ParameterLayout Flute_Plugin_TryAudioProcessor::createParameters()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;
    
    params.push_back(std::make_unique<AudioParameterFloat>("RELEASE_TIME","Release_Time",0.1f,10.0f,0.1f));
    
    params.push_back(std::make_unique<AudioParameterFloat>("IPRESS","ipress",0.3f,0.99f,0.9f));
    
    params.push_back(std::make_unique<AudioParameterFloat>("F_LPF","f_LPF",500.0f,15000.0f,3000.0f));
    params.push_back(std::make_unique<AudioParameterFloat>("VIBRATO","vibrato",0.0f,0.5f,0.02f));
    params.push_back(std::make_unique<AudioParameterFloat>("F_VIBRATO","freq_vibrato",0.1f,10.0f,5.0f));
    params.push_back(std::make_unique<AudioParameterFloat>("PITCH","pitch",-12.0f,12.0f,0.0f));

    return { params.begin(), params.end()};
}






