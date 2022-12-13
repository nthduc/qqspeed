// EMERGENT GAME TECHNOLOGIES PROPRIETARY INFORMATION
//
// This software is supplied under the terms of a license agreement or
// nondisclosure agreement with Emergent Game Technologies and may not 
// be copied or disclosed except in accordance with the terms of that 
// agreement.
//
//      Copyright (c) 1996-2006 Emergent Game Technologies.
//      All Rights Reserved.
//
// Emergent Game Technologies, Chapel Hill, North Carolina 27517
// http://www.emergent.net
#include "Top-KartPCH.h"
#include <NiApplication.h>
#include <NiColor.h>
#include <NiNode.h>
#include <NiShaderFactory.h>
#include <NiStencilProperty.h>
#include <NiTriShape.h>
#include <NiTriShapeDynamicData.h>
#include <NiVertexColorProperty.h>
#include <crtdbg.h>
#include "common.h"
#include "variant.h"
#include "Utility.h"
#include "Swoosh.h"

Swoosh::Swoosh(NiAVObject* pkTop, float fInitialTime)
{
	//读入swoosh参数
	CVarTable varTable("swoosh");
	CVar::LoadVarsFromFile(&varTable, NiApplication::ConvertMediaFilename("effect/swoosh.lua"));
	m_fVelocityMinimum = varTable.Element("v_min")->Float();
	m_fVelocityMaximum = varTable.Element("v_max")->Float();
	m_fWidthRef = varTable.Element("width_ref")->Float();
	m_nQuadCount = varTable.Element("quads_count")->Integer();
	_ASSERTE(m_fVelocityMinimum<m_fVelocityMaximum);
	//防止超过最大值
	_ASSERTE(m_nQuadCount>=2 && m_nQuadCount<=NUM_QUADS_LIMITE);

	m_kVertices=NiNew	NiPoint3[m_nQuadCount * 2];
	m_kTexCoords=NiNew	NiPoint2[m_nQuadCount * 2];
	m_afVelocityScales=new	float[m_nQuadCount * 2];
    //Create the arrays we'll need for the NiTriShapeDynamicData object
    NiPoint3* pkVerts = NiNew NiPoint3[m_nQuadCount * 2];
    NiPoint3* pkNormals = NiNew NiPoint3[m_nQuadCount * 2];
    NiPoint2* pkTexCoords = NiNew NiPoint2[m_nQuadCount * 2];
    NiColorA* pkColors = NiNew NiColorA[m_nQuadCount * 2];
    unsigned short* pkConnect = NiAlloc(unsigned short, m_nQuadCount * 2 * 3);

    //Initialize everything
    float fUStep = 1.0f / m_nQuadCount;
    int i = 0;
    for (; i < m_nQuadCount * 2; i++)
    {
        pkVerts[i] = pkTop->GetWorldBound().GetCenter();/*pkTop->GetWorldTranslate()*/;
		pkNormals[i] = NiPoint3(0.0f, 0.0f,1.0f);
        
        float fU = fUStep * (i / 2);
        float fV = 1.0f * (i % 2);
        pkTexCoords[i] = NiPoint2(fU, fV);
        m_kTexCoords[i] = NiPoint2(fU, fV);
        
        m_afVelocityScales[i] = 0.0f;
        
        pkColors[i] = NiColorA(1.0f, 1.0f, 1.0f, 1.0f - (i/2) * fUStep);
    }

    //Esatblish the connectivity array.  This will not change.
    for (i = 0; i < m_nQuadCount; i++)
    {
        pkConnect[6 * i] = 2 * i;
        pkConnect[6 * i + 1] = 2 * i + 1;
        pkConnect[6 * i + 2] = 2 * i + 2;
        pkConnect[6 * i + 3] = 2 * i + 1;
        pkConnect[6 * i + 4] = 2 * i + 3;
        pkConnect[6 * i + 5] = 2 * i + 2;
    }


    //Seed the system with one quad.
    m_iStartingIndex = 0;
    m_iNumberOfQuads = 1;

    m_kVertices[m_iStartingIndex] = pkTop->GetWorldBound().GetCenter();//pkTop->GetWorldTranslate();
    m_kLastPoint = pkTop->GetWorldBound().GetCenter();///pkTop->GetWorldTranslate();
    m_kVertices[m_iStartingIndex + 1] = (pkTop->GetWorldRotate() * 
        NiPoint3(m_fWidthRef, 0.0f, 0.0f)) + pkTop->GetWorldBound().GetCenter();//pkTop->GetWorldTranslate();

    NiTriShapeDynamicData* pkData = NiNew NiTriShapeDynamicData(
        m_nQuadCount * 2, pkVerts, pkNormals, 
        pkColors, pkTexCoords, 1, NiGeometryData::NBT_METHOD_NONE, 
        m_nQuadCount * 2 - 2, pkConnect, 0, 0);

    //Set a fake bound that insures rendering to begin with.
    NiBound kBound;
    kBound.SetCenter(0.0f, 0.0f, 0.0f);
    kBound.SetRadius(100.0f);
    pkData->SetBound(kBound);

    m_pkTriShape = NiNew NiTriShape(pkData);

    m_pkGeomGroup = NiGeometryGroupManager::GetManager()->
        CreateGroup(NiGeometryGroupManager::DYNAMIC);
    NiGeometryGroupManager::GetManager()->AddObjectToGroup(m_pkGeomGroup, 
        m_pkTriShape->GetModelData());

    m_pkTriShape->SetName("The Swoosh");
    //pkScene->AttachChild(m_pkTriShape);

    //The shader takes care of all the necessary properties like making it
    //double sided, etc...
    NiMaterial* pkMaterial = NiSingleShaderMaterial::Create("Swoosh");
    m_pkTriShape->ApplyAndSetActiveMaterial(pkMaterial);

    //The shader depends on a base texture being there.
	NiTexturingProperty* pkTex = NiNew NiTexturingProperty(
    NiApplication::ConvertMediaFilename("swoosh.dds"));

    m_pkTriShape->AttachProperty(pkTex);

    m_pkTopObject = pkTop;
    m_fLastTime = fInitialTime - 0.001f;
    
    //Take the rist sample to get the swoosh started.
    TakeSample(fInitialTime);

    m_pkTriShape->Update(0.0f);
    m_pkTriShape->UpdateProperties();
    m_pkTriShape->UpdateEffects();

}

