/*
 *	ImageLib.h
 *	Image library & manager functions header.
 *
 *	Jim, 2003.12.12.
 */
#ifndef _IMAGELIB_H_
#define _IMAGELIB_H_
#include "vfilewrap.h"

//////////////////////////////////////////////////////////////////////////
// 

//通过影像的类型和索引,获得影像的ID
//-mikechen 2003.4.28
//#define MAKE_IMAGE_ID(type,idx)		((DWORD)(((DWORD)((type)&0xff)<<16)|(DWORD)((idx)&0xffff)))

//从影像ID获得影像类型
#define GET_IMAGE_TYPE(id)		(((id)>>16)&0xff)

//从影像ID获得世界类型(对于NPC类别,为对应动作索引)
#define GET_IMAGE_SET(id)       (((id)>>24)&0xff)

//从影像ID获得影像索引
//-mikechen 2003.4.28
//#define GET_IMAGE_INDEX(id)		((id)&0xffff)

#define	INVALID_IMAGE_ID		0x00000000
#define INVALID_IMAGE_TYPE		0
#define INVALID_IMAGE_INDEX		0

//qujianbiao 2004.9.1
const int MAX_BINDDOTCOUNT      =   4;

//mikechen 2004.6.1
//物件影像类型
enum	LibImageTypes
{
	libImageTypeInvalid			=	0,		//非法类型(保留)
//	libImageTypeAni				=	1,		//地图动画
//	libImageTypeSculp			=	2,		//雕塑
	
	libImageTypeMapBegin		=	1,		//map begin
	libImageTypeCC				=	1,		//草丛
	libImageTypeCCP				=	2,		//草丛瓦片
	libImageTypeGM				=	3,		//灌木
	libImageTypeGMP				=	4,		//灌木瓦片
	libImageTypeHP				=	5,		//花盆		
	libImageTypeHPP				=	6,		//花盆瓦片
	libImageTypeHT				=	7,		//花坛
	libImageTypeHTP				=	8,		//花坛瓦片
	libImageTypeSM				=	9,		//树木
	libImageTypeSMP				=	10,		//树木瓦片
	libImageTypeLD				=	11,		//路灯
	libImageTypeLDP				=	12,		//路灯瓦片
	libImageTypeZZ				=	13,		//柱子
	libImageTypeZZP				=	14,		//柱子瓦片
	libImageTypeM				=	15,		//门
	libImageTypeMP				=	16,		//门瓦片
	libImageTypeDT				=	17,		//地摊
	libImageTypeDTP				=	18,		//地摊瓦片
	libImageTypeDX				=	19,		//雕像
	libImageTypeDXP				=	20,		//雕像瓦片
	libImageTypeTT				=	21,		//图腾
	libImageTypeTTP				=	22,		//图腾瓦片
	libImageTypeSK				=	23,		//石块
	libImageTypeSKP				=	24,		//石块瓦片
	libImageTypeZW				=	25,		//杂物
	libImageTypeZWP				=	26,		//杂物瓦片
	libImageTypeDD				=	27,		//地洞
	libImageTypeDDP				=	28,		//地洞瓦片
	libImageTypeHH				=	29,		//花
	libImageTypeHHP				=	30,		//花瓦片
	libImageTypeGT				=	31,		//骨头
	libImageTypeGTP				=	32,		//骨头瓦片
	libImageTypeMK				=	33,		//木块
	libImageTypeMKP				=	34,		//木块瓦片
	libImageTypeZY				=	35,		//桌椅
	libImageTypeZYP				=	36,		//桌椅瓦片
	libImageTypeSC				=	37,		//水池
	libImageTypeSCP				=	38,		//水池瓦片
	libImageTypeSW				=	39,		//生物
	libImageTypeSWP				=	40,		//生物瓦片
	libImageTypeLG				=	41,		//栏杆
	libImageTypeLGP				=	42,		//栏杆瓦片
	libImageTypeXJZ				=	43,		//小建筑
	libImageTypeXQQ				=	44,		//小桥
	libImageTypeJZ				=	45,		//建筑
	libImageTypeYB				=	46,		//岩壁
	libImageTypeYD				=	47,		//云朵
	libImageTypeDH				=	48,		//动画
    libImageTypeYBCC			=	49,		//岩壁草丛
    libImageTypeNT  			=	50,		//泥土地
    libImageTypeQQ  			=	51,		//桥
    libImageTypeJZP  			=	52,		//桥瓦片
    libImageTypeQQP  			=	53,		//桥瓦片
	libImageTypeMapEnd			=	53,		//map end
	
