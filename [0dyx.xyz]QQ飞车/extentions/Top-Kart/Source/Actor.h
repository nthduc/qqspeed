

#ifndef LENGUINMANAGER_H
#define LENGUINMANAGER_H

#include <NiActorManager.h>
#include <NiTriShape.h>
#include "ShadowGeometry.h"
#include <NiPhysX.h>
#include <NiScreenTexture.h>
#include <NiScreenElements.h>
#include <NiRenderTargetGroup.h>
#include "GameMain.h"


#include "UIFace.h"
#define FUR_LOD_COUNT 4

class Actor : public NiActorManager::CallbackObject
{
public:
    Actor();
    virtual ~Actor();

//    void Initialize(NiActorManager* pkCharacterAM, NiActorManager* spKartAM, unsigned int uiShadowDetail, const char * pcName);
	void Initialize(unsigned int uiShadowDetail, const char * pcName);

    virtual void Update(float fTime, float fFrameTime, const NiPoint2& kDir, 
        const float& kStrafeDir, bool bAnimationUpdate = true);

    void Draw(NiCamera* pkCamera);

	// 绘制屏幕肖像 
	//void SetVisible2DPortrait(bool bShow);
	//void SetPortraitRectAndCamera(const char* szCameraName,int left,int top,int width,int height,bool bShowCar = true);
	//void RenderPortrait();
	//void DrawPortraitUI(float fScale = 1.0f);//用于UI回调 
	//void DrawPortraitGame();//用于游戏中 
	//void ReplaceDisplayTextureWithRenderedTexture();

	void OnLostDevice(bool bReset);
	void ReBindAll();

	// Shadow
	void StartShadow(unsigned int uiShadowDetail, unsigned int uiMaxShadowTriSize);
	void RenderShadow(NiCamera* pkSceneCamera);
	void ForceRenderShadow();
	void DrawShadow(NiCamera* pkCamera);
	void TerminateShadow();

	// Name, by wincoqin
	bool InitializeName(const char * pcName);
	void ShowName(bool bShow);
	void RenderName();

	// Author: wincoqin
	// Param: pkNode, 被照亮的物体
	// Function: 照亮物体/解除物体照亮状态
	void LightObject(NiNode * pkNode);
	void UnLightObject(NiNode * pkNode);

	void ToggleBoneLOD(int nLod);

    NiGeometry* GetEyes() const;
    NiNode* GetSkin() const;

    const NiPoint3& GetLenguinTranslate() const;
    const NiPoint2 Get2DLenguinTranslate() const;
    const NiPoint2 GetLenguinDirection() const;

    void Damage(float fDamage);
    float GetHealth();

	/*
	修改人：	ray
	日期：	2/26/2007
	描叙：	将一结点挂接到某一dummy点上或从某一dummy点分离。
	*/
	//将一结点挂接到某一dummy点上，name为挂接点的名字
	void AttachDummy(NiFixedString name, NiNodePtr spNode);

	//从dummy点分离结点，name为挂接点的名字
	void DetachDummy(NiFixedString name, NiNodePtr spNode);

	bool AttachHint(NiAVObject* pObj);
	bool DetachHint(NiAVObject* pObj);

    void SetElevation(float fElevation);
//    float GetSpeedMultiplier();   
//    void SetSpeedMultiplier(float fNewSpeedMultiplier);
    
    float GetMinColliderDistance() const;
    float GetMinColliderDistanceSQRD() const;
    void SetMinColliderDistance(float fDistance);
    float GetAvoidCollisionsFactor() const;
    void SetAvoidCollisionsFactor(float fFactor);

	void ChangeFaceTexture(const NiSourceTexturePtr& spTex);
	void ChangeFaceTexture(const NiFixedString & kName);
	void RecoverFaceTexture();

	NiCamera * GetCamera(const char * szCameraName);

    NiNode* GetBoneRoot() const;
	/*
		修改人：	ray
		日期：	1/15/2007
		描叙：	人物模型和车模型分离。
	*/
	NiNode* GetKartNIFRoot() const;			//得到车的根结点
    NiNode* GetCharacterNIFRoot() const;	//得到角色的根结点

    NiActorManager::SequenceID GetTargetAnimation();
    NiPoint3 GetLocation();

    static void SetEyesStripified(bool bStripified);
    static void SetSkinStripified(bool bStripified);

    static bool ColorIsWalkable(const NiColorA &kColor);

