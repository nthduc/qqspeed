/*
**	IDImage.cpp
**	id image functions.
**
**	Jim, 2003.12.23.
**
**
**  Jim, 2004.2.10. 
**      -- update idimage file version, VER -> VER_1_1.
**         (0x00010000 -> 0x00010001)
**
**
*/
#include "Engine.h"
#include "Display.h"
#include "ImageLib.h"
#include "IDImage.h"
#include "vfilewrap.h"
#include "string"
#include "assert.h"
// add by qjb 2005-12-8
//{{{ ���ǵ�������Χ
//const int MAX_ROLE_INDEX = 200;
typedef struct tagROLESRANGE
{
    int minidx;
    int maxidx;
}ROLESRANGE;
const ROLESRANGE ROLES_RANGE[] = 
{ 
    {0,    200},  // 0-200
    {1000, 2000}, // 1000-2000
};

BOOL IsRoleRes(int idx)
{
    for ( int i=0; i<sizeof(ROLES_RANGE)/sizeof(ROLESRANGE); i++ )
    {
        if ( idx >= ROLES_RANGE[i].minidx && idx <=  ROLES_RANGE[i].maxidx )
            return TRUE;
    }
    return FALSE;
}
//}}}

const int MAX_IMAGE_SIZE = 256;

static char* cityNames[][2] = {
    "FY",   "�����",
    "XL",   "���޳�",
    "TD",   "�춼",
    "TY",   "��Դ",
    "CL",   "����",
    "DFYW", "����Ұ��",
    "SW",   "_����ɽ",
    "YL",   "_���޵�",
    "PL",   "_�����ɾ�",
    "TT",   "_ͨ����",
    "SJ",   "ʥ�θ�",
    "HJ",   "�ƽ��",
    "SMYW", "ɳĮҰ��",
    "HS",   "��ɽ��",
    "YM",   "�����Թ�",
    "GC",   "�ų���ַ",
    "MY",   "���϶�",
    "TH",   "�����",
    "XFYW", "����Ұ��",
    "TTD",  "ͨ����",
    "YLD",  "���޵�",
    "PLSJ", "����ˮ��",
    "SWS",  "����ɽ",
    "DXWG",  "��������",
    "CLMG",  "�����Թ�",
    "TYMG",  "��Դ�Թ�",
};

struct mapObjEntry{
	char*	enName;
	char*	chName;
	bool	isTile;
};

static mapObjEntry mapObjNames[] = {
	"CC",   "�ݴ�",		0,
	"CCP",  "�ݴ���Ƭ", 1,
	"GM",   "��ľ",		0,
	"GMP",  "��ľ��Ƭ", 1,
	"HP",   "����",		0, 
	"HPP",  "������Ƭ", 1,
	"HT",   "��̳",		0, 
	"HTP",  "��̳��Ƭ", 1,
	"SM",   "��ľ",		0,
	"SMP",  "��ľ��Ƭ", 1,
	"LD",   "·��",		0,
	"LDP",  "·����Ƭ", 1,
	"ZZ",   "����",		0,
	"ZZP",  "������Ƭ", 1,
	"M",    "��",		0,
	"MP",   "����Ƭ",	1,
	"DT",   "��̯",		0,
	"DTP",  "��̯��Ƭ", 1,
	"DX",   "����",		0,
	"DXP",  "������Ƭ", 1,
	"TT",   "ͼ��",		0,
	"TTP",  "ͼ����Ƭ", 1,
	"SK",   "ʯ��",		0,
	"SKP",  "ʯ����Ƭ", 1,
	"ZW",   "����",		0,
	"ZWP",  "������Ƭ", 1,
	"DD",   "�ض�",		0,
	"DDP",  "�ض���Ƭ", 1,
	"HH",   "��",		0,
	"HHP",  "����Ƭ",	1,
	"GT",   "��ͷ",		0,
	"GTP",  "��ͷ��Ƭ", 1,
	"MK",   "ľ��",		0,
	"MKP",  "ľ����Ƭ", 1,
	"ZY",   "����",		0,
	"ZYP",  "������Ƭ", 1,
	"SC",   "ˮ��",		0,
	"SCP",  "ˮ����Ƭ", 1,
	"SW",   "����", 	0,
	"SWP",  "������Ƭ", 1,
	"LG",   "����",		0,
	"LGP",  "������Ƭ",	1,
	"XJZ",	"С����",	0,
	"XQQ",	"С��",		0,
	"JZ",	"����",		0,
	"YB",	"�ұ���Ƭ", 1,
	"YD",	"�ƶ�",		0,
	"DH",	"����",		0,
    "YBCC", "�ұڲݴ�", 0,
    "NT",   "������Ƭ", 1,
    "QQ",   "��",       0,
    "JZP",  "������Ƭ", 1,
    "QQP",  "����Ƭ",   1,
};

#define	IDIMAGE_COPYRIGHT	MAKEFOURCC('Q','Q','F',0x1a)
#define IDIMAGE_FILE_ID		MAKEFOURCC('D','I','M','G')

bool	IS_IMAGE_TILE( DWORD id){
	DWORD type = GET_IMAGE_TYPE( id);
	if ( (type >= libImageTypeMapBegin) && (type <= libImageTypeMapEnd)){
		return mapObjNames[type-libImageTypeMapBegin].isTile;		
	}else
		return false;
}

//////////////////////////////////////////////////////////////////////////
// class CIDImage

CIDImage::CIDImage()
{
	m_dwImageID = INVALID_IMAGE_ID;
	m_dwRef = 0;
	memset(&m_BoundInfo, 0, sizeof(m_BoundInfo));
	m_FrameInfo = NULL;
    m_version = nFileVerLast;

    //Jim, add [2/10/2004]
    //{{{
    m_DirInfo = NULL;
    //}}}
	m_HotTick = 0;
}


CIDImage::~CIDImage()
{
	FreeMemory();
}