	libImageTypeOther			=	99,		//其它
	
	libImageTypeNpc				=	200,	//NPC物件
	libImageTypeMagic			=	201,	//法术效果组件
	libImageTypeItem			=	202,	//道具物件

    //Jim, add [2/10/2004]
    //{{{
    libImageTypeNpcHair         =   203,    //NPC头发
    //}}}

    //Jim, add [4/7/2004]
    //{{{
    libImageTypeItemPic         =   204,    //道具图片物件
    //}}}

	//Mikechen + [6/17/2004]
    libImageTypeNpcFx			=   205,    //NPC特效
    //Qjb, add [04/25/2005]
    libImageTypeNpcTop          =   206,    //骑宠的最顶层

    //Qjb, add [07/28/2004]
    //{{{
    libImageTypeBodyBegin       =   210,
    libImageTypeCap             =   210,    //帽子
    libImageTypeFace            =   211,    //面饰
    libImageTypeHead            =   212,    //头发（包含头发和脸部）
    libImageTypeBackpack        =   213,    //背部（包含披风，翅膀，背包等）
    libImageTypeWeaponL         =   214,    //左手武器
    libImageTypeWeaponR         =   215,    //右手武器
    libImageTypeBodyEnd         =   215,
    //}}}
    
};

//Qjb, add [07/28/2004]
//绑定点的序号
//{{{
enum LibImageBindTypes
{
    LibImageBindTypeHead        =   0,      //头的绑定点的序号，在TFrameInfo::bindPointList[]中
    LibImageBindTypeBackpack    =   1,      //背包的绑定点的序号，在TFrameInfo::bindPointList[]中
    LibImageBindTypeWeapon      =   2,      //武器的绑定点的序号，在TFrameInfo::bindPointList[]中
    LibImageBindTypeWeaponL     =   2,      //左武器的绑定点的序号，在TFrameInfo::bindPointList[]中
    LibImageBindTypeWeaponR     =   3,      //右武器的绑定点的序号，在TFrameInfo::bindPointList[]中
};
//}}}

//Qjb, add [07/28/2004]
//绑定点的类型序号
//{{{
enum LibImageBindIndex
{
    libImageBindBody            =   0,    //身体
    libImageBindCap             =   1,    //帽子
    libImageBindFace            =   2,    //面饰
    libImageBindHead            =   3,    //头发（包含头发和脸部）
    libImageBindBackpack        =   4,    //背部（包含披风，翅膀，背包等）
    libImageBindWeaponL         =   5,    //左手武器
    libImageBindWeaponR         =   6,    //右手武器
    libImageBindPet             =   7,  //骑宠
    libImageBindPetTop          =   8,  //骑宠的顶层
};
//}}}

//Qjb, add [07/28/2004]
//绑定点的动作序号
//{{{
enum LibImageActionType
{
    libImageActionStand         =   1,      //站立
    libImageActionRun           =   2,      //跑
    libImageActionSit           =   3,      //坐
    libImageActionDie           =   4,      //死亡
    libImageActionAtt1          =   5,      //物理攻击动作1
    libImageActionAtt2          =   6,      //物理攻击动作2
    libImageActionAtt3          =   7,      //物理攻击动作3
    libImageActionMagicAtt      =   8,      //魔法攻击
    libImageActionPrepare1      =   9,      //准备动作1
    libImageActionPrepare2      =   10,      //准备动作2
    libImageActionPrepare3      =   11,      //准备动作3
    libImageActionBeHurt        =   12,      //被攻击
    libImageActionRide          =   13,     //骑宠坐动作
};

const char C_ACTION_NAME [][20] =
{
    " ",
    "stand",
    "run",
    "sit",
    "die",
    "attack1",
    "attack2",
    "attack3",
    "magic",
    "prepare1",
    "prepare2",
    "prepare3",
    "hurt",
    "ride",
};
//}}}


enum LibImageCityTypes{
	libImageCityInvalid		=	0,

