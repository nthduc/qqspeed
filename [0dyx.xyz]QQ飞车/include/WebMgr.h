#pragma once

class IQQTWebSink
{
public:
	virtual HRESULT OnBeforeNavigate2( 
	/* [in] */ const CHAR *szURL,
	/* [in] */ BOOL *pbCancel) = 0;

	virtual HRESULT OnNavigateComplete2( 
	/* [in] */ const CHAR *szURL) = 0;

};

class IWebMgr
{
public:
    virtual void ShowWeb(char *szName, RECT rc, char *szURL, IQQTWebSink *pSink) = 0;

	virtual void HideWeb(char *szName) = 0;

	virtual void GetWebURL(char *szName, char *szURL) = 0;

	virtual void GoForward(char *szName) = 0;

	virtual BOOL IsMouseEnter(POINT pnt) = 0;

	virtual void GoBack(char *szName) = 0;

	virtual void DestroyWeb(char *szName) = 0;

	virtual void DestroyAllWebs() = 0;

	virtual void Release() = 0;
};

#endif