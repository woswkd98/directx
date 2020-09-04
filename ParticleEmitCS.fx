cbuffer ParticleEmitCB : register(b0)
{
	float3 position; // ��ġ
	float3 force;
	float3 velocity;
	float mass;
	float Mode; // ���߿� �߰��� ���� ��� 
	float particleSize;
};

struct Particle
{
	float3 accel;
	float3 velocity;
	float3 position;
	float currentTime; // ����ð�
};


RWStructuredBuffer<Particle> srcParticleBuffer : register(u0); // �б� ���Ⱑ ������ UAV 

RWStructuredBuffer<int> inactiveIndices : register(u1);  // �� ������ �Ǹ����� �ε����� ��ƼŬ�� ���� �Ǵ� ���ο� �ε����� ���� ������ 
RWStructuredBuffer<int> activeIndices : register(u2);

[numthreads(1, 1, 1)]
void ParticleEmitCS(uint3 Gid : SV_GroupID, uint3 DTid : SV_DispatchThreadID, uint3 GTid : SV_GroupThreadID, uint GI : SV_GroupIndex)
{
	int lastindex = inactiveIndices[particleSize];

	if (lastindex == 0) return;

	// ��ƼŬ�� ������ �°� �ʱ�ȭ ���Ѽ� �־��ָ��

	int index = inactiveIndices[inactiveIndices[particleSize]];

	Particle p = srcParticleBuffer[index];
	p.position = position;
	p.velocity = velocity;
	p.currentTime = 0;
	p.accel = force / mass;
	srcParticleBuffer[index] = p;

	activeIndices[activeIndices[particleSize]] = index;

	activeIndices[particleSize]++;

	inactiveIndices[particleSize]--;

}