	libImageCityBegin		=	1,
	libImageCityFY			=	1,	
	libImageCityXL			=	2,
	libImageCityTD			=	3,
	libImageCityTY			=	4,
	libImageCityCL			=	5,
	libImageCityDFYW		=	6,
	libImageCitySW			=	7,
	libImageCityYL			=	8,
	libImageCityPL			=	9,
	libImageCityTT			=	10,
	libImageCitySJ			=	11,
	libImageCityHJ			=	12,
	libImageCitySMYW		=	13,
	libImageCityHS			=	14,
	libImageCityYM			=	15,
	libImageCityGC			=	16,
	libImageCityMY			=	17,
	libImageCityTH			=	18,
	libImageCityXFYW		=	19,
    libImageCityXTTD		=	20,
    libImageCityYLD		    =	21,
    libImageCityPLSJ		=	22,
    libImageCitySWS    		=	23,
    libImageCityDXWG 		=	24,
    libImageCityCLMG 		=	25,
    libImageCityTYMG 		=	26,
	libImageCityEnd			=	26,
    
};

//+ mikechen 2004.4.8
//通过影像的类型和索引,获得影像的ID
//npc的格式变为：  动作号 + type + npc的ID
inline DWORD MAKE_IMAGE_ID( DWORD type, DWORD idx){
	if (  libImageTypeNpc== type || libImageTypeNpcHair==type ||libImageTypeNpcFx==type
        || libImageTypeNpcTop == type
        || libImageTypeCap== type || libImageTypeFace==type ||libImageTypeHead==type 
        || libImageTypeBackpack== type
        || libImageTypeWeaponL==type || libImageTypeWeaponR==type ){
		return ((idx%100)<<24) | ((type&0xff)<<16) | ((idx/100)&0xffff);
	}else
    {
        if ( type == libImageTypeMagic )
        {
            // 当前方向(31-24) 类型(23-16) 索引(15-0)
            return ((DWORD)(((DWORD)((type)&0xff)<<16)|(DWORD)((idx)&0xffff)|(DWORD)((idx<<8)&0xff000000)));
        }
        return ((DWORD)(((DWORD)((type)&0xff)<<16)|(DWORD)((idx)&0xffff)));
    }
}
//+ mikechen 2004.6.1
//通过影像的类型,城市和索引, 获得影像的ID
//地图物件的格式为：  city号 + type + npc的ID
inline DWORD MAKE_MAP_IMAGE_ID( DWORD type, DWORD city, DWORD idx){
		return ((city&0xff)<<24) | ((type&0xff)<<16)|(idx&0xffff);
}

//从影像ID获得影像索引
inline DWORD GET_IMAGE_INDEX( DWORD id){
	DWORD type = GET_IMAGE_TYPE(id);
	if ( libImageTypeNpc== type || libImageTypeNpcHair==type || libImageTypeNpcFx==type
        || libImageTypeNpcTop == type
        || libImageTypeCap== type || libImageTypeFace==type ||libImageTypeHead==type 
        || libImageTypeBackpack== type
        || libImageTypeWeaponL==type || libImageTypeWeaponR==type )
    {
    	return (id&0xffff)*100 + ((id>>24)&0xff);
    }
	else
    {
		return id&0xffff;
    }
}


//从影像ID获得影像当前的方向(对libImageTypeMagic有效)
inline DWORD GET_IMAGE_CURDIR( DWORD id){
    DWORD type = GET_IMAGE_TYPE(id);
    if ( type == libImageTypeMagic )
    {
        return (id>>24)&0x000000ff;
    }
    else
    {
        return 0;
    }
}

inline bool isMapImage( DWORD dwID){
	DWORD type = GET_IMAGE_TYPE(dwID);
	return (type >= libImageTypeMapBegin) && (type <= libImageTypeMapEnd);
}

bool	IS_IMAGE_TILE( DWORD id);

//Jim, add [2/10/2004]
//影像帧特征标志定义
//{{{
enum FrameFlags
{
    frmFlagKeyAttack                =   0x00000001, 
            //本帧为攻击关键帧

    frmFlagReferAttached            =   0x00000100, 
            //本帧图片起始坐标参照另外某动画的对应帧的影像起点坐标
            //常用在多个影像动画组合显示的应用中
            //如NPC的头发部分和NPC身体部分组合成完整NPC
};
//}}}

//////////////////////////////////////////////////////////////////////////
// structs

