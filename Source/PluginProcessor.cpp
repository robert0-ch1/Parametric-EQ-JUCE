#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
_5_Bands_EQAudioProcessor::_5_Bands_EQAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
}

_5_Bands_EQAudioProcessor::~_5_Bands_EQAudioProcessor()
{
}

//==============================================================================
const juce::String _5_Bands_EQAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool _5_Bands_EQAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool _5_Bands_EQAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool _5_Bands_EQAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double _5_Bands_EQAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int _5_Bands_EQAudioProcessor::getNumPrograms()
{
    return 1;
            
}

int _5_Bands_EQAudioProcessor::getCurrentProgram()
{
    return 0;
}

void _5_Bands_EQAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String _5_Bands_EQAudioProcessor::getProgramName (int index)
{
    return {};
}

void _5_Bands_EQAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void _5_Bands_EQAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Spec object to initialise all components of the filter chain.
    juce::dsp::ProcessSpec spec;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = 1;
    spec.sampleRate = sampleRate;
    leftChain.prepare(spec);
    rightChain.prepare(spec);
    


    
    // Recall Chain Settings
    auto chainSettings = getChainSettings(apvts);
    
    //Low Cut Coefficients

    auto lowCutCoefficients = juce::dsp::FilterDesign<float>::designIIRHighpassHighOrderButterworthMethod(chainSettings.lowCut.frequency,
                                                                                                          sampleRate,
                                                                                                       2*(chainSettings.lowCutSlope + 1));


    
    //Initialise left channel Low Cut Slopes
    auto& leftLowCut = leftChain.get<ChainPositions::LowCut>();
    
    leftLowCut.setBypassed<0>(true);
    leftLowCut.setBypassed<1>(true);
    leftLowCut.setBypassed<2>(true);
    leftLowCut.setBypassed<3>(true);
    
    switch (chainSettings.lowCutSlope) {
        case Slope_12:
        {
            *leftLowCut.get<0>().coefficients = *lowCutCoefficients[0];
            leftLowCut.setBypassed<0>(false);
            break;
        }
        case Slope_24:
        {
            *leftLowCut.get<0>().coefficients = *lowCutCoefficients[0];
            leftLowCut.setBypassed<0>(false);
            *leftLowCut.get<1>().coefficients = *lowCutCoefficients[1];
            leftLowCut.setBypassed<1>(false);
            break;
        }
        case Slope_36:
        {
            *leftLowCut.get<0>().coefficients = *lowCutCoefficients[0];
            leftLowCut.setBypassed<0>(false);
            *leftLowCut.get<1>().coefficients = *lowCutCoefficients[1];
            leftLowCut.setBypassed<1>(false);
            *leftLowCut.get<2>().coefficients = *lowCutCoefficients[2];
            leftLowCut.setBypassed<2>(false);
            break;
        }
        case Slope_48:
        {
            *leftLowCut.get<0>().coefficients = *lowCutCoefficients[0];
            leftLowCut.setBypassed<0>(false);
            *leftLowCut.get<1>().coefficients = *lowCutCoefficients[1];
            leftLowCut.setBypassed<1>(false);
            *leftLowCut.get<2>().coefficients = *lowCutCoefficients[2];
            leftLowCut.setBypassed<2>(false);
            *leftLowCut.get<3>().coefficients = *lowCutCoefficients[3];
            leftLowCut.setBypassed<3>(false);
            break;
        }
            break;
    }
    
    //Initialise right channel Low Cut Slopes
    auto& rightLowCut = rightChain.get<ChainPositions::LowCut>();
    
    rightLowCut.setBypassed<0>(true);
    rightLowCut.setBypassed<1>(true);
    rightLowCut.setBypassed<2>(true);
    rightLowCut.setBypassed<3>(true);
    
    switch (chainSettings.lowCutSlope) {
        case Slope_12:
        {
            *rightLowCut.get<0>().coefficients = *lowCutCoefficients[0];
            rightLowCut.setBypassed<0>(false);
            break;
        }
        case Slope_24:
        {
            *rightLowCut.get<0>().coefficients = *lowCutCoefficients[0];
            rightLowCut.setBypassed<0>(false);
            *rightLowCut.get<1>().coefficients = *lowCutCoefficients[1];
            rightLowCut.setBypassed<1>(false);
            break;
        }
        case Slope_36:
        {
            *rightLowCut.get<0>().coefficients = *lowCutCoefficients[0];
            rightLowCut.setBypassed<0>(false);
            *rightLowCut.get<1>().coefficients = *lowCutCoefficients[1];
            rightLowCut.setBypassed<1>(false);
            *rightLowCut.get<2>().coefficients = *lowCutCoefficients[2];
            rightLowCut.setBypassed<2>(false);
            break;
        }
        case Slope_48:
        {
            *rightLowCut.get<0>().coefficients = *lowCutCoefficients[0];
            rightLowCut.setBypassed<0>(false);
            *rightLowCut.get<1>().coefficients = *lowCutCoefficients[1];
            rightLowCut.setBypassed<1>(false);
            *rightLowCut.get<2>().coefficients = *lowCutCoefficients[2];
            rightLowCut.setBypassed<2>(false);
            *rightLowCut.get<3>().coefficients = *lowCutCoefficients[3];
            rightLowCut.setBypassed<3>(false);
            break;
        }
            break;
    }
    
    
    
    
    // Low Shelf Coefficients
    
    auto LSCoefficients = juce::dsp::IIR::Coefficients<float>::makeLowShelf(sampleRate,
                                                                            chainSettings.lowShelf.frequency,
                                                                            chainSettings.lowShelf.quality,
                                                                            juce::Decibels::decibelsToGain(chainSettings.lowShelf.gain));
    
    *leftChain.get<ChainPositions::LowShelf>().coefficients = *LSCoefficients;
    *rightChain.get<ChainPositions::LowShelf>().coefficients = *LSCoefficients;
    
    
    // Peak Coefficients
    
    
    auto PKCoefficients = juce::dsp::IIR::Coefficients<float>::makePeakFilter(sampleRate,
                                                                              chainSettings.peak.frequency,
                                                                              chainSettings.peak.quality,
                                                                              juce::Decibels::decibelsToGain(chainSettings.peak.gain));
    
    *leftChain.get<ChainPositions::Peak>().coefficients = *PKCoefficients;
    *rightChain.get<ChainPositions::Peak>().coefficients = *PKCoefficients;
    
    
    
    // High Shelf Coefficients

    auto HSCoefficients = juce::dsp::IIR::Coefficients<float>::makeHighShelf(sampleRate,
                                                                             chainSettings.highShelf.frequency,
                                                                             chainSettings.highShelf.quality,
                                                                             juce::Decibels::decibelsToGain(chainSettings.highShelf.gain));
    
    *leftChain.get<ChainPositions::HighShelf>().coefficients = *HSCoefficients;
    *rightChain.get<ChainPositions::HighShelf>().coefficients = *HSCoefficients;
    
    DBG("HZ");
    DBG(chainSettings.highCut.frequency);
    
    // High Cut Coefficients
    auto hicutCoefficients = juce::dsp::FilterDesign<float>::designIIRLowpassHighOrderButterworthMethod(chainSettings.highCut.frequency,
                                                                                                        sampleRate,
                                                                                                       2*(chainSettings.highCutSlope + 1));

    //Initialise left channel High Cut Slopes
    auto& leftHiCut = leftChain.get<ChainPositions::HighCut>();
    
    leftHiCut.setBypassed<0>(true);
    leftHiCut.setBypassed<1>(true);
    leftHiCut.setBypassed<2>(true);
    leftHiCut.setBypassed<3>(true);
    
    switch (chainSettings.highCutSlope) {
        case Slope_12:
        {
            *leftHiCut.get<0>().coefficients = *hicutCoefficients[0];
            leftHiCut.setBypassed<0>(false);
            break;
        }
        case Slope_24:
        {
            *leftHiCut.get<0>().coefficients = *hicutCoefficients[0];
            leftHiCut.setBypassed<0>(false);
            *leftHiCut.get<1>().coefficients = *hicutCoefficients[1];
            leftHiCut.setBypassed<1>(false);
            break;
        }
        case Slope_36:
        {
            *leftHiCut.get<0>().coefficients = *hicutCoefficients[0];
            leftHiCut.setBypassed<0>(false);
            *leftHiCut.get<1>().coefficients = *hicutCoefficients[1];
            leftHiCut.setBypassed<1>(false);
            *leftHiCut.get<2>().coefficients = *hicutCoefficients[2];
            leftHiCut.setBypassed<2>(false);
            break;
        }
        case Slope_48:
        {
            *leftHiCut.get<0>().coefficients = *hicutCoefficients[0];
            leftHiCut.setBypassed<0>(false);
            *leftHiCut.get<1>().coefficients = *hicutCoefficients[1];
            leftHiCut.setBypassed<1>(false);
            *leftHiCut.get<2>().coefficients = *hicutCoefficients[2];
            leftHiCut.setBypassed<2>(false);
            *leftHiCut.get<3>().coefficients = *hicutCoefficients[3];
            leftHiCut.setBypassed<3>(false);
            break;
        }
            break;
    }
    
    //Initialise right channel for High Cut Slopes
    auto& rightHighCut = rightChain.get<ChainPositions::HighCut>();
    
    rightHighCut.setBypassed<0>(true);
    rightHighCut.setBypassed<1>(true);
    rightHighCut.setBypassed<2>(true);
    rightHighCut.setBypassed<3>(true);
    
    switch (chainSettings.highCutSlope) {
        case Slope_12:
        {
            *rightHighCut.get<0>().coefficients = *hicutCoefficients[0];
            rightHighCut.setBypassed<0>(false);
            break;
        }
        case Slope_24:
        {
            *rightHighCut.get<0>().coefficients = *hicutCoefficients[0];
            rightHighCut.setBypassed<0>(false);
            *rightHighCut.get<1>().coefficients = *hicutCoefficients[1];
            rightHighCut.setBypassed<1>(false);
            break;
        }
        case Slope_36:
        {
            *rightHighCut.get<0>().coefficients = *hicutCoefficients[0];
            rightHighCut.setBypassed<0>(false);
            *rightHighCut.get<1>().coefficients = *hicutCoefficients[1];
            rightHighCut.setBypassed<1>(false);
            *rightHighCut.get<2>().coefficients = *hicutCoefficients[2];
            rightHighCut.setBypassed<2>(false);
            break;
        }
        case Slope_48:
        {
            *rightHighCut.get<0>().coefficients = *hicutCoefficients[0];
            rightHighCut.setBypassed<0>(false);
            *rightHighCut.get<1>().coefficients = *hicutCoefficients[1];
            rightHighCut.setBypassed<1>(false);
            *rightHighCut.get<2>().coefficients = *hicutCoefficients[2];
            rightHighCut.setBypassed<2>(false);
            *rightHighCut.get<3>().coefficients = *hicutCoefficients[3];
            rightHighCut.setBypassed<3>(false);
            break;
        }
            break;
    }
    
}

