cbuffer ParticleEmitCB : register(b0)
{
	float3 position; // 위치
	float3 force;
	float3 velocity;
	float mass;
	float Mode; // 나중에 추가로 넣을 모드 
	float particleSize;
};

struct Particle
{
	float3 accel;
	float3 velocity;
	float3 position;
	float currentTime; // 진행시간
};


RWStructuredBuffer<Particle> srcParticleBuffer : register(u0); // 읽기 쓰기가 가능한 UAV 

RWStructuredBuffer<int> inactiveIndices : register(u1);  // 이 버퍼의 맨마지막 인덱스는 파티클의 개수 또는 새로운 인덱스를 넣을 시작점 
RWStructuredBuffer<int> activeIndices : register(u2);

[numthreads(1, 1, 1)]
void ParticleEmitCS(uint3 Gid : SV_GroupID, uint3 DTid : SV_DispatchThreadID, uint3 GTid : SV_GroupThreadID, uint GI : SV_GroupIndex)
{
	int lastindex = inactiveIndices[particleSize];

	if (lastindex == 0) return;

	// 파티클을 정보에 맞게 초기화 시켜서 넣어주면됨

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