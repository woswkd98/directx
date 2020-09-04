#include "stdafx.h"
#include "cSphereCollider.h"
#include "cBoxCollider.h"


void cSphereCollider::Awake()
{
	cCollider::Awake();

	//float xy = sqrtf(m_scale.x * m_scale.x + m_scale.y * m_scale.y);

	//m_fRadius = sqrtf(xy * xy + m_scale.z * m_scale.z);

	g_GizmoManager->AddSphere(m_pTransformOBB);

	m_pTransformOBB->SetLocalPosition(0, 0, 0);
	g_GizmoManager->AddSphere(m_pTransformAABB);
}

void cSphereCollider::FixedUpdate()
{
	cCollider::FixedUpdate();
}

void cSphereCollider::Calculate()
{
	
}

void cSphereCollider::LateUpdate()
{
	m_colliders.clear();
}

void cSphereCollider::SetRadius(float rad)
{
	m_fRadius = rad;

	m_pTransformOBB->SetScale(rad, rad , rad);
}

XMFLOAT3 cSphereCollider::GetMOI(float mass)
{
	return XMFLOAT3();
}

cSphereCollider::cSphereCollider()
{
}


cSphereCollider::~cSphereCollider()
{
}
