RENDERING_ATTRIBUTES
{
	RENDERING_ATTR_RIGID,
}
MATERIAL_ATTRIBUTES
{
}

float4x4 matWVP ;

texture LightMap ;
sampler LightMapSamp = sampler_state
{
	Texture = <LightMap> ;
	MinFilter = LINEAR ;
	MagFilter = LINEAR ;
	MipFilter = LINEAR ;
	
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

struct VS_OUTPUT
{
    float4 Pos			: POSITION;
    float2 Tex          : TEXCOORD0;
};

VS_OUTPUT VS(float4 Pos    : POSITION, float2 Tex : TEXCOORD0)
{
	VS_OUTPUT Out = (VS_OUTPUT)0;        // 출력데이터

	Out.Pos = Pos ;
	Out.Tex = Tex ;

	return Out;
}
float4 PS(VS_OUTPUT In) : COLOR
{
    float4 light = tex2D(LightMapSamp, In.Tex) ;
    float3 diffuse = tex2D(DiffuseMapSamp, In.Tex).xyz ;
    
    return float4(diffuse.x*light.x+light.w, diffuse.y*light.y+light.w, diffuse.z*light.z+light.w, 1.0f) ;
    //return float4(light.x*diffuse.x, light.y*diffuse.y, light.z*diffuse.z, 1.0f) ;
}

technique TShader
{
    pass P0
    {
        VertexShader = compile vs_2_0 VS();
        PixelShader  = compile ps_2_0 PS();
    }
}