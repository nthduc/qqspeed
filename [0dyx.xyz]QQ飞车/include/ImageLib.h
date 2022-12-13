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

//ͨ��Ӱ������ͺ�����,���Ӱ���ID
//-mikechen 2003.4.28
//#define MAKE_IMAGE_ID(type,idx)		((DWORD)(((DWORD)((type)&0xff)<<16)|(DWORD)((idx)&0xffff)))

//��Ӱ��ID���Ӱ������
#define GET_IMAGE_TYPE(id)		(((id)>>16)&0xff)

//��Ӱ��ID�����������(����NPC���,Ϊ��Ӧ��������)
#define GET_IMAGE_SET(id)       (((id)>>24)&0xff)

//��Ӱ��ID���Ӱ������
//-mikechen 2003.4.28
//#define GET_IMAGE_INDEX(id)		((id)&0xffff)

#define	INVALID_IMAGE_ID		0x00000000
#define INVALID_IMAGE_TYPE		0
#define INVALID_IMAGE_INDEX		0

//qujianbiao 2004.9.1
const int MAX_BINDDOTCOUNT      =   4;

//mikechen 2004.6.1
//���Ӱ������
enum	LibImageTypes
{
	libImageTypeInvalid			=	0,		//�Ƿ�����(����)
//	libImageTypeAni				=	1,		//��ͼ����
//	libImageTypeSculp			=	2,		//����
	
	libImageTypeMapBegin		=	1,		//map begin
	libImageTypeCC				=	1,		//�ݴ�
	libImageTypeCCP				=	2,		//�ݴ���Ƭ
	libImageTypeGM				=	3,		//��ľ
	libImageTypeGMP				=	4,		//��ľ��Ƭ
	libImageTypeHP				=	5,		//����		
	libImageTypeHPP				=	6,		//������Ƭ
	libImageTypeHT				=	7,		//��̳
	libImageTypeHTP				=	8,		//��̳��Ƭ
	libImageTypeSM				=	9,		//��ľ
	libImageTypeSMP				=	10,		//��ľ��Ƭ
	libImageTypeLD				=	11,		//·��
	libImageTypeLDP				=	12,		//·����Ƭ
	libImageTypeZZ				=	13,		//����
	libImageTypeZZP				=	14,		//������Ƭ
	libImageTypeM				=	15,		//��
	libImageTypeMP				=	16,		//����Ƭ
	libImageTypeDT				=	17,		//��̯
	libImageTypeDTP				=	18,		//��̯��Ƭ
	libImageTypeDX				=	19,		//����
	libImageTypeDXP				=	20,		//������Ƭ
	libImageTypeTT				=	21,		//ͼ��
	libImageTypeTTP				=	22,		//ͼ����Ƭ
	libImageTypeSK				=	23,		//ʯ��
	libImageTypeSKP				=	24,		//ʯ����Ƭ
	libImageTypeZW				=	25,		//����
	libImageTypeZWP				=	26,		//������Ƭ
	libImageTypeDD				=	27,		//�ض�
	libImageTypeDDP				=	28,		//�ض���Ƭ
	libImageTypeHH				=	29,		//��
	libImageTypeHHP				=	30,		//����Ƭ
	libImageTypeGT				=	31,		//��ͷ
	libImageTypeGTP				=	32,		//��ͷ��Ƭ
	libImageTypeMK				=	33,		//ľ��
	libImageTypeMKP				=	34,		//ľ����Ƭ
	libImageTypeZY				=	35,		//����
	libImageTypeZYP				=	36,		//������Ƭ
	libImageTypeSC				=	37,		//ˮ��
	libImageTypeSCP				=	38,		//ˮ����Ƭ
	libImageTypeSW				=	39,		//����
	libImageTypeSWP				=	40,		//������Ƭ
	libImageTypeLG				=	41,		//����
	libImageTypeLGP				=	42,		//������Ƭ
	libImageTypeXJZ				=	43,		//С����
	libImageTypeXQQ				=	44,		//С��
	libImageTypeJZ				=	45,		//����
	libImageTypeYB				=	46,		//�ұ�
	libImageTypeYD				=	47,		//�ƶ�
	libImageTypeDH				=	48,		//����
    libImageTypeYBCC			=	49,		//�ұڲݴ�
    libImageTypeNT  			=	50,		//������
    libImageTypeQQ  			=	51,		//��
    libImageTypeJZP  			=	52,		//����Ƭ
    libImageTypeQQP  			=	53,		//����Ƭ
	libImageTypeMapEnd			=	53,		//map end
	
