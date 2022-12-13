

#include "Top-KartPCH.h"

#include "Clock.h"
#include <NiSystem.h>

//
// This class manages time for the scheduler
//
// For clarity, this keeps tracks of both real time and virtual time.
//
// System time ....... real time (zero-based)
// Simulation time ... virtual time (zero-based)
// Frame ............. current frame
//

//---------------------------------------------------------------------------
Clock::Clock() : m_fScale(1.0f), m_fMinFrameTime(0.0f), 
    m_bFrameTimeLocked(false), m_bConstantFrameLengthEnabled(false), 
    m_fConstantFrameLength(1.0f/100.0f), m_fFrameTime(0)
{
	Reset();
}
//---------------------------------------------------------------------------
Clock::~Clock()
{
    /* */
}
//---------------------------------------------------------------------------
void Clock::Reset()
{
	m_bRunning = false;

	m_fThisTime = NiGetCurrentTimeInSec();
	m_fLastTime = m_fLastFrameTime = m_fThisTime;

    m_fUnScaledSystemTime = 0.0f;
	m_fSystemTime = 0.0f;
	m_fPauseTime = 0.0f;
	m_fSystemOffset = m_fThisTime;

	m_uiFrameCount = 0;
	m_fFrameStart = 0.0f;
	m_fFrameEnd = 0.0f;

	m_fSimTime = 0.0f;
	m_fSimOffset = 0.0f;
}
//---------------------------------------------------------------------------
void Clock::Run()
{
	if (!m_bRunning)
	{
		Update();

		m_fSimOffset += (m_fSystemTime - m_fPauseTime);
	}

	m_bRunning = true;
}
//---------------------------------------------------------------------------
void Clock::Stop()
{
	if (m_bRunning)
	{
		Update();

		m_fPauseTime = m_fSystemTime;
	}

	m_bRunning = false;
}
//---------------------------------------------------------------------------
void Clock::Update()
{
	m_fLastTime = m_fThisTime;
    m_fThisTime = NiGetCurrentTimeInSec();

	// Convert to elapsed time, also handle rollover
    float fElapsed;
    if (m_fThisTime < m_fLastTime)
        fElapsed = m_fLastTime - m_fThisTime;
    else
        fElapsed = m_fThisTime - m_fLastTime;

	// System time is scaled real or fixed time. it never pauses.
    m_fSystemTime += fElapsed * m_fScale;
    m_fUnScaledSystemTime += fElapsed;
}
//---------------------------------------------------------------------------
void Clock::BeginFrame()
{
	// Begin a new frame. This method is normally called
	// immediately AFTER rendering the previous frame, and
	// sets the context for all work which will be rendered
	// on the next frame.
	//
	// This method increments the frame count and samples real
	// time to determine the elapsed time since the last frame.
	//
	// <Render frame n>
	// <Begin frame n+1>
	// <Task1>
	// <Task2>
	// <Task3...>
	// <Render frame n+1>
	//
    
    
    float fCurrentFrameTime = NiGetCurrentTimeInSec();

    float fElapsed;
    if (fCurrentFrameTime < m_fLastFrameTime)
        fElapsed = m_fLastFrameTime - fCurrentFrameTime;
    else
        fElapsed = fCurrentFrameTime - m_fLastFrameTime;

    m_fFrameTime = fElapsed;
    m_fLastFrameTime = NiGetCurrentTimeInSec();

    // If running with a locked frame time, determine how much time
    // to sleep (in milliseconds), if any, and sleep this thread.
    // Note that this code uses the real clock directly.
    if (m_bFrameTimeLocked && !m_bConstantFrameLengthEnabled) 
    {
        float fRemain = m_fMinFrameTime - fElapsed;
        unsigned int uiSleepMS = 0;

        if (fRemain > 0.0f)
        {
            unsigned int uiSleepMS = (unsigned int)(fRemain * 1000.0f);

            if (uiSleepMS > 0)
            {
                NiSleep(uiSleepMS);
            }
        }
     
    }
   
    m_uiFrameCount++;

    // Update system time
	Update(); 

	if (m_bRunning)
	{
        // Start of this frame = end of last frame
		m_fFrameStart = m_fFrameEnd; 
        // End of this frame = current time
		m_fFrameEnd = m_fSystemTime - m_fSimOffset; 
        // Set simulation time to start of frame
        if (m_bConstantFrameLengthEnabled)
        {
            m_fSimTime += m_fConstantFrameLength;
        }
		else
        {
            m_fSimTime = m_fFrameStart; 
            m_fUnScaledSimTime = m_fUnScaledSystemTime;
        }
	}
}
//---------------------------------------------------------------------------
void Clock::AdvanceTo(float fNewTime)
{
	if (m_bRunning && fNewTime >= m_fSimTime)
	{
		m_fSimTime = fNewTime;
	}
}
//---------------------------------------------------------------------------
void Clock::AdvanceToEnd()
{
	if (m_bRunning)
	{
		m_fSimTime = m_fFrameEnd;
	}
}
//---------------------------------------------------------------------------
