float4x4 g_mViewProj:GLOBAL;
float4x4 g_mViewProjPrev:GLOBAL;

float3	 g_vPositionOffset:GLOBAL;
float3   g_vVelocity:GLOBAL;
float    g_fAlpha:GLOBAL;

float3	 g_vCameraPosition:GLOBAL;
float2   g_vSizeScale:GLOBAL;
float4	 g_vLightingColor:GLOBAL;

float3   g_vForward:GLOBAL;
float	 g_fParticleBox:GLOBAL;

 
float4x4 g_mTopVP:GLOBAL;
 

//bounding
float3 g_vCenter:GLOBAL;
float g_fRadius:GLOBAL;


texture BaseMap 
< 
    string NTM = "base";
>;

bool g_bIndoorMapValid:GLOBAL;
texture ShaderMap 
< 
    string NTM = "shader";
>;

 


// Samplers
sampler BaseSampler = sampler_state
{ 
    Texture = (BaseMap);
    AddressU = Clamp;
    AddressV = Clamp;
    MipFilter = POINT;
    MinFilter = POINT;
    MagFilter = POINT;
};

sampler ShaderSampler = sampler_state
{ 
    Texture = (ShaderMap);
    AddressU = Clamp;
    AddressV = Clamp;
    MipFilter = POINT;
    MinFilter = POINT;
    MagFilter = POINT;
};


struct VS_OUTPUT
{
    float4 position		: POSITION;		//Pixel position (world-view-proj space)
    float2 uv			: TEXCOORD0;
    float4 colour		: COLOR0;		//Diffuse texture
	float4 vIndoorMapPos : TEXCOORD1;
};

struct VS_INPUT
{
	float3 position		: POSITION;
	float2 uv			: TEXCOORD0;
};



VS_OUTPUT VSmain(VS_INPUT input)
{
	VS_OUTPUT output;
	
	output.uv = input.uv;
	
	float3 position = input.position;
		
	// subtract the offset then fmod into (0.0f, PARTICLE_BOX)
	position.xyz -= g_vPositionOffset.xyz;
	position.xyz = fmod(position.xyz, g_fParticleBox);
	
	// centre box on origin
	position.xyz -= (g_fParticleBox/2);
	// push along view vector so box is positioned more infront of camera
	position.xyz += g_vForward.xyz;
	
	// calculate colour based on distance from origin
	float subtractColour = length(position.xyz) / g_fParticleBox;
	float distanceColourScale = 1.0f - subtractColour * subtractColour;
	
	// get world position
	float4 worldPositionBottom = float4(position.xyz + g_vCameraPosition.xyz, 1.0f);
	float4 worldPositionTop = float4(worldPositionBottom.xyz + g_vVelocity.xyz * g_vSizeScale.y, 1.0f);
		
	// get projected positions of top and bottom of particle, and top of particle in previous frame
	float4 bottom = mul(worldPositionBottom, g_mViewProj);		
	float4 top = mul(worldPositionTop, g_mViewProj);
	float4 topPrev = mul(worldPositionTop, g_mViewProjPrev);
	
	// get 2d vector in screenspace between top and bottom of particle
	float2 dir = (top.xy/top.w) - (bottom.xy/bottom.w);
	float2 dirPrev = (topPrev.xy/topPrev.w) - (bottom.xy/bottom.w);
	
	// calculate lengths
	float len = length(dir);
	float lenPrev = length(dirPrev);
	
	// scale colour depending on length
	float lenColourScale = saturate(len/lenPrev);
	
	// get 2d vector perpendicular to velocity
	float2 dirPerp = normalize(float2(-dirPrev.y, dirPrev.x));

	// choose either the top or bottom projected position using uv.y
	output.position = lerp(topPrev, bottom, output.uv.y);
	
	// offset the position of each side of the particle using uv.x 
	output.position.xy += (0.5f - output.uv.x) * dirPerp * g_vSizeScale.x;
	
	// get the final colour including the lighting, distance and length scales, and per instance alpha
	output.colour = g_vLightingColor * distanceColourScale * lenColourScale * g_fAlpha.x;
	
	output.vIndoorMapPos=mul(worldPositionBottom,g_mTopVP);
	output.vIndoorMapPos/=output.vIndoorMapPos.w;
	output.vIndoorMapPos.x=0.5f+output.vIndoorMapPos.x*0.5f;
	output.vIndoorMapPos.y=0.5f-output.vIndoorMapPos.y*0.5f;
	 

	
	return output;
}









 

struct PS_OUTPUT
{
	float4 colour		: COLOR0;
};

PS_OUTPUT PSmain(VS_OUTPUT input)
{
	PS_OUTPUT output;
	
 
    output.colour = tex2D(BaseSampler, input.uv) * input.colour;
	
   if(g_bIndoorMapValid)
   {
 
	float fMapValue=tex2D(ShaderSampler,input.vIndoorMapPos.xy).r;
	
	float fMyDepth=input.vIndoorMapPos.z;
    if(fMapValue < fMyDepth )
      output.colour=0.0f; 
   
   }
	
	
	return output;
}








// techniques
technique WeatherShader
{
    pass P0
    {
        VertexShader = compile vs_2_0 VSmain();
        PixelShader = compile ps_2_0 PSmain();

        ZEnable = true;
        ZWriteEnable     = false;
        AlphaBlendEnable = true;
        AlphaTestEnable = false;
       
 
        SrcBlend = ONE;
        DestBlend = ONE;
        CullMode = NONE;
    }
}










