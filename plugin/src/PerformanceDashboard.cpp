#include "PerformanceDashboard.h"
#include <algorithm>
#include <cmath>
#include <sstream>
#include <iomanip>
#include <string>

namespace scalechord {

// ========== Constructor & Lifecycle ==========

PerformanceDashboard::PerformanceDashboard() {
    cpuHistory.fill(0.0f);
    voiceHistory.fill(0.0f);
    latencyHistory.fill(0.0f);
    audioLevelHistory.fill(-60.0f);
    spectrumMetrics.bands.fill(-60.0f);
}

PerformanceDashboard::~PerformanceDashboard() {
}

void PerformanceDashboard::initialize(int sampleRate, int blockSize) {
    sampleRate_ = sampleRate;
    blockSize_ = blockSize;
    uptime_ = 0.0f;
    reset();
}

void PerformanceDashboard::reset() {
    cpuHistory.fill(0.0f);
    voiceHistory.fill(0.0f);
    latencyHistory.fill(0.0f);
    audioLevelHistory.fill(-60.0f);
    spectrumMetrics.bands.fill(-60.0f);
    
    historyIndex = 0;
    uptime_ = 0.0f;
    
    voiceMetrics = VoiceMetrics();
    cpuMetrics = CPUMetrics();
    latencyMetrics = LatencyMetrics();
    audioMetrics = AudioMetrics();
    effectMetrics = EffectMetrics();
}

// ========== Real-time Updates ==========

void PerformanceDashboard::updateVoiceMetrics(int activeVoiceCount, const bool voiceStates[MAX_VOICES]) {
    voiceMetrics.activeVoiceCount = std::max(0, std::min(activeVoiceCount, MAX_VOICES));
    voiceMetrics.peakVoiceCount = std::max(voiceMetrics.peakVoiceCount, voiceMetrics.activeVoiceCount);
    
    // Smooth average
    voiceMetrics.averageVoiceCount = voiceMetrics.averageVoiceCount * 0.95f + voiceMetrics.activeVoiceCount * 0.05f;
    
    // Store individual voice states
    if (voiceStates) {
        std::memcpy(voiceMetrics.voiceActivity, voiceStates, sizeof(bool) * MAX_VOICES);
    }
    
    // Add to history
    voiceHistory[historyIndex] = static_cast<float>(activeVoiceCount);
}

void PerformanceDashboard::updateCPUMetrics(float cpuPercentage, float blockTimeMs) {
    cpuPercentage = std::max(0.0f, std::min(cpuPercentage, 100.0f));
    
    cpuMetrics.currentCPU = cpuPercentage;
    cpuMetrics.peakCPU = std::max(cpuMetrics.peakCPU, cpuPercentage);
    cpuMetrics.minCPU = std::min(cpuMetrics.minCPU, cpuPercentage);
    
    // Smooth average
    cpuMetrics.averageCPU = cpuMetrics.averageCPU * 0.95f + cpuPercentage * 0.05f;
    
    cpuMetrics.processBlockCount++;
    cpuMetrics.totalProcessTime += blockTimeMs / 1000.0f;  // Convert to seconds
    
    // Update uptime
    uptime_ += blockTimeMs / 1000.0f;
    
    // Add to history
    cpuHistory[historyIndex] = cpuPercentage;
}

void PerformanceDashboard::updateLatencyMetrics(float midiLatencyMs, float blockLatencyMs) {
    midiLatencyMs = std::max(0.0f, midiLatencyMs);
    blockLatencyMs = std::max(0.0f, blockLatencyMs);
    
    latencyMetrics.midiLatencyMs = midiLatencyMs;
    latencyMetrics.blockLatencyMs = blockLatencyMs;
    latencyMetrics.peakLatencyMs = std::max(latencyMetrics.peakLatencyMs, midiLatencyMs);
    
    // Smooth average
    latencyMetrics.averageLatencyMs = latencyMetrics.averageLatencyMs * 0.95f + midiLatencyMs * 0.05f;
    
    // Calculate jitter (simplified)
    static float lastLatency = 0.0f;
    latencyMetrics.jitterMs = std::abs(midiLatencyMs - lastLatency);
    lastLatency = midiLatencyMs;
    
    // Add to history
    latencyHistory[historyIndex] = midiLatencyMs;
}

void PerformanceDashboard::updateAudioMetrics(const float* buffer, int numSamples, bool updateSpec) {
    if (!buffer || numSamples <= 0) {
        return;
    }
    
    // Find peak level
    float peakLevel = 0.0f;
    bool isClipping = false;
    
    for (int i = 0; i < numSamples; ++i) {
        float absLevel = std::abs(buffer[i]);
        peakLevel = std::max(peakLevel, absLevel);
        
        if (absLevel > 1.0f) {
            isClipping = true;
        }
    }
    
    audioMetrics.peakLevel = levelToDb(peakLevel);
    audioMetrics.isClipping = isClipping;
    
    // Calculate RMS level
    audioMetrics.rmsLevel = calculateRMSLevel(buffer, numSamples);
    
    // Update peak hold
    if (audioMetrics.peakLevel > audioMetrics.peakHold) {
        audioMetrics.peakHold = audioMetrics.peakLevel;
        audioMetrics.peakHoldFrames = peakHoldFrames_;
    } else {
        audioMetrics.peakHoldFrames--;
        if (audioMetrics.peakHoldFrames <= 0) {
            audioMetrics.peakHold = audioMetrics.peakLevel;
        }
    }
    
    // Update spectrum if requested
    if (updateSpec) {
        updateSpectrum(buffer, numSamples);
    }
    
    // Add to history
    audioLevelHistory[historyIndex] = audioMetrics.rmsLevel;
}

void PerformanceDashboard::updateEffectMetrics(
    float reverbCPU, float delayCPU, float chorusCPU,
    float distortionCPU, float eqCPU, float compressionCPU) {
    
    effectMetrics.reverbCPU = std::max(0.0f, std::min(reverbCPU, 100.0f));
    effectMetrics.delayCPU = std::max(0.0f, std::min(delayCPU, 100.0f));
    effectMetrics.chorusCPU = std::max(0.0f, std::min(chorusCPU, 100.0f));
    effectMetrics.distortionCPU = std::max(0.0f, std::min(distortionCPU, 100.0f));
    effectMetrics.eqCPU = std::max(0.0f, std::min(eqCPU, 100.0f));
    effectMetrics.compressionCPU = std::max(0.0f, std::min(compressionCPU, 100.0f));
}

// ========== Query Methods ==========

PerformanceDashboard::DashboardSnapshot PerformanceDashboard::getSnapshot() const {
    DashboardSnapshot snapshot;
    snapshot.voices = voiceMetrics;
    snapshot.cpu = cpuMetrics;
    snapshot.latency = latencyMetrics;
    snapshot.audio = audioMetrics;
    snapshot.spectrum = spectrumMetrics;
    snapshot.effects = effectMetrics;
    snapshot.uptime = uptime_;
    snapshot.sampleRate = sampleRate_;
    snapshot.blockSize = blockSize_;
    return snapshot;
}

PerformanceDashboard::VoiceMetrics PerformanceDashboard::getVoiceMetrics() const {
    return voiceMetrics;
}

PerformanceDashboard::CPUMetrics PerformanceDashboard::getCPUMetrics() const {
    return cpuMetrics;
}

PerformanceDashboard::LatencyMetrics PerformanceDashboard::getLatencyMetrics() const {
    return latencyMetrics;
}

PerformanceDashboard::AudioMetrics PerformanceDashboard::getAudioMetrics() const {
    return audioMetrics;
}

PerformanceDashboard::SpectrumMetrics PerformanceDashboard::getSpectrumMetrics() const {
    return spectrumMetrics;
}

PerformanceDashboard::EffectMetrics PerformanceDashboard::getEffectMetrics() const {
    return effectMetrics;
}

// ========== Historical Data Access ==========

int PerformanceDashboard::getCPUHistory(float* outBuffer) const {
    if (!outBuffer) return 0;
    
    for (int i = 0; i < HISTORY_SIZE; ++i) {
        outBuffer[i] = cpuHistory[(historyIndex + i) % HISTORY_SIZE];
    }
    return HISTORY_SIZE;
}

int PerformanceDashboard::getVoiceHistory(float* outBuffer) const {
    if (!outBuffer) return 0;
    
    for (int i = 0; i < HISTORY_SIZE; ++i) {
        outBuffer[i] = voiceHistory[(historyIndex + i) % HISTORY_SIZE];
    }
    return HISTORY_SIZE;
}

int PerformanceDashboard::getLatencyHistory(float* outBuffer) const {
    if (!outBuffer) return 0;
    
    for (int i = 0; i < HISTORY_SIZE; ++i) {
        outBuffer[i] = latencyHistory[(historyIndex + i) % HISTORY_SIZE];
    }
    return HISTORY_SIZE;
}

int PerformanceDashboard::getAudioLevelHistory(float* outBuffer) const {
    if (!outBuffer) return 0;
    
    for (int i = 0; i < HISTORY_SIZE; ++i) {
        outBuffer[i] = audioLevelHistory[(historyIndex + i) % HISTORY_SIZE];
    }
    return HISTORY_SIZE;
}

// ========== Analysis & Statistics ==========

std::array<float, 5> PerformanceDashboard::getCPUStatistics() const {
    float minVal = 100.0f, maxVal = 0.0f, sumVal = 0.0f;
    
    for (float val : cpuHistory) {
        minVal = std::min(minVal, val);
        maxVal = std::max(maxVal, val);
        sumVal += val;
    }
    
    float avgVal = sumVal / HISTORY_SIZE;
    
    // Calculate standard deviation
    float sumSquaredDiff = 0.0f;
    for (float val : cpuHistory) {
        float diff = val - avgVal;
        sumSquaredDiff += diff * diff;
    }
    float stdDev = std::sqrt(sumSquaredDiff / HISTORY_SIZE);
    
    return {minVal, cpuMetrics.currentCPU, avgVal, maxVal, stdDev};
}

std::array<float, 5> PerformanceDashboard::getLatencyStatistics() const {
    float minVal = 1000.0f, maxVal = 0.0f, sumVal = 0.0f;
    
    for (float val : latencyHistory) {
        minVal = std::min(minVal, val);
        maxVal = std::max(maxVal, val);
        sumVal += val;
    }
    
    float avgVal = sumVal / HISTORY_SIZE;
    float jitterVal = maxVal - minVal;
    
    return {minVal, latencyMetrics.midiLatencyMs, avgVal, maxVal, jitterVal};
}

std::array<float, 4> PerformanceDashboard::getVoiceStatistics() const {
    float minVal = 16.0f, maxVal = 0.0f, sumVal = 0.0f;
    
    for (float val : voiceHistory) {
        minVal = std::min(minVal, val);
        maxVal = std::max(maxVal, val);
        sumVal += val;
    }
    
    float avgVal = sumVal / HISTORY_SIZE;
    
    return {minVal, static_cast<float>(voiceMetrics.activeVoiceCount), avgVal, maxVal};
}

float PerformanceDashboard::getHealthScore() const {
    // Health score based on CPU and latency
    // 1.0 = excellent, 0.0 = poor
    
    float cpuScore = 1.0f - std::min(1.0f, cpuMetrics.currentCPU / 50.0f);  // 50% CPU = 0.5 score
    float latencyScore = 1.0f - std::min(1.0f, latencyMetrics.midiLatencyMs / 10.0f);  // 10ms = 0.5 score
    
    return (cpuScore + latencyScore) / 2.0f;
}

// ========== Information Methods ==========

int PerformanceDashboard::getSampleRate() const {
    return sampleRate_;
}

int PerformanceDashboard::getBlockSize() const {
    return blockSize_;
}

float PerformanceDashboard::getUptime() const {
    return uptime_;
}

std::string PerformanceDashboard::getStatusString() const {
    std::ostringstream oss;
    oss << "Voices: " << voiceMetrics.activeVoiceCount << "/" << MAX_VOICES << " | "
        << "CPU: " << std::fixed << std::setprecision(1) << cpuMetrics.currentCPU << "% | "
        << "Latency: " << latencyMetrics.midiLatencyMs << " ms";
    
    if (audioMetrics.isClipping) {
        oss << " | ⚠ CLIPPING";
    }
    
    return oss.str();
}

std::string PerformanceDashboard::getFrequencyLabel(int bandIndex) const {
    if (bandIndex < 0 || bandIndex >= SPECTRUM_BANDS) {
        return "";
    }
    
    // Map bands to frequency ranges (20Hz - 20kHz)
    // Logarithmic spacing
    const float minFreq = 20.0f;
    const float maxFreq = 20000.0f;
    
    float freq = minFreq * std::pow(maxFreq / minFreq, static_cast<float>(bandIndex) / (SPECTRUM_BANDS - 1));
    
    std::ostringstream oss;
    if (freq < 1000.0f) {
        oss << std::fixed << std::setprecision(0) << freq << "Hz";
    } else {
        oss << std::fixed << std::setprecision(1) << (freq / 1000.0f) << "kHz";
    }
    
    return oss.str();
}

// ========== Peak Hold Control ==========

void PerformanceDashboard::setPeakHold(float holdTimeMs) {
    peakHoldTime_ = holdTimeMs;
    peakHoldFrames_ = static_cast<int>(holdTimeMs * sampleRate_ / (1000.0f * blockSize_));
}

void PerformanceDashboard::resetPeakHold() {
    audioMetrics.peakHold = audioMetrics.peakLevel;
    audioMetrics.peakHoldFrames = 0;
}

// ========== Display Formatting ==========

std::string PerformanceDashboard::formatMetric(float value, const std::string& metric) {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(1);
    
    if (metric == "cpu") {
        oss << value << "%";
    } else if (metric == "latency") {
        oss << value << " ms";
    } else if (metric == "level") {
        oss << value << " dB";
    } else if (metric == "frequency") {
        if (value < 1000.0f) {
            oss << value << " Hz";
        } else {
            oss << (value / 1000.0f) << " kHz";
        }
    } else if (metric == "time") {
        oss << value << " s";
    } else {
        oss << value;
    }
    
    return oss.str();
}

std::string PerformanceDashboard::formatDB(float db) {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(1) << db << " dB";
    return oss.str();
}

std::string PerformanceDashboard::formatPercent(float percent) {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(1) << percent << "%";
    return oss.str();
}

std::string PerformanceDashboard::formatTime(float ms) {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2) << ms << " ms";
    return oss.str();
}

