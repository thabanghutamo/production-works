#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <memory>

namespace scalechord {

/**
 * @class Preset
 * @brief Represents a complete plugin configuration preset
 */
struct Preset {
    // Metadata
    std::string name;
    std::string description;
    std::string category;  // "Jazz", "Blues", "Classical", etc.
    std::string author = "ScaleChord";
    std::string createdDate;
    int version = 1;
    
    // Plugin Configuration
    struct {
        int rootNote = 0;              // 0-11 (C-B)
        int scale = 0;                 // 0-14 (15 scale types)
        int voicing = 0;               // 0-2 (Triad, Seventh, Open)
        int octaveOffset = 0;          // -2 to +2
        int outputChannel = 0;         // 0-15 (MIDI channels)
    } musicSettings;
    
    // Envelope Parameters
    struct {
        float attack = 10.0f;          // ms
        float decay = 100.0f;          // ms
        float sustain = 0.7f;          // 0.0-1.0
        float release = 300.0f;        // ms
        float velocitySensitivity = 0.5f;
        bool humanize = false;
        float humanizeAmount = 0.05f;
    } envelopeSettings;
    
    // MIDI Effects
    struct {
        bool arpeggiatorEnabled = false;
        int arpeggiatorMode = 0;       // 0-5 (Up, Down, UpDown, Random, Hold, Strum)
        int arpeggiatorSync = 0;       // 0-3 (sync options)
        
        bool humanizerEnabled = false;
        int humanizerMode = 0;         // 0-2 (Velocity, Timing, Pitch)
        
        bool probabilityEnabled = false;
        float probabilityAmount = 0.5f;
    } midiEffects;
};

/**
 * @class PresetManager
 * @brief Manages saving, loading, and organizing presets
 */
class PresetManager {
public:
    PresetManager();
    
    // ===== Preset Creation =====
    
    /**
     * Create a new preset with default values
     */
    static Preset createDefaultPreset(const std::string& name, const std::string& category);
    
    // ===== File Operations =====
    
    /**
     * Save preset to file (JSON format)
     */
    bool savePreset(const Preset& preset, const std::string& filepath);
    
    /**
     * Load preset from file
     */
    bool loadPreset(const std::string& filepath, Preset& outPreset);
    
    // ===== Preset Library =====
    
    /**
     * Get all available presets organized by category
     */
    std::unordered_map<std::string, std::vector<Preset>> getAllPresets() const;
    
    /**
     * Get presets by category
     */
    std::vector<Preset> getPresetsByCategory(const std::string& category) const;
    
    /**
     * Add preset to library
     */
    void addPreset(const Preset& preset);
    
    /**
     * Remove preset by name
     */
    bool removePreset(const std::string& name);
    
    // ===== Factory Presets =====
    
    /**
     * Load all factory presets into the library
     */
    void loadFactoryPresets();
    
    /**
     * Get count of presets
     */
    size_t getPresetCount() const { return presets_.size(); }
    
    // ===== JSON Serialization =====
    
    /**
     * Convert preset to JSON string
     */
    static std::string presetToJson(const Preset& preset);
    
    /**
     * Convert JSON string to preset
     */
    static bool jsonToPreset(const std::string& json, Preset& outPreset);

private:
    std::vector<Preset> presets_;
    
    // Internal helpers
    static std::vector<Preset> createFactoryPresets();
};

} // namespace scalechord