void CIDImage::ImageID2FileName(DWORD dwID, CHAR *szFileName)
{
	int type = GET_IMAGE_TYPE(dwID);
	switch(type)
	{
	case libImageTypeNpc:
		sprintf(szFileName, "avatar\\%s\\%s%04d.gsn", GetTypeDir(dwID), GetTypeDir(dwID), GET_IMAGE_INDEX(dwID));
        //sprintf(szFileName, "object\\%s\\%s%03d%02d.gsn", GetTypeDir(dwID), GetTypeDir(dwID), 
          //  GET_IMAGE_INDEX(dwID), GET_IMAGE_SET(dwID));
		break;
    case libImageTypeNpcHair:
        sprintf(szFileName, "avatar\\%s\\%s%04dh.gsn", GetTypeDir(dwID), GetTypeDir(dwID), GET_IMAGE_INDEX(dwID));
        break;
    case libImageTypeNpcFx:
        sprintf(szFileName, "avatar\\%s\\%s%04de.gsn", GetTypeDir(dwID), GetTypeDir(dwID), GET_IMAGE_INDEX(dwID));
        break;
    case libImageTypeNpcTop:
        sprintf(szFileName, "avatar\\%s\\%s%04dt.gsn", GetTypeDir(dwID), GetTypeDir(dwID), GET_IMAGE_INDEX(dwID));
        break;
    case libImageTypeCap:
    case libImageTypeFace:
    case libImageTypeHead:
    case libImageTypeBackpack:
    case libImageTypeWeaponL:
    case libImageTypeWeaponR:
        sprintf(szFileName, "avatar\\%s\\%s%04d.gsn", GetTypeDir(dwID), GetTypeDir(dwID), GET_IMAGE_INDEX(dwID));
        break;
    case libImageTypeMagic:
        {
            DWORD dwCurDir = GET_IMAGE_CURDIR(dwID);
            if ( dwCurDir > 0 )
            {
                sprintf(szFileName, "magic\\%s\\%s%04d_%02d.gso", GetTypeDir(dwID), GetTypeDir(dwID), GET_IMAGE_INDEX(dwID), dwCurDir);
            }
            else
            {
                sprintf(szFileName, "magic\\%s\\%s%04d.gso", GetTypeDir(dwID), GetTypeDir(dwID), GET_IMAGE_INDEX(dwID));
            }
        }
        break;
    case libImageTypeItem:
	case libImageTypeItemPic:
        sprintf(szFileName, "item\\%s\\%s%04d.gso", GetTypeDir(dwID), GetTypeDir(dwID), GET_IMAGE_INDEX(dwID));
        break;
	default:
		if (isMapImage(dwID))
			sprintf(szFileName, "object\\%s\\%s_%s_%04d.gso", GetTypeDir(dwID), GetTypeDir(dwID), GetCityEnName(dwID), GET_IMAGE_INDEX(dwID));
		else
        {
            sprintf(szFileName, "object\\%s\\%s%04d.gso", GetTypeDir(dwID), GetTypeDir(dwID), GET_IMAGE_INDEX(dwID));
        }
		break;
	}
}


DWORD CIDImage::FileNameToImageID(CHAR *szFileName)
{
    std::string fname = szFileName;
    int pos = fname.find_first_of(".");
    if ( pos < 0 )
        return 0;
    std::string fname2 = fname.substr(0,pos);
    pos = fname2.find_first_of("_");
    if ( pos < 0 )
        return 0;
    std::string tname = fname2.substr(0,pos);
    int oldpos = pos;
    pos = fname2.find_first_of("_", oldpos+1);
    if ( pos < 0 )
        return 0;
    std::string cname = fname2.substr(oldpos+1, pos-oldpos-1);
    std::string idx = fname2.substr(pos+1, fname2.length());
    
    int count = sizeof(cityNames)/sizeof(cityNames[0]);
    for ( int i=0; i<count; i++ )
    {
        if ( strcmpi(cname.c_str(), cityNames[i][0]) == 0 )
        {
            break;
        }
    }

    if ( count == i )
        return 0;
    DWORD dwCity = i+libImageCityBegin;

    count = sizeof(mapObjNames)/sizeof(mapObjNames[0]);
    for ( i=0; i<count; i++ )
    {
        if ( strcmpi(tname.c_str(), mapObjNames[i].enName) == 0 )
        {
            break;
        }
    }
    
    if ( count == i )
        return 0;

    DWORD dwType = i+libImageTypeMapBegin;

    DWORD dwIdx = atoi(idx.c_str());

    return MAKE_MAP_IMAGE_ID(dwType, dwCity, dwIdx);
}


CHAR* CIDImage::GetTypeDir(DWORD dwID)
{
	DWORD type = GET_IMAGE_TYPE(dwID);

	if ( (type>=libImageTypeMapBegin) && (type<=libImageTypeMapEnd))
		return mapObjNames[type-libImageTypeMapBegin].enName;
	
	switch(type){
	case libImageTypeOther:		return (CHAR*)"other";
	case libImageTypeNpc:		return (CHAR*)"npc";
	case libImageTypeMagic:		return (CHAR*)"magic";
	case libImageTypeItem:		return (CHAR*)"item";
    case libImageTypeItemPic:   return (CHAR*)"itempic";
    case libImageTypeNpcHair:   return (CHAR*)"npc"; //npchair�����npcĿ¼��
    case libImageTypeNpcFx:		return (CHAR*)"npc"; //npc Fx�����npcĿ¼��
    case libImageTypeNpcTop:	return (CHAR*)"npc"; //npc Fx�����npcĿ¼��
    case libImageTypeCap:       return (CHAR*)"cap";
    case libImageTypeFace:      return (CHAR*)"face";
    case libImageTypeHead:      return (CHAR*)"head";
    case libImageTypeBackpack:  return (CHAR*)"backpack";
    case libImageTypeWeaponL:   return (CHAR*)"weaponl";
    case libImageTypeWeaponR:   return (CHAR*)"weaponr";
		//case libImageTypeInvalid:	
	default:		return (CHAR*)"____";
	}
}


