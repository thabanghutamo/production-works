#include <iostream>
#include <cmath>
#include <vector>
#include "../include/PerformanceDashboard.h"

using namespace scalechord;

void printTestResult(const std::string& testName, bool passed) {
    std::cout << (passed ? "✓ " : "✗ ") << testName << std::endl;
}

// ========== Basic Functionality Tests ==========

bool test_dashboard_creation() {
    PerformanceDashboard dashboard;
    return true;
}

bool test_dashboard_initialize() {
    PerformanceDashboard dashboard;
    dashboard.initialize(44100, 256);
    
    return dashboard.getSampleRate() == 44100 && dashboard.getBlockSize() == 256;
}

bool test_dashboard_reset() {
    PerformanceDashboard dashboard;
    dashboard.initialize(44100, 256);
    dashboard.updateCPUMetrics(50.0f, 1.0f);
    dashboard.reset();
    
    auto cpu = dashboard.getCPUMetrics();
    return cpu.currentCPU == 0.0f && cpu.processBlockCount == 0;
}

// ========== Voice Metrics Tests ==========

bool test_voice_metrics_update() {
    PerformanceDashboard dashboard;
    dashboard.initialize(44100, 256);
    
    bool voiceStates[16] = {true, true, true, false, false, false, false, false,
                            false, false, false, false, false, false, false, false};
    
    dashboard.updateVoiceMetrics(3, voiceStates);
    
    auto voices = dashboard.getVoiceMetrics();
    return voices.activeVoiceCount == 3;
}

bool test_voice_metrics_peak() {
    PerformanceDashboard dashboard;
    dashboard.initialize(44100, 256);
    
    bool voiceStates[16] = {};
    
    dashboard.updateVoiceMetrics(5, voiceStates);
    dashboard.updateVoiceMetrics(3, voiceStates);
    dashboard.updateVoiceMetrics(8, voiceStates);
    
    auto voices = dashboard.getVoiceMetrics();
    return voices.peakVoiceCount == 8;
}

bool test_voice_metrics_average() {
    PerformanceDashboard dashboard;
    dashboard.initialize(44100, 256);
    
    bool voiceStates[16] = {};
    
    dashboard.updateVoiceMetrics(10, voiceStates);
    
    auto voices = dashboard.getVoiceMetrics();
    return voices.averageVoiceCount > 0.0f && voices.averageVoiceCount <= 10.0f;
}

// ========== CPU Metrics Tests ==========

bool test_cpu_metrics_update() {
    PerformanceDashboard dashboard;
    dashboard.initialize(44100, 256);
    
    dashboard.updateCPUMetrics(25.0f, 1.0f);
    
    auto cpu = dashboard.getCPUMetrics();
    return std::abs(cpu.currentCPU - 25.0f) < 0.1f;
}

bool test_cpu_metrics_peak() {
    PerformanceDashboard dashboard;
    dashboard.initialize(44100, 256);
    
    dashboard.updateCPUMetrics(10.0f, 1.0f);
    dashboard.updateCPUMetrics(30.0f, 1.0f);
    dashboard.updateCPUMetrics(20.0f, 1.0f);
    
    auto cpu = dashboard.getCPUMetrics();
    return cpu.peakCPU == 30.0f;
}

bool test_cpu_metrics_average() {
    PerformanceDashboard dashboard;
    dashboard.initialize(44100, 256);
    
    dashboard.updateCPUMetrics(20.0f, 1.0f);
    dashboard.updateCPUMetrics(20.0f, 1.0f);
    
    auto cpu = dashboard.getCPUMetrics();
    return cpu.averageCPU > 0.0f;
}

bool test_cpu_metrics_bounds() {
    PerformanceDashboard dashboard;
    dashboard.initialize(44100, 256);
    
    dashboard.updateCPUMetrics(150.0f, 1.0f);  // Should be clamped
    dashboard.updateCPUMetrics(-10.0f, 1.0f);  // Should be clamped
    
    auto cpu = dashboard.getCPUMetrics();
    return cpu.currentCPU >= 0.0f && cpu.currentCPU <= 100.0f;
}

