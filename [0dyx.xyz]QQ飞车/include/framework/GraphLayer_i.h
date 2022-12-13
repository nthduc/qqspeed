////////////////////////////////////////////////
//     FileName:    GraphLayer_i.h
//     Author:      Fish
//     Date:        2006-10-10 15:17:03
//     Description: []
////////////////////////////////////////////////

#pragma once

#include "kernel.h"
#include "gameui.h"
#define INFINITE_SMALL	0.000001
#define MAX_OBJECTNAME	20

class Pos3D
{
public:
	Pos3D()
	{ x = y = z = 0.0; }
	Pos3D(float fx, float fy, float fz)
	{ x = fx; y = fy; z = fz; }

	friend FLOAT DistanceSquare(const Pos3D& lhs, const Pos3D& rhs)
	{
		return (lhs.x - rhs.x) * (lhs.x - rhs.x) + (lhs.y - rhs.y) * (lhs.y - rhs.y) + (lhs.z -rhs.z) * (lhs.z -rhs.z);
	}

	friend bool operator == (const Pos3D& lhs, const Pos3D& rhs)
	{
		return DistanceSquare(lhs, rhs) < INFINITE_SMALL;
	}

	friend Pos3D operator + (const Pos3D& lhs, const Pos3D& rhs)
	{
		return Pos3D(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z);
	}

	friend Pos3D& operator += (Pos3D& lhs, const Pos3D& rhs)
	{
		lhs.x += rhs.x;
		lhs.y += rhs.y;
		lhs.z += rhs.z;
		return lhs;
	}

	friend Pos3D& operator -= (Pos3D& lhs, const Pos3D& rhs)
	{
		lhs.x -= rhs.x;
		lhs.y -= rhs.y;
		lhs.z -= rhs.z;
		return lhs;
	}

	friend Pos3D operator * (const Pos3D& lhs, FLOAT scale)
	{
		return Pos3D(lhs.x * scale, lhs.y * scale, lhs.z * scale);
	}

	friend Pos3D& operator *= (Pos3D& lhs, FLOAT scale)
	{
		lhs.x *= scale;
		lhs.y *= scale;
		lhs.z *= scale;
		return lhs;
	}

	float x, y, z;
};

typedef enum tagMoveMode
{
	eDirectMove,	//直线运动
	eWalk,			//导航片行走
	eCustomMove,	//自定义运动
}eMoveMode;

interface	IRDWorld;
interface	IRDObject;
interface	IRDCamera;
interface	IObjectMoveSink;
interface	IObjectMoveCtrl;
interface	IRDEffectMgr;

EXTERN_C const IID IID_IRDDevice;
struct DECLSPEC_UUID("A4DE5BA1-B95B-430b-8379-1551EBF8A8C3") DECLSPEC_NOVTABLE
IRDDevice : IUnknown
{
public:
	STDMETHOD(CreateWorld)(IRDWorld**) PURE;
	STDMETHOD(CreateCamera)(IRDCamera**) PURE;
	STDMETHOD(CreateObject)(LPCSTR, IRDObject**) PURE;
	STDMETHOD(GetEffectMgr)(IRDEffectMgr**) PURE;
};

EXTERN_C const IID IID_IRDWorld;
struct DECLSPEC_UUID("685CF476-6ECC-4193-8C38-3E66A08A7C35") DECLSPEC_NOVTABLE
IRDWorld : IUnknown
{
	/*
	Update:
	主循环函数，通常每帧调用一次
	*/
	STDMETHOD(Update)(FLOAT) PURE;

	/*
	Render:
	*/
	STDMETHOD(Render)() PURE;

	/*
	Load:
	从一个资源加载场景
	*/
	STDMETHOD(Load)(LPCSTR ModuleName) PURE;

	/*
	AddObject:
	往场景中的指定位置添加一个物体
	Pos指定世界中的位置
	*/
	STDMETHOD(AddObject)(IRDObject*) PURE;

	/*
	AddObject:
	往场景中的指定位置添加一个物体
	NodeName指定绑定点名字
	*/
	STDMETHOD(AddObjectToNode)(IRDObject*, LPCSTR NodeName) PURE;

	/*
	GetObjectByID:
	*/
	STDMETHOD(GetObjectByID)(INT ActorID, IRDObject**) PURE;

	/*
	GetObjectByName:
	*/
	STDMETHOD(GetObjectByName)(LPCSTR ObjName, IRDObject**) PURE;

	/*
	RemoveObject:
	删除场景中已存在的物体
	*/
	STDMETHOD(RemoveObject)(IRDObject*) PURE;

	/*
	RemoveObjectByID:
	删除场景中已存在的物体
	*/
	STDMETHOD(RemoveObjectByID)(INT ActorID) PURE;

	/*
	Pick:
	根据屏幕坐标返回被Pick中的物体
	*/
	STDMETHOD(Pick)(INT x, INT y, IRDObject** ppv) PURE;

	/*
	UseCamera:
	激活指定的相机
	*/
	STDMETHOD(UseCamera)(LPCSTR CameraName, IRDCamera**) PURE;

	/*
	SetCamera:
	手工给场景指定相机
	*/
	STDMETHOD(SetCamera)(IRDCamera*) PURE;

	/*
	GetCurrentCamera:
	得到当前相机
	*/
	STDMETHOD(GetCurrentCamera)(IRDCamera**) PURE;
};

