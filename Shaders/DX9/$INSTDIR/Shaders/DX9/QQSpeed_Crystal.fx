//============================================================
// QQ飞车中星光闪烁材质.
// @author watsonsong
//============================================================

// --------------预设变量--------------
// 非蒙皮的世界变换信息.
float4x4 WorldViewMat
<
    string VarType = "Predefined";
    string DefinedMapping = "WorldView";
>;

float4x4 WorldViewMatIT
<
    string VarType = "Predefined";
    string DefinedMapping = "InvWorldViewTranspose";
>;

float4x4 ViewMat
<
    string VarType = "Predefined";
    string DefinedMapping = "View";
>;

float4x4 ViewMatIT
<
    string VarType = "Predefined";
    string DefinedMapping = "InvViewTranspose";
>;

float4x4 ProjectionMat
<
    string VarType = "Predefined";
    string DefinedMapping = "Projection";
>;

// 蒙皮的世界变换信息.
float4x4 SkinWorldMat
<
    string VarType = "Predefined";
    string DefinedMapping = "SkinWorld";
>;

float4x4 SkinWorldViewMat
<
    string VarType = "Predefined";
    string DefinedMapping = "SkinWorldView";
>;

static const int MAX_BONES = 20;
float4x3 BoneMats[MAX_BONES]
<
    string VarType = "Predefined";
    string DefinedMapping = "BoneMatrix3";
>;

// 材质的曝光值.
float Shininess
<
    string VarType = "Predefined";
    string DefinedMapping = "MATERIALPOWER";
> = 50.0f;

float Time : TIME = 1.0f;

// --------------材质调整变量--------------
// 闪烁纹理及其采样器.
texture BaseMap
< 
    string NTM = "base";
>;
sampler BaseSampler
<
    string NTM = "base";
    int NTMIndex = 0;
> = sampler_state 
{
    texture = (BaseMap);
    AddressU  = CLAMP;
    AddressV  = CLAMP;
    MIPFILTER = LINEAR;
    MINFILTER = LINEAR;
    MAGFILTER = LINEAR;
};

float FlickerSpeed : ATTRIBUTE 
<
    string description = "The flicker speed of the crystal.";
> = 2.0f;

float MoveRangeScale : ATTRIBUTE 
<
    string description = "The movement range scale of the crystal.";
> = 0.01f;

float2 CrystalMinSize : ATTRIBUTE 
<
    string description = "The min size of crystal.";
> = float2(0.01f, 0.01f);

float2 CrystalMaxSize : ATTRIBUTE 
<
    string description = "The max size of crystal.";
> = float2(0.1f, 0.1f);

// 水晶表,记录水晶每个面的反射向量,一共30个面.
static const int SLICE_COUNT = 30;
static const float2 CrystalTable[SLICE_COUNT] = 
{
	{1.000000,0.000000},
	{1.000000,0.000000},
	{0.642788,0.766044},
    {0.500000,0.866025},
    {0.642788,0.766044},
    {0.766044,0.642788},
    {0.374607,0.927184},
    {0.766044,0.642788},
    {1.000000,0.000000},
    {0.999848,0.017452},
    {0.996195,0.087156},
    {0.567844,0.823136},
    {0.996195,0.087156},
    {0.999848,0.017452},
    {0.939693,0.342020},
    {0.766044,0.642788},
    {0.866025,0.500000},
    {0.999391,0.034899},
    {1.000000,0.000000},
    {1.000000,0.000000},
    {0.939693,0.342020},
    {1.000000,0.000000},
    {0.984808,0.173648},
    {0.939693,0.342020},
    {0.984808,0.173648},
    {0.996195,0.087156},
    {0.939693,0.342020},
    {0.996195,0.087156},
    {1.000000,0.000000},
    {1.000000,0.000000}
};

// 变换蒙皮坐标
void TransformSkinnedPosition(float4 Position, float3 Normal,
    int4 BlendIndices, float3 BlendWeights,
    float4x3 Bones[MAX_BONES], float4x4 SkinToWorldTransform,
    out float4 OutPos, out float3 OutNormal)
{
    int4 indices = D3DCOLORtoUBYTE4(BlendIndices);

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
	float3 BoneSpaceNor = mul(Normal, (float3x3)BoneTransform);
	OutPos = mul(float4(BoneSpacePos, 1.0), SkinToWorldTransform);
	OutNormal = mul(BoneSpaceNor, (float3x3)SkinToWorldTransform);
}

