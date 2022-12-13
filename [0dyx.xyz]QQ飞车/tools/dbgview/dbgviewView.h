// dbgviewView.h : CdbgviewView 类的接口
//


#pragma once


class CdbgviewView : public CListView
{
protected: // 仅从序列化创建
	CdbgviewView();
	DECLARE_DYNCREATE(CdbgviewView)

// 属性
public:
	CdbgviewDoc* GetDocument() const;

// 操作
public:

	void AddMessage(LPCSTR);

// 重写
	public:
virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void OnInitialUpdate(); // 构造后第一次调用

// 实现
public:
	virtual ~CdbgviewView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // dbgviewView.cpp 的调试版本
inline CdbgviewDoc* CdbgviewView::GetDocument() const
   { return reinterpret_cast<CdbgviewDoc*>(m_pDocument); }
#endif

