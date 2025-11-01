#include <cstdio>
#include <cassert>
#include <string>
#include <vector>
#include "PresetManager.h"

using namespace scalechord;

// Test counter
int testCount = 0;
int passedCount = 0;

#define TEST(name) \
    printf("Testing: %s... ", name); \
    testCount++; \
    bool testPassed = true

#define ASSERT(condition) \
    if (!(condition)) { \
        printf("FAILED\n  Assertion failed: " #condition "\n"); \
        testPassed = false; \
    }

#define END_TEST() \
    if (testPassed) { \
        printf("✓\n"); \
        passedCount++; \
    }

int main() {
    printf("╔═══════════════════════════════════════════════════╗\n");
    printf("║  ScaleChord Task 7: Preset Manager Unit Tests     ║\n");
    printf("╚═══════════════════════════════════════════════════╝\n\n");

    // ========================================================================
    // Test 1: PresetManager Initialization
    // ========================================================================
    {
        TEST("PresetManager initialization and factory presets");
        PresetManager manager;
        
        ASSERT(manager.getPresetCount() > 0);
        ASSERT(manager.getPresetCount() == 20);  // We created 20 factory presets
        
        END_TEST();
    }

    // ========================================================================
    // Test 2: Category Organization
    // ========================================================================
    {
        TEST("Presets organized by category");
        PresetManager manager;
        
        auto organized = manager.getAllPresets();
        ASSERT(organized.find("Jazz") != organized.end());
        ASSERT(organized.find("Blues") != organized.end());
        ASSERT(organized.find("Classical") != organized.end());
        ASSERT(organized.find("Folk") != organized.end());
        ASSERT(organized.find("Modern") != organized.end());
        ASSERT(organized.find("Experimental") != organized.end());
        
        END_TEST();
    }

    // ========================================================================
    // Test 3: Get Presets by Category
    // ========================================================================
    {
        TEST("Get presets by category");
        PresetManager manager;
        
        auto jazzPresets = manager.getPresetsByCategory("Jazz");
        ASSERT(jazzPresets.size() == 4);  // Bebop, Cool Jazz, Fusion, Smooth Jazz
        
        auto bluesPresets = manager.getPresetsByCategory("Blues");
        ASSERT(bluesPresets.size() == 4);  // 12-Bar, Minor, Chicago, Delta
        
        auto classicalPresets = manager.getPresetsByCategory("Classical");
        ASSERT(classicalPresets.size() == 3);  // Baroque, Classical, Romantic
        
        END_TEST();
    }

    // ========================================================================
    // Test 4: Preset Details - Jazz
    // ========================================================================
    {
        TEST("Jazz Bebop preset configuration");
        PresetManager manager;
        
        auto jazzPresets = manager.getPresetsByCategory("Jazz");
        auto bebop = jazzPresets[0];
        
        ASSERT(bebop.name == "Bebop");
        ASSERT(bebop.musicSettings.scale == 6);  // Dorian
        ASSERT(bebop.musicSettings.voicing == 1); // Seventh
        ASSERT(bebop.midiEffects.humanizerEnabled == true);
        
        END_TEST();
    }

    // ========================================================================
    // Test 5: Preset Details - Blues
    // ========================================================================
    {
        TEST("Blues presets have correct configurations");
        PresetManager manager;
        
        auto bluesPresets = manager.getPresetsByCategory("Blues");
        
        // Check 12-Bar Blues
        ASSERT(bluesPresets[0].name == "12-Bar Blues");
        ASSERT(bluesPresets[0].musicSettings.scale == 11);  // Minor Blues
        ASSERT(bluesPresets[0].midiEffects.arpeggiatorEnabled == true);
        
        // Check Delta Blues
        ASSERT(bluesPresets[3].name == "Delta Blues");
        ASSERT(bluesPresets[3].envelopeSettings.humanize == true);
        
        END_TEST();
    }

    // ========================================================================
    // Test 6: Preset Details - Classical
    // ========================================================================
    {
        TEST("Classical presets have romantic character");
        PresetManager manager;
        
        auto classicalPresets = manager.getPresetsByCategory("Classical");
        auto romantic = classicalPresets[2];  // Romantic
        
        ASSERT(romantic.name == "Romantic");
        ASSERT(romantic.envelopeSettings.attack > 10.0f);  // Slow attack
        ASSERT(romantic.envelopeSettings.release > 400.0f); // Long release
        
        END_TEST();
    }

    // ========================================================================
    // Test 7: Preset Details - Modern
    // ========================================================================
    {
        TEST("Modern EDM preset has arpeggiator");
        PresetManager manager;
        
        auto modernPresets = manager.getPresetsByCategory("Modern");
        auto edm = modernPresets[0];
        
        ASSERT(edm.name == "EDM");
        ASSERT(edm.midiEffects.arpeggiatorEnabled == true);
        ASSERT(edm.midiEffects.arpeggiatorMode == 2);  // UpDown
        ASSERT(edm.envelopeSettings.attack < 2.0f);    // Fast attack
        
        END_TEST();
    }

    // ========================================================================
    // Test 8: Preset Details - Experimental
    // ========================================================================
    {
        TEST("Experimental presets use exotic scales");
        PresetManager manager;
        
        auto expPresets = manager.getPresetsByCategory("Experimental");
        
        // Whole Tone
        ASSERT(expPresets[0].musicSettings.scale == 13);
        
        // Diminished
        ASSERT(expPresets[1].musicSettings.scale == 14);
        
        // Atonal Modulation
        ASSERT(expPresets[2].name == "Atonal Modulation");
        
        END_TEST();
    }

    // ========================================================================
    // Test 9: Create Default Preset
    // ========================================================================
    {
        TEST("Create custom preset with defaults");
        auto custom = PresetManager::createDefaultPreset("My Preset", "Custom");
        
        ASSERT(custom.name == "My Preset");
        ASSERT(custom.category == "Custom");
        ASSERT(custom.musicSettings.scale == 0);  // Default Ionian
        ASSERT(custom.musicSettings.voicing == 0); // Default Triad
        ASSERT(!custom.createdDate.empty());
        
        END_TEST();
    }

    // ========================================================================
    // Test 10: Add Custom Preset
    // ========================================================================
    {
        TEST("Add custom preset to manager");
        PresetManager manager;
        size_t initialCount = manager.getPresetCount();
        
        auto custom = PresetManager::createDefaultPreset("Test Preset", "TestCategory");
        manager.addPreset(custom);
        
        ASSERT(manager.getPresetCount() == initialCount + 1);
        
        auto testPresets = manager.getPresetsByCategory("TestCategory");
        ASSERT(testPresets.size() == 1);
        ASSERT(testPresets[0].name == "Test Preset");
        
        END_TEST();
    }

    // ========================================================================
    // Test 11: Remove Preset
    // ========================================================================
    {
        TEST("Remove preset from manager");
        PresetManager manager;
        size_t initialCount = manager.getPresetCount();
        
        bool removed = manager.removePreset("Bebop");
        ASSERT(removed == true);
        ASSERT(manager.getPresetCount() == initialCount - 1);
        
        auto jazzPresets = manager.getPresetsByCategory("Jazz");
        bool found = false;
        for (const auto& p : jazzPresets) {
            if (p.name == "Bebop") found = true;
        }
        ASSERT(found == false);
        
        END_TEST();
    }

    // ========================================================================
    // Test 12: JSON Serialization - String Escaping
    // ========================================================================
    {
        TEST("JSON string escaping and unescaping");
        auto preset = PresetManager::createDefaultPreset(
            "Test \"Quote\" Name", "Test"
        );
        preset.description = "Description with\nnewline";
        
        std::string json = PresetManager::presetToJson(preset);
        ASSERT(json.find("\\\"") != std::string::npos);  // Escaped quotes
        ASSERT(json.find("\\n") != std::string::npos);    // Escaped newline
        
        END_TEST();
    }

    // ========================================================================
    // Test 13: JSON Serialization - All Fields
    // ========================================================================
    {
        TEST("JSON contains all preset fields");
        auto preset = PresetManager::createDefaultPreset("Test", "Test");
        preset.author = "TestAuthor";
        preset.musicSettings.rootNote = 5;
        preset.musicSettings.scale = 7;
        preset.musicSettings.voicing = 1;
        preset.envelopeSettings.attack = 15.5f;
        preset.midiEffects.arpeggiatorEnabled = true;
        
        std::string json = PresetManager::presetToJson(preset);
        
        // Check key fields are present
        ASSERT(json.find("\"name\"") != std::string::npos);
        ASSERT(json.find("\"Test\"") != std::string::npos);
        ASSERT(json.find("\"rootNote\": 5") != std::string::npos);
        ASSERT(json.find("\"scale\": 7") != std::string::npos);
        ASSERT(json.find("\"voicing\": 1") != std::string::npos);
        ASSERT(json.find("15.50") != std::string::npos);  // Float with precision
        ASSERT(json.find("\"arpeggiatorEnabled\": true") != std::string::npos);
        
        END_TEST();
    }

    // ========================================================================
    // Test 14: JSON Deserialization
    // ========================================================================
    {
        TEST("Parse preset from JSON");
        auto original = PresetManager::createDefaultPreset("Parse Test", "TestCat");
        original.author = "TestAuthor";
        original.musicSettings.rootNote = 7;
        original.musicSettings.scale = 10;
        original.envelopeSettings.attack = 22.5f;
        original.midiEffects.humanizerEnabled = true;
        
        // Serialize to JSON
        std::string json = PresetManager::presetToJson(original);
        
        // Deserialize from JSON
        Preset parsed;
        bool success = PresetManager::jsonToPreset(json, parsed);
        
        ASSERT(success == true);
        ASSERT(parsed.name == "Parse Test");
        ASSERT(parsed.category == "TestCat");
        ASSERT(parsed.author == "TestAuthor");
        ASSERT(parsed.musicSettings.rootNote == 7);
        ASSERT(parsed.musicSettings.scale == 10);
        ASSERT(parsed.envelopeSettings.attack == 22.5f);
        ASSERT(parsed.midiEffects.humanizerEnabled == true);
        
        END_TEST();
    }

    // ========================================================================
    // Test 15: Round-trip Serialization
    // ========================================================================
    {
        TEST("Full round-trip preset serialization");
        auto original = PresetManager::createDefaultPreset("Roundtrip", "Jazz");
        original.description = "Complex description with \"quotes\" and\nnewlines";
        original.musicSettings.octaveOffset = -1;
        original.envelopeSettings.sustain = 0.75f;
        original.envelopeSettings.humanize = true;
        original.midiEffects.arpeggiatorEnabled = true;
        original.midiEffects.arpeggiatorMode = 3;
        
        // Serialize
        std::string json = PresetManager::presetToJson(original);
        
        // Deserialize
        Preset restored;
        bool success = PresetManager::jsonToPreset(json, restored);
        
        // Verify all fields match
        ASSERT(success);
        ASSERT(restored.name == original.name);
        ASSERT(restored.description == original.description);
        ASSERT(restored.musicSettings.octaveOffset == original.musicSettings.octaveOffset);
        ASSERT(restored.envelopeSettings.sustain == original.envelopeSettings.sustain);
        ASSERT(restored.envelopeSettings.humanize == original.envelopeSettings.humanize);
        ASSERT(restored.midiEffects.arpeggiatorEnabled == original.midiEffects.arpeggiatorEnabled);
        ASSERT(restored.midiEffects.arpeggiatorMode == original.midiEffects.arpeggiatorMode);
        
        END_TEST();
    }

    // ========================================================================
    // Test Summary
    // ========================================================================
    printf("\n╔═══════════════════════════════════════════════════╗\n");
    printf("║                   Test Summary                    ║\n");
    printf("╠═══════════════════════════════════════════════════╣\n");
    printf("║ Passed: %d/%d                                     ║\n", passedCount, testCount);
    
    if (passedCount == testCount) {
        printf("║ Status: ✓ ALL TESTS PASSED                       ║\n");
        printf("╚═══════════════════════════════════════════════════╝\n");
        return 0;
    } else {
        printf("║ Status: ✗ SOME TESTS FAILED                      ║\n");
        printf("╚═══════════════════════════════════════════════════╝\n");
        return 1;
    }
}
