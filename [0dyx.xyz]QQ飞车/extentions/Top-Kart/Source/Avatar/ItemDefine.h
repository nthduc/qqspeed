////////////////////////////////////////////////
//FileName:    ItemDefine.h
//Author:      PigHuang
//Date:        2007-03-13
//Description: Avatar装备系统定义文件，配套文档"QQ卡丁车Avatar系统设计.doc"
////////////////////////////////////////////////
//Item可用It缩写代替
#pragma once
#include "../network/msgheader.h"
#include <list>
using namespace std;

const UINT conAvatarMaxAlgInItem=6;
const UINT conAvatarMaxNormalParamNum=5;
///装备基本类型
typedef enum enmItBaseType
{
	EAIBT_ROLERELEVANT = 0x00000100, ///角色相关类型
	EAIBT_CARRELEVANT = 0x00000200, ///赛车相关类型
	EAIBT_NORELEVANT = 0x00000300, ///与角色赛车无关的类型
	EAIBT_NOENTITY = 0x00000400, ///非实体类型
	EAIBT_ROLE = 0x00000500, ///角色
	EAIBT_CAR = 0x00000600, ///赛车
	EAIBT_HAIR= 0x00000700, //头发
}ENMItBaseType;

///装备子类型，需要结合基本类型才有意义
typedef enum enmItSubType
{
	EAIT_RR_TOPHEAD = 0x00000101, ///角色头顶
	EAIT_RR_GLASS = 0x00000102, ///角色眼镜
	EAIT_RR_FACE = 0x00000103, ///角色面部
	EAIT_RR_DV= 0x00000104, ///角色摄像机
	EAIT_CR_TEXTURE = 0x00000201, ///赛车花纹或纹理
	EAIT_CR_PAINT = 0x00000202, ///赛车喷漆
	EAIT_CR_BRAND =0x00000203, ///赛车车牌
	EAIT_CR_TAIL = 0x00000204, ///赛车尾挂
	EAIT_CR_LAMPSEPCIAL = 0x00000205, ///赛车车灯特效
	EAIT_CR_BRANDSPECIAL = 0x00000206, ///赛车车牌特效
	EAIT_CR_BOTTOMLAMP = 0x00000207, ///赛车底灯
	EAIT_CR_EJECTSEPCIAL = 0x00000208, ///赛车喷射特效
	EAIT_NR_TYREMARK = 0x00000301, ///轮胎印
	EAIT_NR_DRIFTSPECIAL = 0x00000302, ///漂移特效
	EAIT_NE_ROOMSPECIAL = 0x00000401, ///房间特效
	EAIT_NE_SCENESPECIAL = 0x00000402, ///赛道场景特效
	EAIT_NE_WINSPECIAL = 0x00000403, ///胜利特效
	EAIT_NE_PROPSPECIAL = 0x00000404, ///游戏道具特效
	EAIT_NE_EXPRESSIONSPECIAL = 0x00000405, ///表情特效
	EAIT_NE_ROLEATTRIBUTECARD = 0x00000406, ///人物属性卡
	EAIT_NE_PROPEXCHANGE = 0x00000407, ///道具换位卡
	EAIT_ROLE_BASE = 0x00000501, ///人物角色
	EAIT_CAR_BASE = 0x00000601, ///赛车

	EAIT_ROLE_NEWTEST=0x00000502,	///测试用新角色系列	
	EAIT_CAR_NEWTEST=0x00000602,	///测试用新赛车系列
	EAIT_HAIR_NEWTEST=0x00000702,	///新人物头发
}ENMItSubType;

/*
//Item根据车库的分类
typedef enum enmGarageType
{
	EIGT_KART=1,	///车辆
	EIGT_PET=2,		///宠物
	EIGT_EQUIP=3,	///装备用道具
	EIGT_ADORN=4,	///装饰用道具
}ENMItGarageType;
*/

