#include "PluginEditor.h"

// ============================================================================
// Constructor & Destructor
// ============================================================================

ScaleChordPluginEditor::ScaleChordPluginEditor(ScaleChordPluginAudioProcessor& p)
    : juce::AudioProcessorEditor(&p),
      audioProcessor(p)
{
    // Set window size
    setSize(600, 500);

    // Set look and feel
    setColour(juce::ResizableWindow::backgroundColourId, juce::Colours::darkgrey);

    // Initialize components
    initializeComponents();

    // Setup styling
    setupComponentStyling();

    // Start timer for real-time updates
    startTimer(100);  // Update every 100ms

    DBG("ScaleChordPluginEditor created");
}

ScaleChordPluginEditor::~ScaleChordPluginEditor()
{
    stopTimer();
    DBG("ScaleChordPluginEditor destroyed");
}

// ============================================================================
// Component Initialization
// ============================================================================

void ScaleChordPluginEditor::initializeComponents()
{
    // ====== Title and Version ======
    titleLabel.setText("ScaleChord", juce::dontSendNotification);
    titleLabel.setJustificationType(juce::Justification::centred);
    titleLabel.setFont(24.0f);
    addAndMakeVisible(titleLabel);

    versionLabel.setText("v1.0.0", juce::dontSendNotification);
    versionLabel.setJustificationType(juce::Justification::centredRight);
    versionLabel.setFont(10.0f);
    addAndMakeVisible(versionLabel);

    // ====== Scale Selection ======
    scaleLabel.setText("Scale:", juce::dontSendNotification);
    addAndMakeVisible(scaleLabel);

    scaleSelector.addItemList(scaleOptions, 1);
    scaleSelector.setSelectedItemIndex(0, juce::dontSendNotification);
    scaleSelector.addListener(this);
    addAndMakeVisible(scaleSelector);

    // ====== Voice Leading ======
    voiceLeadingLabel.setText("Voice Leading:", juce::dontSendNotification);
    addAndMakeVisible(voiceLeadingLabel);

    voiceLeadingSelector.addItemList(voiceLeadingOptions, 1);
    voiceLeadingSelector.setSelectedItemIndex(1, juce::dontSendNotification);
    voiceLeadingSelector.addListener(this);
    addAndMakeVisible(voiceLeadingSelector);

    // ====== Toggles ======
    midiEffectsButton.setButtonText("MIDI Effects");
    midiEffectsButton.setToggleState(true, juce::dontSendNotification);
    midiEffectsButton.addListener(this);
    addAndMakeVisible(midiEffectsButton);

    jazzReharmonizationButton.setButtonText("Jazz Reharmonization");
    jazzReharmonizationButton.setToggleState(false, juce::dontSendNotification);
    jazzReharmonizationButton.addListener(this);
    addAndMakeVisible(jazzReharmonizationButton);

    // ====== Sliders ======
    voicingThresholdLabel.setText("Voicing Threshold:", juce::dontSendNotification);
    addAndMakeVisible(voicingThresholdLabel);
    voicingThresholdSlider.setRange(0.0, 1.0, 0.01);
    voicingThresholdSlider.setValue(0.5);
    voicingThresholdSlider.addListener(this);
    voicingThresholdSlider.setTextBoxStyle(juce::Slider::TextBoxRight, false, 60, 20);
    addAndMakeVisible(voicingThresholdSlider);

    analyzingSmoothingLabel.setText("Analysis Smoothing:", juce::dontSendNotification);
    addAndMakeVisible(analyzingSmoothingLabel);
    analyzingSmoothingSlider.setRange(0.0, 1.0, 0.01);
    analyzingSmoothingSlider.setValue(0.3);
    analyzingSmoothingSlider.addListener(this);
    analyzingSmoothingSlider.setTextBoxStyle(juce::Slider::TextBoxRight, false, 60, 20);
    addAndMakeVisible(analyzingSmoothingSlider);

    polyphonyLabel.setText("Polyphony:", juce::dontSendNotification);
    addAndMakeVisible(polyphonyLabel);
    polyphonySlider.setRange(1, 16, 1);
    polyphonySlider.setValue(8);
    polyphonySlider.addListener(this);
    polyphonySlider.setTextBoxStyle(juce::Slider::TextBoxRight, false, 60, 20);
    addAndMakeVisible(polyphonySlider);

    // ====== Status Display ======
    statusLabel.setText("Ready", juce::dontSendNotification);
    statusLabel.setJustificationType(juce::Justification::centredLeft);
    addAndMakeVisible(statusLabel);

    activeNotesLabel.setText("Active Notes: 0", juce::dontSendNotification);
    addAndMakeVisible(activeNotesLabel);

    voicingDisplayLabel.setText("Voicing: None", juce::dontSendNotification);
    addAndMakeVisible(voicingDisplayLabel);
}

