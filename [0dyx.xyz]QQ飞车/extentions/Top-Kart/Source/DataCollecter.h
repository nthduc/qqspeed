////////////////////////////////////////////////
//     FileName:    DataCollecter.h
//     Author:      Rayhuang
//     Date:        2007-4-4
//     Description: 数据收集
////////////////////////////////////////////////

#pragma once 

#include "Play.h"
#include "singlet.h"

#include <fstream>
using namespace std;

class CDataCollecter : public CSingleT<CDataCollecter>
{
public:
	CDataCollecter();
	~CDataCollecter();

	void Update(float fTime);
	void Start();
	void Continue();
	void Stop();

	void SetTarget(Play* pPlay);
	void SetKeyboard(NiInputKeyboardPtr spKeyboard);
	void SetOutPutFile(char* strFilePath);
protected:
	void Clear();
	void OutPut();


private:
	ofstream m_Fout;

	Play* m_pTarget;
	NiInputKeyboardPtr m_spKeyboard;

	bool m_bIsUpdate;
	//
	float m_fTime;
	float m_fSpeed;
	float m_fKartAngle;
	float m_fRotRadius;
	float m_fGradient;

	short m_bIsUp;
	short m_bIsLeft;
	short m_bIsRight;
	short m_bIsDown;
	short m_bIsShift;
	short m_bIsDrift;
	short m_bIsNo2;
};