// ========== Latency Metrics Tests ==========

bool test_latency_metrics_update() {
    PerformanceDashboard dashboard;
    dashboard.initialize(44100, 256);
    
    dashboard.updateLatencyMetrics(2.5f, 1.0f);
    
    auto latency = dashboard.getLatencyMetrics();
    return std::abs(latency.midiLatencyMs - 2.5f) < 0.1f;
}

bool test_latency_metrics_peak() {
    PerformanceDashboard dashboard;
    dashboard.initialize(44100, 256);
    
    dashboard.updateLatencyMetrics(1.0f, 1.0f);
    dashboard.updateLatencyMetrics(3.0f, 1.0f);
    dashboard.updateLatencyMetrics(2.0f, 1.0f);
    
    auto latency = dashboard.getLatencyMetrics();
    return latency.peakLatencyMs == 3.0f;
}

// ========== Audio Metrics Tests ==========

bool test_audio_metrics_update() {
    PerformanceDashboard dashboard;
    dashboard.initialize(44100, 256);
    
    std::vector<float> buffer(256, 0.5f);
    dashboard.updateAudioMetrics(buffer.data(), 256, false);
    
    auto audio = dashboard.getAudioMetrics();
    return audio.peakLevel > -60.0f && audio.rmsLevel > -60.0f;
}

bool test_audio_metrics_clipping() {
    PerformanceDashboard dashboard;
    dashboard.initialize(44100, 256);
    
    std::vector<float> buffer(256, 1.5f);  // Values > 1.0
    dashboard.updateAudioMetrics(buffer.data(), 256, false);
    
    auto audio = dashboard.getAudioMetrics();
    return audio.isClipping;
}

bool test_audio_metrics_no_clipping() {
    PerformanceDashboard dashboard;
    dashboard.initialize(44100, 256);
    
    std::vector<float> buffer(256, 0.5f);  // Values < 1.0
    dashboard.updateAudioMetrics(buffer.data(), 256, false);
    
    auto audio = dashboard.getAudioMetrics();
    return !audio.isClipping;
}

// ========== Spectrum Analysis Tests ==========

bool test_spectrum_analysis() {
    PerformanceDashboard dashboard;
    dashboard.initialize(44100, 256);
    
    std::vector<float> buffer(256, 0.1f);
    dashboard.updateAudioMetrics(buffer.data(), 256, true);
    
    auto spectrum = dashboard.getSpectrumMetrics();
    bool hasSpectrum = false;
    for (float band : spectrum.bands) {
        if (band > -60.0f) {
            hasSpectrum = true;
            break;
        }
    }
    
    return hasSpectrum;
}

// ========== Effect Metrics Tests ==========

bool test_effect_metrics_update() {
    PerformanceDashboard dashboard;
    dashboard.initialize(44100, 256);
    
    dashboard.updateEffectMetrics(1.0f, 0.5f, 0.3f, 0.2f, 0.4f, 0.6f);
    
    auto effects = dashboard.getEffectMetrics();
    return std::abs(effects.reverbCPU - 1.0f) < 0.1f &&
           std::abs(effects.delayCPU - 0.5f) < 0.1f;
}

// ========== Snapshot Tests ==========

bool test_snapshot_complete() {
    PerformanceDashboard dashboard;
    dashboard.initialize(44100, 256);
    
    bool voiceStates[16] = {true, false, false, false, false, false, false, false,
                            false, false, false, false, false, false, false, false};
    
    dashboard.updateVoiceMetrics(1, voiceStates);
    dashboard.updateCPUMetrics(15.0f, 1.0f);
    dashboard.updateLatencyMetrics(1.5f, 0.5f);
    
    std::vector<float> buffer(256, 0.3f);
    dashboard.updateAudioMetrics(buffer.data(), 256, true);
    
    dashboard.updateEffectMetrics(0.5f, 0.3f, 0.2f, 0.1f, 0.2f, 0.3f);
    
    auto snapshot = dashboard.getSnapshot();
    return snapshot.voices.activeVoiceCount == 1 &&
           snapshot.cpu.currentCPU == 15.0f &&
           snapshot.sampleRate == 44100;
}