void _5_Bands_EQAudioProcessor::releaseResources()
{
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool _5_Bands_EQAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
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

void _5_Bands_EQAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    
    
    // Recall Chain Settings
    auto chainSettings = getChainSettings(apvts);


    
    //Low Cut Coefficients

    auto lowCutCoefficients = juce::dsp::FilterDesign<float>::designIIRHighpassHighOrderButterworthMethod(chainSettings.lowCut.frequency,
                                                                                                       getSampleRate(),
                                                                                                       2*(chainSettings.lowCutSlope + 1));

    //Initialise left channel Low Cut Slopes
    auto& leftLowCut = leftChain.get<ChainPositions::LowCut>();
    
    leftLowCut.setBypassed<0>(true);
    leftLowCut.setBypassed<1>(true);
    leftLowCut.setBypassed<2>(true);
    leftLowCut.setBypassed<3>(true);
    
    switch (chainSettings.lowCutSlope) {
        case Slope_12:
        {
            *leftLowCut.get<0>().coefficients = *lowCutCoefficients[0];
            leftLowCut.setBypassed<0>(false);
            break;
        }
        case Slope_24:
        {
            *leftLowCut.get<0>().coefficients = *lowCutCoefficients[0];
            leftLowCut.setBypassed<0>(false);
            *leftLowCut.get<1>().coefficients = *lowCutCoefficients[1];
            leftLowCut.setBypassed<1>(false);
            break;
        }
        case Slope_36:
        {
            *leftLowCut.get<0>().coefficients = *lowCutCoefficients[0];
            leftLowCut.setBypassed<0>(false);
            *leftLowCut.get<1>().coefficients = *lowCutCoefficients[1];
            leftLowCut.setBypassed<1>(false);
            *leftLowCut.get<2>().coefficients = *lowCutCoefficients[2];
            leftLowCut.setBypassed<2>(false);
            break;
        }
        case Slope_48:
        {
            *leftLowCut.get<0>().coefficients = *lowCutCoefficients[0];
            leftLowCut.setBypassed<0>(false);
            *leftLowCut.get<1>().coefficients = *lowCutCoefficients[1];
            leftLowCut.setBypassed<1>(false);
            *leftLowCut.get<2>().coefficients = *lowCutCoefficients[2];
            leftLowCut.setBypassed<2>(false);
            *leftLowCut.get<3>().coefficients = *lowCutCoefficients[3];
            leftLowCut.setBypassed<3>(false);
            break;
        }
            break;
    }
    
    //Initialise right channel Low Cut Slopes
    auto& rightLowCut = rightChain.get<ChainPositions::LowCut>();
    
    rightLowCut.setBypassed<0>(true);
    rightLowCut.setBypassed<1>(true);
    rightLowCut.setBypassed<2>(true);
    rightLowCut.setBypassed<3>(true);
    
    switch (chainSettings.lowCutSlope) {
        case Slope_12:
        {
            *rightLowCut.get<0>().coefficients = *lowCutCoefficients[0];
            rightLowCut.setBypassed<0>(false);
            break;
        }
        case Slope_24:
        {
            *rightLowCut.get<0>().coefficients = *lowCutCoefficients[0];
            rightLowCut.setBypassed<0>(false);
            *rightLowCut.get<1>().coefficients = *lowCutCoefficients[1];
            rightLowCut.setBypassed<1>(false);
            break;
        }
        case Slope_36:
        {
            *rightLowCut.get<0>().coefficients = *lowCutCoefficients[0];
            rightLowCut.setBypassed<0>(false);
            *rightLowCut.get<1>().coefficients = *lowCutCoefficients[1];
            rightLowCut.setBypassed<1>(false);
            *rightLowCut.get<2>().coefficients = *lowCutCoefficients[2];
            rightLowCut.setBypassed<2>(false);
            break;
        }
        case Slope_48:
        {
            *rightLowCut.get<0>().coefficients = *lowCutCoefficients[0];
            rightLowCut.setBypassed<0>(false);
            *rightLowCut.get<1>().coefficients = *lowCutCoefficients[1];
            rightLowCut.setBypassed<1>(false);
            *rightLowCut.get<2>().coefficients = *lowCutCoefficients[2];
            rightLowCut.setBypassed<2>(false);
            *rightLowCut.get<3>().coefficients = *lowCutCoefficients[3];
            rightLowCut.setBypassed<3>(false);
            break;
        }
            break;
    }
    
    
    
    // Low Shelf Coefficients
    
    auto LSCoefficients = juce::dsp::IIR::Coefficients<float>::makeLowShelf(getSampleRate(),
                                                                            chainSettings.lowShelf.frequency,
                                                                            chainSettings.lowShelf.quality,
                                                                            juce::Decibels::decibelsToGain(chainSettings.lowShelf.gain));
    
    *leftChain.get<ChainPositions::LowShelf>().coefficients = *LSCoefficients;
    *rightChain.get<ChainPositions::LowShelf>().coefficients = *LSCoefficients;
    
    
    // Peak Coefficients
    
    
    auto PKCoefficients = juce::dsp::IIR::Coefficients<float>::makePeakFilter(getSampleRate(),
                                                                              chainSettings.peak.frequency,
                                                                              chainSettings.peak.quality,
                                                                              juce::Decibels::decibelsToGain(chainSettings.peak.gain));
    
    *leftChain.get<ChainPositions::Peak>().coefficients = *PKCoefficients;
    *rightChain.get<ChainPositions::Peak>().coefficients = *PKCoefficients;
    
    
    
    // High Shelf Coefficients
    
    auto HSCoefficients = juce::dsp::IIR::Coefficients<float>::makeHighShelf(getSampleRate(),
                                                                             chainSettings.highShelf.frequency,
                                                                             chainSettings.highShelf.quality,
                                                                             juce::Decibels::decibelsToGain(chainSettings.highShelf.gain));
    
    *leftChain.get<ChainPositions::HighShelf>().coefficients = *HSCoefficients;
    *rightChain.get<ChainPositions::HighShelf>().coefficients = *HSCoefficients;
    
    
    
    // High Cut Coefficients
    
    auto hicutCoefficients = juce::dsp::FilterDesign<float>::designIIRLowpassHighOrderButterworthMethod(chainSettings.highCut.frequency,
                                                                                                       getSampleRate(),
                                                                                                       2*(chainSettings.highCutSlope + 1));
    //Initialise left channel High Cut Slopes
    auto& leftHiCut = leftChain.get<ChainPositions::HighCut>();
    
    leftHiCut.setBypassed<0>(true);
    leftHiCut.setBypassed<1>(true);
    leftHiCut.setBypassed<2>(true);
    leftHiCut.setBypassed<3>(true);
    
    switch (chainSettings.highCutSlope) {
        case Slope_12:
        {
            *leftHiCut.get<0>().coefficients = *hicutCoefficients[0];
            leftHiCut.setBypassed<0>(false);
            break;
        }
        case Slope_24:
        {
            *leftHiCut.get<0>().coefficients = *hicutCoefficients[0];
            leftHiCut.setBypassed<0>(false);
            *leftHiCut.get<1>().coefficients = *hicutCoefficients[1];
            leftHiCut.setBypassed<1>(false);
            break;
        }
        case Slope_36:
        {
            *leftHiCut.get<0>().coefficients = *hicutCoefficients[0];
            leftHiCut.setBypassed<0>(false);
            *leftHiCut.get<1>().coefficients = *hicutCoefficients[1];
            leftHiCut.setBypassed<1>(false);
            *leftHiCut.get<2>().coefficients = *hicutCoefficients[2];
            leftHiCut.setBypassed<2>(false);
            break;
        }
        case Slope_48:
        {
            *leftHiCut.get<0>().coefficients = *hicutCoefficients[0];
            leftHiCut.setBypassed<0>(false);
            *leftHiCut.get<1>().coefficients = *hicutCoefficients[1];
            leftHiCut.setBypassed<1>(false);
            *leftHiCut.get<2>().coefficients = *hicutCoefficients[2];
            leftHiCut.setBypassed<2>(false);
            *leftHiCut.get<3>().coefficients = *hicutCoefficients[3];
            leftHiCut.setBypassed<3>(false);
            break;
        }
            break;
    }
    
    //Initialise right channel for High Cut Slopes
    auto& rightHighCut = rightChain.get<ChainPositions::HighCut>();
    
    rightHighCut.setBypassed<0>(true);
    rightHighCut.setBypassed<1>(true);
    rightHighCut.setBypassed<2>(true);
    rightHighCut.setBypassed<3>(true);
    
    switch (chainSettings.highCutSlope) {
        case Slope_12:
        {
            *rightHighCut.get<0>().coefficients = *hicutCoefficients[0];
            rightHighCut.setBypassed<0>(false);
            break;
        }
        case Slope_24:
        {
            *rightHighCut.get<0>().coefficients = *hicutCoefficients[0];
            rightHighCut.setBypassed<0>(false);
            *rightHighCut.get<1>().coefficients = *hicutCoefficients[1];
            rightHighCut.setBypassed<1>(false);
            break;
        }
        case Slope_36:
        {
            *rightHighCut.get<0>().coefficients = *hicutCoefficients[0];
            rightHighCut.setBypassed<0>(false);
            *rightHighCut.get<1>().coefficients = *hicutCoefficients[1];
            rightHighCut.setBypassed<1>(false);
            *rightHighCut.get<2>().coefficients = *hicutCoefficients[2];
            rightHighCut.setBypassed<2>(false);
            break;
        }
        case Slope_48:
        {
            *rightHighCut.get<0>().coefficients = *hicutCoefficients[0];
            rightHighCut.setBypassed<0>(false);
            *rightHighCut.get<1>().coefficients = *hicutCoefficients[1];
            rightHighCut.setBypassed<1>(false);
            *rightHighCut.get<2>().coefficients = *hicutCoefficients[2];
            rightHighCut.setBypassed<2>(false);
            *rightHighCut.get<3>().coefficients = *hicutCoefficients[3];
            rightHighCut.setBypassed<3>(false);
            break;
        }
            break;
    }
    
    // Process Audio Through Filter Chain
    juce::dsp::AudioBlock<float> block(buffer);
    auto leftBlock = block.getSingleChannelBlock(0);
    auto rightBlock = block.getSingleChannelBlock(1);
    juce::dsp::ProcessContextReplacing<float> leftContext(leftBlock);
    juce::dsp::ProcessContextReplacing<float> rightContext(rightBlock);
    leftChain.process(leftContext);
    rightChain.process(rightContext);
    }