CHAR* CIDImage::GetTypeName(DWORD dwID, BOOL *bMainType){
    DWORD type = GET_IMAGE_TYPE(dwID);

	if(bMainType) *bMainType = TRUE; 

	if ( (type>=libImageTypeMapBegin) && (type<=libImageTypeMapEnd))
		return mapObjNames[type-libImageTypeMapBegin].chName;
	
    switch(type)
    {
	case libImageTypeOther:		return (CHAR*)"����";
    case libImageTypeNpc:		return (CHAR*)"NPC";
    case libImageTypeMagic:		return (CHAR*)"����";
    case libImageTypeItem:		return (CHAR*)"����";

    case libImageTypeNpcHair:   
        if(bMainType) *bMainType = FALSE; 
        return (CHAR*)"NPCͷ��"; //npchair�����npcĿ¼��
    case libImageTypeNpcFx:   
        if(bMainType) *bMainType = FALSE; 
        return (CHAR*)"NPC��Ч"; //npchair�����npcĿ¼��
    case libImageTypeNpcTop:
        if(bMainType) *bMainType = FALSE; 
        return (CHAR*)"���Ķ���"; //�����npcĿ¼��
    case libImageTypeItemPic:	return (CHAR*)"����ͼƬ";
        //case libImageTypeInvalid:	
    default:		
        if(bMainType) *bMainType = FALSE; 
        return (CHAR*)"____";
    }
}

CHAR* CIDImage::GetCityChName( DWORD dwID){
    DWORD city = GET_IMAGE_SET(dwID);
	
	if ( (city>=libImageCityBegin) && (city<=libImageCityEnd))
		return cityNames[city-libImageCityBegin][1];
	else
        return "____";

}

CHAR* CIDImage::GetCityEnName( DWORD dwID){
    DWORD city = GET_IMAGE_SET(dwID);
	
	if ( (city>=libImageCityBegin) && (city<=libImageCityEnd))
		return cityNames[city-libImageCityBegin][0];
	else
        return "____";
	
}

ULONG CIDImage::AddRef()
{
	if(m_dwRef == 0)
	{
		if(!LoadImageFile())
		{
		    return 0;
		}
	}
	m_dwRef ++;
	return m_dwRef;
}

ULONG CIDImage::GetRef()
{
    return m_dwRef;
}


ULONG CIDImage::Release()
{
	if(m_dwRef == 0)
		return 0;

	m_dwRef--;
	//if( m_dwRef == 0 ) FreeMemory();

	return m_dwRef;
}


int	CIDImage::GetTotalDirs()
{
	return m_nTotalDirs;
}


int	CIDImage::GetTotalFrames()
{
	return m_nTotalFrames;
}


int	CIDImage::GetDirFrames(int nDir)
{
	return m_nDirFrames;
}


TBoundInfo * CIDImage::GetBoundInfo()
{
	return &m_BoundInfo;
}


TFrameInfo * CIDImage::GetFrameInfo(int nDir, int nDirFrame)
{
	if(! m_FrameInfo)
		return NULL;

	int frame;
	frame = nDir * m_nDirFrames + nDirFrame;
	if(frame < 0 || frame >= m_nTotalFrames)
		return NULL;

	return &m_FrameInfo[frame];
}


void CIDImage::FreeMemory()
{
	if(m_BoundInfo.ClipRoot.ptrClipBy)
	{
		free(m_BoundInfo.ClipRoot.ptrClipBy);
		m_BoundInfo.ClipRoot.ptrClipBy = NULL;
	}
    
    //qjb, add [2/2/2005]
    //{{{
    if ( m_BoundInfo.ClipRoot.ptrClipBx )
    {
        free(m_BoundInfo.ClipRoot.ptrClipBx);
        m_BoundInfo.ClipRoot.ptrClipBx = NULL;
    }

    if ( m_BoundInfo.ClipRoot.ptrClipBw )
    {
        free(m_BoundInfo.ClipRoot.ptrClipBw);
        m_BoundInfo.ClipRoot.ptrClipBw = NULL;
    }
    //}}}

	if(m_FrameInfo)
	{
		for(int i=0; i<m_nTotalFrames; i++)
		{
			if(m_FrameInfo[i].ptrImage)
			{
				free(m_FrameInfo[i].ptrImage);
				m_FrameInfo[i].ptrImage = NULL;
			}

            if ( m_FrameInfo[i].ptrMskImage )
            {
                free(m_FrameInfo[i].ptrMskImage);
                m_FrameInfo[i].ptrMskImage = NULL;
            }
		}
		free(m_FrameInfo);
		m_FrameInfo = NULL;
	}

    //Jim, add [2/10/2004]
    //{{{
    if(m_DirInfo)
    {
        free(m_DirInfo);
        m_DirInfo = NULL;
    }
    //}}}
}


BOOL CIDImage::CreateClipRoot(int nClipWidth)
{
	if(nClipWidth < 1)
		return FALSE;

	int count;
	count = (m_BoundInfo.nWidth + nClipWidth - 1) / nClipWidth;

	if(m_BoundInfo.ClipRoot.ptrClipBy)
	{
		free(m_BoundInfo.ClipRoot.ptrClipBy);
	}
	m_BoundInfo.ClipRoot.ptrClipBy = (int *)malloc(count * sizeof(int));

	m_BoundInfo.ClipRoot.nClipWidth = nClipWidth;
	m_BoundInfo.ClipRoot.nClipCount = count;
	for(int i=0; i<count; i++)
	{
		m_BoundInfo.ClipRoot.ptrClipBy[i] = m_BoundInfo.nHeight;
	}
	return TRUE;
}


void CIDImage::SetClipRootY(int nClipIndex, int nClipBy)
{
	if(!m_BoundInfo.ClipRoot.ptrClipBy)
		return;
	if(nClipIndex < 0 || nClipIndex >= m_BoundInfo.ClipRoot.nClipCount)
		return;

	m_BoundInfo.ClipRoot.ptrClipBy[nClipIndex] = nClipBy;
}


void CIDImage::InitFileHeader(TFileHeader *pHeader)
{
	if(!pHeader)
		return;
	memset(pHeader, 0, sizeof(TFileHeader));
	pHeader->copyright = IDIMAGE_COPYRIGHT;
	pHeader->id = IDIMAGE_FILE_ID;
	
    //Jim, change [2/10/2004]
    //{{{
    //pHeader->version = nFileVer;
    pHeader->version = nFileVerLast;
    //}}}

	pHeader->head_size = sizeof(TFileHeader);
}