///商品主类型(也涵盖了车库的分类)
typedef enum enmCommodityBaseType
{
	ECBT_BASE_KART 			= 1,		///赛车
	ECBT_BASE_CHARACTOR 	= 2,		///角色
	ECBT_BASE_KARTEQP		= 4,		///赛车装备
	ECBT_BASE_CHARACTOREQP	= 8,		///角色装备
	ECBT_BASE_PET			= 16,		///宠物
	ECBT_BASE_EFFECT		= 32,		///特效
	ECBT_BASE_OTHER			= 64,		///其他
	ECBT_BASE_HOTRECOMMAND	= 128,		///人气推荐
}ENMCommodityBaseType;

//装备使用资源类型(2的n次幂数值可自由组合其他标志值)
typedef enum enmItResType
{
	EIRES_SIG_NIF=1,		///单模型
	EIRES_SIG_TEX=2,		///单贴图
	EIRES_MIX_SIG_NIFTEX=3,	///混合单模型贴图
	EIRES_MUL_NIF=4,		///多模型
	EIRES_MUL_TEX=8,		///多贴图
	EIRES_MIX_MUL_NIFTEX=12,///混合单模型贴图
	EIRES_TEX_COLOR=16,		///带颜色属性
	EIRES_SIG_COLOR_TEX=18,	///带颜色属性单贴图
	EIRES_SIG_SKIN=32,		///带骨骼数据
	EIRES_MIX_SIG_SKINNIF=33,	///带骨骼单模型
	EIRES_EXT_MODE=512,		///扩展模式，使用独立类型处理
}ENMItResType;

///装备属性算法类型
typedef enum enmItPropAlgType
{
	EIAAT_ADDMONEY = 1000,			///加上金币, 操作数3个: 条件, 金币增量, 是否需要人物属性卡(0或1)
	EIAAT_ADDEXPERIENCE = 1001,		///加上经验, 操作数3个: 条件, 经验增量, 是否需要人物属性卡(0或1)
	EIAAT_MULTIPLYMONEY = 1002,		///乘以金币倍数, 操作数3个: 条件, 金币倍数, 是否需要人物属性卡(0或1)
	EIAAT_MULTIPLYEXPERIENCE = 1003,	///乘以经验倍数, 操作数3个: 条件, 经验倍数, 是否需要人物属性卡(0或1)
	EIAAT_MONEYBONUS = 1004,		///金币奖励, 操作数4个: 条件, 加权类型, 加权值, 是否需要人物属性卡(0或1)
	EIAAT_EXPERIENCEBONUS = 1005,	///经验奖励, 操作数4个: 条件, 加权类型, 加权值, 是否需要人物属性卡(0或1)
//	EIAAT_PROPTRANSFER = 1006,		///道具转换, 操作数3个:转换概率, 源道具ID, 目的道具ID
	EIAAT_EFFECTTRANSFER = 1007,	///效果转换, 操作数5个：转换概率, 源效果ID, 目的效果ID, 消耗值, 金币增量
//	EIAAT_PROPEXCHANGE = 1008,		///道具换位, 操作数无
//	EIAAT_ROLEATTRIBUTECARD = 1009,	///人物属性卡，操作数无
//	EIAAT_NOFOG = 1010,				///穿透云层, 操作数无
//	EIAAT_EXTRAEXPRESSION = 1011,	///额外表情, 操作数无
//	EIAAT_WINFE = 1012,				///胜利特效, 操作数1个: 胜利特效ID
//	EIAAT_ROOMFE = 1013,			///房间特效, 操作数1个: 房间特效ID
	EIAAT_SCENEFE = 1014,			///场景特效, 操作数最多5个: 场景（赛道）ID
//	EIAAT_PHYSICSPARAMETER = 1015,	///物理参数, 待定义
}ENMItPropAlgType;

