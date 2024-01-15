#include <engine/Timer.h>
#include <chrono>

void Timer::setEFPSCnt(bool eFPSCnt)
{
	this->eFPSCnt = eFPSCnt;
}

void Timer::setTimeScale(float timeScale) 
{ 
	this->timeScale = timeScale; 
}

void Timer::tick()
{

	this->computeSecondsPerCount();
	this->computeCurrTime();
	/////////////////////////
	//TotalTime
	/////////////////////////
	if (this->isInPause)
		this->pauseTime = this->currTime - this->stopTime + this->prevPauseTime;

	this->totalTime = ((this->currTime - this->baseTime) - this->pauseTime) * this->secondsPerCount;

	/////////////////////////
	//DeltaTime
	/////////////////////////
	this->deltaTime = (this->currTime - this->prevTime) * this->secondsPerCount;

	this->prevTime = this->currTime;

	/////////////////////////
	//fps
	/////////////////////////

	if (!this->eFPSCnt)
		return;

	this->frameCnt++;

	if ((this->totalTime - this->timeElapsed) >= 1.0f)
	{
		this->fps = this->frameCnt;
		this->timeElapsed += 1.0f;
		this->frameCnt = 0;
	}

}

void Timer::reset()
{
	this->computeCurrTime();
	this->baseTime = this->currTime;
	this->prevTime = this->currTime;
	this->stopTime = 0;
	this->pauseTime = 0;
	this->prevPauseTime = 0;
	this->isInPause = false;
	this->timeElapsed = 0;
	this->frameCnt = 0;
}

void Timer::resume()
{
	this->computeCurrTime();
	this->pauseTime = this->currTime - this->stopTime + this->prevPauseTime;
	this->prevPauseTime = this->pauseTime;
	this->isInPause = false;
}

void Timer::stop()
{
	this->computeCurrTime();
	this->stopTime = this->currTime;
	this->isInPause = true;
}

void Timer::computeCurrTime()
{
	std::chrono::steady_clock::time_point startTime = std::chrono::steady_clock::now();
	this->currTime = startTime.time_since_epoch().count();
}

void Timer::computeSecondsPerCount()
{

	const auto tickDuration = std::chrono::steady_clock::duration::period();
	this->secondsPerCount = 1.0 / static_cast<double>(tickDuration.den);

}

double Timer::getPauseTime()
{

	this->computeSecondsPerCount();
	return this->pauseTime * this->secondsPerCount;

}



