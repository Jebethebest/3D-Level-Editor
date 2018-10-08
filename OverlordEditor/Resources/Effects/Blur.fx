Texture2D gTexture;

SamplerState samLinear
{
    Filter = MIN_MAG_MIP_POINT;
    AddressU = Clamp;
    AddressV = Clamp;
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

struct VS_INPUT_STRUCT
{
    float3 Pos : POSITION;
	float2 TexCoord : TEXCOORD0;

};
struct PS_INPUT_STRUCT
{
    float4 Pos : SV_POSITION;
	float2 TexCoord : TEXCOORD1;
};

PS_INPUT_STRUCT VS(VS_INPUT_STRUCT input)
{
	PS_INPUT_STRUCT output = (PS_INPUT_STRUCT)0;
	output.Pos = float4(input.Pos,1);
	output.TexCoord = input.TexCoord;
	return output;
}

float4 PS(PS_INPUT_STRUCT input):SV_TARGET
{
	float3 color = float3(0,0,0);
	float colorOpacity = 1;
	
	float width,height;
	gTexture.GetDimensions(width,height);
	
	float dy = 1.0/height;
	float dx = 1.0/width;
	
	int passes = 5;
	int steps = 2;
	int halfPas = passes / 2;
	
	float3 finalColor = float3(0,0,0);
	
	for(int i = -halfPas; i<passes-halfPas; ++i)
	{
		for(int j = -halfPas; j<passes-halfPas; ++j)
		{
			float2 offset = float2(dx*(i*steps), dy*(j*steps));
			finalColor += gTexture.Sample(samLinear, input.TexCoord + offset).rgb;
		}
	}
	
	finalColor/=(passes*passes);

	return float4(finalColor, 1);
}

technique11 Blur
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