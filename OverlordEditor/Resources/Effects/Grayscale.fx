//=============================================================================
//// Shader uses position and texture
//=============================================================================
SamplerState samLinear
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = Mirror;
    AddressV = Mirror;
};

DepthStencilState DepthEnabled
{
	DepthEnable = TRUE;
	DepthWriteMask = 1;
};

RasterizerState BackCulling 
{ 
	CullMode = BACK; 
};

Texture2D gTexture;

struct VS_INPUT_STRUCT
{
	float3 Pos : POSITION;
    float2 Tex : TEXCOORD;

};
struct PS_INPUT_STRUCT
{
	float4 Pos : SV_POSITION;
	float2 Tex : TEXCOORD1;

};
//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
PS_INPUT_STRUCT VS( VS_INPUT_STRUCT input )
{
	PS_INPUT_STRUCT output = (PS_INPUT_STRUCT)0;
	output.Pos = float4(input.Pos,1);
	output.Tex = input.Tex;

	return output;
}
//--------------------------------------------------------------------------------------
// Pixel XMeshShader
//--------------------------------------------------------------------------------------
float4 PS( PS_INPUT_STRUCT input): SV_Target
{
	float4 rgb = gTexture.Sample( samLinear, input.Tex ) ;
	float mean = ( rgb.r + rgb.g + rgb.b ) / 3.0f;
    return float4( mean, mean, mean, 1.0f );
}

technique10 Grayscale
{
    pass P0
    {          
        SetRasterizerState(BackCulling);
		SetDepthStencilState(DepthEnabled, 0);
		SetVertexShader( CompileShader( vs_4_0, VS() ) );
        SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_4_0, PS() ) );
    }
}

