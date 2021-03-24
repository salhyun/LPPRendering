RENDERING_ATTRIBUTES
{
}
MATERIAL_ATTRIBUTES
{
}

const float2 samples[4] = { float2(1.0f, 0.0f), float2(-1.0f, 0.0f), float2(0.0f, 1.0f), float2(0.0f, -1.0f) } ;
float sample_rad=16.0f ;
float scale=2.0f ;
float bias=0.001f ;
float intensity=1.0f ;

static const int MAX_SAMPLES=12 ;
float2 avSampleOffsets[MAX_SAMPLES] ;
float4x4 matInvProj ;
float4x4 matInvVP ;
float4x4 matView ;

float2 vScreenSize ;
float2 vRandomSize ;
float fNearToFar ;

texture NormalMap ;
sampler NormalMapSamp = sampler_state
{
	Texture = <NormalMap> ;
	MinFilter = POINT;
	MagFilter = POINT;
	MipFilter = POINT;
		
	AddressU = Clamp ;
	AddressV = Clamp ;
} ;

texture DepthMap ;
sampler DepthMapSamp = sampler_state
{
	Texture = <DepthMap> ;
	MinFilter = POINT;
	MagFilter = POINT;
	MipFilter = POINT;
	
	AddressU = Clamp ;
	AddressV = Clamp ;
} ;
texture RandomMap ;
sampler RandomMapSamp = sampler_state
{
	Texture = <RandomMap> ;
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

float3 getPosition(in float2 tex)
{
	float depth = (float)tex2D(DepthMapSamp, tex) ;
	float2 t = (tex*2.0f)-1.0f ;
    float4 a = mul(float4(t.x, -t.y, depth, 1.0f), matInvProj); //to view space, because 카메라로 부터 깊이값이 필요함
	return float3(a.x/a.w, a.y/a.w, a.z/a.w) ;
}
float3 getNormal(in float2 tex)
{
	float3 n = normalize(tex2D(NormalMapSamp, tex).xyz*2.0f-1.0f) ;
    return mul(n, (float3x3) matView).xyz; //to view space, because 카메라로 부터 깊이값이 필요함
}
float2 getRandom(in float2 tex)
{
	//렌덤텍스쳐를 소스텍스쳐에다가 타일링시켜서 위치를 얻어낸다
	tex.x = vScreenSize.x*tex.x/vRandomSize.x ;
	tex.y = vScreenSize.y*tex.y/vRandomSize.y ;
	return normalize(tex2D(RandomMapSamp, tex).xy*2.0f-1.0f) ;
	//return float3(0.0f, 0.0f, 1.0f) ;
}
float calculateAmbientOcclusion(in float2 tex, in float2 offset, in float3 p, in float3 cnorm)
{
	float3 diff = getPosition(offset+tex) - p ;
	float3 n = normalize(diff) ;
	float d = length(diff)*scale ;
	return max(0.0f, dot(cnorm, n)-bias)*(1.0f/(1.0f+d))*intensity ;
}

float4 PS(VS_OUTPUT In) : COLOR
{
    float depth = (float) tex2D(DepthMapSamp, In.Tex);
    if (depth <= 0.0f)//렌더링이 안된 픽셀영역
    {
        return float4(1.0f, 0.0f, 0.0f, 1.0f);
    }
    
	float ao=0.0f ;    
    float3 p = getPosition(In.Tex);
    float3 n = getNormal(In.Tex);
    float2 rand = getRandom(In.Tex);
    float rad = sample_rad / p.z;
	
    for (int i = 0; i < 4; i++)
    {
        float2 t1 = reflect(samples[i], rand) * rad;
        float2 t2 = float2(t1.x * 0.7071f - t1.y * 0.7071f, t1.x * 0.7071f + t1.y * 0.7071f);
		
        ao += calculateAmbientOcclusion(In.Tex, t1 * 0.25f, p, n);
        ao += calculateAmbientOcclusion(In.Tex, t2 * 0.5f, p, n);
        ao += calculateAmbientOcclusion(In.Tex, t1 * 0.75f, p, n);
        ao += calculateAmbientOcclusion(In.Tex, t2, p, n);
    }
	//ao /= 16.0f ;
    ao = 1.0f - (ao * 0.0625f);

    return float4(ao, ao, ao, 1.0f);
	
	//float4 color ;
	//color = (ao <= 0.0f) ? float4(1.0f, 0.0f, 0.0f, 1.0f) : float4(ao, ao, ao, 1.0f) ;
}

technique TShader
{
    pass P0
    {
        VertexShader = compile vs_3_0 VS();
        PixelShader  = compile ps_3_0 PS();
    }
}