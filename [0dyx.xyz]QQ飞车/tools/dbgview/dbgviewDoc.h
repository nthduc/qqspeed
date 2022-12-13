// dbgviewDoc.h :  CdbgviewDoc 类的接口
//


#pragma once

class CdbgviewDoc : public CDocument
{
protected: // 仅从序列化创建
	CdbgviewDoc();
	DECLARE_DYNCREATE(CdbgviewDoc)

// 属性
public:
	PROCESS_INFORMATION	m_kProcessInfo;
	HANDLE	m_hThread;
// 操作
public:

// 重写
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// 实现
public:
	void OnMessage(LPCSTR);
	
	virtual ~CdbgviewDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	afx_msg void OnTerminateCapture();
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
};


