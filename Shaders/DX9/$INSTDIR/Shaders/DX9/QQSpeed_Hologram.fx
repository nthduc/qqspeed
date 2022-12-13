//============================================================
// QQ飞车中全息影像材质.
// @author watsonsong
//============================================================

// --------------预设变量--------------
// 非蒙皮的世界变换信息.
float4x4 WorldMat
<
    string VarType = "Predefined";
    string DefinedMapping = "World";
>;

float4x4 ViewProjMat
<
    string VarType = "Predefined";
    string DefinedMapping = "ViewProj";
>;

float4x4 WorldViewProjMat
<
    string VarType = "Predefined";
    string DefinedMapping = "WorldViewProj";
>;

float4x4 SkinWorldMat
<
    string VarType = "Predefined";
    string DefinedMapping = "SkinWorld";
>;

float4x4 SkinWorldViewProjMat
<
    string VarType = "Predefined";
    string DefinedMapping = "SkinWorldViewProj";
>;

static const int MAX_BONES = 20;
float4x3 BoneMats[MAX_BONES]
<
    string VarType = "Predefined";
    string DefinedMapping = "BoneMatrix3";
>;

float WaveControl = 1.f;

// --------------材质调整变量--------------
// 纹理及其采样器.
texture BaseMap
< 
    string NTM = "base";
>;

texture ShaderMap0 
< 
    string NTM = "shader";
	int NTMIndex = 0;
>;

sampler BaseSampler
<
    string NTM = "base";
    int NTMIndex = 0;
> = sampler_state 
{
    texture = (BaseMap);
    AddressU  = WRAP;
    AddressV  = WRAP;
    MIPFILTER = LINEAR;
    MINFILTER = LINEAR;
    MAGFILTER = LINEAR;
};

sampler AdjustSampler = sampler_state
{ 
    Texture = (ShaderMap0);
    AddressU = WRAP;
    AddressV = WRAP;
    MipFilter = LINEAR;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
};

// 变换蒙皮坐标
void TransformSkinnedPosition(float4 Position, int4 BlendIndices, 
    float4 BlendWeights, float4x3 Bones[MAX_BONES], 
	float4x4 SkinToWorldTransform, out float4 OutPos)
{
	//int4 indices = D3DCOLORtoUBYTE4(BlendIndices);
	int4 indices = BlendIndices;

    // Calculate normalized fourth bone weight
    float weight4 = 1.0f - BlendWeights[0] - BlendWeights[1] - BlendWeights[2];
    float4 weights = float4(BlendWeights[0], BlendWeights[1], BlendWeights[2], weight4);

    // Calculate bone transform
    float4x3 BoneTransform;
	BoneTransform = weights[0] * Bones[indices[0]];
	BoneTransform += weights[1] * Bones[indices[1]];
	BoneTransform += weights[2] * Bones[indices[2]];
	BoneTransform += weights[3] * Bones[indices[3]];
	
	float3 BoneSpacePos = mul(Position, BoneTransform);
	OutPos = mul(float4(BoneSpacePos, 1.0), SkinToWorldTransform);
}

// ----------顶点输出----------
struct VS_OUTPUT
{
    float4 Pos			: POSITION;
	float2 Tex0  		: TEXCOORD0;
	float2 Tex1  		: TEXCOORD1;
};

struct VS_OUTPUT_SIMPLE
{
    float4 Pos			: POSITION;
	float2 Tex0  		: TEXCOORD0;
};

// ----------不带蒙皮VS----------
VS_OUTPUT HologramVS(
    float4 Position : POSITION, 
    float2 Tex0 : TEXCOORD0)
{
    VS_OUTPUT output;
	
	float4 wposition = mul(Position, WorldMat);
	
    output.Pos = mul(wposition, ViewProjMat);
	output.Tex0 = Tex0;
	
	float3 uvposition = wposition - float3(
	    WorldMat[3][0], WorldMat[3][1], WorldMat[3][2]);
	float length = uvposition.x * uvposition.x + 
	               uvposition.y * uvposition.y + 
				   uvposition.z * uvposition.z;
	output.Tex1 = float2(uvposition.x / length, 0.65 * uvposition.z - 0.3f);
	
	return output;
}

// ----------带蒙皮VS----------
VS_OUTPUT HologramVS_Skin(
    float4 Position : POSITION, 
	float4 BlendWeights : BLENDWEIGHT0,
    int4 BlendIndices : BLENDINDICES0,
    float2 Tex0 : TEXCOORD0)
{
    VS_OUTPUT output;

    float4 wposition;
    TransformSkinnedPosition(Position, 
        BlendIndices, BlendWeights, BoneMats, SkinWorldMat,
        wposition);

    output.Pos = mul(wposition, ViewProjMat);
	output.Tex0 = Tex0;
	
	float3 uvposition = wposition - float3(
	    SkinWorldMat[3][0], SkinWorldMat[3][1], SkinWorldMat[3][2]);
	float length = uvposition.x * uvposition.x + 
	               uvposition.y * uvposition.y + 
				   uvposition.z * uvposition.z;
	output.Tex1 = float2(uvposition.x / length, 0.65 * uvposition.z - 0.3f);
	
	return output;
}


