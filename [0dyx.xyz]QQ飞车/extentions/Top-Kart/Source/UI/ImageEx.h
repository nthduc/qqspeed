////////////////////////////////////////////////
//     FileName:    ImageEx.h
//     Author:      Fish
//     Date:        2006-10-11 15:48:05
//     Description: []
////////////////////////////////////////////////

#pragma once
#include "Display.h"
#include "ImageLib.h"
#include "vector"
using namespace std;

typedef vector<HIMG>	ImgHandleContainer;
class CImageEx //: 
//	public CComObjectRootEx<CComSingleThreadModel>,
//	public CComCoClass<CImageEx>,
//	public IUnknown
{
public:
	CImageEx();
	~CImageEx();

	void Init(IDisplay*);
	BOOL Load(LPCSTR);
	void Draw(int frame, int x, int y);

//	DECLARE_PROTECT_FINAL_CONSTRUCT()
//	BEGIN_COM_MAP(CImageEx)
//		COM_INTERFACE_ENTRY(IUnknown)
//	END_COM_MAP()

public:
	int   m_nBlockX, m_nBlockY;
	int   m_Width, m_Height;
	int   m_AniWidth, m_AniHeight;
	int   m_nFrame;
	std::string m_FilePath;

private:
	IDisplay*	m_pDisplay;
	TImageAni*	m_pImageAni;
	ImgHandleContainer m_ImgHandles;
};


