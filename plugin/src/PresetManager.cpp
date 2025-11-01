#include "PresetManager.h"
#include <fstream>
#include <sstream>
#include <iomanip>
#include <ctime>
#include <algorithm>

namespace scalechord {

// ============================================================================
// JSON Serialization Helpers (built-in, no external dependencies)
// ============================================================================

namespace json {
    // Escape special characters for JSON strings
    static std::string escapeString(const std::string& str) {
        std::string result;
        for (char c : str) {
            switch (c) {
                case '"':  result += "\\\""; break;
                case '\\': result += "\\\\"; break;
                case '\b': result += "\\b"; break;
                case '\f': result += "\\f"; break;
                case '\n': result += "\\n"; break;
                case '\r': result += "\\r"; break;
                case '\t': result += "\\t"; break;
                default:
                    if (c < 32) {
                        char buf[8];
                        snprintf(buf, sizeof(buf), "\\u%04x", (unsigned char)c);
                        result += buf;
                    } else {
                        result += c;
                    }
            }
        }
        return result;
    }

    // Unescape JSON string
    static std::string unescapeString(const std::string& str) {
        std::string result;
        size_t i = 0;
        while (i < str.length()) {
            if (str[i] == '\\' && i + 1 < str.length()) {
                char next = str[++i];
                switch (next) {
                    case '"':  result += '"'; break;
                    case '\\': result += '\\'; break;
                    case '/':  result += '/'; break;
                    case 'b':  result += '\b'; break;
                    case 'f':  result += '\f'; break;
                    case 'n':  result += '\n'; break;
                    case 'r':  result += '\r'; break;
                    case 't':  result += '\t'; break;
                    default:   result += next;
                }
            } else {
                result += str[i];
            }
            ++i;
        }
        return result;
    }

