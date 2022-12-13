#pragma once

class CGUIWin32Platform : public CGUIPlatform
{
protected:
	static WNDPROC		mpOldProc;
	static bool			mIsLButtonDown;
	static bool			mIsLButtonUp;
	static bool			mIsLButtonDblClk;
	static bool			mIsRButtonDown;
	static bool			mIsRButtonUp;
	static bool			mIsMButtonDown;
	static bool			mIsMButtonUp;
	static bool			mIsMouseMove;
	static bool			mIsMouseWheel;
	static bool			mIsKeyDown;
	static bool			mIsKeyUp;
	static bool			mIsUniChar;
	static bool			mIsSysKeyDown;
	static bool			mIsSysKeyUp;
	static bool			mIsDeadChar;
	static bool			mIsSysDeadChar;
	static int			mCharIndex;

	static CGUIPoint	mMouseWheelPoint;
	static CGUIPoint	mLButtonDownPoint;
	static CGUIPoint	mLButtonUpPoint;
	static CGUIPoint	mLButtonDblClkPoint;
	static CGUIPoint	mRButtonDownPoint;
	static CGUIPoint	mRButtonUpPoint;
	static CGUIPoint	mMButtonDownPoint;
	static CGUIPoint	mMButtonUpPoint;
	static CGUIPoint	mMouseMovePoint;

	static unsigned int mLButtonDownFlag;
	static unsigned int mLButtonUpFlag;
	static unsigned int mLButtonDblClkFlag;
	static unsigned int mRButtonDownFlag;
	static unsigned int mRButtonUpFlag;
	static unsigned int mMButtonDownFlag;
	static unsigned int mMButtonUpFlag;
	static unsigned int mMouseMoveFlag;
	static unsigned int mMouseWheelFlag;

	static unsigned int mWParamKeyDown;
	static unsigned int mWParamKeyUp;
	static unsigned int mWParamSysKeyDown;
	static unsigned int mWParamSysKeyUp;
	static unsigned int mWParamUniChar;
	static unsigned int mWParamDeadChar;
	static unsigned int mWParamSysDeadChar;
	static std::vector< unsigned int > mWParamChars;

	static unsigned int mLParamKeyDown;
	static unsigned int mLParamKeyUp;
	static unsigned int mLParamSysKeyDown;
	static unsigned int mLParamSysKeyUp;
	static unsigned int mLParamUniChar;
	static unsigned int mLParamDeadChar;
	static unsigned int mLParamSysDeadChar;
	static std::vector< unsigned int > mLParamChars;
public:
	CGUIWin32Platform( HWND vWnd );
public:
	~CGUIWin32Platform( )	{	}

public:
	static LRESULT CALLBACK WindowProc( HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

public:
	virtual void MbToWc( const char* pText, int tTextCount, wchar_t* pUnicode, int tUniCount );
	virtual bool IsLButtonDown( CGUIPoint& rPoint, unsigned int& rFlag );		//! 得到鼠标坐标，设备坐标
	virtual bool IsLButtonUp( CGUIPoint& rPoint, unsigned int& rFlag );		
	virtual bool IsLButtonDblClk( CGUIPoint& rPoint, unsigned int& rFlag );
	virtual bool IsRButtonDown( CGUIPoint& rPoint, unsigned int& rFlag );
	virtual bool IsRButtonUp( CGUIPoint& rPoint, unsigned int& rFlag );
	virtual bool IsMButtonDown( CGUIPoint& rPoint, unsigned int& rFlag );
	virtual bool IsMButtonUp( CGUIPoint& rPoint, unsigned int& rFlag );
	virtual bool IsMouseMove( CGUIPoint& rPoint, unsigned int& rFlag );
	virtual bool IsMouseWheel( CGUIPoint& rPoint, unsigned int& rFlag );
	virtual bool IsKeyDown( unsigned int& rVirtualKey, unsigned int& rFlag );
	virtual bool IsKeyUp( unsigned int& rVirtualKey, unsigned int& rFlag );
	virtual bool IsChar( unsigned int& rCode, unsigned int& rFlag );
	virtual bool IsUniChar( unsigned int& rCode, unsigned int& rFlag );
	virtual bool IsSysKeyDown( unsigned int& rVirtualKey, unsigned int& rFlag );
	virtual bool IsSysKeyUp( unsigned int& rVirtualKey, unsigned int& rFlag );
	virtual bool IsDeadChar( unsigned int& rCode, unsigned int& rFlag );
	virtual bool IsSysDeadChar( unsigned int& rCode, unsigned int& rFlag );
	virtual int ClipboardCopy( const char* pText, int tLength );
	virtual int ClipboardPaste( char* pText, int& rSize );
	virtual int GetKeyState( int vVirtualKey );
	virtual int GetTick( );
};