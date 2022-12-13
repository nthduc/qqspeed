// -------------------------------------------------------------
// Ahsikhmin Reflection Model + Faked HDR Rendering
// 
// Copyright (c) 2003 - 2004 Wolfgang F. Engel (wolf@gameversity.com)
// All rights reserved.
// -------------------------------------------------------------

// -------------------------------------------------------------
// variables that are provided by the application
// -------------------------------------------------------------
float4x4 matWorldViewProj : WORLDVIEWPROJ;
float4x4 matWorld;			
float	fIteration : GLOBAL;
float4  pixelSize : GLOBAL;
float    ExposureLevel : GLOBAL = 1.4;
float	GlowIntense : GLOBAL = 0.25;
float2    MotionCenter : GLOBAL = float2(0.5,0.5);
float	MotionDegree : GLOBAL = 0.85;

texture RenderMap;
sampler RenderMapSampler = sampler_state
{
   Texture = <RenderMap>;
   MinFilter = LINEAR;
   MagFilter = LINEAR;
   MipFilter = LINEAR;   
   AddressU  = Clamp;
   AddressV  = Clamp;
};

texture FullResMap <int NTMIndex = 1;>;
sampler FullResMapSampler = sampler_state
{
   Texture = <FullResMap>;
   MinFilter = LINEAR;
   MagFilter = LINEAR;
   MipFilter = LINEAR;   
   AddressU  = Clamp;
   AddressV  = Clamp;
};

// -------------------------------------------------------------
// Output channels
// -------------------------------------------------------------
struct VS_OUTPUTScaleBuffer
{
    float4 Pos		: POSITION;
	float2 Tex	    : TEXCOORD0;	
};

// -------------------------------------------------------------
// vertex shader function (input channels)
// -------------------------------------------------------------
VS_OUTPUTScaleBuffer VSScaleBuffer(float4 Pos : POSITION, float2 Tex: TEXCOORD0)
{
    VS_OUTPUTScaleBuffer Out = (VS_OUTPUTScaleBuffer)0;        
 	Out.Pos = mul( Pos, matWorldViewProj);
  	Out.Tex = Tex;
    return Out;
}

// -------------------------------------------------------------
// Pixel Shader (input channels):output channel
// -------------------------------------------------------------
float4 PSScaleBuffer(float2 Tex	: TEXCOORD0) : COLOR			
				
{
    float4 RGBA = tex2D(FullResMapSampler, Tex);
    float Luminance = dot(RGBA.rgb, float3(0.299f, 0.587f, 0.114f));
    //RGBA*= Luminance;
    //RGBA.a = 1.0f;
    return RGBA;
}

// -------------------------------------------------------------
// Output channels
// -------------------------------------------------------------
struct VS_OUTPUTBloom
{
    float4 Pos			: POSITION;
	float2 TopLeft          : TEXCOORD0;
	float2 TopRight          : TEXCOORD1;
	float2 BottomRight          : TEXCOORD2;
	float2 BottomLeft          : TEXCOORD3;
	
};



// -------------------------------------------------------------
// vertex shader function (input channels)
// -------------------------------------------------------------
VS_OUTPUTBloom VSBloom(float4 Pos    : POSITION, float2 Tex : TEXCOORD0)
{
    VS_OUTPUTBloom Out = (VS_OUTPUTBloom)0;        

	Out.Pos = mul(Pos, matWorldViewProj);
	
	float2 halfPixelSize = pixelSize.xy / 2.0f;
	//Out.Pos.xy -= halfPixelSize.xy;
	//Out.Pos.xy += pixelSize.xy;
	float2 dUV = (pixelSize.xy * fIteration) + halfPixelSize.xy;
	
	// sample top left
	Out.TopLeft = float2(Tex.x - dUV.x, Tex.y + dUV.y); 
	
	// sample top right
	Out.TopRight = float2(Tex.x + dUV.x, Tex.y + dUV.y);
	
	// sample bottom right
	Out.BottomRight = float2(Tex.x + dUV.x, Tex.y - dUV.y);
	
	// sample bottom left
	Out.BottomLeft = float2(Tex.x - dUV.x, Tex.y - dUV.y);
		
    return Out;
}

