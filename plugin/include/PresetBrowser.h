#pragma once

#include "PluginEditor.h"
#include "PresetManager.h"
#include <memory>
#include <vector>
#include <functional>

/**
 * @class PresetBrowser
 * @brief VST3 Plugin Preset Browser UI Component
 *
 * Provides an interactive preset browsing and management interface with:
 * - Category-based preset filtering
 * - Preset preview and quick access
 * - Save/load functionality
 * - Favorite presets marking
 * - Search/filter capabilities
 *
 * Designed to integrate into the PluginEditor as a resizable panel.
 */
class PresetBrowser {
public:
    // ===== Constants =====
    static constexpr int MIN_WIDTH = 200;
    static constexpr int MIN_HEIGHT = 300;
    static constexpr int ITEM_HEIGHT = 30;
    static constexpr int CATEGORY_HEIGHT = 35;
    static constexpr int BUTTON_HEIGHT = 28;
    
    // ===== Preset Item Structure =====
    struct PresetItem {
        std::string name;
        std::string category;
        bool isFavorite = false;
        int index = 0;
    };
    
    // ===== Constructor/Destructor =====
    explicit PresetBrowser(const scalechord::PresetManager& manager);
    ~PresetBrowser() = default;
    
    // ===== UI Management =====
    void paint(juce::Graphics& g, const RectInt& bounds);
    void resized(const RectInt& bounds);
    
    // ===== Preset Management =====
    void loadPreset(int index);
    void saveCurrentAsPreset(const std::string& name, const std::string& category);
    void deletePreset(int index);
    void toggleFavorite(int index);
    
    // ===== Category & Filter =====
    void setActiveCategory(const std::string& category);
    std::string getActiveCategory() const { return activeCategory_; }
    std::vector<std::string> getAvailableCategories() const;
    
    // ===== Data Access =====
    int getPresetCount() const { return presetItems_.size(); }
    const PresetItem& getPreset(int index) const { return presetItems_[index]; }
    
    // ===== Callbacks =====
    using PresetSelectedCallback = std::function<void(int presetIndex)>;
    void setPresetSelectedCallback(PresetSelectedCallback callback) { 
        onPresetSelected_ = callback; 
    }

private:
    // ===== Data =====
    const scalechord::PresetManager& presetManager_;
    std::vector<PresetItem> presetItems_;
    std::string activeCategory_;
    int selectedPresetIndex_ = -1;
    
    // ===== Layout State =====
    int categoryPanelWidth_ = 150;
    int previewPanelHeight_ = 100;
    
    // ===== Callbacks =====
    PresetSelectedCallback onPresetSelected_;
    
    // ===== Helper Methods =====
    void refreshPresetList();
    void drawCategoryPanel(juce::Graphics& g, const RectInt& area);
    void drawPresetList(juce::Graphics& g, const RectInt& area);
    void drawPresetPreview(juce::Graphics& g, const RectInt& area);
    void drawPresetItem(juce::Graphics& g, const RectInt& area, const PresetItem& item, bool isSelected);
};