EXTERN_C const IID IID_IRDCamera;
struct DECLSPEC_UUID("E1B67868-9100-4c5f-A5B7-6DDC6A82F13A") DECLSPEC_NOVTABLE
IRDCamera : IUnknown
{
	STDMETHOD(SetFOV)(FLOAT) PURE;
	STDMETHOD(Load)(LPCSTR) PURE;
	STDMETHOD(Play)(UINT id) PURE;
	STDMETHOD(Stop)() PURE;
	STDMETHOD(Follow)(IRDObject*) PURE;
	STDMETHOD(SetTarget)(FLOAT x, FLOAT y, FLOAT z) PURE;
	STDMETHOD(GetTarget)(FLOAT* x, FLOAT* y, FLOAT* z) PURE;
	STDMETHOD(SetInfo)(FLOAT Distance, FLOAT Pitch, FLOAT Yaw) PURE;
	STDMETHOD(GetInfo)(FLOAT* Distance, FLOAT* Pitch, FLOAT* Yaw) PURE;
	STDMETHOD(WorldToScreen)(FLOAT x, FLOAT y, FLOAT z, FLOAT* sx, FLOAT* sy) PURE;
};

EXTERN_C const IID IID_IRDEffectMgr;
struct DECLSPEC_UUID("8E4A02BE-E866-42cd-A4A7-00910F725A5D") DECLSPEC_NOVTABLE
IRDEffectMgr : IUnknown
{
	/*
	AddEffect:
	载入一个特效编辑器的输出文件
	*/
	STDMETHOD(RegisterEffectFile)(LPCSTR, INT*) PURE;

	/*
	TriggleEffect:
	触发一个特效
	*/
	STDMETHOD(TriggleEffect)(INT index) PURE;

	/*
	SetEffectParam:
	为某个特效组添加必要的参数
	*/
	STDMETHOD(SetEffectParam)(INT index, LPCSTR name, VARIANT value) PURE;
};

EXTERN_C const IID IID_IRDObject;
struct DECLSPEC_UUID("B3120A3B-431B-4ff6-90AC-16D3445769FF") DECLSPEC_NOVTABLE
IRDObject : IUnknown
{
	/*
	更换资源
	*/
	STDMETHOD(LoadModel)(LPCSTR) PURE;

	/*
	MoveTo:
	按照指定的运动方式让物体移动到指定位置，参加eMoveMode
	如果是2D物体，Pos将会忽略z坐标，只有x,y才有效
	*/
	STDMETHOD(MoveTo)(FLOAT x, FLOAT y, FLOAT z, eMoveMode MoveMode, FLOAT Speed) PURE;

	/*
	StopMove:
	*/
	STDMETHOD(StopMove)() PURE;

	/*
	SetMoveControl:
	设置自定义的物体移动方式，只有在MoveMode==eCustomMove才会发挥作用
	*/
	STDMETHOD(SetMoveControl)(IObjectMoveCtrl*) PURE;

	/*
	SetMoveSink:
	设置运动回调
	*/
	STDMETHOD(SetMoveSink)(IObjectMoveSink*) PURE;

	/*
	DoAnimation:
	播放一个动画
	*/
	STDMETHOD(DoAnimation)(DWORD AnimID, FLOAT Freq = 1.0, FLOAT Offset = 1.0) PURE;

	/*
	AddEffect:
	给物体添加一个特效
	*/
	STDMETHOD(AddEffect)(LPCSTR EffectFile, LPCSTR EffectName, LPCSTR Joint, FLOAT x = 0.0, FLOAT y = 0.0, FLOAT z = 0.0) PURE;

	/*
	GetRotate:
	SetRotate:
	GetPos:
	SetPos:
	获取和设置物体的世界中的位置和旋转
	*/
	STDMETHOD(SetRotate)(FLOAT Angle, FLOAT x, FLOAT y, FLOAT z) PURE;
	STDMETHOD(SetPos)(FLOAT x, FLOAT y, FLOAT z) PURE;
	STDMETHOD(GetPos)(FLOAT* x, FLOAT* y, FLOAT* z) PURE;

	/*
	SetVisible:
	设置物体的场景中是否可见
	*/
	STDMETHOD(SetVisible)(BOOL) PURE;

	/*
	EnterState:
	进入一个指定状态，这种状态对应的行为通过脚本配置
	*/
	STDMETHOD(EnterState)(DWORD State) PURE;

	//Avatar Support
	/*
	更换部件模型（很多部件在同一个NIF中，挂件是单独的NIF，通过挂接点挂到模型上）
	*/
	STDMETHOD(ModelReplace)(LPCSTR ModelFileName, LPCSTR TypeName) PURE;

	/*
	更换部件和挂件的贴图
	*/
	STDMETHOD(SetTexture)(LPCSTR TextureName, LPCSTR TexturePath, LPCSTR TypeName) PURE;

	/*
	加挂件
	参数:
	ItemName:标志符，可用于DetachItem
	*/
	STDMETHOD(AddItemModel)(LPCSTR ModelFileName, LPCSTR ItemName, LPCSTR Joint) PURE;

	/*
	删除挂件
	*/
	STDMETHOD(DetachItem)(LPCSTR ItemName) PURE;

	/*
	GetID:
	取得对应的ActorID
	*/
	STDMETHOD(GetID)(INT*) PURE;

	/*
	SetName：
	给Object取个名字
	*/
	STDMETHOD(SetName)(LPCSTR) PURE;

	/*
	GetName:
	获得Object名字
	*/
	STDMETHOD(GetName)(LPSTR) PURE;

	/*
	SetClientData:
	设置客户绑定数据
	*/
	STDMETHOD(SetClientData)(IUnknown*) PURE;

	/*
	GetClientData：
	取得用户绑定数据
	*/
	STDMETHOD(GetClientData)(IUnknown**) PURE;
};

