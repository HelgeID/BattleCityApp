//FINAL
#pragma once
#include <SFML\Graphics.hpp>

class FPS
{

	clock_t startFrame, finishFrame;
	clock_t deltaFrame{ 0 };
	unsigned int countFrames{ 0 };
	double rateFrame{ 30 };
	double averageTimeFrame{ 33.333 }; //milliseconds

	double clockToMilliseconds(clock_t ticks)
	{
		// units/(units/time) => time (seconds) * 1000 = milliseconds
		return (ticks / (double)CLOCKS_PER_SEC) * 1000.0;
	}

	void DeltaFrame() { deltaFrame += finishFrame - startFrame; return; }

private:
	sf::Clock mClock;
	unsigned int fps;
	unsigned int frame;

public:
	FPS() : fps(0), frame(0) {}
	const unsigned int getFPS() const { return fps; }
	const unsigned int updateFPS();

	void StartFrame() { startFrame = clock(); return; }
	void FinishFrame() { finishFrame = clock(); return; }
	const double getTimeFrame() const { return averageTimeFrame; }
	const double updateTimeFrame();
};

inline const unsigned int FPS::updateFPS()
{
	if (mClock.getElapsedTime().asSeconds() >= 1.f) {
		fps = frame;
		frame = 0;
		mClock.restart();
	}
	++frame;
	return fps;
}

inline const double FPS::updateTimeFrame()
{
	DeltaFrame();
	countFrames = countFrames + 1;
	if (clockToMilliseconds(deltaFrame) > 1000.0) {
		rateFrame = (double)countFrames * 0.5 + rateFrame * 0.5;
		countFrames = 0;
		deltaFrame -= CLOCKS_PER_SEC;
		averageTimeFrame = 1000.0 / (rateFrame == 0 ? 0.001 : rateFrame);
	}
	return averageTimeFrame;
}
