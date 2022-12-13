// dbgviewDoc.h :  CdbgviewDoc ��Ľӿ�
//


#pragma once

class CdbgviewDoc : public CDocument
{
protected: // �������л�����
	CdbgviewDoc();
	DECLARE_DYNCREATE(CdbgviewDoc)

// ����
public:
	PROCESS_INFORMATION	m_kProcessInfo;
	HANDLE	m_hThread;
// ����
public:

// ��д
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// ʵ��
public:
	void OnMessage(LPCSTR);
	
	virtual ~CdbgviewDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	afx_msg void OnTerminateCapture();
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
};


