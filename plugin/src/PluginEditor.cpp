#include "PluginEditor.h"

#ifdef JUCE_VERSION
    // Full JUCE implementation
    #include "JUCEPluginProcessor.h"
    #include <cmath>
    #include <cstdlib>
#endif

// ============================================================================
// PluginEditor - Constructor
// ============================================================================
#ifdef JUCE_VERSION
PluginEditor::PluginEditor(PluginProcessor* ownerFilter)
    : juce::AudioProcessorEditor(ownerFilter),
      processor(ownerFilter)
{
    // Window setup (JUCE mode)
    setSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    setResizable(true, true);
    
    // Initialize spectrum bands with peaks and hold counters
    spectrumBands.resize(SPECTRUM_BANDS, 0.0f);
    spectrumPeaks.resize(SPECTRUM_BANDS, 0.0f);
    peakHoldCounters.resize(SPECTRUM_BANDS, 0);
    
    // Initialize sliders and labels
    initializeSliders();
    initializeLabels();
    bindParametersToAPVTS();
    
    // Start 60 FPS update timer
    startTimer(16);  // ~60 FPS
}
#else
PluginEditor::PluginEditor(PluginProcessor* ownerFilter)
    : processor(ownerFilter)
{
    // Mock mode - minimal initialization
}
#endif

// ============================================================================
// PluginEditor - Destructor
// ============================================================================
#ifdef JUCE_VERSION
PluginEditor::~PluginEditor()
{
    stopTimer();
}
#else
PluginEditor::~PluginEditor()
{
    // Mock mode - nothing to clean up
}
#endif

#ifdef JUCE_VERSION

// ============================================================================
// Initialize Sliders (12 parameters) - JUCE MODE
// ============================================================================
void PluginEditor::initializeSliders()
{
    for (int i = 0; i < NumParams; ++i) {
        auto slider = std::make_unique<juce::Slider>(juce::Slider::RotaryVerticalDrag, 
                                                      juce::Slider::NoTextBox);
        slider->setRange(0.0, 1.0, 0.01);
        slider->setValue(0.5);
        slider->setColour(juce::Slider::thumbColourId, juce::Colour(ScaleChordTheme::SliderThumb));
        slider->setColour(juce::Slider::rotarySliderFillColourId, juce::Colour(ScaleChordTheme::SliderThumb));
        slider->setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colour(ScaleChordTheme::Panel));
        
        addAndMakeVisible(*slider);
        paramSliders.push_back(std::move(slider));
    }
}

// ============================================================================
// Initialize Labels (12 parameter names) - JUCE MODE
// ============================================================================
void PluginEditor::initializeLabels()
{
    for (int i = 0; i < NumParams; ++i) {
        auto label = std::make_unique<juce::Label>(juce::String(i), 
                                                    juce::String(parameterNames[i]));
        label->setFont(juce::Font(12.0f));
        label->setColour(juce::Label::textColourId, juce::Colour(ScaleChordTheme::TextPrimary));
        label->setJustificationType(juce::Justification::centred);
        
        addAndMakeVisible(*label);
        paramLabels.push_back(std::move(label));
    }
}

// ============================================================================
// Bind Parameters to APVTS - JUCE MODE
// ============================================================================
void PluginEditor::bindParametersToAPVTS()
{
    // This method would bind each slider to APVTS when PluginProcessor is updated
    // For now, it's a placeholder that will be expanded when PluginProcessor
    // is updated with proper AudioProcessorValueTreeState integration
    
    if (processor == nullptr) return;
    
    // Placeholder for APVTS binding
    // Each slider will be attached to corresponding parameter
}

