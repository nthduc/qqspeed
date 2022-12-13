////////////////////////////////////////////////
//FileName:    ItemDefine.h
//Author:      PigHuang
//Date:        2007-03-13
//Description: Avatarװ��ϵͳ�����ļ��������ĵ�"QQ������Avatarϵͳ���.doc"
////////////////////////////////////////////////
//Item����It��д����
#pragma once
#include "../network/msgheader.h"
#include <list>
using namespace std;

const UINT conAvatarMaxAlgInItem=6;
const UINT conAvatarMaxNormalParamNum=5;
///װ����������
typedef enum enmItBaseType
{
	EAIBT_ROLERELEVANT = 0x00000100, ///��ɫ�������
	EAIBT_CARRELEVANT = 0x00000200, ///�����������
	EAIBT_NORELEVANT = 0x00000300, ///���ɫ�����޹ص�����
	EAIBT_NOENTITY = 0x00000400, ///��ʵ������
	EAIBT_ROLE = 0x00000500, ///��ɫ
	EAIBT_CAR = 0x00000600, ///����
	EAIBT_HAIR= 0x00000700, //ͷ��
}ENMItBaseType;

///װ�������ͣ���Ҫ��ϻ������Ͳ�������
typedef enum enmItSubType
{
	EAIT_RR_TOPHEAD = 0x00000101, ///��ɫͷ��
	EAIT_RR_GLASS = 0x00000102, ///��ɫ�۾�
	EAIT_RR_FACE = 0x00000103, ///��ɫ�沿
	EAIT_RR_DV= 0x00000104, ///��ɫ�����
	EAIT_CR_TEXTURE = 0x00000201, ///�������ƻ�����
	EAIT_CR_PAINT = 0x00000202, ///��������
	EAIT_CR_BRAND =0x00000203, ///��������
	EAIT_CR_TAIL = 0x00000204, ///����β��
	EAIT_CR_LAMPSEPCIAL = 0x00000205, ///����������Ч
	EAIT_CR_BRANDSPECIAL = 0x00000206, ///����������Ч
	EAIT_CR_BOTTOMLAMP = 0x00000207, ///�����׵�
	EAIT_CR_EJECTSEPCIAL = 0x00000208, ///����������Ч
	EAIT_NR_TYREMARK = 0x00000301, ///��̥ӡ
	EAIT_NR_DRIFTSPECIAL = 0x00000302, ///Ư����Ч
	EAIT_NE_ROOMSPECIAL = 0x00000401, ///������Ч
	EAIT_NE_SCENESPECIAL = 0x00000402, ///����������Ч
	EAIT_NE_WINSPECIAL = 0x00000403, ///ʤ����Ч
	EAIT_NE_PROPSPECIAL = 0x00000404, ///��Ϸ������Ч
	EAIT_NE_EXPRESSIONSPECIAL = 0x00000405, ///������Ч
	EAIT_NE_ROLEATTRIBUTECARD = 0x00000406, ///�������Կ�
	EAIT_NE_PROPEXCHANGE = 0x00000407, ///���߻�λ��
	EAIT_ROLE_BASE = 0x00000501, ///�����ɫ
	EAIT_CAR_BASE = 0x00000601, ///����

	EAIT_ROLE_NEWTEST=0x00000502,	///�������½�ɫϵ��	
	EAIT_CAR_NEWTEST=0x00000602,	///������������ϵ��
	EAIT_HAIR_NEWTEST=0x00000702,	///������ͷ��
}ENMItSubType;

/*
//Item���ݳ���ķ���
typedef enum enmGarageType
{
	EIGT_KART=1,	///����
	EIGT_PET=2,		///����
	EIGT_EQUIP=3,	///װ���õ���
	EIGT_ADORN=4,	///װ���õ���
}ENMItGarageType;
*/

///��Ʒ������(Ҳ�����˳���ķ���)
typedef enum enmCommodityBaseType
{
	ECBT_BASE_KART 			= 1,		///����
	ECBT_BASE_CHARACTOR 	= 2,		///��ɫ
	ECBT_BASE_KARTEQP		= 4,		///����װ��
	ECBT_BASE_CHARACTOREQP	= 8,		///��ɫװ��
	ECBT_BASE_PET			= 16,		///����
	ECBT_BASE_EFFECT		= 32,		///��Ч
	ECBT_BASE_OTHER			= 64,		///����
	ECBT_BASE_HOTRECOMMAND	= 128,		///�����Ƽ�
}ENMCommodityBaseType;

