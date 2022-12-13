////////////////////////////////////////////////
//     FileName:    RenderBase.h
//     Author:      Rayhuang
//     Date:        2007-2-1
//     Description: Ϊ��Ҫ��Ⱦ�����ṩ�ӿ�
////////////////////////////////////////////////

#pragma once 
#include "singlet.h"

#include <list>
using namespace std;
//-----------------------------------------------------------
//��Ⱦ����Ľӿڣ�����Ⱦ������ԴӴ�����
//-----------------------------------------------------------
interface IRenderBase
{
	//���ö����Ծ��
	virtual void SetActivity(bool bActive) = 0;

	//�õ�����Ļ�Ծ��
	virtual bool IsActive() = 0;

	//��Ⱦ����
	virtual void Draw(NiRendererPtr spRenderer) = 0;

	//���¶���
	virtual void Update(float fTime) = 0;

};
//-------------------------------------------------------------


//--------------------------------------------------------
//2D��Ⱦ�������
//--------------------------------------------------------
class CRenderBase2D : public IRenderBase
{
public:
	CRenderBase2D();
	~CRenderBase2D();

	//���ö����Ƿ��Ծ
	virtual void SetActivity(bool bActive);

	//�õ�����Ļ�Ծ��
	virtual bool IsActive();

	//��Ⱦ����
	virtual void Draw(NiRendererPtr spRenderer) = 0;

	//���¶���
	virtual void Update(float fTime) = 0;
private:
	bool m_bActive;
};
//-------------------------------------------------------------


//-------------------------------------------------------------
//3D��Ⱦ�������
//-------------------------------------------------------------
class CRenderBase3D : public IRenderBase
{
public:
	CRenderBase3D();
	~CRenderBase3D();

	//���ö����Ƿ��Ծ
	virtual void SetActivity(bool bActive);

	//�õ�����Ļ�Ծ��
	virtual bool IsActive();

	//��Ⱦ����
	virtual void Draw(NiRendererPtr spRenderer) = 0;

	//���¶���
	virtual void Update(float fTime) = 0;
private:
	bool m_bActive;
};
//----------------------------------------------------------------


//----------------------------------------------------------------
//CRenderMgr��Ⱦ������
//���е���Ⱦ�������˴���ͳһ���£���Ⱦ
//----------------------------------------------------------------
typedef std::list<IRenderBase*>				Render_List;
typedef std::list<IRenderBase*>::iterator	Render_List_It;
//��Ⱦ���ƣ����е���Ⱦ�������˴���ͳһ���£���Ⱦ
class CRenderMgr: public CSingleT<CRenderMgr>
{
public:
	CRenderMgr();
	~CRenderMgr();

	//��Ⱦ�������
	void Render();
	void ScreenRender();

	//���¶������
	void Update(float fTime);

	//����һ2D������Ⱦ������
	void ActivateObject2D(IRenderBase* pObject);

	//����һ3D������Ⱦ������
	void ActivateObject3D(IRenderBase* pObject);

	//�Ӷ�����ɾ��һ���󣬲�δ�ͷŶ���ֻ�Ǵ��б���ɾ������ָ�롣
	//����������������ʩ�Ŷ������ͷŶ���ǰӦ�ȵ��ô˺���
	void TerminateObject2D(IRenderBase* pObject);
	
	//�Ӷ�����ɾ��һ���󣬲�δ�ͷŶ���ֻ�Ǵ��б���ɾ������ָ�롣
	//����������������ʩ�Ŷ������ͷŶ���ǰӦ�ȵ��ô˺���
	void TerminateObject3D(IRenderBase* pObject);
/*
	//���ص���ָ��
	static CRenderMgr* Get();
	//�����������
	static bool Create();
	//���ٶ���
	static void Destroy();
*/
private:
	//3D�����б�
	Render_List			m_listRenderObject3D;
	//3D���������
	Render_List_It		m_renderListIt3D;

	//2D�����б�
	Render_List			m_listRenderObject2D;
	//2D���������
	Render_List_It		m_renderListIt2D;

//	static CRenderMgr*	ms_pRenderMgr;
};