	libImageTypeOther			=	99,		//����
	
	libImageTypeNpc				=	200,	//NPC���
	libImageTypeMagic			=	201,	//����Ч�����
	libImageTypeItem			=	202,	//�������

    //Jim, add [2/10/2004]
    //{{{
    libImageTypeNpcHair         =   203,    //NPCͷ��
    //}}}

    //Jim, add [4/7/2004]
    //{{{
    libImageTypeItemPic         =   204,    //����ͼƬ���
    //}}}

	//Mikechen + [6/17/2004]
    libImageTypeNpcFx			=   205,    //NPC��Ч
    //Qjb, add [04/25/2005]
    libImageTypeNpcTop          =   206,    //�������

    //Qjb, add [07/28/2004]
    //{{{
    libImageTypeBodyBegin       =   210,
    libImageTypeCap             =   210,    //ñ��
    libImageTypeFace            =   211,    //����
    libImageTypeHead            =   212,    //ͷ��������ͷ����������
    libImageTypeBackpack        =   213,    //�������������磬��򣬱����ȣ�
    libImageTypeWeaponL         =   214,    //��������
    libImageTypeWeaponR         =   215,    //��������
    libImageTypeBodyEnd         =   215,
    //}}}
    
};

//Qjb, add [07/28/2004]
//�󶨵�����
//{{{
enum LibImageBindTypes
{
    LibImageBindTypeHead        =   0,      //ͷ�İ󶨵����ţ���TFrameInfo::bindPointList[]��
    LibImageBindTypeBackpack    =   1,      //�����İ󶨵����ţ���TFrameInfo::bindPointList[]��
    LibImageBindTypeWeapon      =   2,      //�����İ󶨵����ţ���TFrameInfo::bindPointList[]��
    LibImageBindTypeWeaponL     =   2,      //�������İ󶨵����ţ���TFrameInfo::bindPointList[]��
    LibImageBindTypeWeaponR     =   3,      //�������İ󶨵����ţ���TFrameInfo::bindPointList[]��
};
//}}}

//Qjb, add [07/28/2004]
//�󶨵���������
//{{{
enum LibImageBindIndex
{
    libImageBindBody            =   0,    //����
    libImageBindCap             =   1,    //ñ��
    libImageBindFace            =   2,    //����
    libImageBindHead            =   3,    //ͷ��������ͷ����������
    libImageBindBackpack        =   4,    //�������������磬��򣬱����ȣ�
    libImageBindWeaponL         =   5,    //��������
    libImageBindWeaponR         =   6,    //��������
    libImageBindPet             =   7,  //���
    libImageBindPetTop          =   8,  //���Ķ���
};
//}}}

//Qjb, add [07/28/2004]
//�󶨵�Ķ������
//{{{
enum LibImageActionType
{
    libImageActionStand         =   1,      //վ��
    libImageActionRun           =   2,      //��
    libImageActionSit           =   3,      //��
    libImageActionDie           =   4,      //����
    libImageActionAtt1          =   5,      //����������1
    libImageActionAtt2          =   6,      //����������2
    libImageActionAtt3          =   7,      //����������3
    libImageActionMagicAtt      =   8,      //ħ������
    libImageActionPrepare1      =   9,      //׼������1
    libImageActionPrepare2      =   10,      //׼������2
    libImageActionPrepare3      =   11,      //׼������3
    libImageActionBeHurt        =   12,      //������
    libImageActionRide          =   13,     //���������
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
//ͨ��Ӱ������ͺ�����,���Ӱ���ID
//npc�ĸ�ʽ��Ϊ��  ������ + type + npc��ID
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
            // ��ǰ����(31-24) ����(23-16) ����(15-0)
            return ((DWORD)(((DWORD)((type)&0xff)<<16)|(DWORD)((idx)&0xffff)|(DWORD)((idx<<8)&0xff000000)));
        }
        return ((DWORD)(((DWORD)((type)&0xff)<<16)|(DWORD)((idx)&0xffff)));
    }
}
//+ mikechen 2004.6.1
//ͨ��Ӱ�������,���к�����, ���Ӱ���ID
//��ͼ����ĸ�ʽΪ��  city�� + type + npc��ID
inline DWORD MAKE_MAP_IMAGE_ID( DWORD type, DWORD city, DWORD idx){
		return ((city&0xff)<<24) | ((type&0xff)<<16)|(idx&0xffff);
}