//װ��ʹ����Դ����(2��n������ֵ���������������־ֵ)
typedef enum enmItResType
{
	EIRES_SIG_NIF=1,		///��ģ��
	EIRES_SIG_TEX=2,		///����ͼ
	EIRES_MIX_SIG_NIFTEX=3,	///��ϵ�ģ����ͼ
	EIRES_MUL_NIF=4,		///��ģ��
	EIRES_MUL_TEX=8,		///����ͼ
	EIRES_MIX_MUL_NIFTEX=12,///��ϵ�ģ����ͼ
	EIRES_TEX_COLOR=16,		///����ɫ����
	EIRES_SIG_COLOR_TEX=18,	///����ɫ���Ե���ͼ
	EIRES_SIG_SKIN=32,		///����������
	EIRES_MIX_SIG_SKINNIF=33,	///��������ģ��
	EIRES_EXT_MODE=512,		///��չģʽ��ʹ�ö������ʹ���
}ENMItResType;

///װ�������㷨����
typedef enum enmItPropAlgType
{
	EIAAT_ADDMONEY = 1000,			///���Ͻ��, ������3��: ����, �������, �Ƿ���Ҫ�������Կ�(0��1)
	EIAAT_ADDEXPERIENCE = 1001,		///���Ͼ���, ������3��: ����, ��������, �Ƿ���Ҫ�������Կ�(0��1)
	EIAAT_MULTIPLYMONEY = 1002,		///���Խ�ұ���, ������3��: ����, ��ұ���, �Ƿ���Ҫ�������Կ�(0��1)
	EIAAT_MULTIPLYEXPERIENCE = 1003,	///���Ծ��鱶��, ������3��: ����, ���鱶��, �Ƿ���Ҫ�������Կ�(0��1)
	EIAAT_MONEYBONUS = 1004,		///��ҽ���, ������4��: ����, ��Ȩ����, ��Ȩֵ, �Ƿ���Ҫ�������Կ�(0��1)
	EIAAT_EXPERIENCEBONUS = 1005,	///���齱��, ������4��: ����, ��Ȩ����, ��Ȩֵ, �Ƿ���Ҫ�������Կ�(0��1)
//	EIAAT_PROPTRANSFER = 1006,		///����ת��, ������3��:ת������, Դ����ID, Ŀ�ĵ���ID
	EIAAT_EFFECTTRANSFER = 1007,	///Ч��ת��, ������5����ת������, ԴЧ��ID, Ŀ��Ч��ID, ����ֵ, �������
//	EIAAT_PROPEXCHANGE = 1008,		///���߻�λ, ��������
//	EIAAT_ROLEATTRIBUTECARD = 1009,	///�������Կ�����������
//	EIAAT_NOFOG = 1010,				///��͸�Ʋ�, ��������
//	EIAAT_EXTRAEXPRESSION = 1011,	///�������, ��������
//	EIAAT_WINFE = 1012,				///ʤ����Ч, ������1��: ʤ����ЧID
//	EIAAT_ROOMFE = 1013,			///������Ч, ������1��: ������ЧID
	EIAAT_SCENEFE = 1014,			///������Ч, ���������5��: ������������ID
//	EIAAT_PHYSICSPARAMETER = 1015,	///�������, ������
}ENMItPropAlgType;

///װ��ʹ����Դ����
typedef enum enmItQueryItemType
{
	EIQIT_ALL_ITEM=1,		///��������item
	EIQIT_EQUIPPED_ITEM=2,	///������װ��item
	EIQIT_UNEQUIPPED_ITEM=3,///����δװ��item
	EIQIT_MAX=255,
}ENMItQueryItemType;

///װ���Ա�����
typedef enum enmItSexType
{
	EASE_MALE=1,	//����
	EASE_FEMALE=2,	//Ů��
}ENMItSexType;

///�ҽӵ㵥Ԫ�ṹ
typedef struct tagItHitchUnit
{
	UINT			nType;		///�ҽӵ�����
	string			strName;	///�ҽӵ�����
	UINT			nDefaultItem;	///Ĭ�Ϲҽӵ�itemid��û����Ϊ0
	tagItHitchUnit();
	~tagItHitchUnit();
}ItHitchUnit,*PItHitchUnit;

