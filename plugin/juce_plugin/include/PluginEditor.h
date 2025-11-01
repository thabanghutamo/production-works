#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

/**
 * ScaleChordPluginEditor
 *
 * GUI editor for the ScaleChord VST3 plugin.
 * Provides intuitive controls for:
 * - Scale selection
 * - Voice leading modes
 * - MIDI effects
 * - Jazz reharmonization
 * - Real-time visualization
 */
class ScaleChordPluginEditor : public juce::AudioProcessorEditor,
                               public juce::Slider::Listener,
                               public juce::ComboBox::Listener,
                               public juce::Button::Listener,
                               public juce::Timer
{
public:
    ScaleChordPluginEditor(ScaleChordPluginAudioProcessor&);
    ~ScaleChordPluginEditor() override;

    // ========================================================================
    // JUCE Component Overrides
    // ========================================================================

    void paint(juce::Graphics&) override;
    void resized() override;

    // ========================================================================
    // Component Interaction Callbacks
    // ========================================================================

    void sliderValueChanged(juce::Slider* slider) override;
    void comboBoxChanged(juce::ComboBox* comboBoxThatHasChanged) override;
    void buttonClicked(juce::Button* buttonThatWasClicked) override;

    // ========================================================================
    // Timer Callback for Real-time Updates
    // ========================================================================

    void timerCallback() override;

private:
    // ========================================================================
    // Reference to Processor
    // ========================================================================

    ScaleChordPluginAudioProcessor& audioProcessor;

    // ========================================================================
    // UI Components - Header
    // ========================================================================

    juce::Label titleLabel;
    juce::Label versionLabel;

    // ========================================================================
    // UI Components - Scale Selection
    // ========================================================================

    juce::Label scaleLabel;
    juce::ComboBox scaleSelector;

    // Scale options (Major, Minor, Pentatonic, Blues, etc.)
    juce::StringArray scaleOptions{
        "Major",
        "Natural Minor",
        "Harmonic Minor",
        "Melodic Minor",
        "Dorian",
        "Phrygian",
        "Lydian",
        "Mixolydian",
        "Major Pentatonic",
        "Minor Pentatonic",
        "Blues",
        "Whole Tone",
        "Chromatic"
    };

    // ========================================================================
    // UI Components - Voice Leading
    // ========================================================================

    juce::Label voiceLeadingLabel;
    juce::ComboBox voiceLeadingSelector;

    juce::StringArray voiceLeadingOptions{
        "Off",
        "Smooth",
        "Optimal",
        "Jazz"
    };

    // ========================================================================
    // UI Components - Toggles
    // ========================================================================

    juce::ToggleButton midiEffectsButton;
    juce::ToggleButton jazzReharmonizationButton;

    // ========================================================================
    // UI Components - Sliders
    // ========================================================================

    juce::Slider voicingThresholdSlider;
    juce::Label voicingThresholdLabel;

    juce::Slider analyzingSmoothingSlider;
    juce::Label analyzingSmoothingLabel;

    juce::Slider polyphonySlider;
    juce::Label polyphonyLabel;

    // ========================================================================
    // UI Components - Display
    // ========================================================================

    juce::Label statusLabel;
    juce::Label activeNotesLabel;
    juce::Label voicingDisplayLabel;

    // ========================================================================
    // Helper Methods
    // ========================================================================

    /**
     * Initialize all UI components
     */
    void initializeComponents();

    /**
     * Set up component styling
     */
    void setupComponentStyling();

    /**
     * Update display with current processor state
     */
    void updateDisplay();

    /**
     * Sync UI with processor parameters
     */
    void syncWithProcessor();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ScaleChordPluginEditor)
};