// ============================================================================
// Paint - Main Rendering - JUCE MODE
// ============================================================================
void PluginEditor::paint(juce::Graphics& g)
{
    // Background
    g.fillAll(juce::Colour(ScaleChordTheme::Background));
    
    // Get layout areas using responsive layout
    auto bounds = getLocalBounds();
    RectInt leftPanel, centerPanel, rightPanel;
    layoutPanels(bounds.getWidth(), bounds.getHeight(), leftPanel, centerPanel, rightPanel);
    
    // Draw sections
    drawParameterSection(g, leftPanel);
    drawSpectrumBands(g, centerPanel);
    drawSpectrumPeaks(g, centerPanel);  // Overlay peaks on spectrum
    drawMetricsPanel(g, rightPanel);
}

// ============================================================================
// Resized - Layout Management - JUCE MODE
// ============================================================================
void PluginEditor::resized()
{
    auto bounds = getLocalBounds();
    
    // Layout: Left (params), Center (spectrum), Right (metrics)
    int leftWidth = bounds.getWidth() / 3;
    int rightWidth = bounds.getWidth() / 3;
    
    auto leftPanel = bounds.removeFromLeft(leftWidth);
    auto rightPanel = bounds.removeFromRight(rightWidth);
    
    // Layout sliders and labels in left panel
    int yOffset = PANEL_MARGIN;
    const int sliderSpacing = 50;
    
    for (int i = 0; i < NumParams; ++i) {
        if (i < 6) {
            // First column (6 sliders)
            paramLabels[i]->setBounds(leftPanel.getX() + 5, yOffset, 35, LABEL_HEIGHT);
            paramSliders[i]->setBounds(leftPanel.getX() + 45, yOffset, SLIDER_WIDTH, SLIDER_WIDTH);
            yOffset += sliderSpacing;
        } else {
            // Second column (6 sliders) - next to first column
            int offset = 100;
            int adjustedIndex = i - 6;
            paramLabels[i]->setBounds(leftPanel.getX() + offset + 5, 
                                     PANEL_MARGIN + (adjustedIndex * sliderSpacing), 
                                     35, LABEL_HEIGHT);
            paramSliders[i]->setBounds(leftPanel.getX() + offset + 45, 
                                      PANEL_MARGIN + (adjustedIndex * sliderSpacing), 
                                      SLIDER_WIDTH, SLIDER_WIDTH);
        }
    }
}

// ============================================================================
// TimerCallback - Real-Time Updates (60 FPS) - JUCE MODE
// ============================================================================
void PluginEditor::timerCallback()
{
    updateMetricsDisplay();
    updateSpectrumAnalyzer();
    updateSpectrumWithPeaks();  // Update peak hold and decay
    repaint();
}

// ============================================================================
// Update Metrics Display - JUCE MODE
// ============================================================================
void PluginEditor::updateMetricsDisplay()
{
    if (processor == nullptr) return;
    
    // Query PerformanceDashboard for current metrics
    // Note: This requires PluginProcessor to have a PerformanceDashboard instance
    // For now, we simulate reasonable metrics for demonstration
    
    // Simulated metrics (will be replaced with real dashboard queries)
    // In production, this would be:
    // auto voiceMetrics = processor->getDashboard().getVoiceMetrics();
    // auto cpuMetrics = processor->getDashboard().getCPUMetrics();
    // auto latencyMetrics = processor->getDashboard().getLatencyMetrics();
    // auto health = processor->getDashboard().getHealthScore();
    
    // For now, maintain placeholder structure
    // Voice count: typically 1-16 (simulate active voices)
    static int voiceCounter = 0;
    voiceCounter = (voiceCounter + 1) % 10;
    metrics.voiceCount = (voiceCounter < 5) ? 4 : 8;  // Simulate voice activity
    
    // CPU usage: typical plugin is 2-5% per block
    metrics.cpuUsage = 2.5f + (rand() % 30) / 100.0f;
    
    // Latency: typical is 0.5-1.0ms for modern systems
    metrics.latencyMs = 0.8f + (rand() % 20) / 1000.0f;
    
    // Health score: 0.0-1.0, where > 0.7 is good
    float cpuHealth = 1.0f - (metrics.cpuUsage / 100.0f);  // Lower CPU = better
    float latencyHealth = 1.0f - (metrics.latencyMs / 10.0f);  // Lower latency = better
    metrics.healthScore = (cpuHealth + latencyHealth) / 2.0f;
    
    // Status text based on health
    if (metrics.healthScore > 0.8f) {
        metrics.statusText = "EXCELLENT";
    } else if (metrics.healthScore > 0.6f) {
        metrics.statusText = "GOOD";
    } else if (metrics.healthScore > 0.4f) {
        metrics.statusText = "OK";
    } else {
        metrics.statusText = "STRESSED";
    }
}

