////////////////////////////////////////////////
//     FileName:    PowerController.h
//     Author:      Rayhuang
//     Date:        2007-3-21
//     Description: ���������ƣ������������������
////////////////////////////////////////////////


#ifndef POWERCONTROLLER_H
#define POWERCONTROLLER_H

#include "RenderBase.h"
#include "singlet.h"

class CBarItem;
class CBlinkItem;


//���˾�������������
class CSinglePower : public CRenderBase2D, public NiMemObject
{
public:
	CSinglePower();

	~CSinglePower();

	//���£��˴�������۵������߼�
	virtual void Update(float fTime);

	//��Ⱦ
	virtual void Draw(NiRendererPtr spRenderer);

	//���û��
	virtual void SetActivity(bool bActive);

	//���۵���ʧ�ܣ�����ײʱҪ���ô˽ӿڣ�
	void AccumulateFail();

	//���۵����ɹ�
	void AccumulateSucs();

	//���۵�����ʵʱ���֣�����һ���ɹ���
	void AccumulatePower(unsigned int unPower);

	//���۵�����ʵʱ���֣�����һ���ɹ���
	void SetCurPower(unsigned int unPower);

	//���õ���
	void Reset();

	//���ó������۵������Ĳ���
	void SetKartCoef(float fCoef);

	//����ű�
	void LoadConfig();

private:
	CBarItem	* m_pBarItem;		//������
	CBlinkItem	* m_pBlinkItem;		//��˸����

	unsigned int m_unCurPower;			//��ǰ����
	unsigned int m_unPrePower;			//�ϴε�������failʱ����ǰ����Ӧ��Ϊ��ֵ��
	unsigned int m_unMaxPower;			//�����
	unsigned int m_unCompensatePower;	//��Ư���ٶ�����ʱ�����Ĳ���ֵ

	unsigned int m_unAnimPower;		//��������ֵ��ֻΪ�˱��ֵ����ݼ���Ч����
	bool m_bIsAnim;					//�Ƿ񲥷Ŷ���

	float m_fCurSpeed;				//��ǰ�ٶȣ������۵����ã�
	float m_fPreSpeed;				//��һ֡�ٶ�

	float m_fKartCoef;			//�����������ÿ�������������䣨�ɱ�ģ������۵�������


	bool m_bIsCrash;			//TODO:�Ƿ���ײ��������ײ����Ư�Ƶ��жϣ�������ӿ����ƺ�ȥ����
};


//��ӵ���
class CTeamPower : public CRenderBase2D, public NiMemObject
{
public:
	CTeamPower();
	~CTeamPower();

	//���£��˴�������۵������߼�
	virtual void Update(float fTime);

	//��Ⱦ
	virtual void Draw(NiRendererPtr spRenderer);

	//���û��
	virtual void SetActivity(bool bActive);

	//���۵���
	void AccumulatePower(unsigned int unPower);

	//���õ�ǰ����ֵ
	void SetPower(unsigned int unPower);

	//����
	void Reset();

	//����ű�
	void LoadConfig();

private:

	CBarItem * m_pBarItem;			//������
	CBlinkItem * m_pBlinkItem;		//��˸����

	unsigned int m_unCurPower;		//��ǰ����ֵ
	unsigned int m_unMaxPower;		//�����ֵ

	unsigned int m_unAnimPower;		//��������ֵ��ֻΪ�˱��ֵ����ݼ���Ч����
	bool m_bIsAnim;					//�Ƿ񲥷Ŷ���
};



//���������ࣨ�������Ƶ��˾��ٺ�������ĵ�����
class CPowerController : public CSingleT<CPowerController>
{
public:
	CPowerController();

	~CPowerController();

	//���ص��˾��ٵ�������
	CSinglePower* GetSinglePower();

	//������ӵ�������
	CTeamPower*	GetTeamPower();

	//��ʼ���˾��ٵ����������Ծ��Ϊtrue��
	void StartSgPower();

	//�������˾��ٵ����������Ծ��Ϊfalse��
	void TerminateSgPower();

	//��ʼ��Ӿ��ٵ����������Ծ��Ϊtrue��
	void StartTeamPower();

	//��ʼ��Ӿ��ٵ����������Ծ��Ϊfalse��
	void TerminateTeamPower();

	//�������еĵ���
	void StopAllPowerCtrl();

private:
	CSinglePower * m_pSinglePower;	//���˾��ٵ�������

	CTeamPower * m_pTeamPower;		//��ӵ�������
};

#endif