// -------------------------------------------------------------
// Pixel Shader (input channels):output channel
// -------------------------------------------------------------
float4 PSBloom(float2 TopLeft          : TEXCOORD0,
				float2 TopRight          : TEXCOORD1,
				float2 BottomRight          : TEXCOORD2,
				float2 BottomLeft          : TEXCOORD3) : COLOR0
{
	float4 addedBuffer = 0.0f;
	
	// sample top left
	addedBuffer = tex2D(RenderMapSampler, TopLeft) ;
	
	// sample top right
	addedBuffer += tex2D(RenderMapSampler, TopRight);
	
	// sample bottom right
	addedBuffer += tex2D(RenderMapSampler, BottomRight);
	
	// sample bottom left
	addedBuffer += tex2D(RenderMapSampler, BottomLeft);
	
	// average
	addedBuffer *= 0.25f;
	return addedBuffer;
}

// -------------------------------------------------------------
// Output channels
// -------------------------------------------------------------
struct VS_OUTPUTScreen
{
    float4 Pos			: POSITION;
	float2 Tex          : TEXCOORD0;
};

// -------------------------------------------------------------
// vertex shader function (input channels)
// -------------------------------------------------------------
VS_OUTPUTScreen VSScreen(float4 Pos : POSITION, float2 Tex : TEXCOORD0)
{
    VS_OUTPUTScreen Out = (VS_OUTPUTScreen)0;        
 	Out.Pos = mul( Pos, matWorldViewProj);
  	Out.Tex = Tex;
    return Out;
}

// -------------------------------------------------------------
// Pixel Shader (input channels):output channel
// -------------------------------------------------------------
float4 PSScreen(float2 Tex : TEXCOORD0) : COLOR0
{
	float4 FullScreenImage = tex2D(FullResMapSampler, Tex);
	float4 BlurredImage = tex2D(RenderMapSampler, Tex);
	
	float4 color = lerp(FullScreenImage, BlurredImage, GlowIntense);
	return color * ExposureLevel;
}

// -------------------------------------------------------------
// Output channels
// -------------------------------------------------------------
struct VS_OUTPUTMotion
{
    float4 Pos			: POSITION;
	float2 TopLeft          : TEXCOORD0;
	float2 TopRight          : TEXCOORD1;
	float2 BottomRight          : TEXCOORD2;
	float2 BottomLeft          : TEXCOORD3;
	float2 TopLeft1          : TEXCOORD4;
	float2 TopRight1         : TEXCOORD5;
	float2 BottomRight1          : TEXCOORD6;
	float2 BottomLeft1         : TEXCOORD7;
};



// -------------------------------------------------------------
// vertex shader function (input channels)
// -------------------------------------------------------------
VS_OUTPUTMotion VSMotion(float4 Pos    : POSITION, float2 Tex : TEXCOORD0)
{
    VS_OUTPUTMotion Out = (VS_OUTPUTMotion)0;        

	Out.Pos = mul(Pos, matWorldViewProj);
	
	//float2 halfPixelSize = pixelSize.xy / 2.0f;
	//Out.Pos.xy -= halfPixelSize.xy;
	//Out.Pos.xy += pixelSize.xy;
	//float2 dUV = (pixelSize.xy * fIteration) + halfPixelSize.xy;
	//float2 dUV = MotionCenter - Tex;
	float iter = (1.0 - MotionDegree)/8.0f;
	float cur = MotionDegree ;
	// sample top left
	Out.TopLeft = MotionCenter*(1-  cur)+Tex* cur; 
	cur += iter;
	// sample top right
	Out.TopRight = MotionCenter*(1-  cur)+Tex* cur; 
	cur += iter;
	// sample bottom right
	Out.BottomRight = MotionCenter*(1-  cur)+Tex* cur; 
	cur += iter;
	// sample bottom left
	Out.BottomLeft = MotionCenter*(1-  cur)+Tex* cur; 
        
        cur += iter;
	// sample top left
	Out.TopLeft1 = MotionCenter*(1-  cur)+Tex* cur; 
	cur += iter;
	// sample top right
	Out.TopRight1 = MotionCenter*(1-  cur)+Tex* cur; 
	cur += iter;
	// sample bottom right
	Out.BottomRight1 = MotionCenter*(1-  cur)+Tex* cur; 
	cur += iter;
	// sample bottom left
	Out.BottomLeft1 = MotionCenter*(1-  cur)+Tex* cur; 
    return Out;
}

