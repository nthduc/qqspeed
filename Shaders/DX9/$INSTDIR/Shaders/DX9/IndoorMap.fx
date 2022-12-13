float4x4 g_WVP:WORLDVIEWPROJECTION;


struct VS_Output
{
	float4 position:POSITION;
	float4 color:COLOR0;
};

VS_Output VSmain(float3 position:POSITION)
{
	VS_Output output;
	output.position=mul(float4(position,1.0f),g_WVP);
	output.color=float4(output.position.z/output.position.w,0.0f,0.0f,1.0f);
	return output;
}


float4 PSmain(VS_Output input):COLOR0
{
    //return float4(1,1,1,1);
	 //return float4(1.0f,0.0f,0.0f,1.0f); 
	 return input.color;
}
 

technique IndoorMap
{
	pass P0
    {
        VertexShader = compile vs_2_0 VSmain();
        PixelShader = compile ps_2_0 PSmain();

        ZEnable = true;
        ZWriteEnable = true;
        AlphaBlendEnable = false;
        AlphaTestEnable = false;
        CullMode=NONE;
    }
}; 