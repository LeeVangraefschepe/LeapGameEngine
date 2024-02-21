float4x4 gWorldViewProj : WorldViewProjection;
float4x4 gViewInverse : ViewInverse;
Texture2D gTexture;

SamplerState samPoint
{
    Filter = MIN_MAG_MIP_POINT;
    AddressU = WRAP;
    AddressV = WRAP;
};

BlendState AlphaBlending 
{     
	BlendEnable[0] = TRUE;
	SrcBlend = SRC_ALPHA;
    DestBlend = INV_SRC_ALPHA;
	BlendOp = ADD;
	SrcBlendAlpha = ONE;
	DestBlendAlpha = ZERO;
	BlendOpAlpha = ADD;
	RenderTargetWriteMask[0] = 0x0f;
};

DepthStencilState DisableDepthWriting
{
	DepthEnable = TRUE;
	DepthWriteMask = ZERO;
};

RasterizerState BackCulling
{
	CullMode = BACK;
};

struct VS_DATA
{
	float3 Position : POSITION;
    float4 Color : COLOR;
    float Size : SIZE;
    float Rotation : ROTATION;
};

struct GS_DATA
{
	float4 Position : SV_POSITION;
	float2 TexCoord: TEXCOORD0;
	float4 Color : COLOR;
};

VS_DATA MainVS(VS_DATA input)
{
	return input;
}

void CreateVertex(inout TriangleStream<GS_DATA> triStream, float3 pos, float2 texCoord, float4 col)
{
	GS_DATA vertex = (GS_DATA)0;
	
	vertex.Position = mul(float4(pos,1.0f),gWorldViewProj);
	vertex.TexCoord = texCoord;
	vertex.Color = col;
	
	triStream.Append(vertex);
}

[maxvertexcount(4)]
void MainGS(point VS_DATA vertex[1], inout TriangleStream<GS_DATA> triStream)
{
	float3 topLeft, topRight, bottomLeft, bottomRight;
	float size = vertex[0].Size;
	float3 origin = vertex[0].Position;
	
    topLeft = float3(-size / 2.0f, size / 2.0f, 0.0f);
	topRight = float3(size / 2.0f, size / 2.0f, 0.0f);
	bottomLeft = float3(-size / 2.0f, -size / 2.0f, 0.0f);
	bottomRight = float3(size / 2.0f, -size / 2.0f, 0.0f);
	
    float2x2 uvRotation = { cos(vertex[0].Rotation), -sin(vertex[0].Rotation), sin(vertex[0].Rotation), cos(vertex[0].Rotation) };
	topLeft = float3(mul(topLeft.xy, uvRotation), 0.0f);
	topRight = float3(mul(topRight.xy, uvRotation), 0.0f);
	bottomLeft = float3(mul(bottomLeft.xy, uvRotation), 0.0f);
	bottomRight = float3(mul(bottomRight.xy, uvRotation), 0.0f);
	
	topLeft = origin + mul(float4(topLeft, 0.0f), gViewInverse).xyz;
	topRight = origin + mul(float4(topRight, 0.0f), gViewInverse).xyz;
	bottomLeft = origin + mul(float4(bottomLeft, 0.0f), gViewInverse).xyz;
	bottomRight = origin + mul(float4(bottomRight, 0.0f), gViewInverse).xyz;
	
    float maxUv = 1.0f;
	
    CreateVertex(triStream, bottomLeft, float2(0.0f, maxUv), vertex[0].Color);
    CreateVertex(triStream, topLeft, float2(0.0f, 0.0f), vertex[0].Color);
    CreateVertex(triStream, bottomRight, float2(maxUv, maxUv), vertex[0].Color);
    CreateVertex(triStream, topRight, float2(maxUv, 0.0f), vertex[0].Color);
}

float4 MainPS(GS_DATA input) : SV_TARGET 
{
	float4 result = gTexture.Sample(samPoint,input.TexCoord);
	return input.Color * result;
}

// Default Technique
technique10 DefaultTechnique 
{
	pass p0 {
		SetVertexShader(CompileShader(vs_4_0, MainVS()));
		SetGeometryShader(CompileShader(gs_4_0, MainGS()));
		SetPixelShader(CompileShader(ps_4_0, MainPS()));
		
		SetRasterizerState(BackCulling);       
		SetDepthStencilState(DisableDepthWriting, 0);
        SetBlendState(AlphaBlending, float4( 0.0f, 0.0f, 0.0f, 0.0f ), 0xFFFFFFFF);
	}
}
