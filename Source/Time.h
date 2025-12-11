#pragma once
#include <chrono>

class Time {
private:
    using clock = std::chrono::high_resolution_clock;

public:
    Time() {
        m_startTime = clock::now();
        m_frameTime = clock::now();
    }

    void Tick();
    void Reset() { m_startTime = clock::now(); }
    float GetTime() const { return m_time; }
    float GetDeltaTime() const { return std::min(m_deltaTime * m_timeScale, m_maxDeltaTime); }
    void SetTimeScale(float timeScale) { m_timeScale = timeScale; }

private:
    float m_time = 0;
    float m_deltaTime = 0;
    float m_maxDeltaTime = 1.0f / 30.0f; // Cap at ~33ms (30 FPS equivalent)
    float m_timeScale = 1;
    
    clock::time_point m_startTime;
    clock::time_point m_frameTime;
};