// ========== Historical Data Tests ==========

bool test_cpu_history() {
    PerformanceDashboard dashboard;
    dashboard.initialize(44100, 256);
    
    dashboard.updateCPUMetrics(10.0f, 1.0f);
    dashboard.updateCPUMetrics(20.0f, 1.0f);
    dashboard.updateCPUMetrics(30.0f, 1.0f);
    
    std::vector<float> history(256);
    int count = dashboard.getCPUHistory(history.data());
    
    // Just check that history was retrieved
    return count == 256;
}

bool test_voice_history() {
    PerformanceDashboard dashboard;
    dashboard.initialize(44100, 256);
    
    bool voiceStates[16] = {};
    dashboard.updateVoiceMetrics(5, voiceStates);
    
    std::vector<float> history(256);
    int count = dashboard.getVoiceHistory(history.data());
    
    return count == 256 && history[0] == 5.0f;
}

// ========== Statistics Tests ==========

bool test_cpu_statistics() {
    PerformanceDashboard dashboard;
    dashboard.initialize(44100, 256);
    
    for (int i = 0; i < 10; ++i) {
        dashboard.updateCPUMetrics(50.0f, 1.0f);
    }
    
    auto stats = dashboard.getCPUStatistics();
    // stats = [min, current, average, peak, stddev]
    return stats[0] >= 0.0f && stats[2] > 0.0f && stats[3] >= stats[0];
}

bool test_latency_statistics() {
    PerformanceDashboard dashboard;
    dashboard.initialize(44100, 256);
    
    for (int i = 0; i < 10; ++i) {
        dashboard.updateLatencyMetrics(2.0f, 0.5f);
    }
    
    auto stats = dashboard.getLatencyStatistics();
    return stats[0] >= 0.0f && stats[2] > 0.0f;
}

bool test_voice_statistics() {
    PerformanceDashboard dashboard;
    dashboard.initialize(44100, 256);
    
    bool voiceStates[16] = {};
    for (int i = 0; i < 10; ++i) {
        dashboard.updateVoiceMetrics(8, voiceStates);
    }
    
    auto stats = dashboard.getVoiceStatistics();
    return stats[3] == 8.0f;  // Peak should be 8
}

// ========== Information Methods Tests ==========

bool test_get_sample_rate() {
    PerformanceDashboard dashboard;
    dashboard.initialize(48000, 512);
    
    return dashboard.getSampleRate() == 48000;
}

bool test_get_block_size() {
    PerformanceDashboard dashboard;
    dashboard.initialize(44100, 512);
    
    return dashboard.getBlockSize() == 512;
}

bool test_get_uptime() {
    PerformanceDashboard dashboard;
    dashboard.initialize(44100, 256);
    
    dashboard.updateCPUMetrics(10.0f, 0.5f);
    dashboard.updateCPUMetrics(10.0f, 0.5f);
    
    float uptime = dashboard.getUptime();
    return uptime > 0.0f;
}

// ========== Health Score Tests ==========

bool test_health_score() {
    PerformanceDashboard dashboard;
    dashboard.initialize(44100, 256);
    
    dashboard.updateCPUMetrics(10.0f, 1.0f);  // Low CPU = good
    dashboard.updateLatencyMetrics(1.0f, 0.5f);  // Low latency = good
    
    float health = dashboard.getHealthScore();
    return health > 0.5f && health <= 1.0f;
}

// ========== Display Formatting Tests ==========

bool test_format_db() {
    std::string formatted = PerformanceDashboard::formatDB(-12.5f);
    return !formatted.empty() && formatted.find("dB") != std::string::npos;
}

bool test_format_percent() {
    std::string formatted = PerformanceDashboard::formatPercent(45.5f);
    return !formatted.empty() && formatted.find("%") != std::string::npos;
}

bool test_format_time() {
    std::string formatted = PerformanceDashboard::formatTime(1.23f);
    return !formatted.empty() && formatted.find("ms") != std::string::npos;
}

// ========== Frequency Label Tests ==========

