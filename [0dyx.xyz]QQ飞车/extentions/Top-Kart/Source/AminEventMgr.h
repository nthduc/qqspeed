/*
	author: rayhuang
	date:	2007/1/18
	desc:	�������ƣ����ȼ��߼�
*/



#ifndef	ANIMEVENTMGR_H
#define ANIMEVENTMGR_H

#include <queue>
#include "NiActorManager.h"
#include <map>
#include <string>
using namespace std;

#define MAKE_EVENT_ID(nNum,nFlag)  ( (nNum << EAET_MASK) | (nFlag) )

typedef map<string, int>	StringToIntMap;

class CAnimEventMgr : public NiMemObject
{
public:
	typedef enum enmAminEventType			//��������
	{
		//���ȼ�
		EAET_PRIORITY_1			= 1,
		EAET_PRIORITY_2			= 2,
		EAET_PRIORITY_3			= 4,
		EAET_PRIORITY_4			= 8,
		
		//��ɫ�����¼��򳵶����¼�
		EAET_CHARACTER			= 16,
		EAET_KART				= 32,

		//���͸��ǵ�λ��
		EAET_MASK				= 6,

		//���ȼ����ǵ�λ����ȫ����Ϊ1���������ȼ��жϱȽ�
		EAET_PRIORITY_COVER		= 15,
		
		//�����¼�����
		EAET_IDLE				= MAKE_EVENT_ID(1, EAET_PRIORITY_4|EAET_CHARACTER|EAET_KART),
		EAET_TURNLEFT			= MAKE_EVENT_ID(2, EAET_PRIORITY_4|EAET_CHARACTER),
		EAET_TURNRIGHT			= MAKE_EVENT_ID(3, EAET_PRIORITY_4|EAET_CHARACTER),
		EAET_TURNBACK			= MAKE_EVENT_ID(4, EAET_PRIORITY_4|EAET_CHARACTER),
		EAET_SPPEDUP			= MAKE_EVENT_ID(5, EAET_PRIORITY_2|EAET_CHARACTER|EAET_KART),
		EAET_HIT				= MAKE_EVENT_ID(6, EAET_PRIORITY_3|EAET_CHARACTER),
		EAET_FINISH				= MAKE_EVENT_ID(7, EAET_PRIORITY_1|EAET_CHARACTER),
		EAET_UNFINISH			= MAKE_EVENT_ID(8, EAET_PRIORITY_1|EAET_CHARACTER),
		EAET_WIN				= MAKE_EVENT_ID(9, EAET_PRIORITY_1|EAET_CHARACTER),	
		
		EAET_BANANA				= MAKE_EVENT_ID(10, EAET_PRIORITY_1|EAET_CHARACTER|EAET_KART),
		EAET_SEELEFT			= MAKE_EVENT_ID(11, EAET_PRIORITY_3|EAET_CHARACTER),
		EAET_SEERIGHT			= MAKE_EVENT_ID(12, EAET_PRIORITY_3|EAET_CHARACTER),
		EAET_CRY				= MAKE_EVENT_ID(13, EAET_PRIORITY_3|EAET_CHARACTER),
		EAET_USEITEM			= MAKE_EVENT_ID(14, EAET_PRIORITY_3|EAET_CHARACTER),
		EAET_STILL				= MAKE_EVENT_ID(15, EAET_PRIORITY_4|EAET_CHARACTER|EAET_KART),	
		EAET_TURNLEFTBIG		= MAKE_EVENT_ID(16, EAET_PRIORITY_4|EAET_CHARACTER),
		EAET_TURNRIGHTBIG		= MAKE_EVENT_ID(17, EAET_PRIORITY_4|EAET_CHARACTER),

	} ENMANIMEVENTTYPE;

public:
	CAnimEventMgr(Play* pPlay);
	~CAnimEventMgr();
	void PushAnimEvent(ENMANIMEVENTTYPE enAnimEvent);	//puah �����¼�
	ENMANIMEVENTTYPE GetCurAnimEvent();					//��õ�ǰ���¼�
	void ExecuteEvent();								//ִ���¼�����

private:

	std::queue< ENMANIMEVENTTYPE >	m_queueAnimEvent;

	ENMANIMEVENTTYPE				m_enmCurEvnet;		//��ǰ�¼�
	ENMANIMEVENTTYPE				m_enmNextEvent;		//���е���һ�¼�

	//NiActorManagerPtr				m_spCharacter;		//��ɫ
	//NiActorManagerPtr				m_spKart;			//��
	Play*							m_pPlayer;			//���

	StringToIntMap					m_mapCharacterAnim;	//��ɫ����ͼ
	StringToIntMap					m_mapKartAnim;		//������ͼ

};

#endif