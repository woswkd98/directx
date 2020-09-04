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
	cCollider::Awake(); // ���� �ʱ�ȭ

	// radius ����
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
		w �� ȸ�� �ӵ�
		r �����߽ɿ��� ���������� dm������ �Ÿ�
	*/


	
	
}


//http://www.gingaminga.com/Data/Note/oriented_bounding_boxes/  �ڽ� �浹�� ���� �ڷ� 