BOOL CIDImage::LoadImageFile()
{
//modified by delta
/*	FILE *fp = NULL;
	
	//���ļ�׼��д��
	ImageID2FileName(m_dwImageID, m_szFileName);
	if(NULL == (fp = fopen((const char*)m_szFileName, "rb")))
	{
		goto _error;
	}
*/
    // �Ե�֡�Ĵ�������зָ���
    int nCols;
    int nRows;
    int nOldTotalFrames;
    int nResType;

	FILE_V* fp = NULL;
	//���ļ�׼��д��
	ImageID2FileName(m_dwImageID, m_szFileName);
	if(NULL == (fp = vfopen((const char*)m_szFileName, "rb")))
	{
		goto _error;
	}
//end
	
	//�����ļ�ͷ
	TFileHeader fh;
	memset(&fh, 0, sizeof(fh));
	vfread(&fh.copyright, 1, sizeof(fh.copyright), fp);
	if(IDIMAGE_COPYRIGHT != fh.copyright)
	{
		//copyright error
		goto _error;
	}
	vfread(&fh.id, 1, sizeof(fh.id), fp);
	if(IDIMAGE_FILE_ID != fh.id)
	{
		//file format error
		goto _error;
	}
	vfread(&fh.version, 1, sizeof(fh.version), fp);
	vfread(&fh.head_size, 1, sizeof(fh.head_size), fp);
	vfread(&fh.frames, 1, sizeof(fh.frames), fp);
	vfread(&fh.dirs, 1, sizeof(fh.dirs), fp);

    m_version = fh.version;
    
    //Qjb, add [7/28/2004]
    //{{{
    if ( fh.version >= nFileVer_1_2 )
    {
        vfread(&fh.bBingFlag, 1, sizeof(fh.bBingFlag), fp);
        vfread(&fh.nBindPointCounts, 1, sizeof(fh.nBindPointCounts), fp);
        vfread(&fh.bHaveMask, 1, sizeof(fh.bHaveMask), fp);
    }
    //}}}

	vfseek(fp, fh.head_size, SEEK_SET);

	m_nTotalFrames = fh.frames;
	m_nTotalDirs = fh.dirs;
	m_nDirFrames = fh.frames / max(fh.dirs, 1);
    m_BoundInfo.bBingFlag = fh.bBingFlag;
    m_BoundInfo.nBindPointCounts = fh.nBindPointCounts;
    m_BoundInfo.bHaveMask = fh.bHaveMask; 
    
	//�����Χ������
	vfread(&m_BoundInfo.nWidth, 1, sizeof(int), fp);
	vfread(&m_BoundInfo.nHeight, 1, sizeof(int), fp);
	vfread(&m_BoundInfo.nCenterX, 1, sizeof(int), fp);
	vfread(&m_BoundInfo.nCenterY, 1, sizeof(int), fp);
	vfread(&m_BoundInfo.ClipRoot.nClipWidth, 1, sizeof(int), fp);
	vfread(&m_BoundInfo.ClipRoot.nClipCount, 1, sizeof(int), fp);
	m_BoundInfo.ClipRoot.ptrClipBy = (int*)malloc(sizeof(int) * m_BoundInfo.ClipRoot.nClipCount);
	memset(m_BoundInfo.ClipRoot.ptrClipBy, 0, sizeof(int) * m_BoundInfo.ClipRoot.nClipCount);
	vfread(m_BoundInfo.ClipRoot.ptrClipBy, 1, sizeof(int) * m_BoundInfo.ClipRoot.nClipCount, fp);

    //���벥��ħ��λ�õ�
    //add by qjb[2/28/2005]
    //{{{
    m_BoundInfo.nMagicDotCount = 0;
    memset(m_BoundInfo.aMagicPnt, 0, sizeof(int)*5*2);
    if ( fh.version >= nFileVer_1_4 )
    {
        vfread(&m_BoundInfo.nMagicDotCount, 1, sizeof(int), fp);
        for ( int i=0; i<m_BoundInfo.nMagicDotCount && i<5; i++ )
        {
            vfread(&(m_BoundInfo.aMagicPnt[i][0]), sizeof(int), 1, fp);
            vfread(&(m_BoundInfo.aMagicPnt[i][1]), sizeof(int), 1, fp);
        }
    }
    //}}}

    //
    if ( fh.version >= nFileVer_1_3 )
    {
        int nCount;
        vfread(&nCount, 1, sizeof(int), fp);
        if ( nCount > 0 )
        {
            if ( nCount > m_BoundInfo.ClipRoot.nClipCount )
            {
                if ( 0 != m_BoundInfo.ClipRoot.ptrClipBy )
                {
                    free(m_BoundInfo.ClipRoot.ptrClipBy);
                    m_BoundInfo.ClipRoot.ptrClipBy = (int*)malloc(sizeof(int)*nCount);
                }
            }
            m_BoundInfo.ClipRoot.ptrClipBx = (int*)malloc(sizeof(int)*nCount);
            m_BoundInfo.ClipRoot.ptrClipBw = (int*)malloc(sizeof(int)*nCount);
            int* tmpbuf = (int*)malloc(sizeof(int)*nCount*3);
            vfread(tmpbuf, 1, sizeof(int)*nCount*3, fp);
            for ( int i=0; i<nCount; i++ )
            {
                m_BoundInfo.ClipRoot.ptrClipBx[i] = tmpbuf[i*3+0];
                m_BoundInfo.ClipRoot.ptrClipBw[i] = tmpbuf[i*3+1];
                m_BoundInfo.ClipRoot.ptrClipBy[i] = tmpbuf[i*3+2];
            }
            free(tmpbuf);
            m_BoundInfo.ClipRoot.nClipWidth = 0; //��־
            m_BoundInfo.ClipRoot.nClipCount = nCount;
        }
    }

    
    m_BoundInfo.dwFlag = 0;
    if ( fh.version >= nFileVer_1_2 )
    {
		int nResIndex = (GET_IMAGE_INDEX(m_dwImageID)/100);
		int nResType = GET_IMAGE_TYPE(m_dwImageID);
		if ( (nResType >= libImageTypeBodyBegin && nResType <= libImageTypeBodyEnd)
			|| (nResType == libImageTypeNpc && IsRoleRes(nResIndex)))
		{
			m_BoundInfo.dwFlag = 1;
		}
    }


    nCols = 1;
    nRows = 1;
    nOldTotalFrames = m_nTotalFrames;

#ifndef _MAPEDITOR // ���´���ֻ����������

    nResType = GET_IMAGE_TYPE(m_dwImageID);
    
    //�ж��Ƿ��ǵ�֡��ͬʱͼ��Ŀ�Ȼ�߶ȴ���MAX_IMAGE_SIZE
    //����ǣ��򽫸�ͼ���з�
    if ( 1 == m_nTotalFrames 
        && ( nResType >= libImageTypeMapBegin 
             && nResType <= libImageTypeMapEnd 
             && nResType != libImageTypeYD )
        && ( (m_BoundInfo.nWidth-1) > MAX_IMAGE_SIZE
            || (m_BoundInfo.nHeight-1) > MAX_IMAGE_SIZE ) )
    {
        nCols = ((m_BoundInfo.nWidth-1) + MAX_IMAGE_SIZE - 1) / MAX_IMAGE_SIZE;
        nRows = ((m_BoundInfo.nHeight-1) + MAX_IMAGE_SIZE - 1) / MAX_IMAGE_SIZE;

        m_nTotalFrames = nCols*nRows;
        m_nDirFrames = m_nTotalFrames / max(m_nTotalDirs, 1);
    }

#endif
	//����ÿ֡����
	int i;
	m_FrameInfo = (TFrameInfo*)malloc(sizeof(TFrameInfo) * m_nTotalFrames);
	memset(m_FrameInfo, 0, sizeof(TFrameInfo) * m_nTotalFrames);
	for(i=0; i<nOldTotalFrames; i++)
	{
		vfread(&m_FrameInfo[i].nLeft, 1, sizeof(int), fp);
		vfread(&m_FrameInfo[i].nTop, 1, sizeof(int), fp);

        //Jim, add for file ver1.1 [2/10/2004]
        //�ļ��汾1.1��,֡��Ϣ�������������ݳ�Ա dwFlags, ������Ǹ�֡������.
        //{{{
        if(fh.version >= nFileVer_1_1)
        {
            vfread(&m_FrameInfo[i].dwFlags, 1, sizeof(DWORD), fp);
        }
        //}}}

        if ( nCols * nRows > 1 )
            m_FrameInfo[i].dwFlags = 2; // =2 ��ʾ�������һ֡��ͼ���зֳɶ�֡

        if ( !NakedReadImage(&m_FrameInfo[i].ptrImage, fp) )
            goto _error;

        m_FrameInfo[i].ptrMskImage = NULL;

        //Qjb, add [7/28/2004]
        //{{{
        m_FrameInfo[i].nCenterX = 0;
        m_FrameInfo[i].nCenterY = 0;
        m_FrameInfo[i].nMaskCenterX = 0;
        m_FrameInfo[i].nMaskCenterY = 0;
        m_FrameInfo[i].nPriority = 0;
        m_FrameInfo[i].ptrMskImage = 0;
        if ( fh.version >= nFileVer_1_2 )
        {
            // ���뱾֡�����ĵ�λ�ã�����ڱ�֡ͼ������Ͻ�
            vfread(&m_FrameInfo[i].nCenterX, 1, sizeof(m_FrameInfo[i].nCenterX), fp);
            vfread(&m_FrameInfo[i].nCenterY, 1, sizeof(m_FrameInfo[i].nCenterY), fp);

            // �������ȼ�
            vfread(&m_FrameInfo[i].nPriority, 1, sizeof(m_FrameInfo[i].nPriority), fp);

            // ����󶨵��б�
            if ( fh.nBindPointCounts > 0 )
            {
                vfread(&m_FrameInfo[i].bindPointList, fh.nBindPointCounts, sizeof(POINT), fp);
            }

            // �����ɫͨ��
            if ( fh.bHaveMask )
            {
                // ���뱾֡��ɫͨ�������ĵ�λ�ã�����ڱ�֡��ɫͨ��ͼ������Ͻ�
                vfread(&m_FrameInfo[i].nMaskCenterX, 1, sizeof(m_FrameInfo[i].nMaskCenterX), fp);
                vfread(&m_FrameInfo[i].nMaskCenterY, 1, sizeof(m_FrameInfo[i].nMaskCenterY), fp);
                NakedReadImage(&m_FrameInfo[i].ptrMskImage, fp);
            }
        }
        //}}}
	}

    if ( nCols * nRows > 1 )
    {
        // ͼ���ǵ�֡�ұ��з�
        int j, nFrameIdx=0, f_w, f_h, all_w, all_h, f_x, f_y;
        TImage * pImage = m_FrameInfo[0].ptrImage;
        all_w = pImage->w;
        all_h = pImage->h;
        for( i=0; i<nRows; i++ )
        {
            for ( j=0; j<nCols; j++ )
            {
                m_FrameInfo[nFrameIdx].nLeft = m_FrameInfo[0].nLeft + j*MAX_IMAGE_SIZE;
                m_FrameInfo[nFrameIdx].nTop = m_FrameInfo[0].nTop + i*MAX_IMAGE_SIZE;
                m_FrameInfo[nFrameIdx].dwFlags = m_FrameInfo[0].dwFlags;
                f_w = min(MAX_IMAGE_SIZE, all_w - j*MAX_IMAGE_SIZE);
                f_h = min(MAX_IMAGE_SIZE, all_h - i*MAX_IMAGE_SIZE);
                f_x = j*MAX_IMAGE_SIZE;
                f_y = i*MAX_IMAGE_SIZE;
                m_FrameInfo[nFrameIdx].ptrImage = CreateImage(f_w, f_h, pImage->attrib);
                if ( NULL != m_FrameInfo[nFrameIdx].ptrImage )
                {
                    BltImage(m_FrameInfo[nFrameIdx].ptrImage, -f_x, -f_y, pImage);
                }
                
                m_FrameInfo[nFrameIdx].ptrMskImage = m_FrameInfo[0].ptrMskImage;
                m_FrameInfo[nFrameIdx].nCenterX = m_FrameInfo[0].nCenterX;
                m_FrameInfo[nFrameIdx].nCenterY = m_FrameInfo[0].nCenterY;
                m_FrameInfo[nFrameIdx].nMaskCenterX = m_FrameInfo[0].nMaskCenterX;
                m_FrameInfo[nFrameIdx].nMaskCenterY = m_FrameInfo[0].nMaskCenterY;
                m_FrameInfo[nFrameIdx].nPriority = m_FrameInfo[0].nPriority;
                m_FrameInfo[nFrameIdx].ptrMskImage = m_FrameInfo[0].ptrMskImage;
                
                nFrameIdx++;
            }
        }

        DestroyImage(&pImage);
    }

    //Jim, add [2/10/2004]
    //���붯��ÿ���������Ϣ
    //{{{
    CreateDefaultDirInfo();
    if(fh.version >= nFileVer_1_1)
    {
        for(i=0; i<m_nTotalDirs; i++)
        {
            vfread(&m_DirInfo[i].nFrames, 1, sizeof(int), fp);
            vfread(&m_DirInfo[i].nHitOffsetX, 1, sizeof(int), fp);
            vfread(&m_DirInfo[i].nHitOffsetY, 1, sizeof(int), fp);
        }
    }
    //}}}

	//�������
	vfclose(fp);
    fp = NULL;
	
	return TRUE;
	
_error:
	m_dwImageID = INVALID_IMAGE_ID;
    if ( NULL != fp )
    {
        vfclose(fp);
        fp = NULL;
    }

	return FALSE;
}



