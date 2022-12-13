

#pragma once

#include <NiApplication.h>
//#include "LevelManager.h"
#include "InputManager.h"
#include <list>
using namespace std;

  // hover 还没加完呢 明天继续
class ZKeyEvent : public NiMemObject
{
public:
	/*
		修改人：	ray
		日期：	1/29/2007
		描叙：	增加键盘事件类型
	*/
	typedef enum enmKeyEventType
	{
		EKET_FUNC,
		EKET_SELELTMENU,	//小菜单按键
		EKET_ACCE,			//加速按键
		EKET_SHIFT,			//漂移按键
		EKET_DIR_NORMAL,	//正常方向按键
		EKET_DIR_REVERSE,	//反方向按键
		EKET_NO2,			//小喷检测键盘事件
		EKET_JUMP,

	}ENMKEYEVENTTYPE;

public:
	ZKeyEvent(ENMKEYEVENTTYPE eKeyType);
	virtual ~ZKeyEvent();
	virtual void Trigger(){};
	virtual ENMKEYEVENTTYPE GetType();		//得到键盘事件类型
	virtual bool IsActive();
	virtual void SetActive(bool state);
protected:
	NiInputSystem * m_pInputSystem;
	ENMKEYEVENTTYPE m_eKeyEventType;		//键盘事件类型
	bool			m_bIsActive;

};

class ZKeyFuntionKey :public ZKeyEvent
{
public:
	ZKeyFuntionKey():ZKeyEvent(EKET_FUNC){}; 
	 virtual ~ZKeyFuntionKey(){};
	virtual void Trigger();
};

class ZKeySelectMenu : public ZKeyEvent
{
public:
	ZKeySelectMenu ():ZKeyEvent(EKET_SELELTMENU){};
	virtual ~ZKeySelectMenu(){};
	virtual void Trigger();
};

class ZKeyAcceSpeed : public ZKeyEvent
{
public:
	ZKeyAcceSpeed():ZKeyEvent(EKET_ACCE){};
	virtual ~ZKeyAcceSpeed(){};
	virtual void Trigger();
};

class ZKeyShift: public ZKeyEvent
{
public:
	ZKeyShift():ZKeyEvent(EKET_SHIFT){};
	virtual ~ZKeyShift(){};
	virtual void Trigger();
};

class ZKeyDirection: public ZKeyEvent
{
public:
	ZKeyDirection():ZKeyEvent(EKET_DIR_NORMAL){};
	virtual ~ZKeyDirection(){};
	virtual void Trigger();
};

class ZKeyJump: public ZKeyEvent
{
public:
	ZKeyJump():ZKeyEvent(EKET_JUMP) {};
	virtual ~ZKeyJump(){};
	virtual void Trigger();
};

/*
	修改人：	ray
	日期：	1/29/2007
	描叙：	增加反方向键盘事件
*/	
class ZKeyReverseDir: public ZKeyEvent
{
public:
	ZKeyReverseDir():ZKeyEvent(EKET_DIR_REVERSE){};
	virtual ~ZKeyReverseDir(){};
	virtual void Trigger();
};



/*
修改人：	ray
日期：	2/06/2007
描叙：	增加小喷键盘检测事件
*/

class ZKeyNo2: public ZKeyEvent
{
public:
	ZKeyNo2():ZKeyEvent(EKET_NO2){};
	virtual ~ZKeyNo2(){}
	virtual void Trigger();
};


#include <vector>	

typedef std::list<ZKeyEvent *>				KeyEventList;
typedef std::list<ZKeyEvent *>::iterator	KeyEventIter;

class ZKeyEventManager : public NiMemObject
{	
//	typedef std::vector<ZKeyEvent*>				 KeyEventType;
//	typedef std::vector<ZKeyEvent*>::iterator	 KeyEventIter;
	
public: 
	ZKeyEventManager();
	 virtual ~ZKeyEventManager();

	 static ZKeyEventManager* Get(){ return m_pkZKeyEventManager; }
	static ZKeyEventManager * Create();
	static void Destory();

	bool InitEvents();
	void UnInitEvents();

//	void CheckCommonEvents();
//----------------------------------------------------------------------------------
	/*
		修改人：	ray
		日期：	1/29/2007
		描叙：	增加键盘事件接口
	*/	

	//更新键盘事件
	void Update(); 

	
	//激活某一键盘事件处理
	void ActivateKeyEvent(ZKeyEvent::ENMKEYEVENTTYPE eType);

	//撤销某一键盘事件处理
	void DeactivateKeyEvent(ZKeyEvent::ENMKEYEVENTTYPE eType);

	

	//注册键盘事件
	void RegEvent( ZKeyEvent * );
	//注销键盘事件
	void UnRegEvent(ZKeyEvent::ENMKEYEVENTTYPE eType);

//----------------------------------------------------------------------------------

//	void SetEnable(bool bEnable);
//	KeyEventType m_vpCommonEvent;
//	KeyEventType m_vpSpecialEvent;

protected:
	//在list中查找
	KeyEventIter FindInList(KeyEventList &keyList, ZKeyEvent::ENMKEYEVENTTYPE eType);
	//
	void DeactivateKeyEvent(const KeyEventIter &it);

private:
	/*
		修改人：	ray
		日期：	1/29/2007
		描叙：	键盘事件队列
	*/	
	KeyEventList	m_listActiveEvent;		//当前活跃键盘事件
	KeyEventList	m_listInactiveEvent;	//非活跃键盘事件


public:
	static ZKeyEventManager * m_pkZKeyEventManager;

};


