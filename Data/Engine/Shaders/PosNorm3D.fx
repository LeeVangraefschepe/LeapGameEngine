float4x4 gWorld : WORLD;
float4x4 gWorldViewProj : WORLDVIEWPROJECTION;
float4x4 gLightViewProj : LIGHTVIEWPROJECTION;
float3 gLightDirection = float3(-0.577f, -0.577f, 0.577f);
float4 gColor = float4(1.0f, 1.0f, 1.0f, 1.0f);

Texture2D gShadowMap;

struct VS_INPUT {
	float3 pos : POSITION;
	float3 normal : NORMAL;
};
struct VS_OUTPUT {
	float4 pos : SV_POSITION;
    float3 normal : NORMAL;
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

float gShadowMapBias = 0.0005f;

float2 texOffset(int u, int v)
{
    return float2(u / 1280.0f, v / 720.0f);
}

SamplerComparisonState cmpSampler
{
	// sampler state
    Filter = COMPARISON_MIN_MAG_MIP_LINEAR;
    AddressU = MIRROR;
    AddressV = MIRROR;

	// sampler comparison state
    ComparisonFunc = LESS_EQUAL;
};

float EvaluateShadowMap(float4 lpos)
{
	// Re-homogenize position after interpolation
    lpos.xyz /= lpos.w;

	// If position is not visible to the light - dont illuminate it
	// Results in hard light frustum
    if (lpos.x < -1.0f || lpos.x > 1.0f ||
		lpos.y < -1.0f || lpos.y > 1.0f ||
		lpos.z < 0.0f || lpos.z > 1.0f)
        return 1.0f;

	// Transform clip space coords to texture space coords (-1:1 to 0:1)
    lpos.x = lpos.x / 2.0f + 0.5f;
    lpos.y = lpos.y / -2.0f + 0.5f;

	// Apply shadow map bias
    lpos.z -= gShadowMapBias;
	
	// PCF sampling for shadow map
    float sum = 0;

	// Perform PCF filtering on a 4 x 4 texel neighborhood
    for (float y = -1.5; y <= 1.5; y += 1.0)
    {
        for (float x = -1.5; x <= 1.5; x += 1.0)
        {
            sum += gShadowMap.SampleCmpLevelZero(cmpSampler, lpos.xy + texOffset(x, y), lpos.z);
        }
    }

    float shadowMapDepth = sum / 16.0;
	
    return shadowMapDepth * 0.5f + 0.5f;
}

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
VS_OUTPUT VS(VS_INPUT input) {
	VS_OUTPUT output;
	// Step 1:	convert position into float4 and multiply with matWorldViewProj
	output.pos = mul(float4(input.pos, 1.0f), gWorldViewProj);
	// Step 2:	rotate the normal: NO TRANSLATION
	//			this is achieved by clipping the 4x4 to a 3x3 matrix, 
	//			thus removing the postion row of the matrix
    output.normal = normalize(mul(input.normal, (float3x3) gWorld));
    output.lPos = mul(float4(input.pos, 1.0f), mul(gWorld, gLightViewProj));

	return output;
}

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS(VS_OUTPUT input) : SV_TARGET
{
    float shadowValue = EvaluateShadowMap(input.lPos);
	
	float3 color_rgb = gColor.rgb;
	float color_a = gColor.a;

	//HalfLambert Diffuse :)
	float diffuseStrength = dot(input.normal, -gLightDirection);
	diffuseStrength = diffuseStrength * 0.5 + 0.5;
	diffuseStrength = saturate(diffuseStrength);
	color_rgb = color_rgb * diffuseStrength;

    return float4(color_rgb * shadowValue, color_a);
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

