#pragma once

class RMOperator
{
public:
	// Scene loading utilities
	static NiNodePtr	OpenNif(const char* pcNifName,NiTObjectArray<NiObjectPtr>* kExtraObjects = 0);
	static NiNodePtr	OpenNifAndPrepack(const char* pcNifName,NiTObjectArray<NiObjectPtr>* kExtraObjects = 0);
	static void			RecursivePrepack(NiAVObject* pkObject, bool bBLODOnly);
	static void			RecursiveRemoveMorph(NiAVObject* pkObject);
#if defined(WIN32)
	static void			RecursiveAddToStaticGeometryGroup(NiAVObject* pkObject, NiGeometryGroup* pkGroup);
#endif  //#if defined(WIN32)

	static void			SetUseShaders(bool bUseShaders);
	static bool			GetUseShaders();
	static void			LightObject(NiNode* pkNode);
	static void			RecursiveEnableParticles(NiAVObject* pkObject, bool bEnable);
	static NiAVObject*	GetObjectByType(NiAVObject* pkObject, const NiRTTI* pkRTTI);

	// tree morpher controllers
	void				RecursiveAddToMorphList(NiAVObject* pkObject, float fPhase);
protected:
	NiTPointerList<NiGeomMorpherController*> m_kTreeMorphList;
	static bool ms_bUseShaders;
};
