////////////////////////////////////////////////
//     FileName:    RenderBase.h
//     Author:      Rayhuang
//     Date:        2007-2-1
//     Description: 为需要渲染的类提供接口
////////////////////////////////////////////////

#pragma once 
#include "singlet.h"

#include <list>
using namespace std;
//-----------------------------------------------------------
//渲染对象的接口，各渲染对象可以从此派生
//-----------------------------------------------------------
interface IRenderBase
{
	//设置对象活跃性
	virtual void SetActivity(bool bActive) = 0;

	//得到对象的活跃性
	virtual bool IsActive() = 0;

	//渲染对象
	virtual void Draw(NiRendererPtr spRenderer) = 0;

	//更新对象
	virtual void Update(float fTime) = 0;

};
//-------------------------------------------------------------


//--------------------------------------------------------
//2D渲染对象基类
//--------------------------------------------------------
class CRenderBase2D : public IRenderBase
{
public:
	CRenderBase2D();
	~CRenderBase2D();

	//设置对象是否活跃
	virtual void SetActivity(bool bActive);

	//得到对象的活跃性
	virtual bool IsActive();

	//渲染对象
	virtual void Draw(NiRendererPtr spRenderer) = 0;

	//更新对象
	virtual void Update(float fTime) = 0;
private:
	bool m_bActive;
};
//-------------------------------------------------------------


//-------------------------------------------------------------
//3D渲染对象基类
//-------------------------------------------------------------
class CRenderBase3D : public IRenderBase
{
public:
	CRenderBase3D();
	~CRenderBase3D();

	//设置对象是否活跃
	virtual void SetActivity(bool bActive);

	//得到对象的活跃性
	virtual bool IsActive();

	//渲染对象
	virtual void Draw(NiRendererPtr spRenderer) = 0;

	//更新对象
	virtual void Update(float fTime) = 0;
private:
	bool m_bActive;
};
//----------------------------------------------------------------


//----------------------------------------------------------------
//CRenderMgr渲染控制类
//所有的渲染对象加入此处，统一更新，渲染
//----------------------------------------------------------------
typedef std::list<IRenderBase*>				Render_List;
typedef std::list<IRenderBase*>::iterator	Render_List_It;
//渲染控制，所有的渲染对象加入此处，统一更新，渲染
class CRenderMgr: public CSingleT<CRenderMgr>
{
public:
	CRenderMgr();
	~CRenderMgr();

	//渲染对象队列
	void Render();
	void ScreenRender();

	//更新对象队列
	void Update(float fTime);

	//加入一2D对象到渲染队列中
	void ActivateObject2D(IRenderBase* pObject);

	//加入一3D对象到渲染队列中
	void ActivateObject3D(IRenderBase* pObject);

	//从队列中删除一对象，并未释放对象，只是从列表中删除它的指针。
	//创建对象者有责任施放对象，在释放对象前应先调用此函数
	void TerminateObject2D(IRenderBase* pObject);
	
	//从队列中删除一对象，并未释放对象，只是从列表中删除它的指针。
	//创建对象者有责任施放对象，在释放对象前应先调用此函数
	void TerminateObject3D(IRenderBase* pObject);
/*
	//返回单件指针
	static CRenderMgr* Get();
	//创建自身对象
	static bool Create();
	//销毁对象
	static void Destroy();
*/
private:
	//3D对象列表
	Render_List			m_listRenderObject3D;
	//3D对象迭代器
	Render_List_It		m_renderListIt3D;

	//2D对象列表
	Render_List			m_listRenderObject2D;
	//2D对象迭代器
	Render_List_It		m_renderListIt2D;

//	static CRenderMgr*	ms_pRenderMgr;
};