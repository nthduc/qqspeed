//
// Based directly on the "Scheduling Game Events" Gem from 
// Game Programming Gems 3, which requires:
//
// Intel Open Source License for Game Scheduler Gem sample code
// Copyright (c) 2002, Intel Corporation.
// All rights reserved. 
//

//
// This class manages time for the scheduler
//
// For clarity, this keeps tracks of both real time and virtual time.
//
// System time ....... real time (zero-based)
// Simulation time ... virtual time (zero-based)
// Frame ............. current frame
//

#ifndef CLOCK_H
#define CLOCK_H

class Clock 
{
public:
	Clock();
	~Clock();

	void Reset();
	void Run();
	void Stop();
	bool IsRunning() const;

	void Update();
	void BeginFrame();

	float GetSystem();
	float GetTime() const;
    float GetUnScaledTime() const;
	unsigned int GetFrame() const;
	float GetFrameStart() const;
	float GetFrameEnd() const;

	void AdvanceTo(float fNewTime);
	void AdvanceToEnd();

    // Scale system time
    float GetScale();
    void SetScale(float fScale);

    // Framerate limiting mode
    void GetMinFrameTime(bool& bOn, float& fMinFrameTime);
    void SetMinFrameTime(bool bOn, float fMinFrameTime = (1.0f / 100.0f));

    // Constant frame time functions
    float GetConstantFrameLength();
    void SetConstantFrameLength(float fLength);
    float GetFrameTime();

    bool IsConstantLengthEnabled();
    void EnableConstantLength(bool bEnable);

protected:
	bool m_bRunning;

	float m_fThisTime;
	float m_fLastTime;

	float m_fSystemTime;
    float m_fUnScaledSystemTime;
	float m_fSystemOffset;
	float m_fPauseTime;

	unsigned int m_uiFrameCount;
	float m_fFrameStart;
	float m_fFrameEnd;

	float m_fSimTime;
    float m_fUnScaledSimTime;
	float m_fSimOffset;

    float m_fScale;
    float m_fMinFrameTime;
    float m_fLastFrameTime;
    float m_fFrameTime;
    bool m_bFrameTimeLocked;

    bool m_bConstantFrameLengthEnabled;
    float m_fConstantFrameLength;
};

#include "Clock.inl"

#endif
