float4x4 gWorld : WORLD;
float4x4 gWorldViewProj : WORLDVIEWPROJECTION;
float4x4 gLightViewProj : LIGHTVIEWPROJECTION;
float3 gLightDirection = float3(-0.577f, -0.577f, 0.577f);
float4 gColor = float4(1.0f, 1.0f, 1.0f, 1.0f);

Texture2D gShadowMap;

struct VS_INPUT {
	float3 pos : POSITION;
};
struct VS_OUTPUT {
    float4 pos : SV_POSITION;
    float4 lPos : TEXCOORD1;
};

SamplerState samLinear
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = Wrap; // or Mirror or Clamp or Border
    AddressV = Wrap; // or Mirror or Clamp or Border
};

DepthStencilState EnableDepth
{
	DepthEnable = TRUE;
	DepthWriteMask = ALL;
};

RasterizerState BackCulling
{
    CullMode = BACK;
};

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
VS_OUTPUT VS(VS_INPUT input) {
	VS_OUTPUT output;
	// Step 1:	convert position into float4 and multiply with matWorldViewProj
    output.pos = mul(float4(input.pos, 1.0f), gWorldViewProj);
    output.lPos = mul(float4(input.pos, 1.0f), mul(gWorld, gLightViewProj));

	return output;
}

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS(VS_OUTPUT input) : SV_TARGET
{
    return gColor;
}

//--------------------------------------------------------------------------------------
// Technique
//--------------------------------------------------------------------------------------
technique11 DefaultTechnique
{
	pass P0
	{
        SetRasterizerState(BackCulling);
		SetDepthStencilState(EnableDepth, 0);

		SetVertexShader(CompileShader(vs_4_0, VS()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_4_0, PS()));
	}
}
