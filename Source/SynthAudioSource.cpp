#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"
#include "SynthAudioSource.h"


//Attach processor and MidiKeyboardState to the SynthAudioSource instance.
SynthAudioSource::SynthAudioSource(MidiKeyboardState& keyState,SynthAudioProcessor& p) : keyboardState(keyState),processor(p) {
    
}

void SynthAudioSource::prepareToPlay(int samplePerBlockExpected, double sampleRate) {
    //Prepare initial settings.
    midiCollector.reset(sampleRate);
    processor.prepareToPlay(sampleRate, samplePerBlockExpected);
}

void SynthAudioSource::getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill) {
    //Clear the audio buffer.
    bufferToFill.clearActiveBufferRegion();
    
    //Create a buffer for the incoming MidiMessage.
    MidiBuffer incomingMidi;
    
    //Fetch MidiMessage from midiCollector into the prepared buffer.
    midiCollector.removeNextBlockOfMessages(incomingMidi, bufferToFill.numSamples);
    
    keyboardState.processNextMidiBuffer(incomingMidi, 0, bufferToFill.numSamples, true);
    processor.processBlock(*bufferToFill.buffer, incomingMidi);
}

MidiMessageCollector*  SynthAudioSource::getMidiCollector() {
    return &midiCollector;
}