    // Get current date as ISO 8601 string
    static std::string getCurrentDate() {
        auto now = std::time(nullptr);
        auto tm = *std::localtime(&now);
        std::ostringstream oss;
        oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
        return oss.str();
    }
}

// ============================================================================
// PresetManager Implementation
// ============================================================================

PresetManager::PresetManager() {
    loadFactoryPresets();
}

Preset PresetManager::createDefaultPreset(const std::string& name, const std::string& category) {
    Preset p;
    p.name = name;
    p.category = category;
    p.createdDate = json::getCurrentDate();
    return p;
}

// ============================================================================
// Factory Presets
// ============================================================================

std::vector<Preset> PresetManager::createFactoryPresets() {
    std::vector<Preset> presets;
    
    // JAZZ PRESETS
    {
        auto p = createDefaultPreset("Bebop", "Jazz");
        p.description = "Classic bebop voicing with minor 7th chords";
        p.musicSettings.scale = 6;  // Dorian
        p.musicSettings.voicing = 1; // Seventh
        p.envelopeSettings.attack = 5.0f;
        p.envelopeSettings.decay = 50.0f;
        p.envelopeSettings.sustain = 0.8f;
        p.envelopeSettings.release = 200.0f;
        p.midiEffects.humanizerEnabled = true;
        presets.push_back(p);
    }
    
    {
        auto p = createDefaultPreset("Cool Jazz", "Jazz");
        p.description = "Smooth, relaxed jazz sound";
        p.musicSettings.scale = 5;  // Aeolian (minor)
        p.musicSettings.voicing = 1; // Seventh
        p.envelopeSettings.attack = 20.0f;
        p.envelopeSettings.decay = 100.0f;
        p.envelopeSettings.sustain = 0.7f;
        p.envelopeSettings.release = 400.0f;
        p.envelopeSettings.humanize = true;
        presets.push_back(p);
    }
    
    {
        auto p = createDefaultPreset("Fusion", "Jazz");
        p.description = "High-energy fusion with open voicing";
        p.musicSettings.scale = 7;  // Harmonic Minor
        p.musicSettings.voicing = 2; // Open
        p.envelopeSettings.attack = 2.0f;
        p.envelopeSettings.decay = 30.0f;
        p.envelopeSettings.sustain = 0.9f;
        p.envelopeSettings.release = 100.0f;
        presets.push_back(p);
    }
    
    {
        auto p = createDefaultPreset("Smooth Jazz", "Jazz");
        p.description = "Mellow and atmospheric";
        p.musicSettings.scale = 0;  // Ionian (Major)
        p.musicSettings.voicing = 1; // Seventh
        p.envelopeSettings.attack = 30.0f;
        p.envelopeSettings.decay = 120.0f;
        p.envelopeSettings.sustain = 0.6f;
        p.envelopeSettings.release = 500.0f;
        p.envelopeSettings.velocitySensitivity = 0.3f;
        presets.push_back(p);
    }
    
    // BLUES PRESETS
    {
        auto p = createDefaultPreset("12-Bar Blues", "Blues");
        p.description = "Classic 12-bar blues progression";
        p.musicSettings.scale = 11;  // Minor Blues
        p.musicSettings.voicing = 0;  // Triad
        p.envelopeSettings.attack = 10.0f;
        p.envelopeSettings.decay = 75.0f;
        p.envelopeSettings.sustain = 0.7f;
        p.envelopeSettings.release = 250.0f;
        p.midiEffects.arpeggiatorEnabled = true;
        p.midiEffects.arpeggiatorMode = 0;  // Up
        presets.push_back(p);
    }
    
    {
        auto p = createDefaultPreset("Minor Blues", "Blues");
        p.description = "Moody minor blues with humanization";
        p.musicSettings.scale = 10;  // Minor Pentatonic
        p.musicSettings.voicing = 0;  // Triad
        p.envelopeSettings.attack = 8.0f;
        p.envelopeSettings.decay = 60.0f;
        p.envelopeSettings.sustain = 0.75f;
        p.envelopeSettings.release = 300.0f;
        p.envelopeSettings.humanize = true;
        presets.push_back(p);
    }
    
    {
        auto p = createDefaultPreset("Chicago Blues", "Blues");
        p.description = "Electric Chicago blues sound";
        p.musicSettings.scale = 5;  // Aeolian (minor)
        p.musicSettings.voicing = 1; // Seventh
        p.envelopeSettings.attack = 3.0f;
        p.envelopeSettings.decay = 40.0f;
        p.envelopeSettings.sustain = 0.85f;
        p.envelopeSettings.release = 150.0f;
        presets.push_back(p);
    }
    
    {
        auto p = createDefaultPreset("Delta Blues", "Blues");
        p.description = "Acoustic, soulful delta blues";
        p.musicSettings.scale = 10;  // Minor Pentatonic
        p.musicSettings.voicing = 0;  // Triad
        p.envelopeSettings.attack = 15.0f;
        p.envelopeSettings.decay = 80.0f;
        p.envelopeSettings.sustain = 0.6f;
        p.envelopeSettings.release = 350.0f;
        p.envelopeSettings.humanize = true;
        presets.push_back(p);
    }
    
    // CLASSICAL PRESETS
    {
        auto p = createDefaultPreset("Baroque", "Classical");
        p.description = "Structured, precise baroque voicing";
        p.musicSettings.scale = 0;  // Ionian (Major)
        p.musicSettings.voicing = 0; // Triad
        p.envelopeSettings.attack = 5.0f;
        p.envelopeSettings.decay = 60.0f;
        p.envelopeSettings.sustain = 0.5f;
        p.envelopeSettings.release = 200.0f;
        presets.push_back(p);
    }
    
    {
        auto p = createDefaultPreset("Classical", "Classical");
        p.description = "Clean classical harmonic progressions";
        p.musicSettings.scale = 0;  // Ionian (Major)
        p.musicSettings.voicing = 1; // Seventh
        p.envelopeSettings.attack = 8.0f;
        p.envelopeSettings.decay = 100.0f;
        p.envelopeSettings.sustain = 0.4f;
        p.envelopeSettings.release = 400.0f;
        presets.push_back(p);
    }
    
    {
        auto p = createDefaultPreset("Romantic", "Classical");
        p.description = "Lush, emotional romantic era voicing";
        p.musicSettings.scale = 7;  // Harmonic Minor
        p.musicSettings.voicing = 1; // Seventh
        p.envelopeSettings.attack = 15.0f;
        p.envelopeSettings.decay = 120.0f;
        p.envelopeSettings.sustain = 0.6f;
        p.envelopeSettings.release = 500.0f;
        p.envelopeSettings.humanize = true;
        presets.push_back(p);
    }
    
    // FOLK PRESETS
    {
        auto p = createDefaultPreset("Celtic", "Folk");
        p.description = "Celtic folk scales and patterns";
        p.musicSettings.scale = 2;  // Phrygian
        p.musicSettings.voicing = 0; // Triad
        p.envelopeSettings.attack = 10.0f;
        p.envelopeSettings.decay = 50.0f;
        p.envelopeSettings.sustain = 0.7f;
        p.envelopeSettings.release = 200.0f;
        presets.push_back(p);
    }
    
    {
        auto p = createDefaultPreset("Bluegrass", "Folk");
        p.description = "Bright, energetic bluegrass";
        p.musicSettings.scale = 9;  // Major Pentatonic
        p.musicSettings.voicing = 0; // Triad
        p.envelopeSettings.attack = 2.0f;
        p.envelopeSettings.decay = 30.0f;
        p.envelopeSettings.sustain = 0.8f;
        p.envelopeSettings.release = 100.0f;
        p.midiEffects.arpeggiatorEnabled = true;
        presets.push_back(p);
    }
    
    {
        auto p = createDefaultPreset("Klezmer", "Folk");
        p.description = "Jewish folk music with minor scales";
        p.musicSettings.scale = 7;  // Harmonic Minor
        p.musicSettings.voicing = 0; // Triad
        p.envelopeSettings.attack = 5.0f;
        p.envelopeSettings.decay = 40.0f;
        p.envelopeSettings.sustain = 0.75f;
        p.envelopeSettings.release = 150.0f;
        presets.push_back(p);
    }
    
    // MODERN PRESETS
    {
        auto p = createDefaultPreset("EDM", "Modern");
        p.description = "Electronic dance music with arpeggiation";
        p.musicSettings.scale = 13;  // Whole Tone
        p.musicSettings.voicing = 2;  // Open
        p.envelopeSettings.attack = 1.0f;
        p.envelopeSettings.decay = 20.0f;
        p.envelopeSettings.sustain = 1.0f;
        p.envelopeSettings.release = 50.0f;
        p.midiEffects.arpeggiatorEnabled = true;
        p.midiEffects.arpeggiatorMode = 2;  // UpDown
        presets.push_back(p);
    }
    
    {
        auto p = createDefaultPreset("Hip-Hop", "Modern");
        p.description = "Groove-oriented hip-hop chords";
        p.musicSettings.scale = 5;  // Aeolian (minor)
        p.musicSettings.voicing = 0; // Triad
        p.envelopeSettings.attack = 3.0f;
        p.envelopeSettings.decay = 50.0f;
        p.envelopeSettings.sustain = 0.8f;
        p.envelopeSettings.release = 120.0f;
        presets.push_back(p);
    }
    
    {
        auto p = createDefaultPreset("Ambient", "Modern");
        p.description = "Atmospheric ambient soundscapes";
        p.musicSettings.scale = 14;  // Diminished
        p.musicSettings.voicing = 1;  // Seventh
        p.envelopeSettings.attack = 50.0f;
        p.envelopeSettings.decay = 150.0f;
        p.envelopeSettings.sustain = 0.5f;
        p.envelopeSettings.release = 1000.0f;
        p.envelopeSettings.velocitySensitivity = 0.1f;
        presets.push_back(p);
    }
    
    // EXPERIMENTAL PRESETS
    {
        auto p = createDefaultPreset("Whole Tone", "Experimental");
        p.description = "Experimental whole tone scale";
        p.musicSettings.scale = 13;  // Whole Tone
        p.musicSettings.voicing = 1;  // Seventh
        p.envelopeSettings.attack = 10.0f;
        p.envelopeSettings.decay = 60.0f;
        p.envelopeSettings.sustain = 0.7f;
        p.envelopeSettings.release = 200.0f;
        presets.push_back(p);
    }
    
    {
        auto p = createDefaultPreset("Diminished", "Experimental");
        p.description = "Diminished scale with unique harmonies";
        p.musicSettings.scale = 14;  // Diminished
        p.musicSettings.voicing = 0;  // Triad
        p.envelopeSettings.attack = 8.0f;
        p.envelopeSettings.decay = 40.0f;
        p.envelopeSettings.sustain = 0.8f;
        p.envelopeSettings.release = 150.0f;
        presets.push_back(p);
    }
    
    {
        auto p = createDefaultPreset("Atonal Modulation", "Experimental");
        p.description = "Rapid scale changes for experimental music";
        p.musicSettings.scale = 7;  // Harmonic Minor
        p.musicSettings.voicing = 2; // Open
        p.envelopeSettings.attack = 2.0f;
        p.envelopeSettings.decay = 25.0f;
        p.envelopeSettings.sustain = 0.9f;
        p.envelopeSettings.release = 75.0f;
        presets.push_back(p);
    }
    
    return presets;
}

void PresetManager::loadFactoryPresets() {
    presets_ = createFactoryPresets();
}

void PresetManager::addPreset(const Preset& preset) {
    presets_.push_back(preset);
}

bool PresetManager::removePreset(const std::string& name) {
    auto it = std::find_if(presets_.begin(), presets_.end(),
                          [&name](const Preset& p) { return p.name == name; });
    if (it != presets_.end()) {
        presets_.erase(it);
        return true;
    }
    return false;
}

std::unordered_map<std::string, std::vector<Preset>> PresetManager::getAllPresets() const {
    std::unordered_map<std::string, std::vector<Preset>> organized;
    for (const auto& preset : presets_) {
        organized[preset.category].push_back(preset);
    }
    return organized;
}

std::vector<Preset> PresetManager::getPresetsByCategory(const std::string& category) const {
    std::vector<Preset> result;
    for (const auto& preset : presets_) {
        if (preset.category == category) {
            result.push_back(preset);
        }
    }
    return result;
}

// ============================================================================
// JSON Serialization
// ============================================================================

std::string PresetManager::presetToJson(const Preset& preset) {
    std::ostringstream oss;
    
    oss << "{\n";
    oss << "  \"name\": \"" << json::escapeString(preset.name) << "\",\n";
    oss << "  \"description\": \"" << json::escapeString(preset.description) << "\",\n";
    oss << "  \"category\": \"" << preset.category << "\",\n";
    oss << "  \"author\": \"" << preset.author << "\",\n";
    oss << "  \"createdDate\": \"" << preset.createdDate << "\",\n";
    oss << "  \"version\": " << preset.version << ",\n";
    
    // Music Settings
    oss << "  \"musicSettings\": {\n";
    oss << "    \"rootNote\": " << preset.musicSettings.rootNote << ",\n";
    oss << "    \"scale\": " << preset.musicSettings.scale << ",\n";
    oss << "    \"voicing\": " << preset.musicSettings.voicing << ",\n";
    oss << "    \"octaveOffset\": " << preset.musicSettings.octaveOffset << ",\n";
    oss << "    \"outputChannel\": " << preset.musicSettings.outputChannel << "\n";
    oss << "  },\n";
    
    // Envelope Settings
    oss << "  \"envelopeSettings\": {\n";
    oss << "    \"attack\": " << std::fixed << std::setprecision(2) << preset.envelopeSettings.attack << ",\n";
    oss << "    \"decay\": " << preset.envelopeSettings.decay << ",\n";
    oss << "    \"sustain\": " << preset.envelopeSettings.sustain << ",\n";
    oss << "    \"release\": " << preset.envelopeSettings.release << ",\n";
    oss << "    \"velocitySensitivity\": " << preset.envelopeSettings.velocitySensitivity << ",\n";
    oss << "    \"humanize\": " << (preset.envelopeSettings.humanize ? "true" : "false") << ",\n";
    oss << "    \"humanizeAmount\": " << preset.envelopeSettings.humanizeAmount << "\n";
    oss << "  },\n";
    
    // MIDI Effects
    oss << "  \"midiEffects\": {\n";
    oss << "    \"arpeggiatorEnabled\": " << (preset.midiEffects.arpeggiatorEnabled ? "true" : "false") << ",\n";
    oss << "    \"arpeggiatorMode\": " << preset.midiEffects.arpeggiatorMode << ",\n";
    oss << "    \"arpeggiatorSync\": " << preset.midiEffects.arpeggiatorSync << ",\n";
    oss << "    \"humanizerEnabled\": " << (preset.midiEffects.humanizerEnabled ? "true" : "false") << ",\n";
    oss << "    \"humanizerMode\": " << preset.midiEffects.humanizerMode << ",\n";
    oss << "    \"probabilityEnabled\": " << (preset.midiEffects.probabilityEnabled ? "true" : "false") << ",\n";
    oss << "    \"probabilityAmount\": " << preset.midiEffects.probabilityAmount << "\n";
    oss << "  }\n";
    oss << "}\n";
    
    return oss.str();
}

bool PresetManager::jsonToPreset(const std::string& json, Preset& outPreset) {
    // Simple JSON parser (handles our specific format)
    // For production, would use nlohmann/json or rapidjson
    
    try {
        // Extract string values
        auto extractString = [&json](const std::string& key) -> std::string {
            size_t pos = json.find("\"" + key + "\":");
            if (pos == std::string::npos) return "";
            pos = json.find("\"", pos + key.length() + 3);
            if (pos == std::string::npos) return "";
            size_t end = pos + 1;
            // Find closing quote, handling escapes
            while (end < json.length()) {
                if (json[end] == '"' && json[end - 1] != '\\') break;
                end++;
            }
            if (end == json.length()) return "";
            return json::unescapeString(json.substr(pos + 1, end - pos - 1));
        };
        
        // Extract int values
        auto extractInt = [&json](const std::string& key) -> int {
            size_t pos = json.find("\"" + key + "\":");
            if (pos == std::string::npos) return 0;
            pos = json.find(":", pos);
            size_t end = json.find(",", pos);
            if (end == std::string::npos) end = json.find("}", pos);
            std::string value = json.substr(pos + 1, end - pos - 1);
            // Skip whitespace
            value.erase(0, value.find_first_not_of(" \t\n\r"));
            return std::stoi(value);
        };
        
        // Extract float values
        auto extractFloat = [&json](const std::string& key) -> float {
            size_t pos = json.find("\"" + key + "\":");
            if (pos == std::string::npos) return 0.0f;
            pos = json.find(":", pos);
            size_t end = json.find(",", pos);
            if (end == std::string::npos) end = json.find("}", pos);
            std::string value = json.substr(pos + 1, end - pos - 1);
            // Skip whitespace
            value.erase(0, value.find_first_not_of(" \t\n\r"));
            return std::stof(value);
        };
        
        // Extract boolean values
        auto extractBool = [&json](const std::string& key) -> bool {
            size_t pos = json.find("\"" + key + "\":");
            if (pos == std::string::npos) return false;
            size_t end = json.find(",", pos);
            if (end == std::string::npos) end = json.find("}", pos);
            std::string value = json.substr(pos, end - pos);
            return value.find("true") != std::string::npos;
        };
        
        // Parse metadata
        outPreset.name = extractString("name");
        outPreset.description = extractString("description");
        outPreset.category = extractString("category");
        outPreset.author = extractString("author");
        outPreset.createdDate = extractString("createdDate");
        outPreset.version = extractInt("version");
        
        // Parse music settings
        outPreset.musicSettings.rootNote = extractInt("rootNote");
        outPreset.musicSettings.scale = extractInt("scale");
        outPreset.musicSettings.voicing = extractInt("voicing");
        outPreset.musicSettings.octaveOffset = extractInt("octaveOffset");
        outPreset.musicSettings.outputChannel = extractInt("outputChannel");
        
        // Parse envelope settings
        outPreset.envelopeSettings.attack = extractFloat("attack");
        outPreset.envelopeSettings.decay = extractFloat("decay");
        outPreset.envelopeSettings.sustain = extractFloat("sustain");
        outPreset.envelopeSettings.release = extractFloat("release");
        outPreset.envelopeSettings.velocitySensitivity = extractFloat("velocitySensitivity");
        outPreset.envelopeSettings.humanize = extractBool("humanize");
        outPreset.envelopeSettings.humanizeAmount = extractFloat("humanizeAmount");
        
        // Parse MIDI effects
        outPreset.midiEffects.arpeggiatorEnabled = extractBool("arpeggiatorEnabled");
        outPreset.midiEffects.arpeggiatorMode = extractInt("arpeggiatorMode");
        outPreset.midiEffects.arpeggiatorSync = extractInt("arpeggiatorSync");
        outPreset.midiEffects.humanizerEnabled = extractBool("humanizerEnabled");
        outPreset.midiEffects.humanizerMode = extractInt("humanizerMode");
        outPreset.midiEffects.probabilityEnabled = extractBool("probabilityEnabled");
        outPreset.midiEffects.probabilityAmount = extractFloat("probabilityAmount");
        
        return true;
    } catch (...) {
        return false;
    }
}

bool PresetManager::savePreset(const Preset& preset, const std::string& filepath) {
    try {
        std::ofstream file(filepath);
        if (!file.is_open()) return false;
        
        std::string json = presetToJson(preset);
        file << json;
        file.close();
        
        return true;
    } catch (...) {
        return false;
    }
}

bool PresetManager::loadPreset(const std::string& filepath, Preset& outPreset) {
    try {
        std::ifstream file(filepath);
        if (!file.is_open()) return false;
        
        std::stringstream buffer;
        buffer << file.rdbuf();
        file.close();
        
        return jsonToPreset(buffer.str(), outPreset);
    } catch (...) {
        return false;
    }
}

} // namespace scalechord