///装备使用资源类型
typedef enum enmItQueryItemType
{
	EIQIT_ALL_ITEM=1,		///返回所有item
	EIQIT_EQUIPPED_ITEM=2,	///返回已装备item
	EIQIT_UNEQUIPPED_ITEM=3,///返回未装备item
	EIQIT_MAX=255,
}ENMItQueryItemType;

///装备性别类型
typedef enum enmItSexType
{
	EASE_MALE=1,	//男性
	EASE_FEMALE=2,	//女性
}ENMItSexType;

///挂接点单元结构
typedef struct tagItHitchUnit
{
	UINT			nType;		///挂接点类型
	string			strName;	///挂接点名称
	UINT			nDefaultItem;	///默认挂接的itemid，没有则为0
	tagItHitchUnit();
	~tagItHitchUnit();
}ItHitchUnit,*PItHitchUnit;

typedef map<UINT,ItHitchUnit>	ITEMHITCH_MAP;	//以该结构(类)nType为索引
typedef list<UINT>	ITEMHITCH_LIST;	//以该结构(类)Id为索引
///以该结构(类)nType挂接点为索引，记录该挂接点当前的item id，如果索引不存在或值=0代表
///该挂接点没有item
typedef map<UINT,UINT>		ITEMONHITCH_MAP;

/////算法单元结构
//typedef struct tagItAlgUnit
//{
//	UINT		nId;		///算法Id
//	UINT		nOptNum;	///操作数个数
//	int			iarrOptVal[conAvatarMaxAlgInItem];	///操作数组
//	tagItAlgUnit();
//	~tagItAlgUnit();
//}ItAlgUnit,*PItAlgUnit;
//
/////算法管理结构
//typedef struct tagItAlgMgr
//{
//	UINT nTriOccation;	///触发时机
//	UINT nTriParamNum;	///触发参数个数，最多5个
//	UINT nAlgNum;		///算法个数
//	UINT narrTriParam[conAvatarMaxNormalParamNum];	///触发参数
//	ItAlgUnit	arrAlg[conAvatarMaxNormalParamNum];	///算法操作的数组
//	tagItAlgMgr();
//	~tagItAlgMgr();
//}ItAlgMgr,*PItAlgMgr;


//装备特殊行为类型
typedef enum enmBehaveID
{
	EBI_Animation = 1,	//播放动作
	EBI_Effect = 2,		//播放特效
	EBI_Sound = 3,		//播放声音	
	EBI_MAX = 4,
}ENMBEHAVEID;


//装备转换算法的算子
typedef enum enmOperatorID
{
	EOI_Add = 0,	//加
	EOI_Mul = 1,	//乘
}ENMOPERATORID;


//转换算法类型
typedef enum enmAlgorithmID
{
	EAI_GPTansfer = 1,		//按一定概率转换道具攻击
	EAI_GPReduceTime = 2,	//按一定概率减少攻击作用时间
}ENMALGORITHMID;


//装备拥有的各种转换算法数据
typedef struct tagAlgData
{
	DWORD dwType;		//类型
	DWORD dwOccasion;	//触发时机
	SHORT shOpertor;	//算子
	DWORD dwCondition;	//触发条件
	DWORD dwExpectation;//期望值	
	DWORD dwRate;		//得到期望值的概率
}ALGDATA, *PALGDATA, *LPALGDATA;

typedef list<ALGDATA> AlgDataList;
typedef list<ALGDATA>::const_iterator AlgDataList_It;

//装备特有的动作转换
typedef struct tagAnimData
{
	DWORD dwCondition;		//源动作
	DWORD dwExpectation;	//转换动作
}ANIMDATA, *PANIMDATA, *LPANIMDATA;

typedef list<ANIMDATA> AnimDataList;
typedef list<ANIMDATA>::const_iterator AnimDataList_It;

typedef enum enmAttachType
{
	EATT_Wearer	= 0,
	EATT_World	= 1,
}ATTACHTYPE;