// -------------------------------------------------------------
// Pixel Shader (input channels):output channel
// -------------------------------------------------------------
float4 PSMotion(float2 TopLeft          : TEXCOORD0,
				float2 TopRight          : TEXCOORD1,
				float2 BottomRight          : TEXCOORD2,
				float2 BottomLeft          : TEXCOORD3,
				float2 TopLeft1          : TEXCOORD4,
				float2 TopRight1          : TEXCOORD5,
				float2 BottomRight1          : TEXCOORD6,
				float2 BottomLeft1          : TEXCOORD7) : COLOR0
{
	float4 addedBuffer = 0.0f;
	
	// sample top left
	addedBuffer = tex2D(RenderMapSampler, TopLeft) ;
	
	// sample top right
	addedBuffer += tex2D(RenderMapSampler, TopRight);
	
	// sample bottom right
	addedBuffer += tex2D(RenderMapSampler, BottomRight);
	
	// sample bottom left
	addedBuffer += tex2D(RenderMapSampler, BottomLeft);
	// sample top left
	addedBuffer += tex2D(RenderMapSampler, TopLeft1) ;
	
	// sample top right
	addedBuffer += tex2D(RenderMapSampler, TopRight1);
	
	// sample bottom right
	addedBuffer += tex2D(RenderMapSampler, BottomRight1);
	
	// sample bottom left
	addedBuffer += tex2D(RenderMapSampler, BottomLeft1);
	// average
	addedBuffer *= 0.125f;
	return addedBuffer;
}
//////////////////////////////////////////////////////////////////////////
technique ScaleBuffer
{
    pass P0
    {
        VertexShader = compile vs_1_1 VSScaleBuffer();
        PixelShader  = compile ps_1_1 PSScaleBuffer();
        Lighting = False;
        //enable alpha blending to get the motion blur effect
        AlphaBlendEnable = 	FALSE;
        AlphaTestEnable = False;
        SrcBlend = SRCALPHA;
        DestBlend = INVSRCALPHA;
        ZEnable = False;
	FogEnable = False;
    }
}

technique Bloom
{
    pass P0
    {
        VertexShader = compile vs_1_1 VSBloom();
        PixelShader  = compile ps_1_1 PSBloom();
        Lighting = False;
        AlphaBlendEnable = False;
        AlphaTestEnable = False;
        ZEnable = False;
	FogEnable = False;
    }
}

technique Motion
{
    pass P0
    {
        VertexShader = compile vs_1_1 VSMotion();
        PixelShader  = compile ps_2_0 PSMotion();
        Lighting = False;
        AlphaBlendEnable = False;
        AlphaTestEnable = False;
        ZEnable = False;
	FogEnable = False;
    }
}

technique Screenblit
{
    pass P0
    {
        VertexShader = compile vs_1_1 VSScreen();
        PixelShader  = compile ps_2_0 PSScreen();
        Lighting = False;
        AlphaBlendEnable = False;
        ZEnable = False;
	FogEnable = False;
    }
}

