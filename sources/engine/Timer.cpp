#include <engine/Timer.h>

Timer* initTimer(float timeScale, bool cntFPS)
{

	Timer* t = new Timer();

	t->setTimeScale(timeScale);
	t->reset();

	return t;

}

void Timer::setEFPSCnt(bool eFPSCnt)
{

	this->eFPSCnt = eFPSCnt;

}

bool Timer::getEFPSCnt() const
{

	return this->eFPSCnt;

}

double Timer::getTotalTime() const
{
	return this->totalTime; 
}

int Timer::getFPS() const
{ 
	return this->fps; 
}

void Timer::setTimeScale(float timeScale) 
{ 
	this->timeScale = timeScale; 
}

double Timer::getTimeScale() const
{
	return this->timeScale; 
}

//Return seconds in one cycle
float Timer::getDeltaTime() const
{
	return static_cast<float>(this->deltaTime); 
}

bool Timer::getIsStoped() const 
{
	return this->isInPause; 
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

void Timer::computeSecondsPerCount()
{

	QueryPerformanceFrequency((LARGE_INTEGER*)&this->countsPerSecond);
	this->secondsPerCount = 1.0 / (double)countsPerSecond * timeScale;

}

double Timer::getPauseTime()
{

	this->computeSecondsPerCount();
	return this->pauseTime * this->secondsPerCount;

}



