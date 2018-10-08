//Thomas Goussaert
//Graphics Programming 2
//Digital Arts & Entertainment
//www.digitalartsandentertainment.be

float4x4 gTransform : WorldViewProjection;
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

DepthStencilState DisableDepth
{
	DepthEnable = FALSE;
	DepthWriteMask = ZERO;
};

RasterizerState BackCulling 
{ 
	CullMode = BACK; 
};

//SHADER STRUCTS
//**************
struct VS_DATA
{
	uint TextureId: TEXCOORD0;
	float4 TransformData : POSITION; //PosX, PosY, Depth (PosZ), Rotation
	float4 TransformData2 : POSITION1; //PivotX, PivotY, ScaleX, ScaleY
	float4 Color: COLOR;	
};

struct GS_DATA
{
	float4 Position : SV_POSITION;
	float4 Color: COLOR;
	float2 TexCoord: TEXCOORD0;
};

//VERTEX SHADER
//*************
VS_DATA MainVS(VS_DATA input)
{
	return input;
}

//GEOMETRY SHADER
//***************
void CreateVertex(inout TriangleStream<GS_DATA> triStream, float3 pos, float4 col, float2 texCoord, float rotation, float2 rotCosSin, float2 offset, float2 pivotOffset)
{
	if (rotation != 0)
	{
		float2 originPos = pos.xy;
		originPos -= offset + pivotOffset;

		pos.x = (originPos.x * rotCosSin.x) - (originPos.y * rotCosSin.y);
		pos.y = (originPos.y * rotCosSin.x) + (originPos.x * rotCosSin.y);

		pos.x += offset.x;
		pos.y += offset.y;
	}
	else
	{
		pos.x -= pivotOffset.x;
		pos.y -= pivotOffset.y;
	}

	GS_DATA geomData = (GS_DATA) 0;
	geomData.Position = mul(float4(pos, 1.0f), gTransform);
	geomData.Color = col;
	geomData.TexCoord = texCoord;
	triStream.Append(geomData);
}

[maxvertexcount(4)]
void MainGS(point VS_DATA vertex[1], inout TriangleStream<GS_DATA> triStream)
{
	float3 position = vertex[0].TransformData.xyz;
	float2 offset = vertex[0].TransformData.xy;
	float rotation = vertex[0].TransformData.w;
	float2 pivot = vertex[0].TransformData2.xy;
	float2 scale = vertex[0].TransformData2.zw;
	float2 texCoord = float2(0, 0);

	float2 scaledTextureSize = gTextureSize * scale;
	float2 pivotOffset = pivot * scaledTextureSize;

	float2 rotCosSin = float2(0, 0);
	if (rotation != 0)
	{
		rotCosSin = float2(cos(rotation), sin(rotation));
	}

	// LT----------RT //TringleStrip (LT > RT > LB, LB > RB > RT)
	// |          / |
	// |       /    |
	// |    /       |
	// | /          |
	// LB----------RB

	//VERTEX 1 [LT]
	CreateVertex(triStream, position, vertex[0].Color, texCoord, rotation, rotCosSin, offset, pivotOffset);

	//VERTEX 2 [RT]
	position.x += scaledTextureSize.x;
	texCoord.x = 1;
	CreateVertex(triStream, position, vertex[0].Color, texCoord, rotation, rotCosSin, offset, pivotOffset);

	//VERTEX 3 [LB]
	position.x -= scaledTextureSize.x;
	position.y += scaledTextureSize.y;
	texCoord.x = 0;
	texCoord.y = 1;
	CreateVertex(triStream, position, vertex[0].Color, texCoord, rotation, rotCosSin, offset, pivotOffset);

	//VERTEX 4 [RB]
	position.x += scaledTextureSize.x;
	texCoord.x = 1;
	CreateVertex(triStream, position, vertex[0].Color, texCoord, rotation, rotCosSin, offset, pivotOffset);
}

//PIXEL SHADER
//************
float4 MainPS(GS_DATA input) : SV_TARGET {

	//return input.Color;
	return gSpriteTexture.Sample(samPoint, input.TexCoord) * input.Color;	
}

// Default Technique
technique11 Default {

	pass p0 {
		SetRasterizerState(BackCulling);
		SetBlendState(EnableBlending,float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
		SetDepthStencilState(DisableDepth, 0);
		SetVertexShader(CompileShader(vs_4_0, MainVS()));
		SetGeometryShader(CompileShader(gs_4_0, MainGS()));
		SetPixelShader(CompileShader(ps_4_0, MainPS()));
	}
}
