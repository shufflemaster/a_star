#pragma once

#include <Windows.h>
#include <iostream>

using namespace std;

class CPerfCounter
{
public:

	CPerfCounter()
	{
		QueryPerformanceFrequency(&mFrequency);
	}

	~CPerfCounter()
	{
	}

	void start()
	{
		QueryPerformanceCounter(&mStartingTime);
	}

	void stop()
	{
		QueryPerformanceCounter(&mEndingTime);
	}

	void dump(int loopCnt) {
		cout << "Frequency:" << mFrequency.QuadPart << ", start:" << mStartingTime.QuadPart << ", end:" << mEndingTime.QuadPart << endl;
		LARGE_INTEGER tickCount;
		tickCount.QuadPart = mEndingTime.QuadPart - mStartingTime.QuadPart;
		LARGE_INTEGER elapsedUS, elapsedNS;
		elapsedUS.QuadPart = tickCount.QuadPart * 1000000;
		elapsedUS.QuadPart /= mFrequency.QuadPart;
		elapsedNS.QuadPart = tickCount.QuadPart * 1000000000;
		elapsedNS.QuadPart /= mFrequency.QuadPart;

		cout << "tick count:" << tickCount.QuadPart << ", us:" << elapsedUS.QuadPart << ", ns:" << elapsedNS.QuadPart << endl;

		double us_per_loop = elapsedUS.QuadPart;
		us_per_loop /= loopCnt;

		double ns_per_loop = elapsedNS.QuadPart;
		ns_per_loop /= loopCnt;

		double ticks_per_loop = tickCount.QuadPart;
		ticks_per_loop /= loopCnt;

		cout << "ticks/loop:" << ticks_per_loop << ", us/loop:" << us_per_loop << ", ns/loop:" << ns_per_loop << endl;
	}

private:
	LARGE_INTEGER mFrequency;
	LARGE_INTEGER mStartingTime, mEndingTime;
};