// ============================================================================
// Update Spectrum Analyzer - JUCE MODE
// ============================================================================
void PluginEditor::updateSpectrumAnalyzer()
{
    if (processor == nullptr) return;
    
    // Query PerformanceDashboard for spectrum data
    // For now, simulate audio spectrum with realistic patterns
    // In production, this would be:
    // auto specMetrics = processor->getDashboard().getSpectrumMetrics();
    // for (int i = 0; i < SPECTRUM_BANDS; ++i) {
    //     spectrumBands[i] = specMetrics.bands[i];
    // }
    
    // Simulate spectrum: emphasis on mid and low frequencies (typical of music)
    for (int i = 0; i < SPECTRUM_BANDS; ++i) {
        // Frequency distribution: low heavy (bass and mids), high sparse
        float freqFactor = static_cast<float>(i) / static_cast<float>(SPECTRUM_BANDS);
        
        // Bass emphasis (0-8 bands): higher energy
        float baseEnergy;
        if (i < 8) {
            baseEnergy = 0.6f + (rand() % 40) / 100.0f;  // 0.6-1.0
        }
        // Mids (8-20 bands): moderate energy
        else if (i < 20) {
            baseEnergy = 0.3f + (rand() % 40) / 100.0f;  // 0.3-0.7
        }
        // Highs (20+ bands): lower energy
        else {
            baseEnergy = 0.1f + (rand() % 20) / 100.0f;  // 0.1-0.3
        }
        
        // Apply smooth decay and new random data
        spectrumBands[i] = spectrumBands[i] * SPECTRUM_DECAY + baseEnergy * SPECTRUM_RISE;
        
        // Clamp to 0-1 range
        if (spectrumBands[i] > 1.0f) spectrumBands[i] = 1.0f;
        if (spectrumBands[i] < 0.0f) spectrumBands[i] = 0.0f;
    }
}

// ============================================================================
// Update Spectrum with Peaks - JUCE MODE (Advanced visualization)
// ============================================================================
void PluginEditor::updateSpectrumWithPeaks()
{
    for (int i = 0; i < SPECTRUM_BANDS; ++i) {
        // Update peak if current value exceeds it
        if (spectrumBands[i] > spectrumPeaks[i]) {
            spectrumPeaks[i] = spectrumBands[i];
            peakHoldCounters[i] = PEAK_HOLD_FRAMES;
        }
        
        // Decay peak hold counter
        if (peakHoldCounters[i] > 0) {
            peakHoldCounters[i]--;
        } else {
            // Release peak when hold expires
            spectrumPeaks[i] *= SPECTRUM_DECAY;
        }
    }
}

// ============================================================================
// Layout Panels (Responsive) - JUCE MODE
// ============================================================================
void PluginEditor::layoutPanels(int totalWidth, int totalHeight, 
                                 RectInt& leftPanel, RectInt& centerPanel, RectInt& rightPanel)
{
    // Calculate panel widths based on ratios
    int leftWidth = static_cast<int>(totalWidth * LEFT_PANEL_RATIO);
    int rightWidth = static_cast<int>(totalWidth * RIGHT_PANEL_RATIO);
    int centerWidth = totalWidth - leftWidth - rightWidth;
    
    // Ensure minimum sizes
    leftWidth = (leftWidth < 150) ? 150 : leftWidth;
    rightWidth = (rightWidth < 150) ? 150 : rightWidth;
    
    // Create panel bounds
    leftPanel = RectInt();      // Placeholder - ideally would set bounds
    centerPanel = RectInt();    // Placeholder
    rightPanel = RectInt();     // Placeholder
}