//��Ӱ��ID���Ӱ������
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


//��Ӱ��ID���Ӱ��ǰ�ķ���(��libImageTypeMagic��Ч)
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
//Ӱ��֡������־����
//{{{
enum FrameFlags
{
    frmFlagKeyAttack                =   0x00000001, 
            //��֡Ϊ�����ؼ�֡

    frmFlagReferAttached            =   0x00000100, 
            //��֡ͼƬ��ʼ�����������ĳ�����Ķ�Ӧ֡��Ӱ���������
            //�����ڶ��Ӱ�񶯻������ʾ��Ӧ����
            //��NPC��ͷ�����ֺ�NPC���岿����ϳ�����NPC
};
//}}}

//////////////////////////////////////////////////////////////////////////
// structs

//ͼ��֡��Ϣ�ṹ
struct TFrameInfo
{
	int nLeft;			//ͼ�����Ͻ�λ��, ���հ�Χ�����Ͻ�(ԭ��)
	int nTop;			//ͼ�����Ͻ�λ��, ���հ�Χ�����Ͻ�(ԭ��)
	TImage *ptrImage;	//ָ��ͼ��Ӱ�񻺳�����ָ��

    //qjb, add [7/28/2004] 
    //ֽ����ϵͳ
    //{{{
    TImage *ptrMskImage; //ָ��ͼ��Ӱ�񻺳�����ָ��(���ڱ�ɫ)
    //}}}

    //Jim, add [2/10/2004]
    //���ӱ�֡��������־����,��ʵ�ֱ���ر�֡�ȹ���
    //{{{
    DWORD dwFlags;      //��֡��������־����
    //}}}

    //qjb, add [7/28/2004] 
    //ֽ����ϵͳ�����ĵ㡢�󶨵�����ȼ���
    //{{{
    int nCenterX;                           //����ڱ�֡ͼ�����Ͻ�
    int nCenterY;                           //����ڱ�֡ͼ�����Ͻ�
    int nMaskCenterX;                       //��ɫͨ�������ĵ㣬����ڱ�֡��ɫͨ�������Ͻ�
    int nMaskCenterY;                       //��ɫͨ�������ĵ㣬����ڱ�֡��ɫͨ�������Ͻ�
    POINT bindPointList[MAX_BINDDOTCOUNT];  //�󶨵��б�����ڱ�֡ͼ�����Ͻ�
    int nPriority;                          //����ʾ�����ȼ�(��������ͷ����Ч,��<<ֽ����ϵͳ��Ҫ���>>[2.4����������ڱι�ϵ])
    //}}}
};


//ͼ��ü����߽ṹ
struct TClipRoot 
{
	int nClipWidth;  //�ü��ߵĿ��
	int nClipCount;  //�ü�������
	int *ptrClipBy;  //�ü���Y����ƫ��, ���յ�Ϊ��Χ�����Ͻ�(ԭ��)
    
    //qjb, add [2/2/2005]
    //{{{
    int *ptrClipBx;  //ÿ���ü��ߵ���ʼX����, ���յ�Ϊ��Χ�����Ͻ�(ԭ��)
    int *ptrClipBw;  //ÿ���ü��ߵĿ��
    TClipRoot():nClipCount(0),ptrClipBy(0),ptrClipBx(0),ptrClipBw(0){};
    //}}}
};