void ScaleChordPluginEditor::setupComponentStyling()
{
    // Set fonts and colors
    auto labelColor = juce::Colours::lightgrey;

    titleLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    versionLabel.setColour(juce::Label::textColourId, labelColor);

    scaleLabel.setColour(juce::Label::textColourId, labelColor);
    voiceLeadingLabel.setColour(juce::Label::textColourId, labelColor);
    voicingThresholdLabel.setColour(juce::Label::textColourId, labelColor);
    analyzingSmoothingLabel.setColour(juce::Label::textColourId, labelColor);
    polyphonyLabel.setColour(juce::Label::textColourId, labelColor);

    statusLabel.setColour(juce::Label::textColourId, juce::Colours::green);
    activeNotesLabel.setColour(juce::Label::textColourId, juce::Colours::cyan);
    voicingDisplayLabel.setColour(juce::Label::textColourId, juce::Colours::yellow);
}

// ============================================================================
// Layout
// ============================================================================

void ScaleChordPluginEditor::resized()
{
    int margin = 10;
    int labelWidth = 150;
    int controlWidth = getWidth() - labelWidth - 3 * margin;
    int y = margin;
    int rowHeight = 30;
    int spacer = 5;

    // Title
    titleLabel.setBounds(margin, y, getWidth() - 2 * margin, 40);
    y += 45;

    versionLabel.setBounds(getWidth() - 100, 5, 90, 20);

    // Scale selector
    scaleLabel.setBounds(margin, y, labelWidth, rowHeight);
    scaleSelector.setBounds(labelWidth + 2 * margin, y, controlWidth, rowHeight);
    y += rowHeight + spacer;

    // Voice leading
    voiceLeadingLabel.setBounds(margin, y, labelWidth, rowHeight);
    voiceLeadingSelector.setBounds(labelWidth + 2 * margin, y, controlWidth, rowHeight);
    y += rowHeight + spacer;

    // Toggles
    midiEffectsButton.setBounds(margin, y, 200, rowHeight);
    jazzReharmonizationButton.setBounds(margin + 200, y, 250, rowHeight);
    y += rowHeight + spacer;

    // Sliders
    voicingThresholdLabel.setBounds(margin, y, labelWidth, rowHeight);
    voicingThresholdSlider.setBounds(labelWidth + 2 * margin, y, controlWidth, rowHeight);
    y += rowHeight + spacer;

    analyzingSmoothingLabel.setBounds(margin, y, labelWidth, rowHeight);
    analyzingSmoothingSlider.setBounds(labelWidth + 2 * margin, y, controlWidth, rowHeight);
    y += rowHeight + spacer;

    polyphonyLabel.setBounds(margin, y, labelWidth, rowHeight);
    polyphonySlider.setBounds(labelWidth + 2 * margin, y, controlWidth, rowHeight);
    y += rowHeight + spacer;

    // Separator
    y += 10;

    // Status display
    statusLabel.setBounds(margin, y, getWidth() - 2 * margin, rowHeight);
    y += rowHeight + spacer;

    activeNotesLabel.setBounds(margin, y, getWidth() - 2 * margin, rowHeight);
    y += rowHeight + spacer;

    voicingDisplayLabel.setBounds(margin, y, getWidth() - 2 * margin, rowHeight);
}

// ============================================================================
// Painting
// ============================================================================