//图像帧信息结构
struct TFrameInfo
{
	int nLeft;			//图像左上角位置, 参照包围盒左上角(原点)
	int nTop;			//图像左上角位置, 参照包围盒左上角(原点)
	TImage *ptrImage;	//指向图像影像缓冲区的指针

    //qjb, add [7/28/2004] 
    //纸娃娃系统
    //{{{
    TImage *ptrMskImage; //指向图像影像缓冲区的指针(用于变色)
    //}}}

    //Jim, add [2/10/2004]
    //增加本帧的特征标志数据,以实现标记特别帧等功能
    //{{{
    DWORD dwFlags;      //本帧的特征标志数据
    //}}}

    //qjb, add [7/28/2004] 
    //纸娃娃系统的中心点、绑定点和优先级别
    //{{{
    int nCenterX;                           //相对于本帧图的左上角
    int nCenterY;                           //相对于本帧图的左上角
    int nMaskCenterX;                       //变色通道的中心点，相对于本帧变色通道的左上角
    int nMaskCenterY;                       //变色通道的中心点，相对于本帧变色通道的左上角
    POINT bindPointList[MAX_BINDDOTCOUNT];  //绑定点列表，相对于本帧图的左上角
    int nPriority;                          //该显示的优先级(对武器和头发有效,见<<纸娃娃系统概要设计>>[2.4人物各部分遮蔽关系])
    //}}}
};


//图像裁减底线结构
struct TClipRoot 
{
	int nClipWidth;  //裁减线的宽度
	int nClipCount;  //裁减线数量
	int *ptrClipBy;  //裁减线Y方向偏移, 参照点为包围盒左上角(原点)
    
    //qjb, add [2/2/2005]
    //{{{
    int *ptrClipBx;  //每个裁减线的启始X坐标, 参照点为包围盒左上角(原点)
    int *ptrClipBw;  //每个裁减线的宽度
    TClipRoot():nClipCount(0),ptrClipBy(0),ptrClipBx(0),ptrClipBw(0){};
    //}}}
};


//图像(动画)包围盒及裁减底线数据
struct TBoundInfo 
{
	int	nWidth;			//包围盒宽度
	int	nHeight;		//包围盒高度
	int	nCenterX;		//图像(动画)中心点位置,参照包围盒作上角(原点)
	int	nCenterY;		//图像(动画)中心点位置,参照包围盒作上角(原点)
	TClipRoot ClipRoot; //图像裁减底线数据

    //qjb, add [7/28/2004]
    //
    //{{{
    BOOL bBingFlag;         //该身体部分是否需要绑定到其他身体部分的标志
    int nBindPointCounts;   //拥有绑定点的个数（只对身体有效，=MAX_BINDDOTCOUNT）
    BOOL bHaveMask;         //是否拥有变色通道数据（目前只有身体和头发为TRUE）
    //}}}
    
    //qjb, add [8/7/2004]
    //
    //{{{
    DWORD dwFlag;           //1表示为纸娃娃新图资源, 2表示为单帧大物件被切分成多帧
    //}}}
    
    //qjb, add [2/28/2005]
    // 特效播放位置点
    //{{{
    int nMagicDotCount; // 特效点个数 0, 2 或 5 
    int aMagicPnt[5][2];// 特效播放位置点列表
    //}}}
};

//Jim, add [2/10/2004]
//{{{
struct TDirInfo
{
    int nFrames;        //本方向动画帧数
    int nHitOffsetX;    //被击中心点的X坐标,参照影像动画的中心点
    int nHitOffsetY;    //被击中心点的Y坐标,参照影像动画的中心点
};
//}}}

//////////////////////////////////////////////////////////////////////////
// class IImageLib

//图像库接口定义
class IImageLib
{
public:
	//Delta, add [8/16/2004]
	//{{{ 异步装载资源, 区别于AddRef的同步装载方式
	virtual ULONG Require(DWORD dwID) = 0;
	//}}}

	//引用某个影像资源
	virtual ULONG AddRef(DWORD dwID) = 0;
    
    //获取某个资源当前的引用[add qjb 2005-4-30]
    virtual ULONG GetRef(DWORD dwID) = 0;
	
	//引用结束, 释放该资源的引用
	virtual ULONG Release(DWORD dwID) = 0;
	
	//获得影像资源动画总的方向数目
	virtual int GetTotalDirs(DWORD dwID) = 0;
	