// ============================================================================
// Get Spectrum Colour (Gradient cyan->green->red) - JUCE MODE
// ============================================================================
juce::Colour PluginEditor::getSpectrumColour(float normalized)
{
    // Clamp to 0-1 range
    normalized = (normalized < 0.0f) ? 0.0f : (normalized > 1.0f) ? 1.0f : normalized;
    
    if (normalized < 0.5f) {
        // Cyan to green transition
        float t = normalized * 2.0f;  // 0-1 over first half
        uint8_t r = 0;
        uint8_t g = static_cast<uint8_t>(255 * t);
        uint8_t b = static_cast<uint8_t>(255 * (1.0f - t));
        return juce::Colour(0xFF000000 | (r << 16) | (g << 8) | b);
    } else {
        // Green to red transition
        float t = (normalized - 0.5f) * 2.0f;  // 0-1 over second half
        uint8_t r = static_cast<uint8_t>(255 * t);
        uint8_t g = static_cast<uint8_t>(255 * (1.0f - t));
        uint8_t b = 0;
        return juce::Colour(0xFF000000 | (r << 16) | (g << 8) | b);
    }
}

// ============================================================================
// Draw Parameter Section (Left Panel) - JUCE MODE - PHASE 6: ENHANCED THEMING
// ============================================================================
void PluginEditor::drawParameterSection(juce::Graphics& g, const RectInt& area)
{
    // Use local copies for panel layout
    auto workArea = area;
    
    // ===== Background with subtle gradient effect =====
    g.setColour(juce::Colour(ScaleChordTheme::Background));
    g.fillRect(workArea.getX(), workArea.getY(), workArea.getWidth(), workArea.getHeight());
    
    // Panel border with theme color
    g.setColour(juce::Colour(ScaleChordTheme::Panel));
    g.drawRect(workArea.getX(), workArea.getY(), workArea.getWidth(), workArea.getHeight(), 3);
    
    // Decorative top accent bar
    g.setColour(juce::Colour(ScaleChordTheme::SliderThumb));
    g.fillRect(workArea.getX(), workArea.getY(), workArea.getWidth(), 3);
    
    // Title section with padding
    auto titleArea = workArea.removeFromTop(40);
    g.setColour(juce::Colour(ScaleChordTheme::TextPrimary));
    g.setFont(juce::Font(15.0f).boldened());
    
    // Centered title with professional spacing
    int titlePadding = 10;
    int titleX = titleArea.getX() + titlePadding;
    int titleY = titleArea.getY() + 5;
    int titleWidth = titleArea.getWidth() - (titlePadding * 2);
    int titleHeight = titleArea.getHeight() - 5;
    g.drawText("PARAMETERS", titleX, titleY, titleWidth, titleHeight, juce::Justification::centred);
    
    // Subtle separator line below title
    g.setColour(juce::Colour(ScaleChordTheme::Panel));
    int separatorY = titleArea.getBottomY() - 1;
    g.drawLine(workArea.getX() + 5, separatorY, 
               workArea.getX() + workArea.getWidth() - 5, separatorY, 1.0f);
}

