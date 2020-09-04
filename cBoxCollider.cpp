#include "stdafx.h"
#include "cBoxCollider.h"
#include "cSphereCollider.h"

cBoxCollider::cBoxCollider()
{
	
}


cBoxCollider::~cBoxCollider()
{
	
}

void cBoxCollider::Awake()
{
	cCollider::Awake(); // 상위 초기화

	// radius 생성
	float xy = sqrtf(m_pTransformOBB->GetScale().x / 2 * m_pTransformOBB->GetScale().x / 2 + m_pTransformOBB->GetScale().y / 2 * m_pTransformOBB->GetScale().y / 2);

	m_fRadius = sqrtf(xy * xy + m_pTransformOBB->GetScale().z / 2 * m_pTransformOBB->GetScale().z / 2);
	
	g_GizmoManager->AddBox(m_pTransformOBB);

	g_GizmoManager->AddBox(m_pTransformAABB);
}

void cBoxCollider::FixedUpdate()
{
	cCollider::FixedUpdate();

}

void cBoxCollider::Calculate()
{
}


XMFLOAT3 cBoxCollider::GetMOI(float mass)
{
	/*
		w 이 회전 속도
		r 무게중심에서 각원소질량 dm까지의 거리
	*/


	
	
}


//http://www.gingaminga.com/Data/Note/oriented_bounding_boxes/  박스 충돌에 대한 자료 