typedef map<UINT,ItHitchUnit>	ITEMHITCH_MAP;	//�Ըýṹ(��)nTypeΪ����
typedef list<UINT>	ITEMHITCH_LIST;	//�Ըýṹ(��)IdΪ����
///�Ըýṹ(��)nType�ҽӵ�Ϊ��������¼�ùҽӵ㵱ǰ��item id��������������ڻ�ֵ=0����
///�ùҽӵ�û��item
typedef map<UINT,UINT>		ITEMONHITCH_MAP;

/////�㷨��Ԫ�ṹ
//typedef struct tagItAlgUnit
//{
//	UINT		nId;		///�㷨Id
//	UINT		nOptNum;	///����������
//	int			iarrOptVal[conAvatarMaxAlgInItem];	///��������
//	tagItAlgUnit();
//	~tagItAlgUnit();
//}ItAlgUnit,*PItAlgUnit;
//
/////�㷨����ṹ
//typedef struct tagItAlgMgr
//{
//	UINT nTriOccation;	///����ʱ��
//	UINT nTriParamNum;	///�����������������5��
//	UINT nAlgNum;		///�㷨����
//	UINT narrTriParam[conAvatarMaxNormalParamNum];	///��������
//	ItAlgUnit	arrAlg[conAvatarMaxNormalParamNum];	///�㷨����������
//	tagItAlgMgr();
//	~tagItAlgMgr();
//}ItAlgMgr,*PItAlgMgr;


//װ��������Ϊ����
typedef enum enmBehaveID
{
	EBI_Animation = 1,	//���Ŷ���
	EBI_Effect = 2,		//������Ч
	EBI_Sound = 3,		//��������	
	EBI_MAX = 4,
}ENMBEHAVEID;


//װ��ת���㷨������
typedef enum enmOperatorID
{
	EOI_Add = 0,	//��
	EOI_Mul = 1,	//��
}ENMOPERATORID;


//ת���㷨����
typedef enum enmAlgorithmID
{
	EAI_GPTansfer = 1,		//��һ������ת�����߹���
	EAI_GPReduceTime = 2,	//��һ�����ʼ��ٹ�������ʱ��
}ENMALGORITHMID;


//װ��ӵ�еĸ���ת���㷨����
typedef struct tagAlgData
{
	DWORD dwType;		//����
	DWORD dwOccasion;	//����ʱ��
	SHORT shOpertor;	//����
	DWORD dwCondition;	//��������
	DWORD dwExpectation;//����ֵ	
	DWORD dwRate;		//�õ�����ֵ�ĸ���
}ALGDATA, *PALGDATA, *LPALGDATA;

typedef list<ALGDATA> AlgDataList;
typedef list<ALGDATA>::const_iterator AlgDataList_It;

//װ�����еĶ���ת��
typedef struct tagAnimData
{
	DWORD dwCondition;		//Դ����
	DWORD dwExpectation;	//ת������
}ANIMDATA, *PANIMDATA, *LPANIMDATA;

typedef list<ANIMDATA> AnimDataList;
typedef list<ANIMDATA>::const_iterator AnimDataList_It;

typedef enum enmAttachType
{
	EATT_Wearer	= 0,
	EATT_World	= 1,
}ATTACHTYPE;

//װ��ӵ�е�������Ϊ
typedef struct tagBehaveData
{
	DWORD dwType;		//����
	DWORD dwOccasion;	//����ʱ��
	DWORD dwCondition;	//��������
	DWORD dwLife;		//��������
	DWORD dwParam;		//����
	DWORD dwOption;		//���ɲ���
}BEHAVEDATA, *PBEHAVEDATA, *LPBEHAVEDATA;

typedef list<BEHAVEDATA> BehaveDataList;
typedef list<BEHAVEDATA>::const_iterator BehaveDataList_It;