// ============================================================================
// Draw Spectrum Bands (Center Panel) - JUCE MODE - PHASE 6: ENHANCED THEMING
// ============================================================================
void PluginEditor::drawSpectrumBands(juce::Graphics& g, const RectInt& area)
{
    // Use local copies for panel layout
    auto workArea = area;
    
    // ===== Background with subtle gradient effect =====
    g.setColour(juce::Colour(ScaleChordTheme::Background));
    g.fillRect(workArea.getX(), workArea.getY(), workArea.getWidth(), workArea.getHeight());
    
    // Panel border with theme color
    g.setColour(juce::Colour(ScaleChordTheme::Panel));
    g.drawRect(workArea.getX(), workArea.getY(), workArea.getWidth(), workArea.getHeight(), 3);
    
    // Decorative top accent bar (different color for center panel)
    g.setColour(juce::Colour(ScaleChordTheme::SpectrumMid));
    g.fillRect(workArea.getX(), workArea.getY(), workArea.getWidth(), 3);
    
    // Title section with padding
    auto titleArea = workArea.removeFromTop(40);
    g.setColour(juce::Colour(ScaleChordTheme::TextPrimary));
    g.setFont(juce::Font(15.0f).boldened());
    
    // Centered title
    int titlePadding = 10;
    int titleX = titleArea.getX() + titlePadding;
    int titleY = titleArea.getY() + 5;
    int titleWidth = titleArea.getWidth() - (titlePadding * 2);
    int titleHeight = titleArea.getHeight() - 5;
    g.drawText("SPECTRUM", titleX, titleY, titleWidth, titleHeight, juce::Justification::centred);
    
    // Subtle separator line below title
    g.setColour(juce::Colour(ScaleChordTheme::Panel));
    int separatorY = titleArea.getBottomY() - 1;
    g.drawLine(workArea.getX() + 5, separatorY,
               workArea.getX() + workArea.getWidth() - 5, separatorY, 1.0f);
    
    // ===== Spectrum visualization with enhanced rendering =====
    auto spectrumArea = workArea.removeFromTop(180);
    int bandWidth = spectrumArea.getWidth() / SPECTRUM_BANDS;
    const int PADDING = 10;
    
    for (int i = 0; i < SPECTRUM_BANDS; ++i) {
        float normalizedValue = spectrumBands[i];
        int bandHeight = static_cast<int>(spectrumArea.getHeight() * 0.85f * normalizedValue);
        
        // Color gradient: Cyan -> Green -> Red with smooth interpolation
        juce::Colour bandColour;
        if (normalizedValue < 0.5f) {
            // Cyan to Green
            float blend = normalizedValue * 2.0f;
            bandColour = juce::Colour(ScaleChordTheme::SpectrumLow).interpolatedWith(
                         juce::Colour(ScaleChordTheme::SpectrumMid), blend);
        } else {
            // Green to Red
            float blend = (normalizedValue - 0.5f) * 2.0f;
            bandColour = juce::Colour(ScaleChordTheme::SpectrumMid).interpolatedWith(
                         juce::Colour(ScaleChordTheme::SpectrumHigh), blend);
        }
        
        int x = spectrumArea.getX() + PADDING + (i * bandWidth);
        int y = spectrumArea.getBottomY() - bandHeight - PADDING;
        int drawWidth = bandWidth - 2;
        
        g.setColour(bandColour);
        g.fillRect(x, y, drawWidth, bandHeight);
        
        // Subtle border around each band for definition
        g.setColour(bandColour);
        g.drawRect(x, y, drawWidth, bandHeight, 1);
    }
}

// ============================================================================
// Draw Spectrum Peaks (Overlay on Spectrum) - JUCE MODE - PHASE 6: ENHANCED THEMING
// ============================================================================
void PluginEditor::drawSpectrumPeaks(juce::Graphics& g, const RectInt& area)
{
    if (area.getWidth() <= 0 || area.getHeight() <= 0) return;
    
    int bandWidth = area.getWidth() / SPECTRUM_BANDS;
    const int PADDING = 10;
    const int SPECTRUM_AREA_HEIGHT = 180;
    
    for (int i = 0; i < SPECTRUM_BANDS; ++i) {
        float peakValue = spectrumPeaks[i];
        if (peakValue < 0.01f) continue;  // Skip if peak is negligible
        
        int peakHeight = static_cast<int>(SPECTRUM_AREA_HEIGHT * 0.85f * peakValue);
        int x = area.getX() + PADDING + (i * bandWidth);
        int y = area.getBottomY() - peakHeight - PADDING;
        
        // Peak line color - slightly brighter and more saturated than band color
        juce::Colour peakColour = getSpectrumColour(peakValue);
        g.setColour(peakColour);
        
        // Draw peak line with slightly thicker stroke for visibility
        g.drawLine(x, y, x + bandWidth - 2, y, 2.5f);
        
        // Optional: add subtle glow effect by drawing slightly offset line with reduced opacity
        // This would require more complex rendering, so we keep it simple for compatibility
    }
}

