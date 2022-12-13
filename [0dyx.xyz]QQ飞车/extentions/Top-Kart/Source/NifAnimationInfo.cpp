

// NifAnimationInfo.cpp

#include "Top-KartPCH.h"

#include "NifAnimationInfo.h"

//---------------------------------------------------------------------------
CNifAnimationInfo::CNifAnimationInfo()
{
    m_fMinTime = UNINITIALIZED_FLOAT;
    m_fMaxTime = 0.0f;
	m_fMinStartTime = UNINITIALIZED_FLOAT;
	m_fMaxLastTime = 0.0f;
    m_uiKeyframeCount = 0;
    m_uiControllerCount = 0;
	m_bHasLoop = FALSE;
}
//---------------------------------------------------------------------------
CNifAnimationInfo::~CNifAnimationInfo()
{

}
//---------------------------------------------------------------------------
void CNifAnimationInfo::CollectData(NiNode* pkRoot)
{
    m_fMinTime = UNINITIALIZED_FLOAT;
    m_fMaxTime = 0.0f;
	m_fMinStartTime = UNINITIALIZED_FLOAT;
	m_fMaxLastTime = 0.0f;
    m_uiKeyframeCount = 0;
	m_bHasLoop = FALSE;
    FindAndAdd(pkRoot);
	/*
    char acString[256];
    NiOutputDebugString("CNifAnimationInfo::CollectData returns:\n");
    NiSprintf(acString, 256, "\tNumber of Controllers: %d\n",
        m_uiControllerCount);
    NiOutputDebugString(acString);
    NiSprintf(acString, 256, "\tNumber of Keyframes: %d\n",
        m_uiKeyframeCount);
    NiOutputDebugString(acString);
    NiSprintf(acString, 256, "\tMin Time: %f\n", m_fMinTime);
    NiOutputDebugString(acString);
    NiSprintf(acString, 256, "\tMax Time: %f\n", m_fMaxTime);
    NiOutputDebugString(acString);
	NiSprintf(acString, 256, "\tMin StartTime: %f\n", m_fMinStartTime);
	NiOutputDebugString(acString);
	NiSprintf(acString, 256, "\tMax LastTime: %f\n", m_fMaxLastTime);
	NiOutputDebugString(acString);
	*/
}
//---------------------------------------------------------------------------
unsigned int CNifAnimationInfo::GetNumberOfControllers()
{
    return m_uiControllerCount;
}
//---------------------------------------------------------------------------
unsigned int CNifAnimationInfo::GetNumberOfKeyframes()
{
    return m_uiKeyframeCount;
}
//---------------------------------------------------------------------------
float CNifAnimationInfo::GetMinTime()
{
    if(m_fMinTime == UNINITIALIZED_FLOAT)
        return 0.0f;
    return m_fMinTime;
}
//---------------------------------------------------------------------------
float CNifAnimationInfo::GetMaxTime()
{
    return m_fMaxTime;
}
float CNifAnimationInfo::GetMinStartTime()
{
	return m_fMinStartTime;
}
float CNifAnimationInfo::GetMaxLastTime()
{
	return m_fMaxLastTime;
}
BOOL CNifAnimationInfo::GetLoop()
{
	return m_bHasLoop;
}
//---------------------------------------------------------------------------
void CNifAnimationInfo::Add(NiTimeController* pkController)
{
    if(pkController == NULL)
        return;

    AddInfo(pkController);

    if(pkController->GetBeginKeyTime() < m_fMinTime)
        m_fMinTime = pkController->GetBeginKeyTime();
    if(pkController->GetEndKeyTime() > m_fMaxTime)
        m_fMaxTime = pkController->GetEndKeyTime();
	if ( pkController->GetStartTime() < m_fMinStartTime )
		m_fMinStartTime = pkController->GetStartTime();
	if ( pkController->GetLastTime() > m_fMaxLastTime )
		m_fMaxLastTime = pkController->GetLastTime();

	if ( pkController->GetCycleType() != NiTimeController::CLAMP )
	{
		m_bHasLoop = TRUE;
	}

    m_uiControllerCount++;

    if(NiIsKindOf(NiInterpController, pkController))
        AddInterpInfo((NiInterpController*) pkController);
    else if(NiIsKindOf(NiUVController, pkController))
        AddUVInfo((NiUVController*) pkController);
    
}
//---------------------------------------------------------------------------
void CNifAnimationInfo::FindAndAdd(NiObjectNET* pkRoot)
{
   if(pkRoot == NULL)
        return;

   
   NiTimeController* pkController = pkRoot->GetControllers();

   // Gather all time controllers from this object
   while(pkController != NULL)
   {
       Add(pkController);
       pkController = pkController->GetNext();
   }

   if(NiIsKindOf(NiAVObject, pkRoot))
   {
       // NiProperties can have time controllers, so search them too
       NiAVObject* pkObj = (NiAVObject*) pkRoot;
       NiPropertyList* pkPropList = &(pkObj->GetPropertyList());
       NiTListIterator kIter = pkPropList->GetHeadPos();
       while(pkPropList != NULL && !pkPropList->IsEmpty()&& kIter)
       {
           NiProperty* pkProperty = pkPropList->GetNext(kIter);
           if(pkProperty)
               FindAndAdd(pkProperty);
       }
   }
   if(NiIsKindOf(NiNode, pkRoot))
   {
        NiNode* pkNode = (NiNode*) pkRoot;
        // Search all of the children
        for(unsigned int ui = 0; ui < pkNode->GetArrayCount(); ui++)
        {
            NiAVObject* pkObj = pkNode->GetAt(ui);
            FindAndAdd(pkObj);
        }
        // NiDynamicEffects can have time controllers, so search them too
        const NiDynamicEffectList* pkEffectList= &(pkNode->GetEffectList());

        NiTListIterator kIter = pkEffectList->GetHeadPos();
        while(pkEffectList != NULL && !pkEffectList->IsEmpty() && kIter)
        {
           NiDynamicEffect* pkEffect = pkEffectList->GetNext(kIter);
           if(pkEffect)
               FindAndAdd(pkEffect);
        }
        
   }
}
//---------------------------------------------------------------------------
void CNifAnimationInfo::AddInfo(NiTimeController* pkController)
{
}
//---------------------------------------------------------------------------
void CNifAnimationInfo::AddInterpInfo(NiInterpController* pkController)
{
    for (unsigned int us = 0; us <  pkController->
        GetInterpolatorCount(); us++)
    {
        NiInterpolator* pkInterp = pkController->GetInterpolator(us);
        if (NiIsKindOf(NiKeyBasedInterpolator, pkInterp))
        {
            NiKeyBasedInterpolator* pkKeyInterp =
                (NiKeyBasedInterpolator*) pkInterp;
            for (unsigned short usc = 0; usc < pkKeyInterp->
                GetKeyChannelCount(); usc++)
            {
                m_uiKeyframeCount += pkKeyInterp->GetKeyCount(usc);
            }
        }
    }
}
//---------------------------------------------------------------------------
void CNifAnimationInfo::AddUVInfo(NiUVController* pkController)
{
    m_uiKeyframeCount += pkController->GetNumUOffsetKeys () +
                        pkController->GetNumVOffsetKeys () + 
                        pkController->GetNumUTilingKeys () +
                        pkController->GetNumVTilingKeys ();
    
}
//---------------------------------------------------------------------------