// ----------不带蒙皮VS----------
VS_OUTPUT_SIMPLE SimpleVS(
    float4 Position : POSITION,
	float2 Tex0 : TEXCOORD0)
{
    VS_OUTPUT_SIMPLE Output;
	
    float4 wvpposition = mul(Position, WorldViewProjMat);
	wvpposition.z += 0.0001f;
	
	Output.Pos = wvpposition;
	Output.Tex0 = Tex0;
	
	return Output;
}

// ----------带蒙皮VS----------
VS_OUTPUT_SIMPLE SimpleVS_Skin(
    float4 Position : POSITION, 
	float4 BlendWeights : BLENDWEIGHT0,
    int4 BlendIndices : BLENDINDICES0,
	float2 Tex0 : TEXCOORD0)
{
    VS_OUTPUT_SIMPLE Output;

    float4 wvpposition;
    TransformSkinnedPosition(Position, 
        BlendIndices, BlendWeights, BoneMats, SkinWorldViewProjMat,
        wvpposition);
	wvpposition.z += 0.0001f;
	
	Output.Pos = wvpposition;
	Output.Tex0 = Tex0;

	return Output;
}

// ----------PS----------
float4 HologramPS(float2 Tex0 : TEXCOORD0,
                  float2 Tex1 : TEXCOORD1) : COLOR
{
	float2 UVOffset = float2(0 ,0);
	UVOffset.y += fmod(WaveControl / 2.5f, 1.0f);
	
	float4 kAdjust = tex2D(AdjustSampler, float2(1, 1) - Tex1 - UVOffset);

	float4 kColor = tex2D(BaseSampler, Tex0);
	float fValue = fmod(1 - Tex1.y, 1.0f);
	kColor.r *= min(1 - fValue + 0.5, 1.0f);
	kColor.g *= 1 + 0.25 * min(fValue, 1.0f);
	kColor.b *= 1 + 0.35 * min(fValue, 1.0f);
	return float4(kColor.rgb + kAdjust.rgb, 0.75f * kColor.a);
}

float4 Simple_PS(float2 Tex0 : TEXCOORD0) : COLOR
{	
	return tex2D(BaseSampler, Tex0);
}

// ----------所有的技术----------
technique QQSpeed_HologramPreZ
<
	string Description = "This shader performs a hologram effect.";
	bool UsesNiRenderState = true;
>
{
	pass p0
    {
		ColorWriteEnable = 0;
		VertexShader = compile vs_2_0 SimpleVS();
		PixelShader = compile ps_2_0 Simple_PS();
		AlphaTestEnable = true;
		AlphaRef=64;
		ZEnable = true;
		ZWriteEnable = true;
	}
}

technique QQSpeed_HologramSkinPreZ
<
	string Description = "This shader performs a hologram effect.";
	bool UsesNiRenderState = true;
	int BonesPerPartition = MAX_BONES;
>
{
	pass p0
    {
		ColorWriteEnable = 0;
		VertexShader = compile vs_2_0 SimpleVS_Skin();
		PixelShader = compile ps_2_0 Simple_PS();
		AlphaTestEnable = true;
		AlphaRef=64;
		ZEnable = true;
		ZWriteEnable = true;
	}
}

technique QQSpeed_Hologram
<
	string Description = "This shader performs a hologram effect.";
	bool UsesNiRenderState = true;
>
{
	pass P0
	{
		VertexShader = compile vs_2_0 HologramVS();
		PixelShader = compile ps_2_0 HologramPS();
		AlphaTestEnable = true;
		AlphaRef=64;
		AlphaBlendEnable = true;
		SrcBlend = SrcAlpha;
		DestBlend = InvSrcAlpha;
		ZEnable = true;
		ZWriteEnable = false;
	}
}

technique QQSpeed_HologramSkin
<
	string Description = "This shader performs a hologram effect.";
	bool UsesNiRenderState = true;
	int BonesPerPartition = MAX_BONES;
>
{
	pass P0
	{
	    ColorWriteEnable=RED|GREEN|BLUE|ALPHA;
		VertexShader = compile vs_2_0 HologramVS_Skin();
		PixelShader = compile ps_2_0 HologramPS();
		AlphaTestEnable = true;
		AlphaRef=64;
		AlphaBlendEnable = true;
		SrcBlend = SrcAlpha;
		DestBlend = InvSrcAlpha;
		ZEnable = true;
		ZWriteEnable = false;
	}
}
