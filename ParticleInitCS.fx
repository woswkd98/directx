
struct Particle
{
	float3 accel;
	float3 velocity;
	float3 position;
	float currentTime; // 진행시간

};


RWStructuredBuffer<int> inactive :register(u0);  

[numthreads(1, 1, 1)]
void ParticleInitCS(uint3 Gid : SV_GroupID, uint3 DTid : SV_DispatchThreadID, uint3 GTid : SV_GroupThreadID, uint GI : SV_GroupIndex)
{
	uint num;
	uint temp;
	inactive.GetDimensions(num, temp);

	for (int i = 0; i < num - 1; ++i)
	{
		inactive[i] = i;
	}

	inactive[num - 1] = num; // lastindex 번쨰 또는 개수 
}

/*

*/