// ========== Private Helper Methods ==========

float PerformanceDashboard::calculateRMSLevel(const float* buffer, int numSamples) const {
    if (!buffer || numSamples <= 0) {
        return MIN_LEVEL;
    }
    
    double sumSquared = 0.0;
    for (int i = 0; i < numSamples; ++i) {
        sumSquared += buffer[i] * buffer[i];
    }
    
    float rms = static_cast<float>(std::sqrt(sumSquared / numSamples));
    return levelToDb(rms);
}

void PerformanceDashboard::updateSpectrum(const float* buffer, int numSamples) {
    if (!buffer || numSamples <= 0) {
        return;
    }
    
    // Simple energy-based spectrum analysis (no FFT)
    // Divide frequency range into SPECTRUM_BANDS buckets
    
    const int samplesPerBand = numSamples / SPECTRUM_BANDS;
    if (samplesPerBand <= 0) {
        return;
    }
    
    for (int band = 0; band < SPECTRUM_BANDS; ++band) {
        float energy = 0.0f;
        
        for (int i = 0; i < samplesPerBand; ++i) {
            int sampleIdx = band * samplesPerBand + i;
            if (sampleIdx < numSamples) {
                energy += buffer[sampleIdx] * buffer[sampleIdx];
            }
        }
        
        energy = std::sqrt(energy / samplesPerBand);
        float levelDb = levelToDb(energy);
        
        // Smooth spectrum update
        spectrumMetrics.bands[band] = spectrumMetrics.bands[band] * 0.7f + levelDb * 0.3f;
    }
    
    // Calculate spectral centroid (simplified)
    float weightedSum = 0.0f;
    float totalEnergy = 0.0f;
    
    for (int band = 0; band < SPECTRUM_BANDS; ++band) {
        float freq = 20.0f * std::pow(20000.0f / 20.0f, static_cast<float>(band) / (SPECTRUM_BANDS - 1));
        float energy = std::pow(10.0f, (spectrumMetrics.bands[band] + 60.0f) / 20.0f);
        
        weightedSum += freq * energy;
        totalEnergy += energy;
    }
    
    if (totalEnergy > 0.0f) {
        spectrumMetrics.centroid = weightedSum / totalEnergy;
    }
    
    // Find peak band
    int peakBand = 0;
    float peakEnergy = spectrumMetrics.bands[0];
    for (int i = 1; i < SPECTRUM_BANDS; ++i) {
        if (spectrumMetrics.bands[i] > peakEnergy) {
            peakEnergy = spectrumMetrics.bands[i];
            peakBand = i;
        }
    }
    spectrumMetrics.peakBand = peakBand;
}

void PerformanceDashboard::updateStatistics() {
    // This would be called periodically to update rolling statistics
    // Currently handled inline in update methods
}

float PerformanceDashboard::levelToDb(float level) {
    if (level <= 0.0001f) {
        return MIN_LEVEL;
    }
    return 20.0f * std::log10(level);
}

}  // namespace scalechord