EXTERN_C const IID IID_IObjectMoveCtrl;
struct DECLSPEC_UUID("93BC2677-AF8D-480d-8170-138DE260F817") DECLSPEC_NOVTABLE
IObjectMoveCtrl : IUnknown
{
	STDMETHOD(Update)(FLOAT) PURE;
};

EXTERN_C const IID IID_IObjectMoveSink;
struct DECLSPEC_UUID("79479E02-57B7-49c4-809D-A63CEC6D5A9A") DECLSPEC_NOVTABLE
IObjectMoveSink : IUnknown
{
	STDMETHOD(OnMoveStart)(FLOAT x, FLOAT y, FLOAT z) PURE;
	STDMETHOD(OnMove)(FLOAT x, FLOAT y, FLOAT z) PURE;
	STDMETHOD(OnMoveEnd)(FLOAT x, FLOAT y, FLOAT z) PURE;
};

struct IBubbleMgr
{
	virtual BOOL AddBubble(UINT id, LPCSTR text, Point2D pos, INT life = -1, INT textsize = 12, INT space = 0, INT color = 0, INT width = -1, INT height = -1, INT margin = 10) = 0;
};

class	CScriptState;
class	IWgtData;
class	IDisplay;
enum	Msgbox_Cmd;
EXTERN_C const IID IID_IGUI;
struct DECLSPEC_UUID("8435198B-B4A4-4A3C-B830-B330CE230F35") DECLSPEC_NOVTABLE
IGUI : IUnknown
{
	STDMETHOD(CreateDevice)(BOOL, INT) PURE;
	STDMETHOD(LoadUI)(LPCSTR, INT, INT) PURE;
	STDMETHOD(DestroyGUI)() PURE;
	STDMETHOD(Update)(DWORD) PURE;
	STDMETHOD(GoToScreen)(LPCSTR) PURE;
	STDMETHOD(GetScriptState)(CScriptState**) PURE;
	STDMETHOD(GetWidget)(LPCSTR, IWgtData**) PURE;
	STDMETHOD(ShowWidget)(LPCSTR, BOOL) PURE;
	STDMETHOD(MsgBox)(Msgbox_Cmd, LPCSTR Text = "", LPCSTR Caption = "Message", INT nMsgId =0) PURE;
	STDMETHOD(GetCurScreen)(LPCSTR*) PURE;
	STDMETHOD(GetDisplay)(IDisplay**) PURE;
	STDMETHOD(GetBubbleMgr)(IBubbleMgr**) PURE;
	STDMETHOD(RegisterUICallBack)(UICallbackEvent eventName,CallBackFunc pFunc) PURE;
};

EXTERN_C const CLSID CLSID_RDDevice;
EXTERN_C const CLSID CLSID_GUIImpl;