//装备拥有的特殊行为
typedef struct tagBehaveData
{
	DWORD dwType;		//类型
	DWORD dwOccasion;	//触发时机
	DWORD dwCondition;	//触发条件
	DWORD dwLife;		//生命长度
	DWORD dwParam;		//参数
	DWORD dwOption;		//自由参数
}BEHAVEDATA, *PBEHAVEDATA, *LPBEHAVEDATA;

typedef list<BEHAVEDATA> BehaveDataList;
typedef list<BEHAVEDATA>::const_iterator BehaveDataList_It;


//额外给予角色的物理属性
typedef struct tagPhysicProperty
{
	SHORT shOpertor;	//给予操作的算子
	DWORD dwSpeed;		//速度属性
	DWORD dwErupt;		//爆发属性
	DWORD dwAccumNo2;	//积累氮气属性
	DWORD dwBigNo2;		//大喷属性
	DWORD dwMiniNo2;	//小喷属性

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


//是否可交换道具位置
typedef enum enmBoolFlag
{
	BFG_Forbid = 0,	//禁止
	BFG_Can = 1,	//可以
};

typedef struct tagAvatarProperty
{
	SHORT			shSwitch;		//是否可交换道具位置
	SHORT			shNoFog;		//是否可看穿云雾
	PHYSICPROPERTY	tagPhyPrp;		//角色物理属性	
	AnimDataList	listAnimData;	//avatar拥有的特殊动作表情
	BehaveDataList	listBehaveData;	//avatar拥有行为数据列表
	AlgDataList		listAlgData;	//算法数据列表
}AVATARPROPERTY, *PAVATARPROPERTY, *LPAVATARPROPERTY;

//换颜色用的色度属性
typedef struct tagSColorAttr{
	int		hue;
	float	sat;
	float	val;
}SColorAttr;

//道具信息包括id和生命长度
typedef struct tagGPInfo
{
	DWORD dwID;
	DWORD dwLife;
	SHORT sResult;
	DWORD dwItemID;
}GPINFO;


///装备单元结构
typedef struct tagItemUnit
{
	UINT		nId;		///装备Id
	UINT		nType;		///装备类型
	string		strName;	///装备名称
//	UINT		nAlgNum;	///装备属性算法个数
//	ItAlgMgr	insAlgMgr;	///算法模块
	UINT		nRoleBindNum;	///角色绑定的个数
	UINT		arrRoleBindId[conAvatarMaxNormalParamNum];	///角色绑定的ID
	UINT		nCarBindNum;	///车辆绑定的个数
	UINT		arrCarBindId[conAvatarMaxNormalParamNum];	///车辆绑定的ID
	UINT		nBlockSlotNum;	///屏蔽挂接点的个数,!!不包括主挂接点
	UINT		arrBlockId[conAvatarMaxNormalParamNum];	///屏蔽挂接点的ID
	UINT		nRepellRoleNum;	///排斥角色的个数
	UINT		arrRepellId[conAvatarMaxNormalParamNum];	///排斥角色的ID
	UINT		nResType;		///装备使用资源类型，看enmItResType
	UINT		nGarageType;	///车库展示用类型
	UINT		nSexType;		///道具的性别类型，看ItSexType
	UINT		nHideRoleFoot;		///是否需要在比赛中隐藏玩家脚，1=隐藏，0=不隐藏
	ITEMHITCH_LIST	liHitchList;	///该装备带有的挂接点，例如角色或车
	//NiNodePtr	pkBaseNode;			///原始模型O
	//NiSourceTexturePtr pkBaseTexture;	///替换贴图用模型记录指针，例如车牌
	//DWORD		dwScreenObject;	///用于屏幕显示的句柄
	string		strDesc;	///装备描述名称，商店或车库显示用
	string		strModelName;	///模型内部真正物体多边形总节点名称
	string		strOriTexName;	///原始贴图路径，用于贴图类Item
	AVATARPROPERTY tagProperty;	///道具逻辑属性
	SColorAttr	insColor;		///换色用的属性
	std::deque<std::string>deChgColorName;	///需要换色的模型名称，逐个处理
	std::deque<std::string>deHideObjName;	///游戏过程需要隐藏的模型名称，逐个处理
	tagItemUnit();
	~tagItemUnit();
}ItemUnit,*PItemUnit;

typedef map<UINT,ItemUnit>	ITEMUNIT_MAP;	//以该结构(类)Id为索引

///装备记录单元结构
typedef struct tagItemStore
{
	STPlayerDBItemInfo	insInfo;	///综合信息
	//ITEMHITCH_MAP	maHitchExt;		///关于本实体使用的挂接点，是标准挂接点的副本
										///建立副本的原因是有可能挂接点坐标会随实体而变
	NiNodePtr		pkNode;			///本实例模型调用，clone自ItemUnit
	tagItemStore();
	~tagItemStore();
	///copy指定源的Store对象赋予本实例
	void CopyItemStore(tagItemStore* pStoreSrc);
	///获取本store实例的模型数据
	NiNodePtr GetItemNode(void);
}ItemStore,*PItemStore;

typedef map<UINT,ItemStore>		ITEMSTORE_MAP;	//以该结构(类)Id为索引
typedef map<UINT,ItemStore*>	ITEMPOINTER_MAP;	//以该结构(类)Id为索引


///挂接实体按类型进行分类结构
typedef struct tagItTypeObj
{
	UINT			nType;			///实体类型nType(根据分类来索引)
	ITEMSTORE_MAP	maItemStore;	///目前实体所有的装备列表
	tagItTypeObj();
	~tagItTypeObj();
}ItTypeObj,*PItTypeObj;

typedef map<UINT,ItTypeObj>		ITEMOBJ_MAP;	//以该结构(类)nType为索引

///挂接实体按车库分类进行分类结构
typedef struct tagGarageTypeObj
{
	UINT			nType;			///车库分类nType(根据分类来索引)
	ITEMPOINTER_MAP	maStorePt;		///目前实体所有的装备指针列表
	tagGarageTypeObj();
	~tagGarageTypeObj();
}GarageTypeObj,*PGarageTypeObj;

typedef map<UINT,ItTypeObj>		ITEMOBJ_MAP;	//以该结构(类)nType为索引
typedef map<UINT,GarageTypeObj>		ITEMGARAGE_MAP;	//以车库分类nType为索引

///用户Avatar管理单元结构
typedef struct tagItPlayerUint
{
	ULONG			nlUin;			///用户号码Id
	ITEMOBJ_MAP		maHObjList;		///该实体所有挂接实体的列表
	ITEMONHITCH_MAP	maItemOnHitch;	///记录该挂接点当前的item id
	ITEMSTORE_MAP	maBackupInfo;	///用来备份Avatar信息使用
	ITEMGARAGE_MAP	maGarageList;	///根据车库分类的
	tagItPlayerUint();
	~tagItPlayerUint();
}ItPlayerUint,*PItPlayerUint;

///ItemObjUnit索引的ULONG为用户号码(QQ号码)
typedef map<ULONG,ItPlayerUint*>		ITEMPLAYER_MAP;	//以用户号码(QQ号码)为索引

///查询用户Avatar数据结构，后续_In的参数为输入函数用的参数项，_Out的参数为输出的项
typedef struct tagQueryPlayerAvatarInfo
{
	ULONG	m_unUin_In;				///用户号码Uin
	UCHAR	m_cQueryMode_In;		///查询模式，看enmItQueryItemType
	USHORT	m_nItemNum_InOut;	///查询返回的有效装备项数
	STPlayerDBItemInfo m_stItemInfo_Out[MAXITEMNUMPERPLAYER];	///装备信息
	tagQueryPlayerAvatarInfo();
	~tagQueryPlayerAvatarInfo();
}QueryPlayerAvatarInfo,*PQueryPlayerAvatarInfo;