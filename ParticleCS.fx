
// hlsl data type
//https://msdn.microsoft.com/ko-kr/library/windows/desktop/bb509646(v=vs.85).aspx 
// 

cbuffer ParticleIntegrateCB : register(b0)
{
	float size; // ��ƼŬ ����
	float particleLifeTime; // �� ��ƼŬ�� �ִ�� ����ִ� �ð�
	float frameTime; // ������ �ð� �Ǵ� �ùķ����� �ð�
	float gameTime; // ���� ��ü �ð� 
};

struct Particle
{
	float3 accel;
	float3 velocity;
	float3 position;
	float duration; // ����ð�
};

static const float dampingFactor = 0.999f;

static const int g_numthreads = 256; // 256�� ���������� ����  ���� ������ �ִµ� �� ������ (������ 32 ati 64)�̰� ���� �̰��� ����� �����ִ°��� ���� 



/*
 �� ���� �ڿ��� ���� ���� �ڿ� �並 ���ÿ� ����ϴ� ���� �����ϴ�.(SRV readOnly) 
�׷��� ��Ÿ���Ե� ���� ���� ���� ��� �׷� �� ����. �ϳ��� ��� �� �ڿ��� ���� �б� �� ���� ������ �����ϰ� �ϳ�,
�ϳ��� ���� �ڿ��� ���� ���� ���� ���� �ڿ� �並 ���� ���� ����.
�̴� �̸��׸� �� ���� UAV�� �̿��ؼ� �ѹ��� �ڿ��� ���� �ٸ� �������� ���ҵ��� ���ÿ� �߰��ϰ� �Һ��� ���� ���ٴ� ���̴�. 
�߰� UAV�� �Һ� UAV�� ���ÿ� ����Ϸ��� ���� ���� �ٸ� ���� �ڿ��� �����ؾ� �Ѵ�.

�� readonly ���۴� ������ ���ϼ� ������ UAV�� �� ���ۿ� �Ѱ� ���� ���ϼ� �ִ� 
*/

RWStructuredBuffer<Particle> srcParticleBuffer : register(u0); // �б� ���Ⱑ ������ UAV 

RWStructuredBuffer<int> activeIndices : register(u1);

RWStructuredBuffer<int> inactiveIndices: register(u2);

[numthreads(g_numthreads, 1, 1)]
void IntegrateCS(uint3 DTid : SV_DispatchThreadID) // groupID �� �׷��� �ε��� 
{
	if (DTid.x >= size) return;

	int currentIndex = activeIndices[DTid.x];
	
	Particle p = srcParticleBuffer[currentIndex];

	p.duration += frameTime; // ��ƼŬ ������ �ð� ������Ʈ

	if (particleLifeTime < p.duration)
	{
		inactiveIndices[inactiveIndices[size]] = currentIndex;
		inactiveIndices[size] += 1;
		activeIndices[size] -= 1; // Ȱ��ȭ�� �ε������� �ϳ� ���̰� 

		return;
	}

	float3 dv = p.accel * frameTime; // �ð���ȭ���� ���� �ӵ� 

	p.velocity += dv;

	p.velocity *= pow(dampingFactor, frameTime); // ���ΰ�� ����

	p.position += p.velocity * frameTime;

	srcParticleBuffer[currentIndex] = p;
}



/*

	Start() -> ParticleInitCS (byteBuffer �ʱ�ȭ)

	Update()-> ParticleEmitCS (�����ð����� ����)

	Render()- ParticleDrawGS (byteBuffer �ȿ� �ִ� �ε��� ����) -> PS

	draw ���� 

	dispacth() ; // ��ƼŬ ���

	VS->SetVSResource(activeinddices(Buffer)); // ,���⼭ vertexid�� �ε����� ������ŭ������  
*/