// 计算水晶反射后的顶点坐标
float4 TransformCrystalReflection(
    float4 wvposition, float3 wvnormal, float2 tex, float2 random,
    float4x4 ProjectionMat, float shininess, float2 crystalTable[SLICE_COUNT])
{
	float3 eyedir = -normalize(wvposition.xyz);
	float3 lightdir = normalize(-wvposition.xyz);

	// 镜面反射向量
	float3 halfV = normalize(lightdir + eyedir);
	
	// 计算水晶反射
	// 不使用漫反射部分,只计算镜面反射部分.
	float2 NLCosSin;
	NLCosSin.x = dot(wvnormal, halfV);
	NLCosSin.y = sqrt(1-(NLCosSin.x*NLCosSin.x));
	float index = max(NLCosSin.x * SLICE_COUNT, 4);
	NLCosSin = NLCosSin * crystalTable[index];
	float NDotH = NLCosSin.x + NLCosSin.y;
	
	float scale = pow(max(NDotH, 0), shininess);
	
	// 变换顶点,将四个重合的顶点根据sizeMin和sizeMax的值扩展.
	float4 wvpos2;
	wvpos2.xyz = cross(wvnormal, lightdir) + wvposition.xyz;
	wvpos2.w = 0;
	
	// 8字位移.
	float deltaX = MoveRangeScale * sin(Time + random.x);
	float deltaY = MoveRangeScale * sin(2*(Time + random.x));
	wvposition.x += deltaX;
	wvposition.y += deltaY;
	wvpos2.x += deltaX;
	wvpos2.y += deltaY;
	
	float4 projpos1 = mul(wvposition, ProjectionMat);
	float4 projpos2 = mul(wvpos2, ProjectionMat);
	float2 pos2d1 = projpos1.xy / projpos1.ww;
	float2 pos2d2 = projpos2.xy / projpos2.ww;
	float2 dir2d = normalize(pos2d2 - pos2d1);

    float2 sizeMax = 2 * (tex - float2(0.5, 0.5)) * CrystalMaxSize;
    float2 sizeMin = 2 * (tex - float2(0.5, 0.5)) * CrystalMinSize;

	float timeDelta = (sin(FlickerSpeed*Time + random.x) * 0.5 + 1);
	float s = (scale * sizeMax.x + sizeMin.x * timeDelta) * random.y;
	projpos1.xy = dir2d*s + projpos1.xy;
	
	s = (scale * sizeMax.y + sizeMin.y * timeDelta) * random.y;
	projpos1.x =  dir2d.y*s + projpos1.x;
	projpos1.y = -dir2d.x*s + projpos1.y;

	return projpos1;
}

// 平滑的计算水晶反射后的顶点坐标
float4 TransformCrystalSmoothReflection(
    float4 wvposition, float3 wvnormal, float2 tex, float2 random,
    float4x4 ProjectionMat, float shininess)
{
	float3 eyedir = -normalize(wvposition.xyz);
	float3 lightdir = normalize(-wvposition.xyz);

	// 镜面反射向量
	float3 halfV = normalize(lightdir + eyedir);
	
	// 计算平滑反射
	float NDotL = dot(wvnormal, lightdir);
	float NDotH = dot(wvnormal, halfV);
	
	float scale = lit(NDotL, NDotH, shininess).z;
	
	// 变换顶点,将四个重合的顶点根据sizeMin和sizeMax的值扩展.
	float4 wvpos2;
	wvpos2.xyz = cross(wvnormal, lightdir) + wvposition.xyz;
	wvpos2.w = 0;
	
	// 8字位移.
	float deltaX = MoveRangeScale * sin(Time + random.x);
	float deltaY = MoveRangeScale * sin(2*(Time + random.x));
	wvposition.x += deltaX;
	wvposition.y += deltaY;
	wvpos2.x += deltaX;
	wvpos2.y += deltaY;
	
	float4 projpos1 = mul(wvposition, ProjectionMat);
	float4 projpos2 = mul(wvpos2, ProjectionMat);
	float2 pos2d1 = projpos1.xy / projpos1.ww;
	float2 pos2d2 = projpos2.xy / projpos2.ww;
	float2 dir2d = normalize(pos2d2 - pos2d1);
	
	float2 sizeMax = 2 * (tex - float2(0.5, 0.5)) * CrystalMaxSize;
    float2 sizeMin = 2 * (tex - float2(0.5, 0.5)) * CrystalMinSize;

	float timeDelta = (sin(FlickerSpeed*Time + random.x) * 0.5 + 1);
	float s = (scale * sizeMax.x + sizeMin.x * timeDelta) * random.y;
	projpos1.xy = dir2d*s + projpos1.xy;
	
	s = (scale * sizeMax.y + sizeMin.y * timeDelta) * random.y;
	projpos1.x =  dir2d.y*s + projpos1.x;
	projpos1.y = -dir2d.x*s + projpos1.y;

	return projpos1;
}

