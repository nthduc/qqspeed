/********************************************************************
	作者:	　wincoqin
	时间:	  2007/06/13
    文件名:   FlashScreen.h
	说明:     []
*********************************************************************/
#pragma once

// libfile
#include "map"
#include "NiRenderer.h"
#include "NiScreenTexture.h"
// userfile
#include "FlashRenderer.h"
#include "singlet.h"
#include "idgen.h"
// predefine



class FlashScreen
{
protected:
	FlashScreen();
	virtual bool Initialize(NiTexturePtr spTexture);
	virtual bool Initialize(const int w, const int h);

public:
	static FlashScreen * CreateFlashScreen(NiTexturePtr spTexture);
	static FlashScreen * CreateFlashScreen(const int w, const int h);
	
	virtual ~FlashScreen();

	FlashRenderer * GetFlashRender();

	NiTexture * GetTexture();

    void UpdateFrame();

	virtual void Draw(NiRenderer * pkRenderer);

protected:
	NiTexturePtr    m_spTexture;
	FlashRenderer * m_pkFlashRender;
};

class FlashScreenTexture : public FlashScreen
{
protected:
	FlashScreenTexture();
	virtual bool Initialize(unsigned int unWidth, unsigned int unHeight);
public:
	static FlashScreenTexture * CreateFlashScreen(unsigned int unWidth, unsigned int unHeight);
	virtual ~FlashScreenTexture();
	
	void GetPosInfo(int & x, int & y, int & w, int & h);
	void SetPosition(int x, int y, int w, int h);

	virtual void Draw(NiRenderer * pkRenderer);

protected:
	unsigned int	   m_unScreenRectID;
	NiScreenTexturePtr m_spScreenTexture;
};


class FlashScreenManager : public CSingleT<FlashScreenManager>
{
	typedef std::map<DWORD, FlashScreenTexture*> FlashScreenTexMap;
	typedef FlashScreenTexMap::iterator			 FlashScreenTexIter;
public:
	FlashScreenManager(){};
	~FlashScreenManager();

	unsigned int CreateFlashScreen(const char * path, const int x, const int y, const int w, const int h);
	FlashScreenTexture * GetFlashScreen(const int unID);
	void ReleaseFlashScreen(unsigned int & unID);
	void ReleaseAllFlash();
	void UpdateFrame();

protected:
	CIDGen		m_kIDGen;
	FlashScreenTexMap m_kFlashScreenTexMap;	
};