/*********************************************************************
By PipoXu [2005:12:27] [15:47:28]
Description:  �Ƚ������ļ��Ƿ�һ��
In:
Out:  
Return: 1���,0����,-1��һ���ļ�������,-2�ڶ����ļ�������		     
*********************************************************************/
static int CompareFile(const char* f1, const char* f2)
{
	FILE* fp1 = fopen(f1, "rb");
	if(fp1 == NULL)// 
	{
		return -1;
	}

	FILE* fp2 = fopen(f2, "rb");
	if(fp2 == NULL)
	{
		fclose(fp1);
		return -2;
	}

	int len1,len2;
	len1 = filelength(fileno(fp1));
	len2 = filelength(fileno(fp2));

	if (len1 != len2)
	{
		fclose(fp1);
		fclose(fp2);
		return 0;
	}
	else
	{
		int i=0;
		for(i=0; i<len1; i++)
		{
			int ch1 = fgetc(fp1);
			int ch2 = fgetc(fp2);
			if (ch1 != ch2)
			{
				fclose(fp1);
				fclose(fp2);
				return 0;
			}
		}
		fclose(fp1);
		fclose(fp2);
		return 1;
	}
}



//Jim, change [2/10/2004]
//{{{
//BOOL CIDImage::SaveImageFile()
BOOL CIDImage::SaveImageFile(DWORD dwVersion /*= nFileVerLast*/, int curdir)
//}}}
{
    if ( curdir <= 0 )
    {
	    FILE *fp = NULL;

	    //���ļ�׼��д��
	    ImageID2FileName(m_dwImageID, m_szFileName);

		// {{{{{
		// By PipoXu [2005:12:27] [16:08:16]
		// Description: 
		CHAR	szTmpFile[_MAX_FNAME];	//Ӱ���ļ�����
		strcpy(szTmpFile,m_szFileName);
		strcat(szTmpFile,".tmp");
		// }}}}}
	    if(NULL == (fp = fopen((const char*)szTmpFile, "wb")))
	    {
		    goto _error;
	    }

	    //д���ļ�ͷ
	    TFileHeader fh;
	    InitFileHeader(&fh);
        fh.version = dwVersion;
	    fh.frames = m_nTotalFrames;
	    fh.dirs = m_nTotalDirs;

        if ( m_BoundInfo.nMagicDotCount >= 1 && m_BoundInfo.nMagicDotCount <= 5)
        {
            dwVersion = nFileVerLast;
            fh.version = dwVersion;
        }

        //Qjb, add [7/28/2004]
        //{{{
        if ( dwVersion >= nFileVer_1_2 )
        {
            fh.bBingFlag = m_BoundInfo.bBingFlag;
            fh.nBindPointCounts = m_BoundInfo.nBindPointCounts;
            fh.bHaveMask = m_BoundInfo.bHaveMask;
            fh.version = dwVersion;
        }
        //}}}

	    fwrite(&fh, 1, fh.head_size, fp);

	    //д���Χ������
	    fwrite(&m_BoundInfo.nWidth, 1, sizeof(int), fp);
	    fwrite(&m_BoundInfo.nHeight, 1, sizeof(int), fp);
	    fwrite(&m_BoundInfo.nCenterX, 1, sizeof(int), fp);
	    fwrite(&m_BoundInfo.nCenterY, 1, sizeof(int), fp);
	    fwrite(&m_BoundInfo.ClipRoot.nClipWidth, 1, sizeof(int), fp);
	    fwrite(&m_BoundInfo.ClipRoot.nClipCount, 1, sizeof(int), fp);
	    fwrite(m_BoundInfo.ClipRoot.ptrClipBy, 1, sizeof(int) * m_BoundInfo.ClipRoot.nClipCount, fp);

        //д�벥��ħ��λ�õ�
        //add by qjb[2/28/2005]
        //{{{
        if ( fh.version >= nFileVer_1_4 )
        {
            fwrite(&m_BoundInfo.nMagicDotCount, 1, sizeof(int), fp);
            for ( int i=0; i<m_BoundInfo.nMagicDotCount && i<5; i++ )
            {
                fwrite(&(m_BoundInfo.aMagicPnt[i][0]), sizeof(int), 1, fp);
                fwrite(&(m_BoundInfo.aMagicPnt[i][1]), sizeof(int), 1, fp);
            }
        }
        //}}}

        //�������¶���
        if ( fh.version >= nFileVer_1_3 )
        {
            int nCount = 0;
            fwrite(&nCount, 1, sizeof(int), fp);
        }
    
	    //д��ÿ֡����
	    int i;
	    for(i=0; i<m_nTotalFrames; i++)
	    {
        
		    fwrite(&m_FrameInfo[i].nLeft, 1, sizeof(int), fp);
		    fwrite(&m_FrameInfo[i].nTop, 1, sizeof(int), fp);

            //Jim, add [2/10/2004]
            //{{{
            if(dwVersion >= nFileVer_1_1)
            {
                fwrite(&m_FrameInfo[i].dwFlags, 1, sizeof(int), fp);
            }
            //}}}

		    NakedWriteImage(m_FrameInfo[i].ptrImage, fp);

            //Qjb, add [7/28/2004]
            //{{{
            if(dwVersion >= nFileVer_1_2)
            {
                // д�뱾֡�����ĵ�λ�ã�����ڱ�֡ͼ������Ͻ�
                fwrite(&m_FrameInfo[i].nCenterX, 1, sizeof(m_FrameInfo[i].nCenterX), fp);
                fwrite(&m_FrameInfo[i].nCenterY, 1, sizeof(m_FrameInfo[i].nCenterY), fp);
            
                // д����ʾ���ȼ���
                fwrite(&m_FrameInfo[i].nPriority, 1, sizeof(m_FrameInfo[i].nPriority), fp);

                // д��󶨵��б�
                if ( fh.nBindPointCounts > 0 )
                {
                    fwrite(&m_FrameInfo[i].bindPointList, fh.nBindPointCounts, sizeof(POINT), fp);
                }
            
                // д���ɫͨ��
                if ( fh.bHaveMask > 0 )
                {
                    // д�뱾֡��ɫͨ�������ĵ�λ�ã�����ڱ�֡��ɫͨ��ͼ������Ͻ�
                    fwrite(&m_FrameInfo[i].nMaskCenterX, 1, sizeof(m_FrameInfo[i].nMaskCenterX), fp);
                    fwrite(&m_FrameInfo[i].nMaskCenterY, 1, sizeof(m_FrameInfo[i].nMaskCenterY), fp);
                    NakedWriteImage(m_FrameInfo[i].ptrMskImage, fp);
                }
            }
            //}}}
        }

        //Jim, add [2/10/2004]
        //{{{
        if(dwVersion >= nFileVer_1_1)
        {
            if(!m_DirInfo)
            {
                CreateDefaultDirInfo();
            }

            for(i=0; i<m_nTotalDirs; i++)
            {
                fwrite(&m_DirInfo[i].nFrames, 1, sizeof(int), fp);
                fwrite(&m_DirInfo[i].nHitOffsetX, 1, sizeof(int), fp);
                fwrite(&m_DirInfo[i].nHitOffsetY, 1, sizeof(int), fp);
            }
        }
        //}}}


	    //д�����
	    fclose(fp);

		// {{{{{
		// By PipoXu [2005:12:27] [15:49:03]
		// Description:
		int cmpRet = CompareFile(szTmpFile,m_szFileName);
		if (cmpRet > 0) //�ļ�һ��,ɾtmp
		{
			int rt = DeleteFile(szTmpFile);
			assert(rt);
		}
		else //��һ��,ɾԭ��tmp
		{
			if( cmpRet != -2 ) //ԭ�ļ�������
			{
				int rt = DeleteFile(m_szFileName);
				assert(rt);

			}
			int rt = MoveFile(szTmpFile,m_szFileName);
			assert(rt);
		}
		// }}}}}
    }
    else // ֻ��magic��������
    {
        if ( m_nTotalDirs <=1 || curdir > m_nTotalDirs )
            return FALSE;

	    FILE *fp = NULL;

	    //���ļ�׼��д��
        DWORD dwType = GET_IMAGE_TYPE(m_dwImageID);
        int nIndex = GET_IMAGE_INDEX(m_dwImageID);
        nIndex = (nIndex|(curdir<<16));
        DWORD dwID = MAKE_IMAGE_ID(dwType, nIndex);
	    ImageID2FileName(dwID, m_szFileName);
	    if(NULL == (fp = fopen((const char*)m_szFileName, "wb")))
	    {
		    goto _error;
	    }

	    //д���ļ�ͷ
	    TFileHeader fh;
	    InitFileHeader(&fh);
	    fh.frames = m_nTotalFrames/m_nTotalDirs;
	    fh.dirs = 1;

        //Qjb, add [7/28/2004]
        //{{{
        if ( dwVersion >= nFileVer_1_2 )
        {
            fh.bBingFlag = m_BoundInfo.bBingFlag;
            fh.nBindPointCounts = m_BoundInfo.nBindPointCounts;
            fh.bHaveMask = m_BoundInfo.bHaveMask;
            fh.version = dwVersion;
        }
        //}}}

	    fwrite(&fh, 1, fh.head_size, fp);

	    //д���Χ������
	    fwrite(&m_BoundInfo.nWidth, 1, sizeof(int), fp);
	    fwrite(&m_BoundInfo.nHeight, 1, sizeof(int), fp);
	    fwrite(&m_BoundInfo.nCenterX, 1, sizeof(int), fp);
	    fwrite(&m_BoundInfo.nCenterY, 1, sizeof(int), fp);
	    fwrite(&m_BoundInfo.ClipRoot.nClipWidth, 1, sizeof(int), fp);
	    fwrite(&m_BoundInfo.ClipRoot.nClipCount, 1, sizeof(int), fp);
	    fwrite(m_BoundInfo.ClipRoot.ptrClipBy, 1, sizeof(int) * m_BoundInfo.ClipRoot.nClipCount, fp);


        //д�벥��ħ��λ�õ�
        //add by qjb[2/28/2005]
        //{{{
        if ( fh.version >= nFileVer_1_4 )
        {
            fwrite(&m_BoundInfo.nMagicDotCount, 1, sizeof(int), fp);
            for ( int i=0; i<m_BoundInfo.nMagicDotCount && i<5; i++ )
            {
                fwrite(&(m_BoundInfo.aMagicPnt[i][0]), sizeof(int), 1, fp);
                fwrite(&(m_BoundInfo.aMagicPnt[i][1]), sizeof(int), 1, fp);
            }
        }
        //}}}
        
        //�������¶���
        if ( fh.version >= nFileVer_1_3 )
        {
            int nCount = 0;
            fwrite(&nCount, 1, sizeof(int), fp);
        }
    
	    //д��ÿ֡����
	    int i;
	    for(i=(curdir-1)*fh.frames; i<curdir*fh.frames; i++)
	    {
        
		    fwrite(&m_FrameInfo[i].nLeft, 1, sizeof(int), fp);
		    fwrite(&m_FrameInfo[i].nTop, 1, sizeof(int), fp);

            //Jim, add [2/10/2004]
            //{{{
            if(dwVersion >= nFileVer_1_1)
            {
                fwrite(&m_FrameInfo[i].dwFlags, 1, sizeof(int), fp);
            }
            //}}}

		    NakedWriteImage(m_FrameInfo[i].ptrImage, fp);

            //Qjb, add [7/28/2004]
            //{{{
            if(dwVersion >= nFileVer_1_2)
            {
                // д�뱾֡�����ĵ�λ�ã�����ڱ�֡ͼ������Ͻ�
                fwrite(&m_FrameInfo[i].nCenterX, 1, sizeof(m_FrameInfo[i].nCenterX), fp);
                fwrite(&m_FrameInfo[i].nCenterY, 1, sizeof(m_FrameInfo[i].nCenterY), fp);
            
                // д����ʾ���ȼ���
                fwrite(&m_FrameInfo[i].nPriority, 1, sizeof(m_FrameInfo[i].nPriority), fp);

                // д��󶨵��б�
                if ( fh.nBindPointCounts > 0 )
                {
                    fwrite(&m_FrameInfo[i].bindPointList, fh.nBindPointCounts, sizeof(POINT), fp);
                }
            
                // д���ɫͨ��
                if ( fh.bHaveMask > 0 )
                {
                    // д�뱾֡��ɫͨ�������ĵ�λ�ã�����ڱ�֡��ɫͨ��ͼ������Ͻ�
                    fwrite(&m_FrameInfo[i].nMaskCenterX, 1, sizeof(m_FrameInfo[i].nMaskCenterX), fp);
                    fwrite(&m_FrameInfo[i].nMaskCenterY, 1, sizeof(m_FrameInfo[i].nMaskCenterY), fp);
                    NakedWriteImage(m_FrameInfo[i].ptrMskImage, fp);
                }
            }
            //}}}
        }

        //Jim, add [2/10/2004]
        //{{{
        if(dwVersion >= nFileVer_1_1)
        {
            if(!m_DirInfo)
            {
                CreateDefaultDirInfo();
            }

            fwrite(&m_DirInfo[curdir-1].nFrames, 1, sizeof(int), fp);
            fwrite(&m_DirInfo[curdir-1].nHitOffsetX, 1, sizeof(int), fp);
            fwrite(&m_DirInfo[curdir-1].nHitOffsetY, 1, sizeof(int), fp);
        }
        //}}}


	    //д�����
	    fclose(fp);
    }


	return TRUE;

_error:
	return FALSE;
}


