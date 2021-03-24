RENDERING_ATTRIBUTES
{
	RENDERING_ATTR_RIGID,
}
MATERIAL_ATTRIBUTES
{
}

texture NormalMap ;
sampler NormalMapSamp = sampler_state
{
	Texture = <NormalMap> ;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = NONE;
		
	AddressU = Wrap ;
	AddressV = Wrap ;
} ;

texture DepthMap ;
sampler DepthMapSamp = sampler_state
{
	Texture = <DepthMap> ;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = NONE;
	
	AddressU = Wrap ;
	AddressV = Wrap ;
} ;

texture SpecularParamMap ;
sampler SpecularParamMapSamp = sampler_state
{
	Texture = <SpecularParamMap> ;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = NONE;
	
	AddressU = Wrap ;
	AddressV = Wrap ;
} ;

float4x4 matInvVP ;
float3 vLightColor ;
float3 vLightDir ;
float3 vLightPos ;
float3 vCameraPos ;
float fLightRange ;
float fSpecular ;

float3 getPosition(in float2 tex)
{
	float depth = (float)tex2D(DepthMapSamp, tex) ;
	float2 t = (tex*2.0f)-1.0f ;
	float4 a = mul(float4(t.x, -t.y, depth, 1.0f), matInvVP) ;
	return float3(a.x/a.w, a.y/a.w, a.z/a.w) ;
}
float3 getNormal(in float2 tex)
{
	return normalize(tex2D(NormalMapSamp, tex).xyz*2.0f-1.0f) ;
}
float2 getSpecularParam(in float2 tex)
{
	float2 param = tex2D(SpecularParamMapSamp, tex).xy ;
	param.x *= 100.0f ;
	return param ;
}

struct VS_OUTPUT
{
    float4 Pos			: POSITION;
    float2 Tex	        : TEXCOORD0;
};

VS_OUTPUT VS(float4 Pos : POSITION, float2 Tex : TEXCOORD0)
{
	VS_OUTPUT Out = (VS_OUTPUT)0;        // 출력데이터
	Out.Pos = Pos ;
	Out.Tex = Tex ;
	
	return Out;
}
float4 PS_Directional(VS_OUTPUT In) : COLOR
{
	float3 p = getPosition(In.Tex) ;
	float3 n = getNormal(In.Tex) ;
	float2 specular_param = getSpecularParam(In.Tex) ;
	float NL = max(0.0f, dot(n, -vLightDir)) ;
	float3 camera = normalize(vCameraPos-p) ;
	float3 H = normalize(camera-vLightDir) ;
	float RV = specular_param.y*pow(max(0.0f, dot(n, H)), specular_param.x) ;
	
	return float4(vLightColor.x*NL, vLightColor.y*NL, vLightColor.z*NL, RV*NL) ;
	
	//float4 prev = tex2D(PrevMapSamp, In.Tex) ;
	//return float4(vLightColor.x*NL+prev.x, vLightColor.y*NL+prev.y, vLightColor.z*NL+prev.z, RV*NL+prev.w) ;
	//return float4(vLightColor.x*a, vLightColor.y*a, vLightColor.z*a, 1.0f) ;
}
float4 PS_Point(VS_OUTPUT In) : COLOR
{
	float3 p = getPosition(In.Tex) ;
	float3 n = getNormal(In.Tex) ;
	float2 specular_param = getSpecularParam(In.Tex) ;
	
	if(tex2D(NormalMapSamp, In.Tex).w == 0.0f)
		discard ;
		
	//float4 prev = tex2D(PrevMapSamp, In.Tex) ;

	float3 dir = p-vLightPos ;
	float r = min(1.0f, fLightRange/length(dir)) ;
	
	r = 128.0f ;//32.0f ;
	float d = length(dir) ;
	float Kc = 1.0f ;
	float Kl = 2.0f/r ;
	float Kq = 1.0f/(r*r) ;
	
	float attenuation = 1.0f/(Kc + (Kl*d) + (Kq*d*d)) ;
	
	float3 camera = normalize(vCameraPos-p) ;
	float3 L = normalize(-dir) ;
	float3 H = normalize(L+camera) ;
	float a = max(0.1f, dot(n, L))*attenuation + pow(max(0.0f, dot(n, H)), 10.0f)*attenuation ;
	
	float NL = max(0.1f, dot(n, L))*attenuation ;
	//float RV = pow(max(0.0f, dot(n, H)), 10.0f)*attenuation ;
	float RV = specular_param.y*pow(max(0.0f, dot(n, H)), specular_param.x)*attenuation ;
	//float RV = 1.0f*pow(max(0.0f, dot(n, H)), 10.0f)*attenuation ;

    //return color;
    //return float4(0, 0, 0, 0);
	return float4(vLightColor.x*NL, vLightColor.y*NL, vLightColor.z*NL, RV*NL) ;
	//return float4(vLightColor.x*NL+prev.x, vLightColor.y*NL+prev.y, vLightColor.z*NL+prev.z, RV*NL+prev.w) ;
}

technique TShader
{
    pass P0
    {
        VertexShader = compile vs_2_0 VS();
        PixelShader  = compile ps_2_0 PS_Directional();
    }
    pass P1
    {
        //AlphaBlendEnable = true;
        //SrcBlend = one; //current light buffer
        //DestBlend = one; //back buffersq
        //BlendOp = add;
		
        VertexShader = compile vs_2_0 VS();
        PixelShader  = compile ps_2_0 PS_Point();
    }
}