bool test_frequency_label() {
    PerformanceDashboard dashboard;
    dashboard.initialize(44100, 256);
    
    std::string label0 = dashboard.getFrequencyLabel(0);
    std::string label31 = dashboard.getFrequencyLabel(31);
    
    return !label0.empty() && !label31.empty() && label0 != label31;
}

// ========== Peak Hold Tests ==========

bool test_peak_hold() {
    PerformanceDashboard dashboard;
    dashboard.initialize(44100, 256);
    
    dashboard.setPeakHold(100.0f);
    
    std::vector<float> buffer(256, 0.5f);
    dashboard.updateAudioMetrics(buffer.data(), 256, false);
    
    auto audio = dashboard.getAudioMetrics();
    return audio.peakHoldFrames > 0;
}

bool test_reset_peak_hold() {
    PerformanceDashboard dashboard;
    dashboard.initialize(44100, 256);
    
    dashboard.setPeakHold(100.0f);
    dashboard.resetPeakHold();
    
    return true;
}

// ========== Status String Test ==========

bool test_status_string() {
    PerformanceDashboard dashboard;
    dashboard.initialize(44100, 256);
    
    bool voiceStates[16] = {true, false, false, false, false, false, false, false,
                            false, false, false, false, false, false, false, false};
    
    dashboard.updateVoiceMetrics(1, voiceStates);
    dashboard.updateCPUMetrics(25.0f, 1.0f);
    dashboard.updateLatencyMetrics(2.0f, 1.0f);
    
    std::string status = dashboard.getStatusString();
    return !status.empty() && status.find("Voices") != std::string::npos;
}

// ========== Main Test Suite ==========