//==============================================================================
bool _5_Bands_EQAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* _5_Bands_EQAudioProcessor::createEditor()
{
    //return new _5_Bands_EQAudioProcessorEditor (*this);
    return new juce::GenericAudioProcessorEditor(*this);
}

//==============================================================================
void _5_Bands_EQAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{

}

void _5_Bands_EQAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{

}


//Create a Parameter Layout
juce::AudioProcessorValueTreeState::ParameterLayout
_5_Bands_EQAudioProcessor::createParameterLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;

    // Initialise Parameters:
    
    // Centre Frequency - WB
    layout.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{"LowCut_WB", 1},
                                                           "Low Cut Centre Frequency",
                                                           juce::NormalisableRange<float>(20.0f, 400.0f, 1.0f, 0.5f),
                                                           20.0f));
    
    layout.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{"LowShelf_WB", 1},
                                                           "Low Shelf Centre Frequency",
                                                           juce::NormalisableRange<float>(20.0f, 400.0f, 1.0f, 0.5f),
                                                           20.0f));
   
    layout.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{"Peak_WB", 1},
                                                           "Peak Centre Frequency",
                                                           juce::NormalisableRange<float>(400.0f, 3150.0f, 1.0f, 0.5f),
                                                           1600.0f));
    
    layout.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{"HighShelf_WB", 1},
                                                           "High Shelf Centre Frequency",
                                                           juce::NormalisableRange<float>(3150.0f, 20000.0f, 1.0f, 0.5f),
                                                           3150.0f));
    
    layout.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{"HighCut_WB", 1},
                                                           "High Cut Centre Frequency",
                                                           juce::NormalisableRange<float>(3150.0f, 20000.0f, 1.0f, 0.5f),
                                                           20000.0f));
    
    
    // Gain - G (only for Single Stage Filters)
    
    
    layout.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{"LowShelf_G", 1},
                                                           "Low Shelf Gain",
                                                           juce::NormalisableRange<float>(-24.0f, 24.0f, 0.5f, 1.0f),
                                                           0.0f));
   
    layout.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{"Peak_G", 1},
                                                           "Peak Gain",
                                                           juce::NormalisableRange<float>(-24.0f, 24.0f, 0.5f, 1.0f),
                                                           0.0f));
    
    layout.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{"HighShelf_G", 1},
                                                           "High Shelf Gain",
                                                           juce::NormalisableRange<float>(-24.0f, 24.0f, 0.5f, 1.0f),
                                                           0.0f));
    

    // Quality - Q (only for Single Stage Filters)
    
    layout.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{"LowShelf_Q", 1},
                                                           "Low Shelf Q",
                                                           juce::NormalisableRange<float>(0.1f, 10.0f, 0.05f, 1.0f),
                                                           0.1f));
   
    layout.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{"Peak_Q", 1},
                                                           "Peak Q",
                                                           juce::NormalisableRange<float>(0.1f, 10.0f, 0.05f, 1.0f),
                                                           0.1f));
    
    layout.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{"HighShelf_Q", 1},
                                                           "High Shelf Q",
                                                           juce::NormalisableRange<float>(0.1f, 10.0f, 0.05f, 1.0f),
                                                           0.1f));
    

    // dB strings
    
    juce::StringArray stringArray;
    for (int i = 0; i < 4; i++ ){
        juce::String str;
        str<<(12 + i*12);
        str<<(" dB/Oct");
        stringArray.add(str);
    }
    
    layout.add(std::make_unique<juce::AudioParameterChoice>(juce::ParameterID{"LowCutSlope", 1},
                                                            "Low Cut Slope",
                                                            stringArray, 0));
    
    layout.add(std::make_unique<juce::AudioParameterChoice>(juce::ParameterID{"HighCutSlope", 1},
                                                            "High Cut Slope",
                                                            stringArray, 0));
    
    return layout;
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new _5_Bands_EQAudioProcessor();
}
