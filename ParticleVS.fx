
struct VS_INPUT {
	uint ID : SV_VertexID;
};

struct GS_INPUT {
	float4 pos : SV_POSITION; // ��ġ 
};

struct Particle
{
	float3 accel;
	float3 velocity;
	float3 position;
	float currentTime; // ����ð�
};

StructuredBuffer<Particle> particles: register(u0); // ��ü ��ƼŬ�� ������ ���� 
StructuredBuffer<int> activeIndices : register(u1);

GS_INPUT ParticleVS(VS_INPUT input)
{
	GS_INPUT gs;
	gs.pos = float4(particles[activeIndices[input.ID]].position, 1.f);
	return gs;
}

