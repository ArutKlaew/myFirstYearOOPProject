//MainComponent.h

#pragma once

#include "maximilian.h"
#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "SynthAudioSource.h"

class MainComponent  : public AudioAppComponent
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent();
    //==============================================================================
    void resized() override;
    void prepareToPlay(int samplesPerBlockExpected,double sampleRate) override;
    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;
    void paint(Graphics& g) override;
private:
    AudioDeviceManager audioDeviceManager;
    MidiKeyboardState keyboardState;
    AudioSourcePlayer audioSourcePlayer;
    SynthAudioSource synthAudioSource { keyboardState,processor };
    MidiKeyboardComponent keyboardComponent { keyboardState, MidiKeyboardComponent::horizontalKeyboard};
    
    ComboBox midiInputList;
    Label midiInputListLabel;
    SynthAudioProcessor processor;
    SynthAudioProcessorEditor processorEditor{processor};
    int lastMidiInputIndex = 0;
    void setMidiInput (int index);
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
