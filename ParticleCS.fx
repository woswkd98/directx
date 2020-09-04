
// hlsl data type
//https://msdn.microsoft.com/ko-kr/library/windows/desktop/bb509646(v=vs.85).aspx 
// 

cbuffer ParticleIntegrateCB : register(b0)
{
	float size; // 파티클 개수
	float particleLifeTime; // 한 파티클이 최대로 살아있는 시간
	float frameTime; // 프레임 시간 또는 시뮬레이터 시간
	float gameTime; // 게임 전체 시간 
};

struct Particle
{
	float3 accel;
	float3 velocity;
	float3 position;
	float duration; // 진행시간
};

static const float dampingFactor = 0.999f;

static const int g_numthreads = 256; // 256로 정한이유는 보통  워프 단위가 있는데 그 개수가 (엔비디아 32 ati 64)이고 보통 이것의 배수로 정해주는것이 좋다 



/*
 한 버퍼 자원에 여러 개의 자원 뷰를 동시에 사용하는 것이 가능하다.(SRV readOnly) 
그러나 안타깝게도 순서 없는 접근 뷰는 그럴 수 없다. 하나의 뷰는 그 자원에 대한 읽기 및 쓰기 접근을 가능하게 하나,
하나의 하위 자원에 여러 개의 쓰기 가능 자원 뷰를 붙일 수는 없다.
이는 이를테면 두 개의 UAV를 이용해서 한버퍼 자원의 서로 다른 영역에서 원소들을 동시에 추가하고 소비할 수는 없다는 뜻이다. 
추가 UAV와 소비 UAV를 동시에 사용하려면 둘을 각자 다른 버퍼 자원과 연결해야 한다.

즉 readonly 버퍼는 여러개 붙일수 있지만 UAV는 한 버퍼에 한개 씩만 붙일수 있다 
*/

RWStructuredBuffer<Particle> srcParticleBuffer : register(u0); // 읽기 쓰기가 가능한 UAV 

RWStructuredBuffer<int> activeIndices : register(u1);

RWStructuredBuffer<int> inactiveIndices: register(u2);

[numthreads(g_numthreads, 1, 1)]
void IntegrateCS(uint3 DTid : SV_DispatchThreadID) // groupID 는 그룹의 인덱스 
{
	if (DTid.x >= size) return;

	int currentIndex = activeIndices[DTid.x];
	
	Particle p = srcParticleBuffer[currentIndex];

	p.duration += frameTime; // 파티클 개인의 시간 업데이트

	if (particleLifeTime < p.duration)
	{
		inactiveIndices[inactiveIndices[size]] = currentIndex;
		inactiveIndices[size] += 1;
		activeIndices[size] -= 1; // 활성화된 인덱스에서 하나 줄이고 

		return;
	}

	float3 dv = p.accel * frameTime; // 시간변화량에 대한 속도 

	p.velocity += dv;

	p.velocity *= pow(dampingFactor, frameTime); // 댐핑계수 적용

	p.position += p.velocity * frameTime;

	srcParticleBuffer[currentIndex] = p;
}



/*

	Start() -> ParticleInitCS (byteBuffer 초기화)

	Update()-> ParticleEmitCS (일정시간마다 생성)

	Render()- ParticleDrawGS (byteBuffer 안에 있는 인덱스 렌더) -> PS

	draw 순서 

	dispacth() ; // 파티클 계산

	VS->SetVSResource(activeinddices(Buffer)); // ,여기서 vertexid로 인덱스의 개수만큼돌려짐  
*/

