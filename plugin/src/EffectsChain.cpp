#include "EffectsChain.h"
#include <cstring>
#include <iostream>

namespace scalechord {

// ========== Constructor & Initialization ==========

EffectsChain::EffectsChain(float sampleRate)
    : sampleRate_(sampleRate), blockSize_(256), numChannels_(2) {
    // Initialize all effect parameters to defaults
    for (int i = 0; i < static_cast<int>(EffectType::Count); ++i) {
        effectParams[i] = EffectParameters();
    }
    
    // Initialize reverb buffers (using Schroeder reverberator sizes)
    const std::array<int, 4> combSizes = {1116, 1188, 1277, 1356};
    const std::array<int, 2> allpassSizes = {556, 441};
    
    reverbState.combBuffers[0].resize(combSizes[0], 0.0f);
    reverbState.combBuffers[1].resize(combSizes[1], 0.0f);
    reverbState.combBuffers[2].resize(combSizes[2], 0.0f);
    reverbState.combBuffers[3].resize(combSizes[3], 0.0f);
    reverbState.allpassBuffers[0].resize(allpassSizes[0], 0.0f);
    reverbState.allpassBuffers[1].resize(allpassSizes[1], 0.0f);
    
    std::fill(reverbState.combIndices.begin(), reverbState.combIndices.end(), 0);
    std::fill(reverbState.allpassIndices.begin(), reverbState.allpassIndices.end(), 0);
    std::fill(reverbState.combFilter1.begin(), reverbState.combFilter1.end(), 0.0f);
    std::fill(reverbState.combFilter2.begin(), reverbState.combFilter2.end(), 0.0f);
    std::fill(reverbState.allpassFilter1.begin(), reverbState.allpassFilter1.end(), 0.0f);
    std::fill(reverbState.allpassFilter2.begin(), reverbState.allpassFilter2.end(), 0.0f);
}

EffectsChain::~EffectsChain() {
    // Buffers automatically cleaned up by std::vector
}

void EffectsChain::prepareToPlay(float sampleRate, int blockSize, int numChannels) {
    sampleRate_ = sampleRate;
    blockSize_ = blockSize;
    numChannels_ = numChannels;
    
    // Allocate processing buffers
    dryBuffer.resize(blockSize * numChannels, 0.0f);
    wetBuffer.resize(blockSize * numChannels, 0.0f);
    tempBuffer.resize(blockSize * numChannels, 0.0f);
    
    // Initialize delay buffer (2 seconds max at current sample rate)
    delayState.maxDelayTime = static_cast<int>(2.0f * sampleRate);
    delayState.buffer.resize(delayState.maxDelayTime, 0.0f);
    delayState.writeIndex = 0;
    
    // Initialize chorus buffer (10ms max at current sample rate)
    chorusState.maxDelayTime = static_cast<int>(0.01f * sampleRate);
    chorusState.buffer.resize(chorusState.maxDelayTime, 0.0f);
    chorusState.writeIndex = 0;
    
    // Update EQ coefficients for new sample rate
    updateEQCoefficients();
}

// ========== Audio Processing ==========

void EffectsChain::processBlock(const float* inputBuffer, float* outputBuffer, int numSamples) {
    // Copy input to dry buffer
    std::memcpy(dryBuffer.data(), inputBuffer, numSamples * numChannels_ * sizeof(float));
    
    // Copy input to wet buffer for processing
    std::memcpy(wetBuffer.data(), inputBuffer, numSamples * numChannels_ * sizeof(float));
    
    // Process through active effects in series
    if (!isEffectBypassed(EffectType::Distortion)) {
        processDistortion(wetBuffer.data(), numSamples);
    }
    
    if (!isEffectBypassed(EffectType::Compression)) {
        processCompression(wetBuffer.data(), numSamples);
    }
    
    if (!isEffectBypassed(EffectType::EQ)) {
        processEQ(wetBuffer.data(), numSamples);
    }
    
    if (!isEffectBypassed(EffectType::Delay)) {
        processDelay(wetBuffer.data(), numSamples);
    }
    
    if (!isEffectBypassed(EffectType::Chorus)) {
        processChorus(wetBuffer.data(), numSamples);
    }
    
    if (!isEffectBypassed(EffectType::Reverb)) {
        processReverb(wetBuffer.data(), numSamples);
    }
    
    // Mix dry and wet signals based on master mix
    float dryLevel = 1.0f - masterMix_;
    float wetLevel = masterMix_;
    
    for (int i = 0; i < numSamples * numChannels_; ++i) {
        outputBuffer[i] = dryBuffer[i] * dryLevel + wetBuffer[i] * wetLevel;
    }
}

void EffectsChain::processStereo(float* leftBuffer, float* rightBuffer, int numSamples) {
    // Interleave stereo into temp buffer
    for (int i = 0; i < numSamples; ++i) {
        tempBuffer[i * 2] = leftBuffer[i];
        tempBuffer[i * 2 + 1] = rightBuffer[i];
    }
    
    // Process
    processBlock(tempBuffer.data(), tempBuffer.data(), numSamples);
    
    // De-interleave back to separate buffers
    for (int i = 0; i < numSamples; ++i) {
        leftBuffer[i] = tempBuffer[i * 2];
        rightBuffer[i] = tempBuffer[i * 2 + 1];
    }
}

// ========== Effect Parameters ==========

void EffectsChain::setEffectParameters(EffectType effectType, const EffectParameters& params) {
    effectParams[static_cast<int>(effectType)] = params;
}

EffectsChain::EffectParameters EffectsChain::getEffectParameters(EffectType effectType) const {
    return effectParams[static_cast<int>(effectType)];
}

void EffectsChain::setParameter(EffectType effectType, int parameterIndex, float normalizedValue) {
    // Clamp value to 0-1 range
    normalizedValue = std::max(0.0f, std::min(1.0f, normalizedValue));
    
    auto& params = effectParams[static_cast<int>(effectType)];
    
    // Common parameter (wet/dry mix)
    if (parameterIndex == 0) {
        params.wetDryMix = normalizedValue;
        return;
    }
    
    // Effect-specific parameters
    switch (effectType) {
        case EffectType::Reverb:
            if (parameterIndex == 1) params.reverb.roomSize = normalizedValue;
            else if (parameterIndex == 2) params.reverb.damping = normalizedValue;
            else if (parameterIndex == 3) params.reverb.width = normalizedValue;
            else if (parameterIndex == 4) params.reverb.decay = 0.5f + normalizedValue * 4.5f;  // 0.5-5.0s
            break;
            
        case EffectType::Delay:
            if (parameterIndex == 1) params.delay.delayTime = normalizedValue * 2.0f;  // 0-2s
            else if (parameterIndex == 2) params.delay.feedback = normalizedValue;
            else if (parameterIndex == 3) params.delay.numTaps = 1.0f + std::round(normalizedValue * 3.0f);  // 1-4
            break;
            
        case EffectType::Chorus:
            if (parameterIndex == 1) params.chorus.rate = 0.5f + normalizedValue * 9.5f;  // 0.5-10 Hz
            else if (parameterIndex == 2) params.chorus.depth = normalizedValue;
            else if (parameterIndex == 3) params.chorus.width = normalizedValue * 10.0f;  // 0-10ms
            break;
            
        case EffectType::Distortion:
            if (parameterIndex == 1) params.distortion.drive = normalizedValue;
            else if (parameterIndex == 2) params.distortion.tone = normalizedValue;
            else if (parameterIndex == 3) params.distortion.makeup = 0.5f + normalizedValue * 1.5f;  // 0.5-2.0
            break;
            
        case EffectType::EQ:
            if (parameterIndex == 1) params.eq.lowGain = (normalizedValue - 0.5f) * 24.0f;  // -12 to +12 dB
            else if (parameterIndex == 2) params.eq.lowFreq = 20.0f + normalizedValue * 980.0f;  // 20-1000 Hz
            else if (parameterIndex == 3) params.eq.midGain = (normalizedValue - 0.5f) * 24.0f;
            else if (parameterIndex == 4) params.eq.midFreq = 200.0f + normalizedValue * 9800.0f;  // 200-10000 Hz
            else if (parameterIndex == 5) params.eq.highGain = (normalizedValue - 0.5f) * 24.0f;
            else if (parameterIndex == 6) params.eq.highFreq = 1000.0f + normalizedValue * 19000.0f;  // 1k-20k Hz
            else if (parameterIndex == 7) params.eq.qFactor = 0.1f + normalizedValue * 1.9f;  // 0.1-2.0
            updateEQCoefficients();
            break;
            
        case EffectType::Compression:
            if (parameterIndex == 1) params.compression.threshold = normalizedValue;
            else if (parameterIndex == 2) params.compression.ratio = 0.25f + normalizedValue * 0.75f;  // 2:1 to 8:1
            else if (parameterIndex == 3) params.compression.attack = 0.001f + normalizedValue * 0.099f;  // 1-100ms
            else if (parameterIndex == 4) params.compression.release = 0.01f + normalizedValue * 0.49f;  // 10-500ms
            else if (parameterIndex == 5) params.compression.makeupGain = 0.5f + normalizedValue * 3.5f;  // 0.5-4.0
            break;
            
        default:
            break;
    }
}

float EffectsChain::getParameter(EffectType effectType, int parameterIndex) const {
    const auto& params = effectParams[static_cast<int>(effectType)];
    
    if (parameterIndex == 0) {
        return params.wetDryMix;
    }
    
    switch (effectType) {
        case EffectType::Reverb:
            if (parameterIndex == 1) return params.reverb.roomSize;
            else if (parameterIndex == 2) return params.reverb.damping;
            else if (parameterIndex == 3) return params.reverb.width;
            else if (parameterIndex == 4) return (params.reverb.decay - 0.5f) / 4.5f;
            break;
            
        case EffectType::Delay:
            if (parameterIndex == 1) return params.delay.delayTime / 2.0f;
            else if (parameterIndex == 2) return params.delay.feedback;
            else if (parameterIndex == 3) return (params.delay.numTaps - 1.0f) / 3.0f;
            break;
            
        case EffectType::Chorus:
            if (parameterIndex == 1) return (params.chorus.rate - 0.5f) / 9.5f;
            else if (parameterIndex == 2) return params.chorus.depth;
            else if (parameterIndex == 3) return params.chorus.width / 10.0f;
            break;
            
        case EffectType::Distortion:
            if (parameterIndex == 1) return params.distortion.drive;
            else if (parameterIndex == 2) return params.distortion.tone;
            else if (parameterIndex == 3) return (params.distortion.makeup - 0.5f) / 1.5f;
            break;
            
        case EffectType::EQ:
            if (parameterIndex == 1) return (params.eq.lowGain / 24.0f) + 0.5f;
            else if (parameterIndex == 2) return (params.eq.lowFreq - 20.0f) / 980.0f;
            else if (parameterIndex == 3) return (params.eq.midGain / 24.0f) + 0.5f;
            else if (parameterIndex == 4) return (params.eq.midFreq - 200.0f) / 9800.0f;
            else if (parameterIndex == 5) return (params.eq.highGain / 24.0f) + 0.5f;
            else if (parameterIndex == 6) return (params.eq.highFreq - 1000.0f) / 19000.0f;
            else if (parameterIndex == 7) return (params.eq.qFactor - 0.1f) / 1.9f;
            break;
            
        case EffectType::Compression:
            if (parameterIndex == 1) return params.compression.threshold;
            else if (parameterIndex == 2) return (params.compression.ratio - 0.25f) / 0.75f;
            else if (parameterIndex == 3) return (params.compression.attack - 0.001f) / 0.099f;
            else if (parameterIndex == 4) return (params.compression.release - 0.01f) / 0.49f;
            else if (parameterIndex == 5) return (params.compression.makeupGain - 0.5f) / 3.5f;
            break;
            
        default:
            break;
    }
    
    return 0.0f;
}

std::string EffectsChain::getParameterName(EffectType effectType, int parameterIndex) const {
    if (parameterIndex == 0) return "Wet/Dry Mix";
    
    switch (effectType) {
        case EffectType::Reverb:
            if (parameterIndex == 1) return "Room Size";
            else if (parameterIndex == 2) return "Damping";
            else if (parameterIndex == 3) return "Width";
            else if (parameterIndex == 4) return "Decay Time";
            break;
            
        case EffectType::Delay:
            if (parameterIndex == 1) return "Delay Time";
            else if (parameterIndex == 2) return "Feedback";
            else if (parameterIndex == 3) return "Num Taps";
            break;
            
        case EffectType::Chorus:
            if (parameterIndex == 1) return "LFO Rate";
            else if (parameterIndex == 2) return "Depth";
            else if (parameterIndex == 3) return "Width";
            break;
            
        case EffectType::Distortion:
            if (parameterIndex == 1) return "Drive";
            else if (parameterIndex == 2) return "Tone";
            else if (parameterIndex == 3) return "Makeup Gain";
            break;
            
        case EffectType::EQ:
            if (parameterIndex == 1) return "Low Gain";
            else if (parameterIndex == 2) return "Low Frequency";
            else if (parameterIndex == 3) return "Mid Gain";
            else if (parameterIndex == 4) return "Mid Frequency";
            else if (parameterIndex == 5) return "High Gain";
            else if (parameterIndex == 6) return "High Frequency";
            else if (parameterIndex == 7) return "Q Factor";
            break;
            
        case EffectType::Compression:
            if (parameterIndex == 1) return "Threshold";
            else if (parameterIndex == 2) return "Ratio";
            else if (parameterIndex == 3) return "Attack";
            else if (parameterIndex == 4) return "Release";
            else if (parameterIndex == 5) return "Makeup Gain";
            break;
            
        default:
            break;
    }
    
    return "Unknown";
}

// ========== Effect Control ==========

void EffectsChain::setEffectBypass(EffectType effectType, bool bypass) {
    effectParams[static_cast<int>(effectType)].bypass = bypass;
}

bool EffectsChain::isEffectBypassed(EffectType effectType) const {
    return effectParams[static_cast<int>(effectType)].bypass;
}

void EffectsChain::setMasterMix(float mix) {
    masterMix_ = std::max(0.0f, std::min(1.0f, mix));
}

float EffectsChain::getMasterMix() const {
    return masterMix_;
}

void EffectsChain::resetAll() {
    for (int i = 0; i < static_cast<int>(EffectType::Count); ++i) {
        resetEffect(static_cast<EffectType>(i));
    }
}

void EffectsChain::resetEffect(EffectType effectType) {
    effectParams[static_cast<int>(effectType)] = EffectParameters();
}

void EffectsChain::clearReverb() {
    for (auto& buffer : reverbState.combBuffers) {
        std::fill(buffer.begin(), buffer.end(), 0.0f);
    }
    for (auto& buffer : reverbState.allpassBuffers) {
        std::fill(buffer.begin(), buffer.end(), 0.0f);
    }
    std::fill(reverbState.combFilter1.begin(), reverbState.combFilter1.end(), 0.0f);
    std::fill(reverbState.combFilter2.begin(), reverbState.combFilter2.end(), 0.0f);
    std::fill(reverbState.allpassFilter1.begin(), reverbState.allpassFilter1.end(), 0.0f);
    std::fill(reverbState.allpassFilter2.begin(), reverbState.allpassFilter2.end(), 0.0f);
}

void EffectsChain::clearDelay() {
    std::fill(delayState.buffer.begin(), delayState.buffer.end(), 0.0f);
    delayState.writeIndex = 0;
}

void EffectsChain::clearChorus() {
    std::fill(chorusState.buffer.begin(), chorusState.buffer.end(), 0.0f);
    chorusState.writeIndex = 0;
}

// ========== Analysis & Monitoring ==========

int EffectsChain::getParameterCount(EffectType effectType) const {
    switch (effectType) {
        case EffectType::Reverb:
            return 5;  // wetDry + 4 reverb params
        case EffectType::Delay:
            return 4;  // wetDry + 3 delay params
        case EffectType::Chorus:
            return 4;  // wetDry + 3 chorus params
        case EffectType::Distortion:
            return 4;  // wetDry + 3 distortion params
        case EffectType::EQ:
            return 8;  // wetDry + 7 eq params
        case EffectType::Compression:
            return 6;  // wetDry + 5 compression params
        default:
            return 0;
    }
}

std::string EffectsChain::getEffectName(EffectType effectType) const {
    switch (effectType) {
        case EffectType::Reverb:
            return "Reverb";
        case EffectType::Delay:
            return "Delay";
        case EffectType::Chorus:
            return "Chorus";
        case EffectType::Distortion:
            return "Distortion";
        case EffectType::EQ:
            return "EQ";
        case EffectType::Compression:
            return "Compression";
        default:
            return "Unknown";
    }
}

float EffectsChain::getCPUUsage() const {
    return cpuUsage_;
}

int EffectsChain::getLatencySamples() const {
    // Reverb contributes minimal latency (internal processing)
    // Delay + Chorus contribute buffer delay
    int maxDelay = static_cast<int>(effectParams[static_cast<int>(EffectType::Delay)].delay.delayTime * sampleRate_);
    return maxDelay;
}

// ========== Effect Processing Implementations ==========

void EffectsChain::processReverb(float* buffer, int numSamples) {
    const auto& params = effectParams[static_cast<int>(EffectType::Reverb)];
    float damping = params.reverb.damping * 0.4f;
    float feedback = params.reverb.roomSize;
    float dryWet = params.reverb.width;
    
    for (int i = 0; i < numSamples; ++i) {
        float input = buffer[i] * reverbState.masterGain;
        float output = 0.0f;
        
        // Process comb filters
        for (int j = 0; j < 4; ++j) {
            auto& combBuf = reverbState.combBuffers[j];
            int& idx = reverbState.combIndices[j];
            
            float damp1 = damping;
            float damp2 = 1.0f - damping;
            
            float bufOut = combBuf[idx];
            reverbState.combFilter2[j] = bufOut * damp2 + reverbState.combFilter1[j] * damp1;
            reverbState.combFilter1[j] = reverbState.combFilter2[j];
            
            combBuf[idx] = input + reverbState.combFilter2[j] * feedback;
            
            idx = (idx + 1) % combBuf.size();
            output += bufOut;
        }
        
        // Process allpass filters
        for (int j = 0; j < 2; ++j) {
            auto& allBuf = reverbState.allpassBuffers[j];
            int& idx = reverbState.allpassIndices[j];
            
            float bufOut = allBuf[idx];
            float apout = -output + bufOut;
            allBuf[idx] = output + bufOut * 0.5f;
            
            idx = (idx + 1) % allBuf.size();
            output = apout;
        }
        
        buffer[i] = output * dryWet;
    }
}

void EffectsChain::processDelay(float* buffer, int numSamples) {
    const auto& params = effectParams[static_cast<int>(EffectType::Delay)];
    int delayMs = static_cast<int>(params.delay.delayTime * 1000.0f);
    int delaySamples = std::max(1, static_cast<int>(delayMs * sampleRate_ / 1000.0f));
    delaySamples = std::min(delaySamples, delayState.maxDelayTime);
    float feedback = params.delay.feedback * 0.9f;
    
    for (int i = 0; i < numSamples; ++i) {
        int readIdx = (delayState.writeIndex - delaySamples + delayState.maxDelayTime) % delayState.maxDelayTime;
        float delayedSample = delayState.buffer[readIdx];
        
        buffer[i] = buffer[i] + delayedSample * feedback;
        delayState.buffer[delayState.writeIndex] = buffer[i];
        
        delayState.writeIndex = (delayState.writeIndex + 1) % delayState.maxDelayTime;
    }
}

void EffectsChain::processChorus(float* buffer, int numSamples) {
    const auto& params = effectParams[static_cast<int>(EffectType::Chorus)];
    float rate = params.chorus.rate;
    float depth = params.chorus.depth;
    float width = params.chorus.width * sampleRate_ / 44100.0f;  // Normalize for sample rate
    
    const float twoPi = 6.28318f;
    
    for (int i = 0; i < numSamples; ++i) {
        // Generate LFO (sine wave modulation)
        float lfo = std::sin(chorusState.lfoPhase * twoPi) * depth;
        chorusState.lfoPhase += (rate / sampleRate_);
        if (chorusState.lfoPhase > 1.0f) chorusState.lfoPhase -= 1.0f;
        
        // Calculate modulated delay
        float modDelayMs = width * (0.5f + 0.5f * lfo);
        int modDelaySamples = static_cast<int>(modDelayMs * sampleRate_ / 1000.0f);
        modDelaySamples = std::max(1, std::min(modDelaySamples, chorusState.maxDelayTime - 1));
        
        int readIdx = (chorusState.writeIndex - modDelaySamples + chorusState.maxDelayTime) % chorusState.maxDelayTime;
        float chorusSample = chorusState.buffer[readIdx];
        
        buffer[i] = (buffer[i] + chorusSample) * 0.5f;
        chorusState.buffer[chorusState.writeIndex] = buffer[i];
        
        chorusState.writeIndex = (chorusState.writeIndex + 1) % chorusState.maxDelayTime;
    }
}

void EffectsChain::processDistortion(float* buffer, int numSamples) {
    const auto& params = effectParams[static_cast<int>(EffectType::Distortion)];
    float drive = 1.0f + params.distortion.drive * 10.0f;  // 1x to 11x gain
    float tone = params.distortion.tone;
    float makeup = params.distortion.makeup;
    
    for (int i = 0; i < numSamples; ++i) {
        float driven = buffer[i] * drive;
        float clipped = softClip(driven);
        
        // Tone shaping (simple low-shelf + high-shelf)
        float shaped = clipped;
        if (tone < 0.5f) {
            // Emphasize lows
            distortionState.lowShelf = distortionState.lowShelf * 0.9f + clipped * (1.0f - tone);
            shaped = shaped * 0.5f + distortionState.lowShelf * 0.5f;
        } else {
            // Emphasize highs
            distortionState.highShelf = distortionState.highShelf * 0.9f + clipped * (tone - 0.5f);
            shaped = shaped * 0.5f + distortionState.highShelf * 0.5f;
        }
        
        buffer[i] = shaped * makeup;
    }
}

void EffectsChain::processEQ(float* buffer, int numSamples) {
    const auto& params = effectParams[static_cast<int>(EffectType::EQ)];
    
    // Process each band
    for (int band = 0; band < 3; ++band) {
        auto& state = eqState.bands[band];
        
        for (int i = 0; i < numSamples; ++i) {
            float input = buffer[i];
            float output = state.a0 * input + state.a1 * state.x1 + state.a2 * state.x2
                         - state.b1 * state.y1 - state.b2 * state.y2;
            
            state.x2 = state.x1;
            state.x1 = input;
            state.y2 = state.y1;
            state.y1 = output;
            
            buffer[i] = output;
        }
    }
}

void EffectsChain::processCompression(float* buffer, int numSamples) {
    const auto& params = effectParams[static_cast<int>(EffectType::Compression)];
    float threshold = params.compression.threshold * -60.0f;  // Convert to dB
    float ratio = 2.0f + params.compression.ratio * 6.0f;  // 2:1 to 8:1
    float attackMs = params.compression.attack * 1000.0f;
    float releaseMs = params.compression.release * 1000.0f;
    float makeup = params.compression.makeupGain;
    
    float attackCoef = std::exp(-2.0f * 3.14159f / (attackMs * sampleRate_ / 1000.0f));
    float releaseCoef = std::exp(-2.0f * 3.14159f / (releaseMs * sampleRate_ / 1000.0f));
    
    for (int i = 0; i < numSamples; ++i) {
        float input = std::abs(buffer[i]);
        float inputDb = input > 0.0001f ? 20.0f * std::log10(input) : -80.0f;
        
        // Envelope follower
        if (inputDb > compressionState.envelope) {
            compressionState.envelope = attackCoef * compressionState.envelope + (1.0f - attackCoef) * inputDb;
        } else {
            compressionState.envelope = releaseCoef * compressionState.envelope + (1.0f - releaseCoef) * inputDb;
        }
        
        // Calculate gain reduction
        float gainDb = 0.0f;
        if (compressionState.envelope > threshold) {
            gainDb = (threshold + (compressionState.envelope - threshold) / ratio) - compressionState.envelope;
        }
        
        float gain = std::pow(10.0f, gainDb / 20.0f);
        buffer[i] = buffer[i] * gain * makeup;
    }
}

// ========== Helper Functions ==========

float EffectsChain::softClip(float sample) {
    // Soft-clipping using tanh approximation
    if (sample > 2.0f) return 1.0f;
    if (sample < -2.0f) return -1.0f;
    
    float x = sample * 0.5f;
    return x * (27.0f + x * x) / (27.0f + 9.0f * x * x);
}

float EffectsChain::dbToLinear(float db) {
    return std::pow(10.0f, db / 20.0f);
}

float EffectsChain::linearToDb(float linear) {
    return linear > 0.0001f ? 20.0f * std::log10(linear) : -80.0f;
}

float EffectsChain::normalizedToDb(float normalized, float minDb, float maxDb) {
    return minDb + normalized * (maxDb - minDb);
}

float EffectsChain::dbToNormalized(float db, float minDb, float maxDb) {
    return (db - minDb) / (maxDb - minDb);
}

void EffectsChain::updateEQCoefficients() {
    // Placeholder: Implement biquad filter coefficient calculations
    // This would normally calculate peaking filter coefficients for each band
    // based on frequency, gain, and Q factor parameters
}

}  // namespace scalechord
