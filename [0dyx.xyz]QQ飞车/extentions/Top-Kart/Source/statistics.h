#pragma once

#include "NiVisualTracker.h"
#include <vector>

//primo
class Statistics
{

public:
	typedef enum tagGroupType
	{
		GT_SCENE = 0,
		GT_CAR,
		GT_MINIMAP,
		GT_TOTAL,
		GT_COUNT
	}eGROUPTYPE;


	//typedef enum tag

	//typedef enum tagGroupName
	//{

	//}

	static void Create();
	static void Release();
	//static Statistics* Get();

	static void BeginCount();
	static void EndCount();

	static void AddVisibleArray(const NiVisibleArray* pVArray, eGROUPTYPE group);
	static void AddNode(const NiNode* pkNode, eGROUPTYPE group);
	static unsigned int* GetNum(eGROUPTYPE group);

	static void SetTracker(NiVisualTracker* pTracker){ m_pTracker = pTracker;}
	static void SetGraphID(eGROUPTYPE group, int id){ m_apGraphID[group] = id;}
	
	static void GetAllEdgeObject();
	static NiAVObject* FindObject(const NiNode& node, const NiFixedString& szName);

	//ÌùÍ¼
	static void ReplaceTexture(NiNode* pNode);//, NiTexture* pText);
	static void ReplaceTextureRecursive(NiAVObject* pObject, NiTexture* pText);
	//~

	//alpha
	static void RemoveAlphaPropRecursive(NiAVObject* pObject);

	//Ð¡ÎïÌå
	static void CullSamllObjectRecursive(NiAVObject* pObject);

private:
	Statistics();
	~Statistics();

	static Statistics* m_pStatistics;
	static NiVisualTracker* m_pTracker;

	static unsigned int GetTriangleNum(const NiVisibleArray* pVArray);
	static unsigned int GetTriangleNum(const NiAVObject* pkObject);


	static unsigned int m_anTriangles[GT_COUNT];
	static int m_apGraphID[GT_COUNT];
	static char* m_aszGraphName[GT_COUNT];
	static std::vector<NiAVObject*> m_vEdgeObjects;
};