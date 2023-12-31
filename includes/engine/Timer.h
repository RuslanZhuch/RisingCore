#ifndef TIMER_H
#define TIMER_H

#include <Windows.h>
#include <time.h>

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
	int getFPS() const;
	// Return Pause Time
	double getPauseTime();
	// Return non-Pause Time
	double getTotalTime() const;

	double getTimeScale() const;

	//Return seconds in one cycle
	float getDeltaTime() const;

	bool getIsStoped() const;

	void setEFPSCnt(bool eFPSCnt = true);
	bool getEFPSCnt() const;

private:

	//Return Time (Processor cycles) 
	__int64 getCurrTime() { return this->currTime; }
	void computeCurrTime() { /*this->CurrTime = clock(); }*/QueryPerformanceCounter((LARGE_INTEGER*)&this->currTime); }
	double getSecondsPerCount() { return this->secondsPerCount; }
	void computeSecondsPerCount();

private:

	///////////////////////////////////
	//**Processor Counts
	///////////////////////////////////
	// Current Time
	__int64 currTime{ 0 };
	// Previous Time
	__int64 prevTime{ 0 };
	// Start Time
	__int64 baseTime{ 0 };
	// Stop Time
	__int64 stopTime{ 0 };
	// In Pause Time
	__int64 pauseTime{ 0 };
	// In Previuos Pause Time
	__int64 prevPauseTime{ 0 };

	__int64 countsPerSecond{ 0 };
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

Timer* initTimer(float timeScale = 1, bool cntFPS = false);

#endif // !TIMER_H