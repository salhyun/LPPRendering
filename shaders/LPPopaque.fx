RENDERING_ATTRIBUTES
{
	RENDERING_ATTR_RIGID,
}
MATERIAL_ATTRIBUTES
{
	MATERIAL_ATTR_DIFFUSE,
}

float4x4 matWVP ;
float2 vOffset ;

texture LightMap ;
sampler LightMapSamp = sampler_state
{
	Texture = <LightMap> ;
	MinFilter = LINEAR ;
    MagFilter = LINEAR;
	MipFilter = NONE;//LINEAR ;
	
	AddressU = Wrap ;
	AddressV = Wrap ;
} ;

texture DiffuseMap ;
sampler DiffuseMapSamp = sampler_state
{
	Texture = <DiffuseMap> ;
	MinFilter = LINEAR ;
	MagFilter = LINEAR ;
	MipFilter = LINEAR ;
	
	AddressU = Wrap ;
	AddressV = Wrap ;
} ;
texture SSAOMap;
sampler SSAOMapSamp = sampler_state
{
    Texture = <SSAOMap>;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
    MipFilter = LINEAR;
	
    AddressU = Wrap;
    AddressV = Wrap;
};

struct VS_OUTPUT
{
    float4 Pos			: POSITION;
    float4 Color        : COLOR0 ;
    float2 Tex          : TEXCOORD0;
    float4 screenPos    : TEXCOORD1;
};

VS_OUTPUT VS(float4 Pos    : POSITION, float4 Normal : NORMAL, float2 Tex : TEXCOORD0)
{
	VS_OUTPUT Out = (VS_OUTPUT)0;        // 출력데이터

	Out.Pos = mul(Pos, matWVP);
	Out.Color = Out.Pos ;
	Out.Tex = Tex ;
	Out.screenPos = Out.Pos ;
	return Out;
}

float4 getLightColor(float4 screenPos)
{
    float2 pos = screenPos.xy / screenPos.ww;
    pos.x = (pos.x + 1.0f) * 0.5f;
    pos.y = (1.0f - pos.y) * 0.5f;
    return tex2D(LightMapSamp, pos+vOffset);
}
float4 getSSAOColor(float4 screenPos)
{
    float2 pos = screenPos.xy / screenPos.ww;
    pos.x = (pos.x + 1.0f) * 0.5f;
    pos.y = (1.0f - pos.y) * 0.5f;
    return tex2D(SSAOMapSamp, pos + vOffset);
}


float4 PS(VS_OUTPUT In) : COLOR
{
    float4 ssao = getSSAOColor(In.screenPos);
    float4 light = getLightColor(In.screenPos);

	float3 color = tex2D(DiffuseMapSamp, In.Tex).xyz ;

    color.xyz *= (light.xyz * ssao.xyz);
    //color.xyz *= light.xyz;
	color.xyz += light.www ;//Specular

	//color.xyz = (light.xyz+light.www) ;
	return float4(color.xyz, 1.0f) ;
}

technique TShader
{
    pass P0
    {
        VertexShader = compile vs_2_0 VS();
        PixelShader  = compile ps_2_0 PS();
    }
}