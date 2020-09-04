

struct PS_INPUT {
	float4	pos		: SV_POSITION;
	float2  uv	: TEXCOORD0;
	float3	color	: COLOR;
};

// 텍스쳐 샘플링
Texture2D DiffuseTexture	: register(t0); // 텍스쳐 
Texture2D NormalTexture		: register(t1); // 텍스쳐
Texture2D SpecularTexture	: register(t2); // 텍스쳐
Texture2D EmissiveTexture	: register(t3); // 텍스쳐 

SamplerState LinearSampler	: register(s0); // 샘플러는 같다

float4 ParticlePS(PS_INPUT pIn) : SV_Target
{
	float4 texColor = DiffuseTexture.Sample(LinearSampler, pIn.uv);
	return texColor;
}