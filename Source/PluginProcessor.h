/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/**
*/
/*
struct ChainSettings
{
    float LCFreq { 0 }, LCQuality {1.f};
    float LSFreq { 0 }, LSGainInDecibelst { 0 }, LSQuality {1.f};
    float PKFreq { 0 }, PKGainInDecibelst { 0 }, PKQuality {1.f};
    float HSFreq { 0 }, HSGainInDecibelst { 0 }, HSQuality {1.f};
    float HCFreq { 0 }, HCQuality {1.f};

    int lowCutSlope { 0 }, highCutSlope { 0 };
};
 
 ChainSettings getChainSettings(juce::AudioProcessorValueTreeState &aptvs);
*/

enum Slope
{
    Slope_12,
    Slope_24,
    Slope_36,
    Slope_48
};

struct FilterSettings
{
    float frequency {0.1f};
    float gain {0.f};      // Only for shelf/peak filters
    float quality {0.1f};   // Q factor (resonance)
};

struct ChainSettings
{
    FilterSettings lowCut;
    FilterSettings lowShelf;
    FilterSettings peak;
    FilterSettings highShelf;
    FilterSettings highCut;

    
    //Initialise Chain Settings for Multi Stage Slope
    Slope lowCutSlope { Slope::Slope_12 }, highCutSlope { Slope::Slope_24 };
};

ChainSettings getChainSettings(juce::AudioProcessorValueTreeState& apvts)
{
    ChainSettings settings;

    settings.lowCut.frequency = apvts.getRawParameterValue("LowCut_WB")->load();
    
    settings.lowShelf.frequency = apvts.getRawParameterValue("LowShelf_WB")->load();
    settings.lowShelf.gain = apvts.getRawParameterValue("LowShelf_G")->load();
    settings.lowShelf.quality = apvts.getRawParameterValue("LowShelf_Q")->load();
    
    settings.peak.frequency = apvts.getRawParameterValue("Peak_WB")->load();
    settings.peak.gain = apvts.getRawParameterValue("Peak_G")->load();
    settings.peak.quality = apvts.getRawParameterValue("Peak_Q")->load();
    
    settings.highShelf.frequency = apvts.getRawParameterValue("HighShelf_WB")->load();
    settings.highShelf.gain = apvts.getRawParameterValue("HighShelf_G")->load();
    settings.highShelf.quality = apvts.getRawParameterValue("HighShelf_Q")->load();
    
    settings.highCut.frequency = apvts.getRawParameterValue("HighCut_WB")->load();

    settings.lowCutSlope = static_cast<Slope>(apvts.getRawParameterValue("LowCutSlope")->load());
    settings.highCutSlope = static_cast<Slope>(apvts.getRawParameterValue("HighCutSlope")->load());

    return settings;
}


class _5_Bands_EQAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    _5_Bands_EQAudioProcessor();
    ~_5_Bands_EQAudioProcessor() override;

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
    
    
    
    // Declare and initialise APVTS and its layout
    static juce::AudioProcessorValueTreeState::ParameterLayout
    createParameterLayout();
    juce::AudioProcessorValueTreeState apvts {*this, nullptr, "Parameters", createParameterLayout()};

private:
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (_5_Bands_EQAudioProcessor)
    
    
    // Avoid Alias Pointers
    using Filter = juce::dsp::IIR::Filter<float>;
    using CutFilter = juce::dsp::ProcessorChain<Filter, Filter, Filter, Filter>;
    using MonoChain = juce::dsp::ProcessorChain<CutFilter, Filter, Filter, Filter, CutFilter>;
    
    //Use only this for single stage filters
    // using MonoChain = juce::dsp::ProcessorChain<Filter, Filter, Filter, Filter, Filter>;

    MonoChain leftChain, rightChain;
    
    enum ChainPositions
    {
        LowCut,
        LowShelf,
        Peak,
        HighShelf,
        HighCut
    };

};
