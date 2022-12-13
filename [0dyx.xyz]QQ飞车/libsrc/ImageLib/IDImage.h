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
        nFileVer_1_2        =   0x00010002,     //2004.7.28, v1.2, 增加纸娃娃系统
        nFileVer_1_3        =   0x00010003,     //2005.1.31 添加了split
        nFileVer_1_4        =   0x00010004,     //2005.2.28 增加了播放魔法位置点信息
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
    LONG    m_version;      //版本号
	DWORD	m_dwImageID;	//影像ID
	DWORD	m_dwRef;		//影像被引用次数
	CHAR	m_szFileName[_MAX_FNAME];	//影像文件名称
	
	int		m_nTotalFrames;		//影像总帧数
	int		m_nTotalDirs;		//影像总方向数
	int		m_nDirFrames;		//每方向的图像帧数
	TFrameInfo* m_FrameInfo;	//帧信息
	TBoundInfo	m_BoundInfo;	//包围盒信息

    //Jim, add [2/10/2004]
    //{{{
    TDirInfo *  m_DirInfo;      //每个方向的动画的信息
    //}}}
	unsigned int m_HotTick;    //使用的tick
	
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
    //BOOL	SaveImageFile(); curdir只对magic文件有效
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
		int		frames;			//影像帧数
		int		dirs;			//总方向数

        //qjb, add [7/28/2004]
        //纸娃娃系统
        //{{{
        BOOL bBingFlag;         // 该身体部分是否需要绑定到其他身体部分的标志
        int nBindPointCounts;   // 拥有绑定点的个数（目前只有身体大于零，且等于3）
        BOOL bHaveMask;         // 是否拥有变色通道数据（目前只有身体和头发为TRUE）
        //}}}
	};
#pragma pack(pop)

	static	void	InitFileHeader(TFileHeader *pHeader);
};


//////////////////////////////////////////////////////////////////////////

#endif//_IDIMAGE_H_

