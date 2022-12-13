/*
 *	IDImage.h
 *	ID image functions.
 *
 *	Jim, 2003.12.23.
 */
#ifndef _IDIMAGE_H_
#define _IDIMAGE_H_


//////////////////////////////////////////////////////////////////////////
// class CIDImage

class CIDImage
{
public:
    enum
    {
        nFileVer            =   0x00010000,     //2003.12.23, 1st version.
        nFileVer_1_1        =   0x00010001,     //2004.2.10, 2nd version
        nFileVer_1_2        =   0x00010002,     //2004.7.28, v1.2, ����ֽ����ϵͳ
        nFileVer_1_3        =   0x00010003,     //2005.1.31 �����split
        nFileVer_1_4        =   0x00010004,     //2005.2.28 �����˲���ħ��λ�õ���Ϣ
        nFileVerLast        =   nFileVer_1_4,   //final version 
    };

public:
	CIDImage();
	~CIDImage();
	
	ULONG	AddRef();
    ULONG   GetRef();
	ULONG	Release();
	int		GetTotalDirs();
	int		GetTotalFrames();
	int		GetDirFrames(int nDir);
	TBoundInfo * GetBoundInfo();
	TFrameInfo * GetFrameInfo(int nDir, int nFrame);

    //Jim, add [2/10/2004]
    //{{{
    TDirInfo * GetDirInfo(int nDir);
    //}}}
	
public:
    LONG    m_version;      //�汾��
	DWORD	m_dwImageID;	//Ӱ��ID
	DWORD	m_dwRef;		//Ӱ�����ô���
	CHAR	m_szFileName[_MAX_FNAME];	//Ӱ���ļ�����
	
	int		m_nTotalFrames;		//Ӱ����֡��
	int		m_nTotalDirs;		//Ӱ���ܷ�����
	int		m_nDirFrames;		//ÿ�����ͼ��֡��
	TFrameInfo* m_FrameInfo;	//֡��Ϣ
	TBoundInfo	m_BoundInfo;	//��Χ����Ϣ

    //Jim, add [2/10/2004]
    //{{{
    TDirInfo *  m_DirInfo;      //ÿ������Ķ�������Ϣ
    //}}}
	unsigned int m_HotTick;    //ʹ�õ�tick
	
public:
	//delta, change [8/12/2004]
	//{{{
	static  bool    IsImageExist( DWORD dwID );
	//}}}
	static	void	ImageID2FileName(DWORD dwID, CHAR *szFileName);
	static	CHAR*	GetTypeDir(DWORD dwID);
    static  CHAR*   GetTypeName(DWORD dwID, BOOL *bMainType);
	//mikechen	2004.6.1
    static  CHAR*   GetCityEnName(DWORD dwID);
    static  CHAR*   GetCityChName(DWORD dwID);
    static  DWORD   FileNameToImageID(CHAR *szFileName);

public:
	BOOL	LoadImageFile();

    //Jim, change [2/10/2004]
    //{{{
    //BOOL	SaveImageFile(); curdirֻ��magic�ļ���Ч
    BOOL    SaveImageFile(DWORD dwVersion = nFileVerLast, int curdir=-1);
    //}}}

	void	FreeMemory();
	BOOL	CreateClipRoot(int nClipWidth);
	void	SetClipRootY(int nClipIndex, int nClipBy);
    int     GetNearestClipLineIndex(int x, int y, int iDistance, bool bExt);

    //Jim, add [2/10/2004]
    //{{{
    BOOL    CreateDefaultDirInfo();
    void    ComputeBoundSize(int &left, int &top, int &right, int &bottom);
    //}}}
	
#pragma pack(push)
#pragma pack(1)
	struct TFileHeader 
	{
		ULONG   copyright;      //copyright 'QQF'+0x1a
		ULONG   id;             //file id 'DIMG'
		ULONG   version;        //file version
		ULONG   head_size;      //size of the file head
		//
		int		frames;			//Ӱ��֡��
		int		dirs;			//�ܷ�����

        //qjb, add [7/28/2004]
        //ֽ����ϵͳ
        //{{{
        BOOL bBingFlag;         // �����岿���Ƿ���Ҫ�󶨵��������岿�ֵı�־
        int nBindPointCounts;   // ӵ�а󶨵�ĸ�����Ŀǰֻ����������㣬�ҵ���3��
        BOOL bHaveMask;         // �Ƿ�ӵ�б�ɫͨ�����ݣ�Ŀǰֻ�������ͷ��ΪTRUE��
        //}}}
	};
#pragma pack(pop)

	static	void	InitFileHeader(TFileHeader *pHeader);
};


//////////////////////////////////////////////////////////////////////////

#endif//_IDIMAGE_H_

