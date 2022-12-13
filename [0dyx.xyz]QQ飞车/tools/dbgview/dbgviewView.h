// dbgviewView.h : CdbgviewView ��Ľӿ�
//


#pragma once


class CdbgviewView : public CListView
{
protected: // �������л�����
	CdbgviewView();
	DECLARE_DYNCREATE(CdbgviewView)

// ����
public:
	CdbgviewDoc* GetDocument() const;

// ����
public:

	void AddMessage(LPCSTR);

// ��д
	public:
virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void OnInitialUpdate(); // ������һ�ε���

// ʵ��
public:
	virtual ~CdbgviewView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // dbgviewView.cpp �ĵ��԰汾
inline CdbgviewDoc* CdbgviewView::GetDocument() const
   { return reinterpret_cast<CdbgviewDoc*>(m_pDocument); }
#endif

