#include "SynthVoice.h"

bool SynthVoice::canPlaySound(SynthesiserSound* sound) {
    return dynamic_cast <SynthSound*>(sound) != nullptr;
}

void SynthVoice::setOscType(float* selection){
    waveSelection = *selection;
}

void SynthVoice::setVolume(float* vol) {
    gain = *vol;
}

double SynthVoice::getOscType() {
        if (waveSelection == 0)return oscillator.sinewave(frequency);
        if (waveSelection == 1)return oscillator.saw(frequency);
        if (waveSelection == 2)return oscillator.square(frequency);
        else return oscillator.sinewave(frequency);
}

void SynthVoice::setEnvelopeParams(float *attack, float *decay, float *sustain, float *release) {
    envelope.setAttack(*attack);
    envelope.setDecay(*decay);
    envelope.setSustain(*sustain);
    envelope.setRelease(*release);
}

double SynthVoice::getEnvelope()
{
    return envelope.adsr(getOscType(), envelope.trigger);
}

void SynthVoice::setFilterParams (float* filterType, float* filterCutoff, float* filterRes) {
    filterChoice = *filterType;
    cutoff = *filterCutoff;
    resonance = *filterRes;
}

void SynthVoice::startNote (int midiNoteNumber, float velocity, SynthesiserSound* sound, int currentPitchWheelPosition) {
    envelope.trigger = 1;
    frequency = MidiMessage::getMidiNoteInHertz(midiNoteNumber);
}

void SynthVoice::stopNote (float velocity, bool allowTailOff) {
    envelope.trigger = 0;
    allowTailOff = true;
    if (velocity == 0)
        clearCurrentNote();
}

void SynthVoice::renderNextBlock (AudioBuffer <float> &outputBuffer, int startSample, int numSamples) {
    for (int sample = 0; sample < numSamples; ++sample)
    {
        for (int channel = 0; channel < outputBuffer.getNumChannels(); ++channel)
        {
            outputBuffer.addSample(channel, startSample, getEnvelope() * (gain / 10.0f));
        }
        ++startSample;
    }
}







