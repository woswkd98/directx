#pragma once


struct VS_INPUT {
	uint vertexID : SV_VertexID; // 정점 인덱스 
};

cbuffer {
	
};

struct Particle
{
	float3 Force; // 가속도
	float  Mass; // 질량	
	float3 Velocity; // 속도
	float Size; // 크기
	float LifeTime; // 현재 진행된 시간
};

cbuffer ParticleSystemBuf
{
	float3 ParticleEmitterPosition; // 파티클 이미터의 초기위치
	float  TimeStep; // 흔히 말하는 프레임 타임
};


// quad 파티클 시스템에 들어갈 텍스쳐의 정보
Texture2D DiffuseTexture	: register(t0); // 텍스쳐 
Texture2D NormalTexture		: register(t1); // 텍스쳐
Texture2D SpecularTexture	: register(t2); // 텍스쳐
Texture2D EmissionTexture	: register(t3); // 텍스쳐 

SamplerState LinearSampler	: register(s0); // 샘플러는 같다

cbuffer PixelCB
{
	float4 startColor;
	float4 endColor;
};

cbuffer WVP
{
	float4x4 VP;
	float4x4 World;
};



// 파티클 구조체 배열형태 
StructuredBuffer<Particle> particleBuffer : register(u0); // 여기에 파티클 개수 만큼 저장시켜놓는다

// 스레드 그룹에서 한 그룹 안에 있는 계산은 동기화가 가능하지만 다른 그룹에서는 동기화가 불가
/*
	CS에 대한 설명 directx11을 이용한 게임프로그래밍 입문 책 489 ~
	기본적으로 스레드 그룹은 3차원까지 만들수 있다
*/

/*
	스트림 출력 단계

	기하 셰이더에서 출력한 정점들이 V에 기록되게 할수 있고 그것을  DrawAuto로 그릴 수 있다
*/
float4 ParticleVS(VS_INPUT input)
{
	GS_INPUT output = (GS_INPUT)0;

	output.pos.xyz = particleBuffer[input.vertexID].currPos;
	output.pos.w = 1.0f;

	output.tex0 = float2(0, 0);

	return output;
}




float4 PointParticlePS()
{
	// 여기에 혼합 넣어야함


	return startColor;
}



//point : primitive type 여기서 point 는 point list 형태
//  gin[1] 배열 안에 들어가 있는것은 You operate on only one point at a time.
// 라고 설명되 있는데 일정시간마다 하나의 포인터를 작동시킬수 있다?
[maxvertexcount(2)] // stream에 저장하는 정점의 최대 개수
ParticleStreamOS_GS(point Particle gin[1], inout PointStream<Particle> ptStream)
{


}


// 게임물리 엔진 개발 책에 있는방법
/*
기본적으로 일정시간 t에 대한 위치 변화량이 속도고 속도 변화량이 가속도 이므로 (미분) 거꾸로 적분을 시키면 위치가 된다
기초 오일러 방법에 damping 이라는 시간에 따라 속도를 줄이는 계수를 준다 1 에 가까운 1보다 작은 수를 보통 지정한다

1/2 * t * t * accel 인데 이값은 아주 작은 수 이고 이것을 댐핑계수로 대체하는 형식
*/

float dampingFactor = 0.999f; // 댐핑 계수 

[numthreads(1024, 1, 1)]
void ParticleCS1() // 기초 오일러 방법
{
	float3 accel = Force / mass; // F = MA -> A = F / M

	float3 dv = accel * dt; // 시간변화량에 대한 속도 

	p.velocity += dv;  // 적분

	p.velocity *= pow(dampingFactor, dt); // 댐핑계수 적용

	p.Position += p.velocity * dt; // 적분한걸 더해줌
}
//https://ko.wikipedia.org/wiki/%EB%A3%BD%EA%B2%8C-%EC%BF%A0%ED%83%80_%EB%B0%A9%EB%B2%95 룽게 쿠타 기법

/*
void ParticleCS2()
{
	
}


void ParticleCS3()
{

}
*/