void ScaleChordPluginEditor::paint(juce::Graphics& g)
{
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));

    // Draw a subtle border around the editor
    g.setColour(juce::Colours::grey);
    g.drawRect(getLocalBounds(), 1);

    // Draw separator line
    int separatorY = 280;
    g.setColour(juce::Colours::darkgrey.brighter());
    g.drawLine(10, separatorY, getWidth() - 10, separatorY, 1.0f);
}

// ============================================================================
// Interaction Callbacks
// ============================================================================

void ScaleChordPluginEditor::sliderValueChanged(juce::Slider* slider)
{
    if (slider == &voicingThresholdSlider)
    {
        auto& params = audioProcessor.getValueTreeState();
        params.getParameter("voicingThreshold")->setValueNotifyingHost(
            static_cast<float>(voicingThresholdSlider.getValue()) / 100.0f);
        DBG("Voicing Threshold: " << voicingThresholdSlider.getValue());
    }
    else if (slider == &analyzingSmoothingSlider)
    {
        auto& params = audioProcessor.getValueTreeState();
        params.getParameter("analyzingSmoothing")->setValueNotifyingHost(
            static_cast<float>(analyzingSmoothingSlider.getValue()) / 100.0f);
        DBG("Analysis Smoothing: " << analyzingSmoothingSlider.getValue());
    }
    else if (slider == &polyphonySlider)
    {
        auto& params = audioProcessor.getValueTreeState();
        params.getParameter("polyphony")->setValueNotifyingHost(
            static_cast<float>(polyphonySlider.getValue()) / 16.0f);
        DBG("Polyphony: " << static_cast<int>(polyphonySlider.getValue()));
    }
}

void ScaleChordPluginEditor::comboBoxChanged(juce::ComboBox* comboBoxThatHasChanged)
{
    if (comboBoxThatHasChanged == &scaleSelector)
    {
        int selectedScale = scaleSelector.getSelectedItemIndex();
        DBG("Scale selected: " << selectedScale << " - " << scaleOptions[selectedScale]);
    }
    else if (comboBoxThatHasChanged == &voiceLeadingSelector)
    {
        int selectedMode = voiceLeadingSelector.getSelectedItemIndex();
        audioProcessor.setVoiceLeadingMode(selectedMode);
        DBG("Voice Leading Mode: " << selectedMode << " - " 
            << voiceLeadingOptions[selectedMode]);
    }
}

void ScaleChordPluginEditor::buttonClicked(juce::Button* buttonThatWasClicked)
{
    if (buttonThatWasClicked == &midiEffectsButton)
    {
        bool enabled = midiEffectsButton.getToggleState();
        audioProcessor.setMidiEffectsEnabled(enabled);
        DBG("MIDI Effects: " << (enabled ? "ON" : "OFF"));
    }
    else if (buttonThatWasClicked == &jazzReharmonizationButton)
    {
        bool enabled = jazzReharmonizationButton.getToggleState();
        audioProcessor.setJazzReharmonizationEnabled(enabled);
        DBG("Jazz Reharmonization: " << (enabled ? "ON" : "OFF"));
    }
}

// ============================================================================
// Timer Callback
// ============================================================================

void ScaleChordPluginEditor::timerCallback()
{
    // Update display labels in real-time
    updateDisplay();
}

void ScaleChordPluginEditor::updateDisplay()
{
    // Update status
    statusLabel.setText("Status: Running @ " + 
                       juce::String(static_cast<int>(audioProcessor.getSampleRate())) + " Hz",
                       juce::dontSendNotification);

    // Update active notes display
    activeNotesLabel.setText("Active Notes: 0", juce::dontSendNotification);

    // Update voicing display
    voicingDisplayLabel.setText("Voicing: " + audioProcessor.getCurrentScale(),
                                juce::dontSendNotification);
}

void ScaleChordPluginEditor::syncWithProcessor()
{
    // Sync UI with processor state
    scaleSelector.setSelectedItemIndex(0, juce::dontSendNotification);
    voiceLeadingSelector.setSelectedItemIndex(audioProcessor.getVoiceLeadingMode(),
                                             juce::dontSendNotification);
}
