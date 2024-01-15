#pragma once

#include <cinttypes>

class Timer
{
public:

	// Reset Timer
	void reset();

	// Update Timer
	// Must be executed every cycle
	void tick();

	void stop();
	void resume();

	// Change Timer Speed
	// TimeScale = 1.f - default
	void setTimeScale(float timeScale);

	// Return Fraps(cycles) per second
	[[nodiscard]] int getFPS() const { return this->fps; }
	// Return Pause Time
	[[nodiscard]] double getPauseTime();
	// Return non-Pause Time
	[[nodiscard]] double getTotalTime() const { return this->totalTime; }

	[[nodiscard]] double getTimeScale() const { return this->timeScale; }

	//Return seconds in one cycle
	[[nodiscard]] float getDeltaTime() const { return static_cast<float>(this->deltaTime); }

	[[nodiscard]] bool getIsStoped() const { return this->isInPause; }

	void setEFPSCnt(bool eFPSCnt = true);
	[[nodiscard]] bool getEFPSCnt() const { return this->eFPSCnt; }

private:

	//Return Time (Processor cycles) 
	[[nodiscard]] int64_t getCurrTime() const { return this->currTime; }
	[[nodiscard]] double getSecondsPerCount() const { return this->secondsPerCount; }
	void computeCurrTime();
	void computeSecondsPerCount();

private:

	///////////////////////////////////
	//**Processor Counts
	///////////////////////////////////
	// Current Time
	int64_t currTime{ 0 };
	// Previous Time
	int64_t prevTime{ 0 };
	// Start Time
	int64_t baseTime{ 0 };
	// Stop Time
	int64_t stopTime{ 0 };
	// In Pause Time
	int64_t pauseTime{ 0 };
	// In Previuos Pause Time
	int64_t prevPauseTime{ 0 };

	int64_t countsPerSecond{ 0 };
	double secondsPerCount{ 0 };

	///////////////////////////////////
	//**Non-Processor Counts
	///////////////////////////////////
	int fps{ 0 };

	double timeScale{ 1. };
	// non-Pause Time
	double totalTime{ 0 };

	double deltaTime{ 0 };

private:

	int frameCnt{ 0 };
	double timeElapsed{ 0 };

	bool isInPause{ 0 };

	bool eFPSCnt{ false };
};
