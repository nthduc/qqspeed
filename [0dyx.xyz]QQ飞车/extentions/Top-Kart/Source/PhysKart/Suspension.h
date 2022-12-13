#pragma once

#include <vector>

//struct KEYINFO;
class IStimulator;
class DriftStimulator;

class Suspension
{
public:
	Suspension(CTencentCar* pCar);
	~Suspension();

	void AddStimulator(IStimulator* pStim);
	void Update(float dt, const CTencentCar::KEYINFO& keyinfo);
	
	float GetRotX();
	float GetRotY();

	float GetWheelOffset(int nIndex){return m_afWheelOffset[nIndex];}
	void UpdateWheelOffset(float dt);



private:

	void InitStims(CTencentCar* pCar);
	NiMatrix3 m_rot;
	NiPoint3 m_pos;
	std::vector<IStimulator*> m_vStimulators;
	
	float m_afWheelOffset[4];

	DriftStimulator* m_pDriftStim;



};

class IStimulator{

public:
	IStimulator(CTencentCar* pCar);
	virtual ~IStimulator();


	virtual float GetRotAngleX() = 0;
	virtual float GetRotAngleY() = 0;

	virtual void Update() = 0;
};

class DriftStimulator
{
public:
	DriftStimulator(CTencentCar* pCar);
	virtual ~DriftStimulator();

	virtual float GetRotAngleX() {return m_fRotX;}
	virtual float GetRotAngleY() {return m_fRotY;}

	virtual void Update(float dt, const CTencentCar::KEYINFO& keyinfo);


public:

private:
	void UpdateState(float dt, const CTencentCar::KEYINFO& keyinfo);
	void UpdateAngle(float dt);
	typedef enum tagDriftStage
	{
		DS_UP,
		DS_DOWN,
		DS_POST,
		DS_NONE
	}eDRIFTSTAGE;

	CTencentCar* m_pTCar;

	float m_fRotX;
	float m_fRotY;
	const float MAXROT_X;
	const float MAXROT_Y;

	bool m_bDriftPre;
	bool m_bDriftCur;

	eDRIFTSTAGE m_state;
	float m_fDirection;
	float m_fDirectionPre;

	float m_fVelUp;
	float m_fVelDown;
	float m_fVelPost;

	float m_fTargetAngleDown;

};