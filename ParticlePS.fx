

struct PS_INPUT {
	float4	pos		: SV_POSITION;
	float2  uv	: TEXCOORD0;
	float3	color	: COLOR;
};

// �ؽ��� ���ø�
Texture2D DiffuseTexture	: register(t0); // �ؽ��� 
Texture2D NormalTexture		: register(t1); // �ؽ���
Texture2D SpecularTexture	: register(t2); // �ؽ���
Texture2D EmissiveTexture	: register(t3); // �ؽ��� 

SamplerState LinearSampler	: register(s0); // ���÷��� ����

float4 ParticlePS(PS_INPUT pIn) : SV_Target
{
	float4 texColor = DiffuseTexture.Sample(LinearSampler, pIn.uv);
	return texColor;
}