Swoosh::~Swoosh()
{
	SAFE_NIDELETE_ARRAY(m_kVertices);
	SAFE_NIDELETE_ARRAY(m_kTexCoords);
	SAFE_DELETE_ARRAY(m_afVelocityScales);
	NiDelete(m_pkTriShape);
}

//绘制道具栏和交换器
void Swoosh::Draw(NiRendererPtr spRenderer)
{
	m_pkTriShape->RenderImmediate(spRenderer);
}

void Swoosh::Update(float fTime)
{

}

//---------------------------------------------------------------------------
void Swoosh::TakeSample(float fTime)
{
    //Establish iWriteIndex so we can insert into a circular queue.  Thus,
    //as points age out of the swoosh they are overwritten.
    int iWriteIndex = 0;
    if (m_iNumberOfQuads == m_nQuadCount)
    {
        m_iStartingIndex -= 2;
        if (m_iStartingIndex < 0)
            m_iStartingIndex += m_nQuadCount * 2;
        iWriteIndex = m_iStartingIndex;
    }
    else
    {
        iWriteIndex = m_iStartingIndex + (2 * m_iNumberOfQuads);
        m_iNumberOfQuads++;
    }

    //The swoosh points are the staff and the point -70 on the staff's
    //local y axis which we then rotate into world space.  All swoosh 
    //geometry is in world space.
	
	NiPoint3 kCurPos = m_pkTopObject->GetWorldBound().GetCenter();//m_pkTopObject->GetWorldTranslate();
	kCurPos.z += 0.06f;

    m_kVertices[iWriteIndex] = kCurPos;
    m_kVertices[iWriteIndex + 1] = (m_pkTopObject->GetWorldRotate() * 
        NiPoint3(m_fWidthRef, 0.0f, 0.0f)) + kCurPos;

    NiPoint3 kPoint = m_pkTopObject->GetWorldBound().GetCenter();//m_pkTopObject->GetWorldTranslate();
    kPoint -= m_kLastPoint;
    float fDist = NiSqrt(
        kPoint.x * kPoint.x + kPoint.y * kPoint.y + kPoint.z * kPoint.z) ;

    float fDeltaTime = fTime - m_fLastTime;
	//XTrace("swoosh fdist=%f   fDelta=%f",fDist,fDeltaTime);
    if (fDeltaTime < 0.001f)
    {
        fDist = 0.0f;
        fDeltaTime = 1.0f;
    }

    fDist /= fDeltaTime;

    if (fDist > m_fVelocityMaximum)
    {
        fDist = 1.0f;
    }
    else if (fDist < m_fVelocityMinimum)
    {
        fDist = 0.0f;
    }
    else
    {
        fDist = (fDist - m_fVelocityMinimum) / (m_fVelocityMaximum - 
            m_fVelocityMinimum);
    }

    //Calculate the velocity and lerp between 0 and 1 for the velocity
    //scale.  This means the swoosh only appears when the staff moves
    //quickly.
    m_afVelocityScales[iWriteIndex] = fDist;
    m_afVelocityScales[iWriteIndex + 1] = fDist;

    m_fLastTime = fTime;
    m_kLastPoint = m_pkTopObject->GetWorldBound().GetCenter();//m_pkTopObject->GetWorldTranslate();
	//todo:这里的坐标当绑定指定车灯后就无需修正z值
	m_kLastPoint.z += 0.1f;
	

    NiPoint3* pkVerts = m_pkTriShape->GetVertices();
    NiPoint2* pkTextures = m_pkTriShape->GetTextures();
    NiColorA* pkColors = m_pkTriShape->GetColors();

    //Rebuild the data for the renderable object from the values we have
    //stored in the class members.
    float fColorStep = 1.0f / m_nQuadCount;
    for (int i = 0; i < m_iNumberOfQuads * 2; i++)
    {
        pkVerts[i] = m_kVertices[(i + m_iStartingIndex) % (m_nQuadCount * 2)];
		pkTextures[i] = m_kTexCoords[(i /*todo: + m_iStartingIndex*/) % (m_nQuadCount * 2)];
        pkColors[i] = (NiColorA(1.0f, 1.0f, 1.0f, (1.0f - (i/2) * fColorStep))
            * m_afVelocityScales[(i + m_iStartingIndex) % (m_nQuadCount * 2)]);

        if ((i + m_iStartingIndex) == m_nQuadCount * 2 ||
            (i + m_iStartingIndex) == m_nQuadCount * 2 + 1)
        {
            pkVerts[i] = m_kVertices[i + m_iStartingIndex - 2];
        }
    }

    //Call MarkAsChanged to inform the renderer to repack.
    NiTriShapeDynamicData* pkData = (NiTriShapeDynamicData*)(m_pkTriShape->
        GetModelData());
    pkData->SetActiveTriangleCount(m_iNumberOfQuads * 2 - 2);
    pkData->SetActiveVertexCount(m_iNumberOfQuads * 2);
    pkData->MarkAsChanged(NiGeometryData::VERTEX_MASK |
        NiGeometryData::TEXTURE_MASK | NiGeometryData::COLOR_MASK);

    //Update the bound for proper sorting and culling.
    NiBound kBound;
    kBound.ComputeFromData(m_iNumberOfQuads * 2, pkVerts);
    m_pkTriShape->SetModelBound(kBound);
    m_pkTriShape->Update(0.0f);
}
    