// ----------顶点输出----------
struct VS_OUTPUT
{
    float4 Pos			: POSITION;
	float2 Tex0  		: TEXCOORD0;
};

struct VS_COLOR_OUTPUT
{
    float4 Pos			: POSITION;
    float4 Color		: COLOR0;
	float2 Tex0  		: TEXCOORD0;
};

// ----------不带顶点色不带蒙皮VS----------
VS_OUTPUT CrystalVS(
    float4 Position : POSITION, 
	float3 Normal : NORMAL,
    float2 Tex0 : TEXCOORD0,
    float2 Tex1 : TEXCOORD1)
{
    float4 wvposition = mul(Position, WorldViewMat);
	float3 wvnormal = normalize(mul(Normal, (float3x3)WorldViewMatIT));

    VS_OUTPUT output;
    output.Pos = TransformCrystalReflection(
		wvposition, wvnormal, Tex0, Tex1,
		ProjectionMat, Shininess, CrystalTable);
	output.Tex0 = Tex0;
	return output;
}

// ----------带顶点色不带蒙皮VS----------
VS_COLOR_OUTPUT CrystalVS_Color(
    float4 Position : POSITION, 
	float3 Normal : NORMAL,
	float4 Color : COLOR0,
    float2 Tex0 : TEXCOORD0,
    float2 Tex1 : TEXCOORD1)
{
    float4 wvposition = mul(Position, WorldViewMat);
	float3 wvnormal = normalize(mul(Normal, (float3x3)WorldViewMatIT));
	
	VS_COLOR_OUTPUT output;
	
    output.Pos = TransformCrystalReflection(
		wvposition, wvnormal, Tex0, Tex1,
		ProjectionMat, Shininess, CrystalTable);
		
	output.Color = Color;
	output.Tex0 = Tex0;
	return output;
}

// ----------不带顶点色带蒙皮VS----------
VS_OUTPUT CrystalVS_Skin(
    float4 Position : POSITION, 
	float3 Normal : NORMAL,
	float3 BlendWeights : BLENDWEIGHT0,
    int4 BlendIndices : BLENDINDICES0,
    float2 Tex0 : TEXCOORD0,
    float2 Tex1 : TEXCOORD1)
{
    float4 wvposition;
	float3 wvnormal;
    TransformSkinnedPosition(Position, Normal, 
        BlendIndices, BlendWeights, BoneMats, SkinWorldViewMat,
        wvposition, wvnormal);
    
    VS_OUTPUT output;

    output.Pos = TransformCrystalReflection(
		wvposition, wvnormal, Tex0, Tex1,
		ProjectionMat, Shininess, CrystalTable);
	output.Tex0 = Tex0;
	
	return output;
}

// ----------带顶点色带蒙皮VS----------
VS_COLOR_OUTPUT CrystalVS_SkinColor(
    float4 Position : POSITION, 
	float3 Normal : NORMAL,
	float4 Color : COLOR0,
	float3 BlendWeights : BLENDWEIGHT0,
    int4 BlendIndices : BLENDINDICES0,
    float2 Tex0 : TEXCOORD0,
    float2 Tex1 : TEXCOORD1)
{
    float4 wvposition;
	float3 wvnormal;
    TransformSkinnedPosition(Position, Normal, 
        BlendIndices, BlendWeights, BoneMats, SkinWorldViewMat,
        wvposition, wvnormal);
    
    VS_COLOR_OUTPUT output;
    
    output.Pos = TransformCrystalReflection(
		wvposition, wvnormal, Tex0, Tex1,
		ProjectionMat, Shininess, CrystalTable);
	output.Color = Color;
	output.Tex0 = Tex0;
	
	return output;
}