//ͼ��(����)��Χ�м��ü���������
struct TBoundInfo 
{
	int	nWidth;			//��Χ�п��
	int	nHeight;		//��Χ�и߶�
	int	nCenterX;		//ͼ��(����)���ĵ�λ��,���հ�Χ�����Ͻ�(ԭ��)
	int	nCenterY;		//ͼ��(����)���ĵ�λ��,���հ�Χ�����Ͻ�(ԭ��)
	TClipRoot ClipRoot; //ͼ��ü���������

    //qjb, add [7/28/2004]
    //
    //{{{
    BOOL bBingFlag;         //�����岿���Ƿ���Ҫ�󶨵��������岿�ֵı�־
    int nBindPointCounts;   //ӵ�а󶨵�ĸ�����ֻ��������Ч��=MAX_BINDDOTCOUNT��
    BOOL bHaveMask;         //�Ƿ�ӵ�б�ɫͨ�����ݣ�Ŀǰֻ�������ͷ��ΪTRUE��
    //}}}
    
    //qjb, add [8/7/2004]
    //
    //{{{
    DWORD dwFlag;           //1��ʾΪֽ������ͼ��Դ, 2��ʾΪ��֡��������зֳɶ�֡
    //}}}
    
    //qjb, add [2/28/2005]
    // ��Ч����λ�õ�
    //{{{
    int nMagicDotCount; // ��Ч����� 0, 2 �� 5 
    int aMagicPnt[5][2];// ��Ч����λ�õ��б�
    //}}}
};

//Jim, add [2/10/2004]
//{{{
struct TDirInfo
{
    int nFrames;        //�����򶯻�֡��
    int nHitOffsetX;    //�������ĵ��X����,����Ӱ�񶯻������ĵ�
    int nHitOffsetY;    //�������ĵ��Y����,����Ӱ�񶯻������ĵ�
};
//}}}

//////////////////////////////////////////////////////////////////////////
// class IImageLib

//ͼ���ӿڶ���
class IImageLib
{
public:
	//Delta, add [8/16/2004]
	//{{{ �첽װ����Դ, ������AddRef��ͬ��װ�ط�ʽ
	virtual ULONG Require(DWORD dwID) = 0;
	//}}}

	//����ĳ��Ӱ����Դ
	virtual ULONG AddRef(DWORD dwID) = 0;
    
    //��ȡĳ����Դ��ǰ������[add qjb 2005-4-30]
    virtual ULONG GetRef(DWORD dwID) = 0;
	
	//���ý���, �ͷŸ���Դ������
	virtual ULONG Release(DWORD dwID) = 0;
	
	//���Ӱ����Դ�����ܵķ�����Ŀ
	virtual int GetTotalDirs(DWORD dwID) = 0;
	
	//���Ӱ����Դ�����ܵ�֡��
	virtual int GetTotalFrames(DWORD dwID) = 0;
	
	//���Ӱ����Դĳ�������֡��
	virtual int GetDirFrames(DWORD dwID, int nDir) = 0;

	//���Ӱ����Դ��Χ�е���Ϣ
	virtual TBoundInfo *GetBoundInfo(DWORD dwID) = 0;
	
	//���Ӱ����Դĳ֡��Ӱ������
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
	TImage* m_pImage;	//ָ��Ӱ�����ݻ���
	HIMG	m_hImage;	//ָ�����뵽�Դ��ľ��
	
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

struct TImageAni		// Ӱ�񶯻���ʽ
{
	int	ani_width;		// �������Ⱥ͸߶�
	int	ani_height;		//
	int	plane_width;	// ������ʽӰ���Ⱥ͸߶�
	int	plane_height;	//
	int	width_blocks;	// ��ȷ���Ӱ�����Ŀ: [1, +~)
						// ��ȷ������ = (plane_width + ani_width - 1) / ani_width
	int	height_blocks;	// �߶ȷ���Ӱ�����Ŀ: [1, +~)
						// �߶ȷ������ = (plane_height + ani_height - 1) / ani_height
	int frames;			// Ӱ��֡��: [1, +~)
	TImage	*image[1];	// Ӱ�������б� 
						// ���� = frames * width_blocks * height_blocks
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

