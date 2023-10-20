float4x4 gWorld;
float4x4 gLightViewProj;
 
DepthStencilState depthStencilState
{
    DepthEnable = TRUE;
    DepthWriteMask = ALL;
};

RasterizerState rasterizerState
{
    FillMode = SOLID;
    CullMode = NONE;
};

float4 ShadowMapVS(float3 position : POSITION) : SV_POSITION
{
    return mul(float4(position, 1.0f), mul(gWorld, gLightViewProj));
}

void ShadowMapPS_VOID(float4 position : SV_POSITION)
{
}

technique11 DefaultTechnique
{
    pass P0
    {
        SetRasterizerState(rasterizerState);
        SetDepthStencilState(depthStencilState, 0);
        SetVertexShader(CompileShader(vs_4_0, ShadowMapVS()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_4_0, ShadowMapPS_VOID()));
    }
}