// ----------平滑不带顶点色不带蒙皮VS----------
VS_OUTPUT CrystalVS_Smooth(
    float4 Position : POSITION, 
	float3 Normal : NORMAL,
    float2 Tex0 : TEXCOORD0,
    float2 Tex1 : TEXCOORD1)
{
    float4 wvposition = mul(Position, WorldViewMat);
	float3 wvnormal = normalize(mul(Normal, (float3x3)WorldViewMatIT));

    VS_OUTPUT output;
    output.Pos = TransformCrystalSmoothReflection(
		wvposition, wvnormal, Tex0, Tex1, ProjectionMat, Shininess);
	output.Tex0 = Tex0;
	
	return output;
}

// ----------平滑带顶点色不带蒙皮VS----------
VS_COLOR_OUTPUT CrystalVS_SmoothColor(
    float4 Position : POSITION, 
	float3 Normal : NORMAL,
	float4 Color : COLOR0,
    float2 Tex0 : TEXCOORD0,
    float2 Tex1 : TEXCOORD1)
{
    float4 wvposition = mul(Position, WorldViewMat);
	float3 wvnormal = normalize(mul(Normal, (float3x3)WorldViewMatIT));

    VS_COLOR_OUTPUT output;
    output.Pos = TransformCrystalSmoothReflection(
		wvposition, wvnormal, Tex0, Tex1, ProjectionMat, Shininess);
	output.Color = Color;
	output.Tex0 = Tex0;
	
	return output;
}

// ----------平滑不带顶点色带蒙皮VS----------
VS_OUTPUT CrystalVS_SmoothSkin(
    float4 Position : POSITION, 
	float3 Normal : NORMAL,
	float3 BlendWeights : BLENDWEIGHT0,
    int4 BlendIndices : BLENDINDICES0,
    float2 Tex0 : TEXCOORD0,
    float2 Tex1 : TEXCOORD1)
{
    float4 wvposition;
	float3 wvnormal;
    TransformSkinnedPosition(Position, Normal, 
        BlendIndices, BlendWeights, BoneMats, SkinWorldViewMat,
        wvposition, wvnormal);

	VS_OUTPUT output;
	output.Pos = TransformCrystalSmoothReflection(
		wvposition, wvnormal, Tex0, Tex1, ProjectionMat, Shininess);
	output.Tex0 = Tex0;
	
	return output;
}

// ----------平滑带顶点色带蒙皮VS----------
VS_COLOR_OUTPUT CrystalVS_SmoothSkinColor(
    float4 Position : POSITION, 
	float3 Normal : NORMAL,
	float4 Color : COLOR0,
	float3 BlendWeights : BLENDWEIGHT0,
    int4 BlendIndices : BLENDINDICES0,
    float2 Tex0 : TEXCOORD0,
    float2 Tex1 : TEXCOORD1)
{
    float4 wvposition;
	float3 wvnormal;
    TransformSkinnedPosition(Position, Normal, 
        BlendIndices, BlendWeights, BoneMats, SkinWorldViewMat,
        wvposition, wvnormal);

    VS_COLOR_OUTPUT output;
    output.Pos = TransformCrystalSmoothReflection(
		wvposition, wvnormal, Tex0, Tex1, ProjectionMat, Shininess);
	output.Color = Color;
	output.Tex0 = Tex0;
	return output;
}

// ----------不带颜色PS----------
float4 CrystalPS(float2 TexCoords : TEXCOORD0) : COLOR
{
	return tex2D(BaseSampler, TexCoords);
}

float4 CrystalPS_Color(float2 TexCoords : TEXCOORD0,
					float4 Color : COLOR0) : COLOR
{
	return tex2D(BaseSampler, TexCoords) * Color;
}

// ----------所有的技术----------
technique QQSpeed_Crystal
<
	string Description = "This shader performs a crystal effect.";
	bool UsesNiRenderState = false;
