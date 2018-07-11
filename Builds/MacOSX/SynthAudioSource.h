#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"

class SynthAudioSource : public AudioSource {
public:
    SynthAudioSource(MidiKeyboardState& keyState,JuceSynthFrameworkAudioProcessor& p);
    void prepareToPlay(int samplePerBlockExpected,double sampleRate) override;
    void releaseResources() override {};
    void getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill) override;
    MidiMessageCollector* getMidiCollector();
private:
    MidiMessageCollector midiCollector;
    MidiKeyboardState& keyboardState;
    Synthesiser synth;
    JuceSynthFrameworkAudioProcessor& processor;
};