//��������ɫ����������
typedef struct tagPhysicProperty
{
	SHORT shOpertor;	//�������������
	DWORD dwSpeed;		//�ٶ�����
	DWORD dwErupt;		//��������
	DWORD dwAccumNo2;	//���۵�������
	DWORD dwBigNo2;		//��������
	DWORD dwMiniNo2;	//С������

	tagPhysicProperty& operator =(const tagPhysicProperty& rhs)
	{
		this->dwSpeed		= rhs.dwSpeed;
		this->dwErupt		= rhs.dwErupt;
		this->dwAccumNo2	= rhs.dwAccumNo2;
		this->dwBigNo2		= rhs.dwBigNo2;
		this->dwMiniNo2		= rhs.dwMiniNo2;		
		return *this;
	}
	friend tagPhysicProperty operator +(const tagPhysicProperty& lhs, const tagPhysicProperty& rhs)
	{
		tagPhysicProperty result;
		result.dwSpeed		= lhs.dwSpeed		+ rhs.dwSpeed;
		result.dwErupt		= lhs.dwErupt		+ rhs.dwErupt;
		result.dwAccumNo2	= lhs.dwAccumNo2	+ rhs.dwAccumNo2;
		result.dwBigNo2		= lhs.dwBigNo2		+ rhs.dwBigNo2;
		result.dwMiniNo2	= lhs.dwMiniNo2		+ rhs.dwMiniNo2;
		return result;
	}


}PHYSICPROPERTY, *PPHYSICPROPERTY, *LPPHYSICPROPERTY;


//�Ƿ�ɽ�������λ��
typedef enum enmBoolFlag
{
	BFG_Forbid = 0,	//��ֹ
	BFG_Can = 1,	//����
};

typedef struct tagAvatarProperty
{
	SHORT			shSwitch;		//�Ƿ�ɽ�������λ��
	SHORT			shNoFog;		//�Ƿ�ɿ�������
	PHYSICPROPERTY	tagPhyPrp;		//��ɫ��������	
	AnimDataList	listAnimData;	//avatarӵ�е����⶯������
	BehaveDataList	listBehaveData;	//avatarӵ����Ϊ�����б�
	AlgDataList		listAlgData;	//�㷨�����б�
}AVATARPROPERTY, *PAVATARPROPERTY, *LPAVATARPROPERTY;

//����ɫ�õ�ɫ������
typedef struct tagSColorAttr{
	int		hue;
	float	sat;
	float	val;
}SColorAttr;

//������Ϣ����id����������
typedef struct tagGPInfo
{
	DWORD dwID;
	DWORD dwLife;
	SHORT sResult;
	DWORD dwItemID;
}GPINFO;


///װ����Ԫ�ṹ
typedef struct tagItemUnit
{
	UINT		nId;		///װ��Id
	UINT		nType;		///װ������
	string		strName;	///װ������
//	UINT		nAlgNum;	///װ�������㷨����
//	ItAlgMgr	insAlgMgr;	///�㷨ģ��
	UINT		nRoleBindNum;	///��ɫ�󶨵ĸ���
	UINT		arrRoleBindId[conAvatarMaxNormalParamNum];	///��ɫ�󶨵�ID
	UINT		nCarBindNum;	///�����󶨵ĸ���
	UINT		arrCarBindId[conAvatarMaxNormalParamNum];	///�����󶨵�ID
	UINT		nBlockSlotNum;	///���ιҽӵ�ĸ���,!!���������ҽӵ�
	UINT		arrBlockId[conAvatarMaxNormalParamNum];	///���ιҽӵ��ID
	UINT		nRepellRoleNum;	///�ų��ɫ�ĸ���
	UINT		arrRepellId[conAvatarMaxNormalParamNum];	///�ų��ɫ��ID
	UINT		nResType;		///װ��ʹ����Դ���ͣ���enmItResType
	UINT		nGarageType;	///����չʾ������
	UINT		nSexType;		///���ߵ��Ա����ͣ���ItSexType
	UINT		nHideRoleFoot;		///�Ƿ���Ҫ�ڱ�����������ҽţ�1=���أ�0=������
	ITEMHITCH_LIST	liHitchList;	///��װ�����еĹҽӵ㣬�����ɫ��
	//NiNodePtr	pkBaseNode;			///ԭʼģ��O
	//NiSourceTexturePtr pkBaseTexture;	///�滻��ͼ��ģ�ͼ�¼ָ�룬���糵��
	//DWORD		dwScreenObject;	///������Ļ��ʾ�ľ��
	string		strDesc;	///װ���������ƣ��̵�򳵿���ʾ��
	string		strModelName;	///ģ���ڲ��������������ܽڵ�����
	string		strOriTexName;	///ԭʼ��ͼ·����������ͼ��Item
	AVATARPROPERTY tagProperty;	///�����߼�����
	SColorAttr	insColor;		///��ɫ�õ�����
	std::deque<std::string>deChgColorName;	///��Ҫ��ɫ��ģ�����ƣ��������
	std::deque<std::string>deHideObjName;	///��Ϸ������Ҫ���ص�ģ�����ƣ��������
	tagItemUnit();
	~tagItemUnit();
}ItemUnit,*PItemUnit;

