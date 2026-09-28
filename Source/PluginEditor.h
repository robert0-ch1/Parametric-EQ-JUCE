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
class _5_Bands_EQAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    _5_Bands_EQAudioProcessorEditor (_5_Bands_EQAudioProcessor&);
    ~_5_Bands_EQAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    _5_Bands_EQAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (_5_Bands_EQAudioProcessorEditor)
};
