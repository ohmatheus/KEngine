#pragma once

#include <ctime>

//----------------------------------------------------------
class Timer final
{
public:
	void Start()
	{
		m_StartTime = clock();
		m_IsRunning = true;
	}

	double Stop()
	{
		m_EndTime = clock();
		m_IsRunning = false;
		return ElapsedSeconds();
	}

	double ElapsedMilliseconds()
	{
		return ElapsedSeconds() * 1000.0;
	}

	double ElapsedSeconds()
	{
		clock_t endTime;
		if (m_IsRunning)
			endTime = clock();
		else
			endTime = m_EndTime;
		return (double)((double)(endTime - m_StartTime) / (double)CLOCKS_PER_SEC);
	}

private:
	clock_t		m_StartTime;
	clock_t		m_EndTime;
	bool		m_IsRunning = false;
};
//----------------------------------------------------------
