float3 gLightDirection = float3(-0.577f, -0.577f, 0.577f);

Texture2D gTextureDiffuse;
Texture2D gTextureNormal;


SamplerState samPoint
{
    Filter = MIN_MAG_MIP_POINT;
    AddressU = Wrap;// or Mirror or Clamp or Border
    AddressV = Wrap;// or Mirror or Clamp or Border
};

RasterizerState Solid
{
	FillMode = SOLID;
	CullMode = BACK;
};

struct VS_INPUT{
	float3 pos : POSITION;
	float2 texCoord : TEXCOORD;
};

struct VS_OUTPUT{
	float4 pos : SV_POSITION;
	float2 texCoord : TEXCOORD;
};

DepthStencilState DisableDepth
{
	DepthEnable = FALSE;
	DepthWriteMask = ZERO;
};

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
VS_OUTPUT VS(VS_INPUT input){

	VS_OUTPUT output;

	output.pos = float4(input.pos,1.0f);
	output.texCoord = input.texCoord;

	return output;
}

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS(VS_OUTPUT input) : SV_TARGET{

	float4 normal = gTextureNormal.Sample(samPoint, input.texCoord);
	//clip(normal.w-0.1f);

	float3 diffuse = gTextureDiffuse.Sample(samPoint, input.texCoord).xyz;
	float diffuseStrength = dot(normal.xyz, -gLightDirection);
	diffuseStrength = diffuseStrength * 0.5 + 0.5;
	diffuseStrength = saturate(diffuseStrength);

	return float4(diffuse * diffuseStrength, 1.0f);
}

//--------------------------------------------------------------------------------------
// Technique
//--------------------------------------------------------------------------------------
technique11 Default
{
    pass P0
    {
		SetRasterizerState(Solid);
		SetDepthStencilState(DisableDepth, 0);

		SetVertexShader( CompileShader( vs_4_0, VS() ) );
		SetGeometryShader( NULL );
		SetPixelShader( CompileShader( ps_4_0, PS() ) );
    }
}

