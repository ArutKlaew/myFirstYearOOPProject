//MainComponent.cpp
#include "MainComponent.h"
//==============================================================================
MainComponent::MainComponent() : synthAudioSource  (keyboardState,processor),
                                 keyboardComponent (keyboardState, MidiKeyboardComponent::horizontalKeyboard)
{
    addAndMakeVisible(midiInputListLabel);
    midiInputListLabel.setText("MIDI Input;", dontSendNotification);
    midiInputListLabel.attachToComponent(&midiInputList, true);
    
    addAndMakeVisible(midiInputList);
    midiInputList.setTextWhenNoChoicesAvailable("No MIDI Inputs Enabled");
    auto midiInputs = MidiInput::getDevices();
    midiInputList.addItemList(midiInputs, 1);
    midiInputList.onChange = [this] { setMidiInput (midiInputList.getSelectedItemIndex());};
    for (auto midiInput : midiInputs)
    {
        if (deviceManager.isMidiInputEnabled (midiInput))
        {
            setMidiInput (midiInputs.indexOf (midiInput));
            break;
        }
    }
    if (midiInputList.getSelectedId() == 0) setMidiInput (0);
    
    addAndMakeVisible(keyboardComponent);
    
    setAudioChannels (0, 2);
    addAndMakeVisible(processorEditor);
    setSize (800, 450);
}

MainComponent::~MainComponent()
{
    shutdownAudio();
}

//==============================================================================prepare
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    
    synthAudioSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void MainComponent::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)
{
    synthAudioSource.getNextAudioBlock(bufferToFill);
}

void MainComponent::releaseResources()
{
    synthAudioSource.releaseResources();
}

//==============================================================================
void MainComponent::paint (Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));
    
}

void MainComponent::resized()
{
    Rectangle<int> screenArea = getLocalBounds();
    auto lowerArea = screenArea.removeFromBottom(195);
    auto upperArea = screenArea.removeFromTop(800 - 195);
    auto keyboardArea = lowerArea.removeFromBottom(130);
    auto midiInputArea = lowerArea.removeFromTop(65);
    midiInputArea = midiInputArea.removeFromTop(45);
    midiInputArea = midiInputArea.removeFromBottom(40);
    midiInputArea = midiInputArea.removeFromRight(790);
    midiInputArea = midiInputArea.removeFromLeft(780);
    midiInputList.setBounds(midiInputArea);
    keyboardComponent.setBounds(keyboardArea);
    processorEditor.setBounds(upperArea);
}

void MainComponent::setMidiInput(int index) {
    auto list = MidiInput::getDevices();
    deviceManager.removeMidiInputCallback (list[lastMidiInputIndex], synthAudioSource.getMidiCollector());
    auto newMidiInput = list[index];
    
    if (! deviceManager.isMidiInputEnabled (newMidiInput))
        deviceManager.setMidiInputEnabled (newMidiInput, true);
    
    deviceManager.addMidiInputCallback (newMidiInput, synthAudioSource.getMidiCollector());
    midiInputList.setSelectedId (index + 1, dontSendNotification);
    lastMidiInputIndex = index;
}

