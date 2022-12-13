////////////////////////////////////////////////
//     FileName:    gprender.cpp
//     Author:      Fish
//     Date:        2007-1-23 21:23:06
//     Description: []
////////////////////////////////////////////////

#include "Top-KartPCH.h"
#include "gprender.h"

NiBillboardNodePtr CGPRender::CreateBillboard(int w, int h)
{
	NiBillboardNodePtr spBilloard = NiNew NiBillboardNode();
	spBilloard->SetMode(NiBillboardNode::ALWAYS_FACE_CAMERA);

	NiPoint3 * pkVertex = NiNew NiPoint3[4];
	float fW = w;
	float fH = h;

	pkVertex[0] =  NiPoint3(0, -fW * 0.5, fH * 0.5);
	pkVertex[1] =  NiPoint3(0, -fW * 0.5, -fH * 0.5);
	pkVertex[2] =  NiPoint3(0, fW * 0.5, -fH * 0.5);
	pkVertex[3] =  NiPoint3(0, fW * 0.5, fH * 0.5);


	NiPoint2 * pkUV = NiNew NiPoint2[4];

	pkUV[0]  = NiPoint2(0,0);
	pkUV[1] = NiPoint2(0,1);
	pkUV[2] = NiPoint2(1,1);
	pkUV[3] = NiPoint2(1,0);

	unsigned short* pusConnect = (unsigned short*)NiMalloc(
		sizeof(unsigned short) * (2 * 3));

	pusConnect[0] = 0;
	pusConnect[1] = 1;
	pusConnect[2] = 2;
	pusConnect[3] = 0;
	pusConnect[4] = 2;
	pusConnect[5] = 3;

	NiColorA kColor(0.39f, 0.1f, 0.78f, 1.0f);
	NiColorA * pkColor = NiNew NiColorA[4];
	for ( int i = 0 ; i < 4 ; i++)
	{
		pkColor[i] = kColor;
	}

	NiTriShapeDynamicDataPtr spkTriData = NiNew NiTriShapeDynamicData
		(4,pkVertex,0, pkColor,pkUV,
		1,
		NiGeometryData::NBT_METHOD_NONE,
		2,
		pusConnect,
		4,2);

	NiTriShapePtr spTriShape = NiNew NiTriShape(NiDynamicCast(NiTriShapeData,spkTriData));
	NiTexturingPropertyPtr spTexture = NiNew NiTexturingProperty();

	NiAlphaPropertyPtr spAlphaPro = NiNew NiAlphaProperty;
	NiZBufferPropertyPtr spZbufferPro = NiNew NiZBufferProperty;
	NiAmbientLightPtr spLight = NiNew NiAmbientLight;

	spZbufferPro->SetZBufferTest(true);
	spZbufferPro->SetZBufferWrite(false);
	spTriShape->AttachProperty(spZbufferPro);

	spAlphaPro->SetAlphaBlending(true);
	spAlphaPro->SetSrcBlendMode(NiAlphaProperty::ALPHA_SRCALPHA);
	spAlphaPro->SetDestBlendMode(NiAlphaProperty::ALPHA_INVSRCALPHA);
	spTriShape->AttachProperty(spAlphaPro);

	spLight->SetDimmer(1.0);
	spLight->SetAmbientColor(NiColor(1.0, 1.0, 1.0));
	spLight->AttachAffectedNode(spBilloard);
	spBilloard->AttachChild(spLight);
	spLight->SetSwitch(true);

	NiMatrix3 kRot90;
	kRot90.MakeYRotation(NI_PI/2.0f);
	spTriShape->SetRotate(kRot90);

	spTexture->SetApplyMode(NiTexturingProperty::APPLY_MODULATE);
	spTriShape->AttachProperty(spTexture);

	spBilloard->AttachChild(NiDynamicCast(NiAVObject, spTriShape));
	//spBilloard->SetSelectiveUpdate(true);
	spBilloard->SetSelectiveUpdateTransforms(true);
	spBilloard->SetSelectiveUpdatePropertyControllers(true);
	spBilloard->SetSelectiveUpdateRigid(true);
	spBilloard->UpdateProperties();
	spBilloard->UpdateNodeBound();
	spBilloard->Update(0.0);

	return spBilloard;
}