// ============================================================================
// Draw Metrics Panel (Right Panel) - JUCE MODE - PHASE 6: ENHANCED THEMING
// ============================================================================
void PluginEditor::drawMetricsPanel(juce::Graphics& g, const RectInt& area)
{
    // Use local copies for panel layout
    auto workArea = area;
    
    // ===== Background with subtle gradient effect =====
    g.setColour(juce::Colour(ScaleChordTheme::Background));
    g.fillRect(workArea.getX(), workArea.getY(), workArea.getWidth(), workArea.getHeight());
    
    // Panel border with theme color
    g.setColour(juce::Colour(ScaleChordTheme::Panel));
    g.drawRect(workArea.getX(), workArea.getY(), workArea.getWidth(), workArea.getHeight(), 3);
    
    // Decorative top accent bar (different color for right panel)
    g.setColour(juce::Colour(ScaleChordTheme::HealthGood));
    g.fillRect(workArea.getX(), workArea.getY(), workArea.getWidth(), 3);
    
    // Title section with padding
    auto titleArea = workArea.removeFromTop(40);
    g.setColour(juce::Colour(ScaleChordTheme::TextPrimary));
    g.setFont(juce::Font(15.0f).boldened());
    
    // Centered title
    int titlePadding = 10;
    int titleX = titleArea.getX() + titlePadding;
    int titleY = titleArea.getY() + 5;
    int titleWidth = titleArea.getWidth() - (titlePadding * 2);
    int titleHeight = titleArea.getHeight() - 5;
    g.drawText("METRICS", titleX, titleY, titleWidth, titleHeight, juce::Justification::centred);
    
    // Subtle separator line below title
    g.setColour(juce::Colour(ScaleChordTheme::Panel));
    int separatorY = titleArea.getBottomY() - 1;
    g.drawLine(workArea.getX() + 5, separatorY,
               workArea.getX() + workArea.getWidth() - 5, separatorY, 1.0f);
    
    // ===== Metrics Content with consistent spacing =====
    auto contentArea = workArea.removeFromTop(150);
    g.setFont(juce::Font(12.0f));
    
    const int METRIC_PADDING_LEFT = 15;
    const int METRIC_SPACING = 28;
    
    // Voice count
    g.setColour(juce::Colour(ScaleChordTheme::TextSecondary));
    g.drawText(juce::String("Voices: ") + juce::String(metrics.voiceCount) + "/16", 
               contentArea.getX() + METRIC_PADDING_LEFT, 
               contentArea.getY(), 
               contentArea.getWidth() - METRIC_PADDING_LEFT * 2, 
               METRIC_SPACING, 
               juce::Justification::left);
    
    // CPU usage
    g.drawText(juce::String("CPU: ") + juce::String(metrics.cpuUsage, 1) + "%",
               contentArea.getX() + METRIC_PADDING_LEFT, 
               contentArea.getY() + METRIC_SPACING,
               contentArea.getWidth() - METRIC_PADDING_LEFT * 2, 
               METRIC_SPACING,
               juce::Justification::left);
    
    // Latency
    g.drawText(juce::String("Latency: ") + juce::String(metrics.latencyMs, 2) + " ms",
               contentArea.getX() + METRIC_PADDING_LEFT, 
               contentArea.getY() + (METRIC_SPACING * 2),
               contentArea.getWidth() - METRIC_PADDING_LEFT * 2, 
               METRIC_SPACING,
               juce::Justification::left);
    
    // Health score with color-coded indicator
    auto healthColour = getHealthColour(metrics.healthScore);
    g.setColour(healthColour);
    g.setFont(juce::Font(12.0f).boldened());
    g.drawText(juce::String("Health: ") + juce::String(metrics.healthScore * 100.0f, 0) + "%",
               contentArea.getX() + METRIC_PADDING_LEFT, 
               contentArea.getY() + (METRIC_SPACING * 3),
               contentArea.getWidth() - METRIC_PADDING_LEFT * 2, 
               METRIC_SPACING,
               juce::Justification::left);
    
    // Status section - bottom of metrics panel
    auto statusArea = workArea.removeFromTop(50);
    g.setColour(juce::Colour(ScaleChordTheme::TextPrimary));
    g.setFont(juce::Font(13.0f).boldened());
    
    // Status background highlight
    auto healthColour2 = getHealthColour(metrics.healthScore);
    g.setColour(healthColour2);
    g.fillRect(statusArea.getX() + 5, statusArea.getY() + 5, 
               statusArea.getWidth() - 10, statusArea.getHeight() - 10);
    
    // Status text
    g.setColour(juce::Colour(ScaleChordTheme::Background));
    g.drawText("Status: " + juce::String(metrics.statusText),
               statusArea.getX() + 10, statusArea.getY() + 8,
               statusArea.getWidth() - 20, statusArea.getHeight() - 10,
               juce::Justification::centred);
}

