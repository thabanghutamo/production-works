#pragma once

#include <chrono>
#include <vector>
#include <deque>
#include <algorithm>
#include <cmath>

namespace scalechord {

/**
 * @class PerformanceMetrics
 * @brief Tracks latency and CPU usage for real-time audio operations
 * 
 * Maintains a rolling history of measurements and provides statistics
 * for performance monitoring and optimization validation.
 */
class PerformanceMetrics {
public:
    struct Measurement {
        std::chrono::microseconds duration;
        size_t memoryUsed = 0;
        int notesProcessed = 1;
    };

    explicit PerformanceMetrics(int historySize = 1000)
        : maxHistorySize_(historySize)
    {}

    /**
     * Start a new measurement
     */
    void startMeasurement() {
        startTime_ = std::chrono::steady_clock::now();
    }

    /**
     * End measurement and record it
     * @param notesProcessed Number of notes processed in this measurement
     */
    void endMeasurement(int notesProcessed = 1) {
        auto endTime = std::chrono::steady_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(
            endTime - startTime_
        );
        
        Measurement m;
        m.duration = duration;
        m.notesProcessed = notesProcessed;
        
        history_.push_back(m);
        if (history_.size() > static_cast<size_t>(maxHistorySize_)) {
            history_.pop_front();
        }
    }

    /**
     * Get average latency in milliseconds
     */
    float getAverageLatencyMs() const {
        if (history_.empty()) return 0.0f;
        
        float total = 0.0f;
        for (const auto& m : history_) {
            total += m.duration.count();
        }
        return (total / history_.size()) / 1000.0f;
    }

    /**
     * Get maximum latency in milliseconds
     */
    float getMaxLatencyMs() const {
        if (history_.empty()) return 0.0f;
        
        float maxDuration = 0.0f;
        for (const auto& m : history_) {
            maxDuration = std::max(maxDuration, static_cast<float>(m.duration.count()));
        }
        return maxDuration / 1000.0f;
    }

    /**
     * Get minimum latency in milliseconds
     */
    float getMinLatencyMs() const {
        if (history_.empty()) return 0.0f;
        
        float minDuration = std::numeric_limits<float>::max();
        for (const auto& m : history_) {
            minDuration = std::min(minDuration, static_cast<float>(m.duration.count()));
        }
        return minDuration / 1000.0f;
    }

    /**
     * Get 95th percentile latency in milliseconds
     */
    float getP95LatencyMs() const {
        if (history_.empty()) return 0.0f;
        
        std::vector<float> durations;
        for (const auto& m : history_) {
            durations.push_back(static_cast<float>(m.duration.count()) / 1000.0f);
        }
        
        std::sort(durations.begin(), durations.end());
        
        size_t p95Index = static_cast<size_t>(durations.size() * 0.95f);
        p95Index = std::min(p95Index, durations.size() - 1);
        
        return durations[p95Index];
    }

    /**
     * Get throughput in notes per second
     */
    float getNotesPerSecond() const {
        if (history_.empty()) return 0.0f;
        
        long totalMicroseconds = 0;
        int totalNotes = 0;
        
        for (const auto& m : history_) {
            totalMicroseconds += m.duration.count();
            totalNotes += m.notesProcessed;
        }
        
        if (totalMicroseconds == 0) return 0.0f;
        
        return (totalNotes * 1000000.0f) / totalMicroseconds;
    }

    /**
     * Get estimated CPU usage percentage
     * @param targetBlockTimeMs Target block time in milliseconds
     * @return CPU usage as percentage (0-100)
     */
    float getCPUUsagePercent(float targetBlockTimeMs) const {
        if (targetBlockTimeMs <= 0.0f) return 0.0f;
        
        float avgLatency = getAverageLatencyMs();
        return (avgLatency / targetBlockTimeMs) * 100.0f;
    }

    /**
     * Get number of measurements in history
     */
    size_t getHistorySize() const {
        return history_.size();
    }

    /**
     * Clear measurement history
     */
    void reset() {
        history_.clear();
    }

    /**
     * Print summary statistics
     */
    void printSummary() const {
        if (history_.empty()) {
            printf("No measurements recorded\n");
            return;
        }
        
        printf("=== Performance Metrics ===\n");
        printf("Samples:     %zu\n", history_.size());
        printf("Avg:         %.3f ms\n", getAverageLatencyMs());
        printf("Min:         %.3f ms\n", getMinLatencyMs());
        printf("Max:         %.3f ms\n", getMaxLatencyMs());
        printf("P95:         %.3f ms\n", getP95LatencyMs());
        printf("Throughput:  %.0f notes/sec\n", getNotesPerSecond());
        printf("CPU (10ms):  %.1f%%\n", getCPUUsagePercent(10.0f));
    }

private:
    std::deque<Measurement> history_;
    int maxHistorySize_ = 1000;
    std::chrono::steady_clock::time_point startTime_;
};

} // namespace scalechord
