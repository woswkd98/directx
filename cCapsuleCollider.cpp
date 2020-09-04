#include "stdafx.h"
#include "cCapsuleCollider.h"


XMVECTOR & cCapsuleCollider::GetPosition()
{
	XMVECTOR capsuleStartPos;
	XMVECTOR capsuleDir = this->GetOBBTransform()->GetForward(); // 캡슐의 방향
	XMVECTOR colpos = XMLoadFloat3(&m_pDetectCollider->GetOBBTransform()->GetPositionRef()); // 충돌검사할 컬라이더 위치
	float length = this->GetOBBTransform()->GetScale().z; // 캡슐의 길이 
	capsuleStartPos = XMLoadFloat3(&this->GetOBBTransform()->GetPositionRef()) - capsuleDir *  length / 2; // 캡슐의 시작지점 (내위치에서 -z 방향으로 길이의 반만큼 )

	XMVECTOR v1 = colpos - capsuleStartPos; // 내위치에서 컬라이더 중점으로 나가는 벡터

	float DistOnLIne = XMVectorGetX(XMVector3Dot(v1, capsuleDir)) / length; // 길이비 

	return clampO1(DistOnLIne) * length * capsuleDir + capsuleStartPos;// 수직으로 내려그은 위치구함 (이 위치가 컬라이더 중점이랑 내 선분의 최단거리)
}

XMFLOAT3 cCapsuleCollider::GetMOI(float mass)
{


	
	return XMFLOAT3();
}

void cCapsuleCollider::Calculate()
{
	
}

cCapsuleCollider::cCapsuleCollider()
{
}


cCapsuleCollider::~cCapsuleCollider()
{
}
