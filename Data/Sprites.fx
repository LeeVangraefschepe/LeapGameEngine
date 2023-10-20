Texture2D gTexture;
float4x4 gTransform;
float2 gScreenSize;

SamplerState samPoint
{
    Filter = MIN_MAG_MIP_POINT;
    AddressU = WRAP;
    AddressV = WRAP;
};

BlendState EnableBlending
{
    BlendEnable[0] = TRUE;
    SrcBlend = SRC_ALPHA;
    DestBlend = INV_SRC_ALPHA;
};

DepthStencilState NoDepth
{
    DepthEnable = false;
    DepthWriteMask = zero;
    DepthFunc = less;
    StencilEnable = false;
};

RasterizerState BackCulling
{
    CullMode = BACK;
};

//SHADER STRUCTS
//**************
struct VS_DATA
{
    float3 Position : POSITION;
    float2 Pivot : PIVOT;
    float2 Size : SIZE;
    float4 Color : COLOR;
};

struct GS_DATA
{
    float4 Position : SV_POSITION;
    float4 Color : COLOR;
    float2 TexCoord : TEXCOORD0;
};

//VERTEX SHADER
//*************
VS_DATA MainVS(VS_DATA input)
{
    return input;
}

//GEOMETRY SHADER
//***************
void CreateVertex(inout TriangleStream<GS_DATA> triStream, float3 pos, float4 col, float2 texCoord, float rotation, float2 rotCosSin, float2 size, float2 pivotOffset)
{
    //Geometry Vertex Output
    GS_DATA geomData = (GS_DATA) 0;

    // Move the vertex to origin and apply the pivot offset
    float3 originPos = pos - float3(pivotOffset.x * size.x, pivotOffset.y * size.y, 0.0f);
    // Rotate the vertex
    float NewPosX = originPos.x * rotCosSin.x - originPos.y * rotCosSin.y;
    float NewPosY = originPos.y * rotCosSin.x + originPos.x * rotCosSin.y;
    // Move the vertex back to the original position
    geomData.Position = mul(float4(NewPosX, NewPosY, pos.z, 1.0f), gTransform);

    // Apply the vertex color and UV
    geomData.Color = col;
    geomData.TexCoord = texCoord;

    // Add the vertex to the outputstream
    triStream.Append(geomData);
}

[maxvertexcount(4)]
void MainGS(point VS_DATA vertex[1], inout TriangleStream<GS_DATA> triStream)
{	
	// LT----------RT //TringleStrip (LT > RT > LB, LB > RB > RT)
	// |          / |
	// |       /    |
	// |    /       |
	// | /          |
	// LB----------RB
        
    float3 position = vertex[0].Position;
    float2 size = vertex[0].Size;
    float4 color = vertex[0].Color;
    float rotation = 0;
    float2 rotCosSin = float2(1, 0);
    float2 pivotOffset = vertex[0].Pivot;
    float2 texCoord = float2(0, 0);
    
	//VERTEX 1 [LT]
    CreateVertex(triStream, position + float3(0.0f, size.y, 0.0f), color, texCoord, rotation, rotCosSin, size, pivotOffset);

	//VERTEX 2 [RT]
    CreateVertex(triStream, position + float3(size.x, size.y, 0.0f), color, texCoord + float2(1.0f, 0.0f), rotation, rotCosSin, size, pivotOffset);

	//VERTEX 3 [LB]
    CreateVertex(triStream, position, color, texCoord + float2(0.0f, 1.0f), rotation, rotCosSin, size, pivotOffset);

	//VERTEX 4 [RB]
    CreateVertex(triStream, position + float3(size.x, 0.0f, 0.0f), color, texCoord + float2(1.0f, 1.0f), rotation, rotCosSin, size, pivotOffset);
}

//PIXEL SHADER
//************
float4 MainPS(GS_DATA input) : SV_TARGET
{
    return gTexture.Sample(samPoint, input.TexCoord) * input.Color;
}

// Default Technique
technique11 DefaultTechnique
{
    pass p0
    {
        SetRasterizerState(BackCulling);
        SetBlendState(EnableBlending, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
		SetDepthStencilState(NoDepth,0);
        SetVertexShader(CompileShader(vs_4_0, MainVS()));
        SetGeometryShader(CompileShader(gs_4_0, MainGS()));
        SetPixelShader(CompileShader(ps_4_0, MainPS()));
    }
}