typedef map<UINT,ItemUnit>	ITEMUNIT_MAP;	//�Ըýṹ(��)IdΪ����

///װ����¼��Ԫ�ṹ
typedef struct tagItemStore
{
	STPlayerDBItemInfo	insInfo;	///�ۺ���Ϣ
	//ITEMHITCH_MAP	maHitchExt;		///���ڱ�ʵ��ʹ�õĹҽӵ㣬�Ǳ�׼�ҽӵ�ĸ���
										///����������ԭ�����п��ܹҽӵ��������ʵ�����
	NiNodePtr		pkNode;			///��ʵ��ģ�͵��ã�clone��ItemUnit
	tagItemStore();
	~tagItemStore();
	///copyָ��Դ��Store�����豾ʵ��
	void CopyItemStore(tagItemStore* pStoreSrc);
	///��ȡ��storeʵ����ģ������
	NiNodePtr GetItemNode(void);
}ItemStore,*PItemStore;

typedef map<UINT,ItemStore>		ITEMSTORE_MAP;	//�Ըýṹ(��)IdΪ����
typedef map<UINT,ItemStore*>	ITEMPOINTER_MAP;	//�Ըýṹ(��)IdΪ����


///�ҽ�ʵ�尴���ͽ��з���ṹ
typedef struct tagItTypeObj
{
	UINT			nType;			///ʵ������nType(���ݷ���������)
	ITEMSTORE_MAP	maItemStore;	///Ŀǰʵ�����е�װ���б�
	tagItTypeObj();
	~tagItTypeObj();
}ItTypeObj,*PItTypeObj;

typedef map<UINT,ItTypeObj>		ITEMOBJ_MAP;	//�Ըýṹ(��)nTypeΪ����

///�ҽ�ʵ�尴���������з���ṹ
typedef struct tagGarageTypeObj
{
	UINT			nType;			///�������nType(���ݷ���������)
	ITEMPOINTER_MAP	maStorePt;		///Ŀǰʵ�����е�װ��ָ���б�
	tagGarageTypeObj();
	~tagGarageTypeObj();
}GarageTypeObj,*PGarageTypeObj;

typedef map<UINT,ItTypeObj>		ITEMOBJ_MAP;	//�Ըýṹ(��)nTypeΪ����
typedef map<UINT,GarageTypeObj>		ITEMGARAGE_MAP;	//�Գ������nTypeΪ����

///�û�Avatar����Ԫ�ṹ
typedef struct tagItPlayerUint
{
	ULONG			nlUin;			///�û�����Id
	ITEMOBJ_MAP		maHObjList;		///��ʵ�����йҽ�ʵ����б�
	ITEMONHITCH_MAP	maItemOnHitch;	///��¼�ùҽӵ㵱ǰ��item id
	ITEMSTORE_MAP	maBackupInfo;	///��������Avatar��Ϣʹ��
	ITEMGARAGE_MAP	maGarageList;	///���ݳ�������
	tagItPlayerUint();
	~tagItPlayerUint();
}ItPlayerUint,*PItPlayerUint;

///ItemObjUnit������ULONGΪ�û�����(QQ����)
typedef map<ULONG,ItPlayerUint*>		ITEMPLAYER_MAP;	//���û�����(QQ����)Ϊ����

///��ѯ�û�Avatar���ݽṹ������_In�Ĳ���Ϊ���뺯���õĲ����_Out�Ĳ���Ϊ�������
typedef struct tagQueryPlayerAvatarInfo
{
	ULONG	m_unUin_In;				///�û�����Uin
	UCHAR	m_cQueryMode_In;		///��ѯģʽ����enmItQueryItemType
	USHORT	m_nItemNum_InOut;	///��ѯ���ص���Чװ������
	STPlayerDBItemInfo m_stItemInfo_Out[MAXITEMNUMPERPLAYER];	///װ����Ϣ
	tagQueryPlayerAvatarInfo();
	~tagQueryPlayerAvatarInfo();
}QueryPlayerAvatarInfo,*PQueryPlayerAvatarInfo;