#pragma once
#pragma warning(disable: 4530)

#include "NiMemObject.h"
#include "NiSmartPointer.h"
#include "NiTArray.h"
#include "NiColor.h"
#include "NiMenu/NiScreenText.h"
#include "NiScreenTexture.h"
#include "vector"

typedef std::vector<NiScreenText*> NiScreenTextPool;

class ScreenTextManager :
	public NiMemObject
{
public:
	ScreenTextManager();
	virtual ~ScreenTextManager(void);

public:
	/************************************************************************/
	/* unTextPortID[in/out]，如果unTextPortID超过了最大的capacity，那么Manager
	/* 添加新的NiScreenText,重新修整unTextPortID的值，并以参数的形式返回    */
	/************************************************************************/
	unsigned int SetText(const char * lpText, unsigned int unTextPortID, NiColorA & textColor = NiColorA(1.f, 1.f, 1.f, 1.f));	
	void SetTextEx(const char * lpText, unsigned int & unTexPortID, NiColorA & textColor = NiColorA(1.f, 1.f, 1.f, 1.f), int nPosX = 32, int nPosY = 32);	
	void SetMaxCharacter(unsigned int unMaxCharacter);
	void SetVisible(bool bVisible);
	void SetManagerBasePos(int x, int y);
	UINT GetManagerWidth();
	UINT GetManagerHeight();
	void Draw();
	//const NiScreenTextureArray * GetNiScreenTexture() const;

protected:
	void Free();		

	INT						m_nBasePosX;
	INT						m_nBasePosY;

	UINT					m_unLines;
	UINT					m_unWidth;
	UINT					m_unMaxCharacter;
	NiScreenTextPool		m_kNiScreenTextPool;	
	NiScreenTextureArray  * m_pkScreenTextures;	    		
};


