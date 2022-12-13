//-----------------------------------------------------------------------------
// File: PP_ColorGBlurH.fx
//
// Desc: Effect file for image post-processing sample.  This effect contains
//       a single technique with a pixel shader that blurs an image
//       horizontally using weights that follow a Gaussian distribution.
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//-----------------------------------------------------------------------------

float fScreenWidth:GLOBAL;
float fScreenHeight:GLOBAL;


texture g_txSrcColor
< 
    string NTM = "shader";
    int NTMIndex = 0;

>;
 
 
sampler2D g_samSrcColor =
sampler_state
{
    Texture = <g_txSrcColor>;
    AddressU = Clamp;
    AddressV = Clamp;
    MinFilter = Point;
    MagFilter = Linear;
    MipFilter = Linear;
};
 



static const int g_cKernelSize = 5;

static float2 PixelKernel[g_cKernelSize]:GLOBAL =
{
    { -2, 0 },
    { -1, 0 },
    {  0, 0 },
    {  1, 0 },
    {  2, 0 },
};

  


//-----------------------------------------------------------------------------
// Pixel Shader: HorizontalBlur
// Desc: Blurs the image horizontally
//-----------------------------------------------------------------------------
float4 PostProcessPS( float2 Tex : TEXCOORD0 ) : COLOR0
{
    float fMinDepth=1.0f;
    for (int i = 0; i < g_cKernelSize; i++)
    {    
		float fMapDepth=tex2D( g_samSrcColor, Tex + PixelKernel[i].xy/fScreenWidth ).r;
		fMinDepth=min(fMinDepth,fMapDepth);
    }
    return fMinDepth;
}




//-----------------------------------------------------------------------------
// Technique: PostProcess
// Desc: Performs post-processing effect that converts a colored image to
//       black and white.
//-----------------------------------------------------------------------------
technique GaussianH
{
    pass p0
    {
        VertexShader = null;
        PixelShader = compile ps_2_0 PostProcessPS();
        ZEnable = false;
    }
}