int main() {
    int passed = 0;
    int total = 0;
    
    std::cout << "\n========== PERFORMANCEDASHBOARD TEST SUITE ==========\n" << std::endl;
    
    // Basic functionality
    std::cout << "Basic Functionality:" << std::endl;
    total++; passed += test_dashboard_creation() ? 1 : 0;
    printTestResult("Dashboard creation", test_dashboard_creation());
    
    total++; passed += test_dashboard_initialize() ? 1 : 0;
    printTestResult("Dashboard initialize", test_dashboard_initialize());
    
    total++; passed += test_dashboard_reset() ? 1 : 0;
    printTestResult("Dashboard reset", test_dashboard_reset());
    
    // Voice metrics
    std::cout << "\nVoice Metrics:" << std::endl;
    total++; passed += test_voice_metrics_update() ? 1 : 0;
    printTestResult("Voice metrics update", test_voice_metrics_update());
    
    total++; passed += test_voice_metrics_peak() ? 1 : 0;
    printTestResult("Voice metrics peak", test_voice_metrics_peak());
    
    total++; passed += test_voice_metrics_average() ? 1 : 0;
    printTestResult("Voice metrics average", test_voice_metrics_average());
    
    // CPU metrics
    std::cout << "\nCPU Metrics:" << std::endl;
    total++; passed += test_cpu_metrics_update() ? 1 : 0;
    printTestResult("CPU metrics update", test_cpu_metrics_update());
    
    total++; passed += test_cpu_metrics_peak() ? 1 : 0;
    printTestResult("CPU metrics peak", test_cpu_metrics_peak());
    
    total++; passed += test_cpu_metrics_average() ? 1 : 0;
    printTestResult("CPU metrics average", test_cpu_metrics_average());
    
    total++; passed += test_cpu_metrics_bounds() ? 1 : 0;
    printTestResult("CPU metrics bounds", test_cpu_metrics_bounds());
    
    // Latency metrics
    std::cout << "\nLatency Metrics:" << std::endl;
    total++; passed += test_latency_metrics_update() ? 1 : 0;
    printTestResult("Latency metrics update", test_latency_metrics_update());
    
    total++; passed += test_latency_metrics_peak() ? 1 : 0;
    printTestResult("Latency metrics peak", test_latency_metrics_peak());
    
    // Audio metrics
    std::cout << "\nAudio Metrics:" << std::endl;
    total++; passed += test_audio_metrics_update() ? 1 : 0;
    printTestResult("Audio metrics update", test_audio_metrics_update());
    
    total++; passed += test_audio_metrics_clipping() ? 1 : 0;
    printTestResult("Audio metrics clipping", test_audio_metrics_clipping());
    
    total++; passed += test_audio_metrics_no_clipping() ? 1 : 0;
    printTestResult("Audio metrics no clipping", test_audio_metrics_no_clipping());
    
    // Spectrum
    std::cout << "\nSpectrum Analysis:" << std::endl;
    total++; passed += test_spectrum_analysis() ? 1 : 0;
    printTestResult("Spectrum analysis", test_spectrum_analysis());
    
    // Effect metrics
    std::cout << "\nEffect Metrics:" << std::endl;
    total++; passed += test_effect_metrics_update() ? 1 : 0;
    printTestResult("Effect metrics update", test_effect_metrics_update());
    
    // Snapshot
    std::cout << "\nSnapshot:" << std::endl;
    total++; passed += test_snapshot_complete() ? 1 : 0;
    printTestResult("Snapshot complete", test_snapshot_complete());
    
    // Historical data
    std::cout << "\nHistorical Data:" << std::endl;
    total++; passed += test_cpu_history() ? 1 : 0;
    printTestResult("CPU history", test_cpu_history());
    
    total++; passed += test_voice_history() ? 1 : 0;
    printTestResult("Voice history", test_voice_history());
    
    // Statistics
    std::cout << "\nStatistics:" << std::endl;
    total++; passed += test_cpu_statistics() ? 1 : 0;
    printTestResult("CPU statistics", test_cpu_statistics());
    
    total++; passed += test_latency_statistics() ? 1 : 0;
    printTestResult("Latency statistics", test_latency_statistics());
    
    total++; passed += test_voice_statistics() ? 1 : 0;
    printTestResult("Voice statistics", test_voice_statistics());
    
    // Information
    std::cout << "\nInformation Methods:" << std::endl;
    total++; passed += test_get_sample_rate() ? 1 : 0;
    printTestResult("Get sample rate", test_get_sample_rate());
    
    total++; passed += test_get_block_size() ? 1 : 0;
    printTestResult("Get block size", test_get_block_size());
    
    total++; passed += test_get_uptime() ? 1 : 0;
    printTestResult("Get uptime", test_get_uptime());
    
    // Health score
    std::cout << "\nHealth Score:" << std::endl;
    total++; passed += test_health_score() ? 1 : 0;
    printTestResult("Health score", test_health_score());
    
    // Display formatting
    std::cout << "\nDisplay Formatting:" << std::endl;
    total++; passed += test_format_db() ? 1 : 0;
    printTestResult("Format dB", test_format_db());
    
    total++; passed += test_format_percent() ? 1 : 0;
    printTestResult("Format percent", test_format_percent());
    
    total++; passed += test_format_time() ? 1 : 0;
    printTestResult("Format time", test_format_time());
    
    // Frequency labels
    std::cout << "\nFrequency Labels:" << std::endl;
    total++; passed += test_frequency_label() ? 1 : 0;
    printTestResult("Frequency label", test_frequency_label());
    
    // Peak hold
    std::cout << "\nPeak Hold:" << std::endl;
    total++; passed += test_peak_hold() ? 1 : 0;
    printTestResult("Peak hold", test_peak_hold());
    
    total++; passed += test_reset_peak_hold() ? 1 : 0;
    printTestResult("Reset peak hold", test_reset_peak_hold());
    
    // Status string
    std::cout << "\nStatus String:" << std::endl;
    total++; passed += test_status_string() ? 1 : 0;
    printTestResult("Status string", test_status_string());
    
    // Summary
    std::cout << "\n========== TEST SUMMARY ==========\n" << std::endl;
    std::cout << "Passed: " << passed << "/" << total << std::endl;
    
    if (passed == total) {
        std::cout << "✓ ALL TESTS PASSED!" << std::endl;
        return 0;
    } else {
        std::cout << "✗ SOME TESTS FAILED" << std::endl;
        return 1;
    }
}
