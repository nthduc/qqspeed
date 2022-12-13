

#pragma once

#include <NiApplication.h>
//#include "LevelManager.h"
#include "InputManager.h"
#include <list>
using namespace std;

  // hover ��û������ �������
class ZKeyEvent : public NiMemObject
{
public:
	/*
		�޸��ˣ�	ray
		���ڣ�	1/29/2007
		����	���Ӽ����¼�����
	*/
	typedef enum enmKeyEventType
	{
		EKET_FUNC,
		EKET_SELELTMENU,	//С�˵�����
		EKET_ACCE,			//���ٰ���
		EKET_SHIFT,			//Ư�ư���
		EKET_DIR_NORMAL,	//�������򰴼�
		EKET_DIR_REVERSE,	//�����򰴼�
		EKET_NO2,			//С��������¼�
		EKET_JUMP,

	}ENMKEYEVENTTYPE;

public:
	ZKeyEvent(ENMKEYEVENTTYPE eKeyType);
	virtual ~ZKeyEvent();
	virtual void Trigger(){};
	virtual ENMKEYEVENTTYPE GetType();		//�õ������¼�����
	virtual bool IsActive();
	virtual void SetActive(bool state);
protected:
	NiInputSystem * m_pInputSystem;
	ENMKEYEVENTTYPE m_eKeyEventType;		//�����¼�����
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
	�޸��ˣ�	ray
	���ڣ�	1/29/2007
	����	���ӷ���������¼�
*/	
class ZKeyReverseDir: public ZKeyEvent
{
public:
	ZKeyReverseDir():ZKeyEvent(EKET_DIR_REVERSE){};
	virtual ~ZKeyReverseDir(){};
	virtual void Trigger();
};



/*
�޸��ˣ�	ray
���ڣ�	2/06/2007
����	����С����̼���¼�
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
		�޸��ˣ�	ray
		���ڣ�	1/29/2007
		����	���Ӽ����¼��ӿ�
	*/	

	//���¼����¼�
	void Update(); 

	
	//����ĳһ�����¼�����
	void ActivateKeyEvent(ZKeyEvent::ENMKEYEVENTTYPE eType);

	//����ĳһ�����¼�����
	void DeactivateKeyEvent(ZKeyEvent::ENMKEYEVENTTYPE eType);

	

	//ע������¼�
	void RegEvent( ZKeyEvent * );
	//ע�������¼�
	void UnRegEvent(ZKeyEvent::ENMKEYEVENTTYPE eType);

//----------------------------------------------------------------------------------

//	void SetEnable(bool bEnable);
//	KeyEventType m_vpCommonEvent;
//	KeyEventType m_vpSpecialEvent;

protected:
	//��list�в���
	KeyEventIter FindInList(KeyEventList &keyList, ZKeyEvent::ENMKEYEVENTTYPE eType);
	//
	void DeactivateKeyEvent(const KeyEventIter &it);

private:
	/*
		�޸��ˣ�	ray
		���ڣ�	1/29/2007
		����	�����¼�����
	*/	
	KeyEventList	m_listActiveEvent;		//��ǰ��Ծ�����¼�
	KeyEventList	m_listInactiveEvent;	//�ǻ�Ծ�����¼�


public:
	static ZKeyEventManager * m_pkZKeyEventManager;

};