>
{
	pass P0
	{
		VertexShader = compile vs_2_0 CrystalVS();
		PixelShader = compile ps_2_0 CrystalPS();
		AlphaBlendEnable = true;
		SrcBlend = One;
		DestBlend = One;
		CullMode = None;
		ZEnable = true;
		ZWriteEnable = false;
	}
}

technique QQSpeed_CrystalColor
<
	string Description = "This shader performs a crystal effect, with color.";
	bool UsesNiRenderState = false;
>
{
	pass P0
	{
		VertexShader = compile vs_2_0 CrystalVS_Color();
		PixelShader = compile ps_2_0 CrystalPS_Color();
		AlphaBlendEnable = true;
		SrcBlend = One;
		DestBlend = One;
		CullMode = None;
		ZEnable = true;
		ZWriteEnable = false;
	}
}

technique QQSpeed_CrystalSkinning
<
	string Description = "This shader performs a crystal effect, with skin.";
	int BonesPerPartition = MAX_BONES;
	bool UsesNiRenderState = false;
>
{
	pass P0
	{
		VertexShader = compile vs_2_0 CrystalVS_Skin();
		PixelShader = compile ps_2_0 CrystalPS();
		AlphaBlendEnable = true;
		SrcBlend = One;
		DestBlend = One;
		CullMode = None;
		ZEnable = true;
		ZWriteEnable = false;
	}
}

technique QQSpeed_CrystalColorSkinning
<
	string Description = "This shader performs a crystal effect, with vertex color and skin.";
	int BonesPerPartition = MAX_BONES;
	bool UsesNiRenderState = false;
>
{
	pass P0
	{
		VertexShader = compile vs_2_0 CrystalVS_SkinColor();
		PixelShader = compile ps_2_0 CrystalPS_Color();
		AlphaBlendEnable = true;
		SrcBlend = One;
		DestBlend = One;
		CullMode = None;
		ZEnable = true;
		ZWriteEnable = false;
	}
}

technique QQSpeed_CrystalSmooth
<
	string Description = "This shader performs a crystal effect.";
	bool UsesNiRenderState = false;
>
{
	pass P0
	{
		VertexShader = compile vs_2_0 CrystalVS_Smooth();
		PixelShader = compile ps_2_0 CrystalPS();
		AlphaBlendEnable = true;
		SrcBlend = One;
		DestBlend = One;
		CullMode = None;
		ZEnable = true;
		ZWriteEnable = false;
	}
}

technique QQSpeed_CrystalSmoothColor
<
	string Description = "This shader performs a crystal effect, with color.";
	bool UsesNiRenderState = false;
>
{
	pass P0
	{
		VertexShader = compile vs_2_0 CrystalVS_SmoothColor();
		PixelShader = compile ps_2_0 CrystalPS_Color();
		AlphaBlendEnable = true;
		SrcBlend = One;
		DestBlend = One;
		CullMode = None;
		ZEnable = true;
		ZWriteEnable = false;
	}
}

technique QQSpeed_CrystalSmoothSkinning
<
	string Description = "This shader performs a crystal effect, with skin.";
	int BonesPerPartition = MAX_BONES;
	bool UsesNiRenderState = false;
>
{
	pass P0
	{
		VertexShader = compile vs_2_0 CrystalVS_SmoothSkin();
		PixelShader = compile ps_2_0 CrystalPS();
		AlphaBlendEnable = true;
		SrcBlend = One;
		DestBlend = One;
		CullMode = None;
		ZEnable = true;
		ZWriteEnable = false;
	}
}

technique QQSpeed_CrystalSmoothColorSkinning
<
	string Description = "This shader performs a crystal effect, with vertex color and skin.";
	int BonesPerPartition = MAX_BONES;
	bool UsesNiRenderState = false;
>
{
	pass P0
	{
		VertexShader = compile vs_2_0 CrystalVS_SmoothSkinColor();
		PixelShader = compile ps_2_0 CrystalPS_Color();
		AlphaBlendEnable = true;
		SrcBlend = One;
		DestBlend = One;
		CullMode = None;
		ZEnable = true;
		ZWriteEnable = false;
	}
}

