#pragma once
#include "NiTexture.h"
#ifdef Notify
#undef Notify
#endif
#include "atlbase.h"
#include "atlwin.h"
interface IShockwaveFlash;

class FlashRenderer
{
public:
	FlashRenderer();
	~FlashRenderer();

	bool Initialize(int iWidth, int iHeight);
	bool LoadMovie(const char* pcFilename);
	bool UpdateFrame();
	unsigned char* GetBuffer() const
	{
		return m_pcBuffer;
	}
    
	NiTexturePtr GetSourceTexture(){return m_spTexture;}
	void SetSourceTexture(NiTexturePtr spTexture){m_spTexture=spTexture;}

	void Play();	
	void Rewind();
	void Pause();
	void Stop();
	void Back();
	void Forward();
	void StopPlay();
	void GotoFrame(const unsigned int unFrame);
	unsigned int GetFrameNum();
	bool IsPlaying();
	void SetLoop(bool bLoop);
	bool GetLoop();
	


protected:	
	CAxWindow m_kWindow;
	IUnknown* m_pkUnknown;
	IShockwaveFlash* m_pkShockwareFlash;
	IViewObjectEx* m_pkViewObject;
	long m_lLastFrame;


	unsigned char* m_pcBuffer;	
	RECTL m_kRectl;	

	NiTexturePtr m_spTexture;
};