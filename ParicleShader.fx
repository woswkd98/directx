#pragma once


struct VS_INPUT {
	uint vertexID : SV_VertexID; // ���� �ε��� 
};

cbuffer {
	
};

struct Particle
{
	float3 Force; // ���ӵ�
	float  Mass; // ����	
	float3 Velocity; // �ӵ�
	float Size; // ũ��
	float LifeTime; // ���� ����� �ð�
};

cbuffer ParticleSystemBuf
{
	float3 ParticleEmitterPosition; // ��ƼŬ �̹����� �ʱ���ġ
	float  TimeStep; // ���� ���ϴ� ������ Ÿ��
};


// quad ��ƼŬ �ý��ۿ� �� �ؽ����� ����
Texture2D DiffuseTexture	: register(t0); // �ؽ��� 
Texture2D NormalTexture		: register(t1); // �ؽ���
Texture2D SpecularTexture	: register(t2); // �ؽ���
Texture2D EmissionTexture	: register(t3); // �ؽ��� 

SamplerState LinearSampler	: register(s0); // ���÷��� ����

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



// ��ƼŬ ����ü �迭���� 
StructuredBuffer<Particle> particleBuffer : register(u0); // ���⿡ ��ƼŬ ���� ��ŭ ������ѳ��´�

// ������ �׷쿡�� �� �׷� �ȿ� �ִ� ����� ����ȭ�� ���������� �ٸ� �׷쿡���� ����ȭ�� �Ұ�
/*
	CS�� ���� ���� directx11�� �̿��� �������α׷��� �Թ� å 489 ~
	�⺻������ ������ �׷��� 3�������� ����� �ִ�
*/

/*
	��Ʈ�� ��� �ܰ�

	���� ���̴����� ����� �������� V�� ��ϵǰ� �Ҽ� �ְ� �װ���  DrawAuto�� �׸� �� �ִ�
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
	// ���⿡ ȥ�� �־����


	return startColor;
}



//point : primitive type ���⼭ point �� point list ����
//  gin[1] �迭 �ȿ� �� �ִ°��� You operate on only one point at a time.
// ��� ����� �ִµ� �����ð����� �ϳ��� �����͸� �۵���ų�� �ִ�?
[maxvertexcount(2)] // stream�� �����ϴ� ������ �ִ� ����
ParticleStreamOS_GS(point Particle gin[1], inout PointStream<Particle> ptStream)
{


}


// ���ӹ��� ���� ���� å�� �ִ¹��
/*
�⺻������ �����ð� t�� ���� ��ġ ��ȭ���� �ӵ��� �ӵ� ��ȭ���� ���ӵ� �̹Ƿ� (�̺�) �Ųٷ� ������ ��Ű�� ��ġ�� �ȴ�
���� ���Ϸ� ����� damping �̶�� �ð��� ���� �ӵ��� ���̴� ����� �ش� 1 �� ����� 1���� ���� ���� ���� �����Ѵ�

1/2 * t * t * accel �ε� �̰��� ���� ���� �� �̰� �̰��� ���ΰ���� ��ü�ϴ� ����
*/

float dampingFactor = 0.999f; // ���� ��� 

[numthreads(1024, 1, 1)]
void ParticleCS1() // ���� ���Ϸ� ���
{
	float3 accel = Force / mass; // F = MA -> A = F / M

	float3 dv = accel * dt; // �ð���ȭ���� ���� �ӵ� 

	p.velocity += dv;  // ����

	p.velocity *= pow(dampingFactor, dt); // ���ΰ�� ����

	p.Position += p.velocity * dt; // �����Ѱ� ������
}
//https://ko.wikipedia.org/wiki/%EB%A3%BD%EA%B2%8C-%EC%BF%A0%ED%83%80_%EB%B0%A9%EB%B2%95 ��� ��Ÿ ���

/*
void ParticleCS2()
{
	
}


void ParticleCS3()
{

}
*/