// ============================================================================
// Get Health Colour Based on Score - JUCE MODE
// ============================================================================
juce::Colour PluginEditor::getHealthColour(float healthScore)
{
    if (healthScore > 0.7f) {
        return juce::Colour(ScaleChordTheme::HealthGood);
    } else if (healthScore > 0.4f) {
        return juce::Colour(ScaleChordTheme::HealthWarning);
    } else {
        return juce::Colour(ScaleChordTheme::HealthBad);
    }
}

// ============================================================================
// Update Parameter Display - JUCE MODE
// ============================================================================
void PluginEditor::updateParameterDisplay(int paramId, float value)
{
    if (paramId >= 0 && paramId < NumParams && paramId < static_cast<int>(paramSliders.size())) {
        paramSliders[paramId]->setValue(value, juce::dontSendNotification);
    }
}

#else
// ============================================================================
// Stub implementations for non-JUCE mode
// ============================================================================

void PluginEditor::initializeSliders() {}
void PluginEditor::initializeLabels() {}
void PluginEditor::bindParametersToAPVTS() {}
void PluginEditor::updateSpectrumAnalyzer() {}
void PluginEditor::updateSpectrumWithPeaks() {}
void PluginEditor::layoutPanels(int, int, RectInt&, RectInt&, RectInt&) {}
void PluginEditor::paint(juce::Graphics& g) {}
void PluginEditor::resized() {}
void PluginEditor::timerCallback() {}
void PluginEditor::updateMetricsDisplay() {}
void PluginEditor::drawParameterSection(juce::Graphics& g, const RectInt& area) {}
void PluginEditor::drawSpectrumBands(juce::Graphics& g, const RectInt& area) {}
void PluginEditor::drawSpectrumPeaks(juce::Graphics& g, const RectInt& area) {}
void PluginEditor::drawMetricsPanel(juce::Graphics& g, const RectInt& area) {}
juce::Colour PluginEditor::getSpectrumColour(float normalized) { return juce::Colour(0xFF00ff00); }
juce::Colour PluginEditor::getHealthColour(float healthScore) { return juce::Colour(0xFF00ff00); }
void PluginEditor::updateParameterDisplay(int paramId, float value) {}

#endif // JUCE_VERSION
