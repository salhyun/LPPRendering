RENDERING_ATTRIBUTES
{
	RENDERING_ATTR_RIGID,
}
MATERIAL_ATTRIBUTES
{
	MATERIAL_ATTR_DIFFUSE,
}

float4x4 matWVP ;
float4x4 matWLVP ;
float4x4 matWLVPT ;
float4 vNTs[3] ;

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

texture ShadowMap ;
sampler ShadowMapSamp = sampler_state
{
	Texture = <ShadowMap> ;
	MinFilter = LINEAR ;
	MagFilter = LINEAR ;
	MipFilter = LINEAR ;
	
	AddressU = Wrap ;
	AddressV = Wrap ;
} ;

struct VS_OUTPUT
{
    float4 Pos			: POSITION;
    float4 Color        : COLOR0 ;
    float2 Tex          : TEXCOORD0;
	float4 TexShadow    : TEXCOORD1;
	float2 TexLDepth    : TEXCOORD2;
};

float4 calculateTSMCoord(in float4 Pos)
{
	float4 pos = mul(Pos, matWLVP) ;

	if(vNTs[0].w > 0.0f)
	{
		float4 ret ;
		ret.x = vNTs[0].x*pos.x + vNTs[0].y*pos.y + vNTs[0].z ;
		ret.y = vNTs[1].x*pos.x + vNTs[1].y*pos.y + vNTs[1].z ;
		ret.z = vNTs[2].x*pos.x + vNTs[2].y*pos.y + vNTs[2].z ;

		ret.x /= abs(ret.z) ;
		ret.y /= abs(ret.z) ;
		ret.z = pos.z ;
		ret.w = pos.w ;

		return ret ;
	}
	return pos ;
}


VS_OUTPUT VS(float4 Pos    : POSITION, float4 Normal : NORMAL, float2 Tex : TEXCOORD0)
{
	VS_OUTPUT Out = (VS_OUTPUT)0;        // 출력데이터

	Out.Pos = mul(Pos, matWVP);
	Out.Color = Out.Pos ;
	Out.Tex = Tex ;

	//Out.TexShadow = mul(Pos, matWLVP);// calculateTSMCoord(Pos) ;

	float4 shadowpos = calculateTSMCoord(Pos) ;// mul(Pos, matWLVP);// calculateTSMCoord(Pos) ;

	//float offset = 0.5f + (0.5f/1024.0f) ;
	float offset = 0.5f + (0.5f/vNTs[2].w) ;

	Out.TexShadow.x = shadowpos.x*0.5f + shadowpos.w*offset ;
	Out.TexShadow.y = -shadowpos.y*0.5f + shadowpos.w*offset ;
	Out.TexShadow.z = 0.0f ;
	Out.TexShadow.w = shadowpos.w ;

	float4 depth = mul(Pos, matWLVP);
	Out.TexLDepth = float2(depth.z, depth.w) ;

	return Out;
}
float4 PS(VS_OUTPUT In) : COLOR
{
	float4 diffuse = tex2D(DiffuseMapSamp, In.Tex) ;
	//float shadow = tex2Dproj(ShadowMapSamp, In.TexShadow).x ;
	//float shadow = tex2D(ShadowMapSamp, In.TexShadow).x ;

	//if(In.TexLDepth.x > (shadow-vNTs[1].w))
	//	diffuse *= 0.5f ;

    return diffuse ;
}

technique TShader
{
    pass P0
    {
        VertexShader = compile vs_2_0 VS();
        PixelShader  = compile ps_2_0 PS();
    }
}