	//获得影像资源动画总的帧数
	virtual int GetTotalFrames(DWORD dwID) = 0;
	
	//获得影像资源某个方向的帧数
	virtual int GetDirFrames(DWORD dwID, int nDir) = 0;

	//获得影像资源包围盒的信息
	virtual TBoundInfo *GetBoundInfo(DWORD dwID) = 0;
	
	//获得影像资源某帧的影像资料
	virtual TFrameInfo *GetFrameInfo(DWORD dwID, int nDir, int nFrame) = 0;

    //Jim, add [2/10/2004]
    //{{{
    virtual TDirInfo *GetDirInfo(DWORD dwID, int nDir) = 0;
    //}}}

	//Delta, add [8/12/2004]
	//{{{
	virtual bool IsImageExist( DWORD dwID ) = 0;
	//}}

	virtual void GarbageCollect() = 0;
};


//////////////////////////////////////////////////////////////////////////
// struct DualImage

struct TDualImage
{
	TImage* m_pImage;	//指向影像数据缓冲
	HIMG	m_hImage;	//指向载入到显存后的句柄
	
	// implementations ----------------------
	TDualImage();
	void Init();
	void FreeMemory();
	BOOL CreateImage(int w, int h, int attrib);
	void GetBufferImage(int bw, int bh, BYTE *rgb_buf, int left, int top, BYTE *alpha_buf = NULL);
	void OperImageSize(int *sx, int *sy, int *ex, int *ey);
	TImage*	CopyImage(int sx, int sy, int xl, int yl);
	TImage* CopyVisibleImage(int *sx, int *sy, int *ex, int *ey);
	
	void FreeHandle();
};


//////////////////////////////////////////////////////////////////////////
// ImageAni Utilities

struct TImageAni		// 影像动画格式
{
	int	ani_width;		// 动画块宽度和高度
	int	ani_height;		//
	int	plane_width;	// 阵列形式影像宽度和高度
	int	plane_height;	//
	int	width_blocks;	// 宽度方向影像块数目: [1, +~)
						// 宽度方向块数 = (plane_width + ani_width - 1) / ani_width
	int	height_blocks;	// 高度方向影像块数目: [1, +~)
						// 高度方向块数 = (plane_height + ani_height - 1) / ani_height
	int frames;			// 影像帧数: [1, +~)
	TImage	*image[1];	// 影像资料列表 
						// 数量 = frames * width_blocks * height_blocks
};

extern	TImageAni * CreateImageAni(int nAniWidth, int nAniHeight, 
								   int nPlaneWidth, int nPlaneHeight,
								   int nFrames);
extern	void		DestroyImageAni(TImageAni **pImageAni);
extern	BOOL		SaveImageAni(CHAR *szFileName, TImageAni *pImageAni);
extern	TImageAni * LoadImageAni(CHAR *szFileName);
extern	BOOL		IsImageAniFile(CHAR *szFileName);



//////////////////////////////////////////////////////////////////////////
// Utilities

extern	IImageLib * CreateImageLib();
extern	void	DestroyImageLib(IImageLib *pImageLib);

extern	USHORT  ComposeHi1555(BYTE a, BYTE r, BYTE g, BYTE b);
extern	USHORT  ComposeHi4444(BYTE a, BYTE r, BYTE g, BYTE b);
extern	ULONG   ComposeHi8888(BYTE a, BYTE r, BYTE g, BYTE b);
extern	void	ReduceHi1555(USHORT hi, BYTE *a, BYTE *r, BYTE *g, BYTE *b);
extern	void	ReduceHi4444(USHORT hi, BYTE *a, BYTE *r, BYTE *g, BYTE *b);
extern	void	ReduceHi8888(ULONG hi, BYTE *a, BYTE *r, BYTE *g, BYTE *b);

inline	BYTE	getAlpha1555( USHORT hi) { return (BYTE) ( (hi & 0x8000) ? 255 : 0); }
inline	BYTE	getAlpha4444( USHORT hi) {	return (BYTE) ( (hi & 0xf000) >> 8 ); }

class CVFile;
extern	BOOL	NakedWriteImage(TImage *pImage, FILE *fp);
extern	BOOL	NakedReadImage(TImage **pImage, FILE *fp);
extern	BOOL	NakedReadImage(TImage **pImage, CVFile *fp);

#endif//_IMAGELIB_H_

