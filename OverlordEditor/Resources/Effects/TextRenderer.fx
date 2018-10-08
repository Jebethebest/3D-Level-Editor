float4x4 gTransform : WORLDVIEWPROJECTION;
Texture2D gSpriteTexture;
float2 gTextureSize;

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
	DepthEnable = FALSE;
	DepthWriteMask = ZERO;
};

RasterizerState BackCulling 
{ 
	//FillMode = WIREFRAME;
	CullMode = BACK; 
};

//SHADER STRUCTS
//**************
struct VS_DATA
{
	int Channel : TEXCOORD2;
	float3 Position : POSITION;
	float4 Color: COLOR;
	float2 TexCoord: TEXCOORD0;
	float2 CharSize: TEXCOORD1;
};

struct GS_DATA
{
	float4 Position : SV_POSITION;
	float4 Color: COLOR;
	float2 TexCoord: TEXCOORD0;
	int Channel: TEXCOORD1;
};

//VERTEX SHADER
//*************
VS_DATA MainVS(VS_DATA input)
{
	return input;
}

//GEOMETRY SHADER
//***************
void CreateVertex(inout TriangleStream<GS_DATA> triStream, float3 pos, float4 col, float2 texCoord, int channel)
{
	GS_DATA geomData = (GS_DATA)0;
	geomData.Position = mul(float4(pos, 1.0f), gTransform);
	geomData.Color = col;
	geomData.TexCoord = texCoord;
	geomData.Channel = channel;
	triStream.Append(geomData);
}

[maxvertexcount(4)]
void MainGS(point VS_DATA vertex[1], inout TriangleStream<GS_DATA> triStream)
{
	CreateVertex(triStream, vertex[0].Position, vertex[0].Color, vertex[0].TexCoord, vertex[0].Channel);

	float2 texOffset = vertex[0].CharSize / gTextureSize;

	//VERTEX 2 [RT]
	vertex[0].Position.x += vertex[0].CharSize.x;
	vertex[0].TexCoord.x += texOffset.x;
	CreateVertex(triStream, vertex[0].Position, vertex[0].Color, vertex[0].TexCoord, vertex[0].Channel);

	//VERTEX 3 [LB]
	vertex[0].Position.x -= vertex[0].CharSize.x;
	vertex[0].TexCoord.x -= texOffset.x;
	vertex[0].Position.y += vertex[0].CharSize.y;
	vertex[0].TexCoord.y += texOffset.y;
	CreateVertex(triStream, vertex[0].Position, vertex[0].Color, vertex[0].TexCoord, vertex[0].Channel);

	//VERTEX 4 [RB]
	vertex[0].Position.x += vertex[0].CharSize.x;
	vertex[0].TexCoord.x += texOffset.x;
	CreateVertex(triStream, vertex[0].Position, vertex[0].Color, vertex[0].TexCoord, vertex[0].Channel);
}

//PIXEL SHADER
//************
float4 MainPS(GS_DATA input) : SV_TARGET {
	//return input.Color;
	return gSpriteTexture.Sample(samPoint, input.TexCoord)[input.Channel] * input.Color;
}

// Default Technique
technique10 Default {

	pass p0 {
		SetRasterizerState(BackCulling);
		SetBlendState(EnableBlending,float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
		//SetDepthStencilState(NoDepth,0);
		SetVertexShader(CompileShader(vs_4_0, MainVS()));
		SetGeometryShader(CompileShader(gs_4_0, MainGS()));
		SetPixelShader(CompileShader(ps_4_0, MainPS()));
	}
}