//Jim, add [2/10/2004]
//{{{
TDirInfo * CIDImage::GetDirInfo(int nDir)
{
    if(! m_DirInfo)
        return NULL;

    if(nDir <0 || nDir >= m_nTotalDirs)
        return NULL;

    return &m_DirInfo[nDir];
}

BOOL CIDImage::CreateDefaultDirInfo()
{
    if(m_DirInfo) free(m_DirInfo);

    m_DirInfo = (TDirInfo*)malloc(sizeof(TDirInfo) * m_nTotalDirs);
    memset(m_DirInfo, 0, sizeof(TDirInfo) * m_nTotalDirs);
    
    int i;
    for(i=0; i<m_nTotalDirs; i++)
    {
        m_DirInfo[i].nFrames = m_nDirFrames;
        m_DirInfo[i].nHitOffsetX = 0;
        m_DirInfo[i].nHitOffsetY = 0;
    }

    return TRUE;
}


void CIDImage::ComputeBoundSize(int &left, int &top, int &right, int &bottom){
    left = 999999;
    top = 999999;
    right = 0;
    bottom = 0;

    for( int i=0; i<m_nTotalFrames; i++){
        TFrameInfo *p = &m_FrameInfo[i];
        if(!p) continue;
        if(!p->ptrImage) continue;	//null bmp
			
        if(p->nLeft < left)
            left = p->nLeft;
        if(p->nTop < top)
            top = p->nTop;

        if(p->ptrImage){
            if(p->nLeft + p->ptrImage->w > right)
                right = p->nLeft + p->ptrImage->w;
            if(p->nTop + p->ptrImage->h > bottom)
                bottom = p->nTop + p->ptrImage->h;
        }
    }

    left = max(0, left);
    top = max(0, top);

    m_BoundInfo.nWidth = max(0, right - left);
    m_BoundInfo.nHeight = max(0, bottom - top);

    right = left + m_BoundInfo.nWidth;
    bottom = top + m_BoundInfo.nHeight;
}
//}}}


