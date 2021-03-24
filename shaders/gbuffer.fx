RENDERING_ATTRIBUTES
{
}
MATERIAL_ATTRIBUTES
{
}

texture NormalMap;
sampler NormalMapSamp = sampler_state
{
	Texture = <NormalMap>;
	MinFilter = LINEAR;
    MagFilter = LINEAR;
    MipFilter = NONE;//LINEAR;
    
    AddressU = Wrap;
    AddressV = Wrap;
};

float4x4 matWVP ;
float4x4 matW ;
float fShine=0.07f ;
float fShineStrength=0.8f ;


struct VS_OUTPUT
{
    float4 Pos			: POSITION;
    float3 Normal       : TEXCOORD0;
    float2 Depth        : TEXCOORD1;
};
struct PS_OUTPUT
{
	float4 Color0 : COLOR0;
	float4 Color1 : COLOR1;
	float4 Color2 : COLOR2;
};

VS_OUTPUT VS(float4 Pos : POSITION, float3 Normal : NORMAL)
{
	VS_OUTPUT Out = (VS_OUTPUT)0 ;
	
	Out.Pos = mul(Pos, matWVP) ;
	Out.Normal.xyz = mul(Normal, (float3x3)matW) ;
	Out.Depth = float2(Out.Pos.z, Out.Pos.w) ;
	return Out ;
}
PS_OUTPUT PS(VS_OUTPUT In)
{
	PS_OUTPUT Out = (PS_OUTPUT)0 ;
	
	float3 N = normalize(In.Normal.xyz) ;
	
	N.x = (N.x + 1.0f)*0.5f ;
	N.y = (N.y + 1.0f)*0.5f ;
	N.z = (N.z + 1.0f)*0.5f ;
	
	Out.Color0 = float4(N.xyz, 1.0f) ;
	Out.Color1 = In.Depth.x/In.Depth.y ;
	Out.Color2 = float4(fShine, fShineStrength, 0.0f, 0.0f) ;
	return Out ;
}

struct VS_OUTPUTBUMP
{
    float4 Pos			: POSITION;
    float3 Normal       : TEXCOORD0;
    float3 Tangent      : TEXCOORD1;
    float2 Depth        : TEXCOORD2;
    float2 Tex          : TEXCOORD3;
};

VS_OUTPUTBUMP VS_Bump(float4 Pos : POSITION, float3 Normal : NORMAL, float3 Tangent : TANGENT, float2 Tex : TEXCOORD0)
{
	VS_OUTPUTBUMP Out = (VS_OUTPUTBUMP)0 ;
	
	Out.Pos = mul(Pos, matWVP) ;
	Out.Normal.xyz = mul(Normal, (float3x3)matW) ;
	Out.Tangent.xyz = mul(Tangent, (float3x3)matW) ;
	Out.Depth = float2(Out.Pos.z, Out.Pos.w) ;
	Out.Tex = Tex ;
	return Out ;
}
PS_OUTPUT PS_Bump(VS_OUTPUTBUMP In)
{
	PS_OUTPUT Out = (PS_OUTPUT)0 ;
	
	float3 n = normalize(In.Normal.xyz) ;
	float3 t = normalize(In.Tangent.xyz) ;
	float3 b = cross(n, t) ;
	
	float3 N = 2.0f*tex2D(NormalMapSamp, In.Tex).xyz-1.0f ;
	//inverse tangent space(is local not world space)
	float3x3 matInvTangent = { t.x, t.y, t.z,
							   b.x, b.y, b.z,
							   n.x, n.y, n.z } ;

    N = mul(N, matInvTangent);
	
	N.x = (N.x + 1.0f)*0.5f ;
	N.y = (N.y + 1.0f)*0.5f ;
	N.z = (N.z + 1.0f)*0.5f ;
	
	Out.Color0 = float4(N.xyz, 1.0f) ;
	Out.Color1 = In.Depth.x/In.Depth.y ;
	Out.Color2 = float4(fShine, fShineStrength, 0.0f, 0.0f) ;
	return Out ;
}

technique TShader
{
    pass P0
    {
        VertexShader = compile vs_2_0 VS();
        PixelShader  = compile ps_2_0 PS();
    }
    pass P1
    {
        VertexShader = compile vs_2_0 VS_Bump();
        PixelShader  = compile ps_2_0 PS_Bump();
    }
}