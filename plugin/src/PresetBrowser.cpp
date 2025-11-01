#include "PresetBrowser.h"

// ============================================================================
// Constructor
// ============================================================================
PresetBrowser::PresetBrowser(const scalechord::PresetManager& manager)
    : presetManager_(manager), activeCategory_("All")
{
    refreshPresetList();
}

// ============================================================================
// Paint - Main Rendering
// ============================================================================
void PresetBrowser::paint(juce::Graphics& g, const RectInt& bounds)
{
    // Background
    g.fillAll(juce::Colour(ScaleChordTheme::Panel));
    
    // Title bar
    RectInt titleBar = bounds.removeFromTop(30);
    g.setColour(juce::Colour(ScaleChordTheme::Background));
    g.fillRect(titleBar.getX(), titleBar.getY(), titleBar.getWidth(), titleBar.getHeight());
    
    g.setColour(juce::Colour(ScaleChordTheme::TextPrimary));
    g.setFont(juce::Font(14.0f).boldened());
    g.drawText(juce::String("PRESET BROWSER"), titleBar.getX() + 10, titleBar.getY(), 
               titleBar.getWidth() - 20, titleBar.getHeight(), juce::Justification::left);
    
    // Layout: Category panel (left), Preset list (main), Preview (bottom)
    RectInt categoryArea = bounds.removeFromLeft(categoryPanelWidth_);
    RectInt previewArea = bounds.removeFromBottom(previewPanelHeight_);
    RectInt presetArea = bounds;  // Remaining space
    
    // Draw sections
    drawCategoryPanel(g, categoryArea);
    drawPresetList(g, presetArea);
    drawPresetPreview(g, previewArea);
}

// ============================================================================
// Resized - Layout Management
// ============================================================================
void PresetBrowser::resized(const RectInt& bounds)
{
    // Adjust panel sizes based on new bounds if needed
    // For now, use default proportions
    categoryPanelWidth_ = bounds.getWidth() / 4;  // 25% for categories
    previewPanelHeight_ = bounds.getHeight() / 4;  // 25% for preview
}

// ============================================================================
// Load Preset
// ============================================================================
void PresetBrowser::loadPreset(int index)
{
    if (index >= 0 && index < static_cast<int>(presetItems_.size())) {
        selectedPresetIndex_ = index;
        if (onPresetSelected_) {
            onPresetSelected_(index);
        }
    }
}

// ============================================================================
// Save Current as Preset
// ============================================================================
void PresetBrowser::saveCurrentAsPreset(const std::string& name, const std::string& category)
{
    // This would integrate with PluginProcessor to get current parameters
    // For now, placeholder for structure
}

// ============================================================================
// Delete Preset
// ============================================================================
void PresetBrowser::deletePreset(int index)
{
    if (index >= 0 && index < static_cast<int>(presetItems_.size())) {
        presetItems_.erase(presetItems_.begin() + index);
        refreshPresetList();
    }
}

// ============================================================================
// Toggle Favorite
// ============================================================================
void PresetBrowser::toggleFavorite(int index)
{
    if (index >= 0 && index < static_cast<int>(presetItems_.size())) {
        presetItems_[index].isFavorite = !presetItems_[index].isFavorite;
    }
}

// ============================================================================
// Set Active Category
// ============================================================================
void PresetBrowser::setActiveCategory(const std::string& category)
{
    activeCategory_ = category;
    refreshPresetList();
}

// ============================================================================
// Get Available Categories
// ============================================================================
std::vector<std::string> PresetBrowser::getAvailableCategories() const
{
    std::vector<std::string> categories;
    categories.push_back("All");  // Special "All" category
    
    // Gather unique categories from presets
    // This would ideally come from PresetManager
    categories.push_back("Jazz");
    categories.push_back("Blues");
    categories.push_back("Classical");
    categories.push_back("Modern");
    categories.push_back("Experimental");
    
    return categories;
}

// ============================================================================
// Refresh Preset List
// ============================================================================
void PresetBrowser::refreshPresetList()
{
    presetItems_.clear();
    
    // Get presets from manager filtered by category
    // This is a placeholder - would use actual PresetManager API
    std::vector<scalechord::Preset> presets;
    if (activeCategory_ == "All") {
        // Get all presets
        // presets = presetManager_.getAllPresets();
    } else {
        // Get presets by category
        // presets = presetManager_.getPresetsByCategory(activeCategory_);
    }
    
    // Convert to PresetItem format
    for (size_t i = 0; i < presets.size(); ++i) {
        PresetItem item;
        item.name = presets[i].name;
        item.category = presets[i].category;
        item.index = static_cast<int>(i);
        presetItems_.push_back(item);
    }
}

// ============================================================================
// Draw Category Panel (Left side)
// ============================================================================
void PresetBrowser::drawCategoryPanel(juce::Graphics& g, const RectInt& area)
{
    if (area.getWidth() <= 0) return;
    
    // Background
    g.setColour(juce::Colour(ScaleChordTheme::Background));
    g.fillRect(area.getX(), area.getY(), area.getWidth(), area.getHeight());
    
    // Border
    g.setColour(juce::Colour(ScaleChordTheme::Panel));
    g.drawRect(area, 1);
}

// ============================================================================
// Draw Preset List (Center area)
// ============================================================================
void PresetBrowser::drawPresetList(juce::Graphics& g, const RectInt& area)
{
    if (area.getWidth() <= 0 || area.getHeight() <= 0) return;
    
    // Background
    g.setColour(juce::Colour(ScaleChordTheme::Background));
    g.fillRect(area.getX(), area.getY(), area.getWidth(), area.getHeight());
    
    // Border
    g.setColour(juce::Colour(ScaleChordTheme::Panel));
    g.drawRect(area, 1);
}

// ============================================================================
// Draw Preset Item
// ============================================================================
void PresetBrowser::drawPresetItem(juce::Graphics& g, const RectInt& area, 
                                    const PresetItem& item, bool isSelected)
{
    // Background
    if (isSelected) {
        g.setColour(juce::Colour(ScaleChordTheme::SliderThumb));
    } else {
        g.setColour(juce::Colour(ScaleChordTheme::Panel));
    }
    g.fillRect(area.getX(), area.getY(), area.getWidth(), area.getHeight());
}

// ============================================================================
// Draw Preset Preview (Bottom area)
// ============================================================================
void PresetBrowser::drawPresetPreview(juce::Graphics& g, const RectInt& area)
{
    if (area.getWidth() <= 0 || area.getHeight() <= 0) return;
    
    // Background
    g.setColour(juce::Colour(ScaleChordTheme::Background));
    g.fillRect(area.getX(), area.getY(), area.getWidth(), area.getHeight());
    
    // Border
    g.setColour(juce::Colour(ScaleChordTheme::Panel));
    g.drawRect(area, 1);
}