    // NiActorManager::CallbackObject overrides
    virtual void AnimActivated(NiActorManager* pkManager,
        NiActorManager::SequenceID eSequenceID, float fCurrentTime, 
        float fEventTime) {}
    virtual void AnimDeactivated(NiActorManager* pkManager,
        NiActorManager::SequenceID eSequenceID, float fCurrentTime, 
        float fEventTime) {}
    virtual void TextKeyEvent(NiActorManager* pkManager,
        NiActorManager::SequenceID eSequenceID, const NiFixedString& pcTextKey,
        const NiTextKeyMatch* pkMatchObject,
        float fCurrentTime, float fEventTime);
    virtual void EndOfSequence(NiActorManager* pkManager,
        NiActorManager::SequenceID eSequenceID, float fCurrentTime, 
        float fEventTime) {}

    static const float ms_fJumpSpeed;

protected:
	static NiCriticalSection  m_kCriticalSection;

protected:
    virtual void HandleTextKeyEvent();
    virtual NiActorManager::EventCode InterpretInput(const NiPoint2& kDir, 
        const float& fStrafeDir, float fTime) = 0;
    
	void UpdateBipedRootTransforms(NiPoint2& kDir, 
        const float& fStrafeDir, float fFrameTime);
 
	virtual void InitializeRandomIdle() = 0;

    //NiPoint2 AvoidCollisions(const NiPoint2& kDir) const;
    void ForceUpdateShadowImage();
	
	// Author: wincoqin
	// Param: pkNode, 模型根节点
	// Function: 初始化灯光
	// Return: 成功返回true, 否则false
	bool InitializeLights(NiNode * pkNode);

	void UnloadKart();
	void UnLoadCharacter();
	void UnLoadHair();	

	void Release();

    float m_fMinColliderDistance;
    float m_fMinColliderDistanceSQRD;
    float m_fAvoidCollisionsFactor;

    NiTPrimitiveArray<NiActorManager::EventCode> m_kRandomIdle;
    bool m_bIsIdling;

	/*
	修改人：	ray
	日期：	1/15/2007
	描叙：	人物模型和车模型分离。车和角色的管理结点	
	*/
    NiActorManagerPtr m_spCharacterAM;
	NiActorManagerPtr m_spKartAM;

	//	增加人:	wincoqin
	//  时间:	2007/05/10
	//  目的:	增加头和头发
	NiActorManagerPtr m_spHairAM;
	

    NiNodePtr m_spSkin;
    NiGeometryPtr m_spLowResSkin;
    NiGeometryPtr m_spHighResSkin;
	NiNodePtr m_spPlayer;

    unsigned int m_uiCurrentFurLOD;
    NiTriShapePtr m_spShells[FUR_LOD_COUNT];
    NiTriShapePtr m_spFins;

    NiNodePtr m_spLookAt;
    float m_fElevation;

//    float m_fLookAtResponse;
//    float m_fBodyFollowResponse;

    NiQuaternion m_kBipQuat;
    NiQuaternion m_kLookAtQuat;

    NiAVObject* m_pkRightFoot;
    NiAVObject* m_pkLeftFoot;

//    float m_fSpeedMultiplier;
	ShadowGeometry * m_pkShadowGeometry;

	static bool ms_bEyesStripified;
    static bool ms_bSkinStripified;

    static const float m_sfZeroVelocityMaximum;

    float m_fHealth;
    static const float m_fMaxHealth;

	NiTexturingPropertyPtr m_spOriginFaceTexProp;
	NiTexturePtr		   m_spOriginFaceTex;

	// 灯光节点
	NiNodePtr			   m_spLightNode;
	// 用于存储照亮场景用的灯光
	NiTObjectArray<NiLightPtr> m_arrSceneLights;

	// For 2D Render
	/*NiScreenElementsPtr		m_spPortaritTexture;
	NiRenderTargetGroupPtr  m_spRenderTargetGroup;	
	NiRenderedTexturePtr	m_spRenderedTexture;
	float					m_fXPos;
	float					m_fYPos;
	float					m_fWidth;
	float					m_fHeight;
	bool					m_bShowCar;
	bool					m_bPortraitVisible;
	NiCameraPtr				m_spPortraitCamera;
	*/
	// 名字相关
	NiAVObjectPtr m_spkNameBoard;
	bool		  m_bShowName;
	GFX::CUIFace *m_pkNameFace;
	int			  m_nPlayNameResID;

	NiFixedString m_kLastCameraName;

	bool  m_bFirstRender; // 防止渲染头像的时候出现乱象
	unsigned int m_uiShadowDetail;//标志是否渲染阴影(0: 不渲染)
};

#include "Actor.inl"

#endif // LENGUINMANAGER_H
