#pragma once
#include "NiMemObject.h"
#include "CheckEditor.h"


struct KartInfo
{

};

class CKartPlayBack
	: public IOnEnterPoint
{
	struct Key		
	{
		NiInputKeyboard::KeyCode enKeyCode;
		int						 iStatus;
		float					 fPressTime;
		NxMat34					 kKartPose;
		NxVec3					 kSpeed;
		NxVec3					 kAngleSpeed;
	};

	typedef std::vector<Key*> KeyRecord;


	struct CameraPlayBack
		: NiMemObject
	{
		std::vector<std::string> arrCheckPointName;
		NiCameraPtr						 spCamera;
	};

public:
	CKartPlayBack(const KartInfo & kKartInfo);
	virtual ~CKartPlayBack();

public: // Operation
	void StartRecord(float fTime);
	void OnKeyDown(const NiInputKeyboard::KeyCode keyCode, int status, 
		float fTime);//const NxMat34 & kKartPose, 
	void PlayBack(float fTime);
	void StopRecord(float fTime);
	void Update(float fTime);
	float GetPlayBackTime();
	bool  IsPlayBack();
	unsigned int GetCurKeyIndex();
    
	bool Save(const char * filename);
	bool Load(const char * filename, const char * fCameraFile);

	virtual void OnLapUp();
	virtual void OnEnterPoint(CCheckPoint * pChkPt);
	virtual void OnLeavePoint(CCheckPoint * pChkPt);

protected:
	void Clear();
	void WriteKartInfo(FILE *& fp);
	void ReadKartInfo(FILE *& fp);
	void WriteKeyRecord(FILE *& fp);
	void ReadKeyRecord(FILE *& fp);
	int  ReadKeyStroke(FILE *& fp, int & key, int & status, float & fPresstime);
	int  ReadKartPose(FILE *& fp, NxMat34 & kKartPose);
	int  ReadKartSpeed(FILE *& fp, NxVec3 & kKartSpeed, NxVec3 & kKartAngleSpeed);
	void DisableInput();

	void FindAndAddCamera(NiAVObject * pkNode);
	void RetriveCheckPoint(CameraPlayBack *& pCamPlayBack);
	NiCameraPtr GetCamera(const NiFixedString & kCheckPointName, int & nIndex);
	bool IsPlayBackCamera(NiCamera * pkCamera);

protected: // Data
	KartInfo  m_kKartInfo;
	KeyRecord m_arrKeyRedord;
	unsigned int m_uiCurPos;
	bool	  m_bStartRecord;
	bool      m_bPlayBack;
	float     m_fStartRecordTime;
	float	  m_fStartPlayBackTime;
	float     m_fCurTime;

	NiTPrimitiveArray<CameraPlayBack*> m_arrCamera;
};