//����: x, y -> x/y������հ�Χ�����Ͻǵ�
//      iDistance -> �����ľ���
//      bExt -> ��չ���, true��ʱ�����չ����������Ϊ�ж���
int CIDImage::GetNearestClipLineIndex(int x, int y, int iDistance, bool bExt)
{
    SLONG   n, clip_by;
    
    // �ſ�߽�����ļ��ռ�, ����༭����
    if(!bExt)
    {
        if(x < 0 || x >= m_BoundInfo.nWidth)
            return  -1;
    }
    
    // �жϸõ�������������ĸ��ü�����
    n = x / m_BoundInfo.ClipRoot.nClipWidth;
    
    // �����ڵ�һ���ü������󣬻����ڵ� clip_count ���ü������ң�
    // ���ʾû�нӽ��òü���
    if(n < 0 || n > m_BoundInfo.ClipRoot.nClipCount - 1)
        return  -1;
    
    // �Ƚϸõ�͸òü���������ˮƽ��ƫ������ Y 
    clip_by = m_BoundInfo.ClipRoot.ptrClipBy[n];
    if(abs(y - clip_by) < iDistance)
        return  n;
    
    return  -1;
}

//delta, change [8/12/2004]
//{{{
bool CIDImage::IsImageExist( DWORD dwID )
{
	CHAR fullpath[256];
	CIDImage::ImageID2FileName( dwID, fullpath );

	return vfexists(fullpath);
}
//}}}
