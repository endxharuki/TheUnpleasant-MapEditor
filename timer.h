#pragma once
#include  <chrono>

class Timer
{
private:

	std::chrono::high_resolution_clock::time_point m_LastTime;
	float m_deltaTime = 0.0f;

public:

	Timer() : m_LastTime(std::chrono::high_resolution_clock::now()), m_deltaTime(0.0f) {}
	void Update()
	{

		auto currenTime = std::chrono::high_resolution_clock::now();
		std::chrono::duration<float> elapsedTime = currenTime - m_LastTime;

		m_LastTime = currenTime;
		m_deltaTime = elapsedTime.count();

	}
	float GetDeltaTime() const { return m_deltaTime; }

};

