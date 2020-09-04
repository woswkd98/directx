#include "stdafx.h"
#include "cCollider.h"
#include "cBVHTree.h"
/*
XMFLOAT3 cCollider::g_boxVertex[8] = { // 길이가 1인 기본박스 
	XMFLOAT3(-0.5f, -0.5f, -0.5f),
	XMFLOAT3(-0.5f, 0.5f, -0.5f),	
	XMFLOAT3(0.5f, 0.5f, -0.5f),
	XMFLOAT3(0.5f, -0.5f, -0.5f),
	XMFLOAT3(-0.5f, -0.5f, 0.5f),
	XMFLOAT3(-0.5f, 0.5f, 0.5f),
	XMFLOAT3(0.5f, 0.5f, 0.5f),
	XMFLOAT3(0.5f, -0.5f, 0.5f)
};



void cCollider::UpdateAABBBox()
{
	XMFLOAT3 minXYZ, maxXYZ; // aabb 박스를 만들기위한 최대 최소값

	ZeroMemory(&minXYZ, sizeof(XMFLOAT3));
	ZeroMemory(&maxXYZ, sizeof(XMFLOAT3));

	for (int i = 0; i < 8; ++i)
	{
		XMStoreFloat3(&m_arrAABBboxVertex[i], XMVector3TransformCoord(XMLoadFloat3(&g_boxVertex[i]), GetOBBTransform()->GetFinalMatrix())); // 정점 변환시킴
	}

	for (size_t i = 0; i < 8; ++i) // min max 구함 
	{
		if (m_arrAABBboxVertex[i].x < minXYZ.x) minXYZ.x = m_arrAABBboxVertex[i].x;
		else if (m_arrAABBboxVertex[i].x > maxXYZ.x) maxXYZ.x = m_arrAABBboxVertex[i].x;

		if (m_arrAABBboxVertex[i].y < minXYZ.y) minXYZ.y = m_arrAABBboxVertex[i].y;
		else if (m_arrAABBboxVertex[i].y > maxXYZ.y) maxXYZ.y = m_arrAABBboxVertex[i].y;

		if (m_arrAABBboxVertex[i].z < minXYZ.z) minXYZ.z = m_arrAABBboxVertex[i].z;
		else if (m_arrAABBboxVertex[i].z > maxXYZ.z) maxXYZ.z = m_arrAABBboxVertex[i].z;
	} 

	// aabb 박스 나중에 그릴때 
	
	m_arrAABBboxVertex[0] = XMFLOAT3(minXYZ.x, maxXYZ.y, maxXYZ.z);
	m_arrAABBboxVertex[1] = XMFLOAT3(maxXYZ.x, maxXYZ.y, maxXYZ.z);
	m_arrAABBboxVertex[2] = XMFLOAT3(minXYZ.x, maxXYZ.y, minXYZ.z);
	m_arrAABBboxVertex[3] = XMFLOAT3(maxXYZ.x, maxXYZ.y, minXYZ.z);
	m_arrAABBboxVertex[4] = XMFLOAT3(minXYZ.x, minXYZ.y, maxXYZ.z);
	m_arrAABBboxVertex[5] = XMFLOAT3(maxXYZ.x, minXYZ.y, maxXYZ.z);
	m_arrAABBboxVertex[6] = XMFLOAT3(minXYZ.x, minXYZ.y, minXYZ.z);
	m_arrAABBboxVertex[7] = XMFLOAT3(maxXYZ.x, minXYZ.y, minXYZ.z);

}
cCollider * cCollider::SetBoundBox(XMFLOAT3 CenterPos, XMFLOAT3 scale)
{
	m_pTransformOBB->SetLocalPosition(CenterPos);

	m_pTransformOBB->SetScale(scale);

	// radius 생성
	float xy = sqrtf(m_pTransformOBB->GetScale().x / 2 * m_pTransformOBB->GetScale().x / 2 + m_pTransformOBB->GetScale().y / 2 * m_pTransformOBB->GetScale().y / 2);

	m_fRadius = sqrtf(xy * xy + m_pTransformOBB->GetScale().z / 2 * m_pTransformOBB->GetScale().z / 2);

	g_GizmoManager->AddBox(m_pTransformOBB);

	this->shape = SHAPE_BOX;
	
	return this;
}

cCollider * cCollider::SetBoundSphere(XMFLOAT3 CenterPos, float radius)
{
	m_pTransformOBB->SetLocalPosition(CenterPos);

	m_pTransformOBB->SetScale(radius, radius, radius);

	m_fRadius = radius;

	g_GizmoManager->AddSphere(m_pTransformOBB);

	this->shape = SHAPE_SPHERE;

	return this;
}

cCollider * cCollider::SetBoundCapsule(XMFLOAT3 CenterPos, float radius, float Length)
{
	m_pTransformOBB->SetLocalPosition(CenterPos); // 위치잡고

	m_pTransformOBB->SetScale(1, 1, Length); // 선분의 길이 (z 축길이로 놓았다)

	m_fRadius = radius; // 나중에 길이비교할 반지름

	this->shape = SHAPE_CAPSULE;
	
	return this;
}

cCollider * cCollider::SetTerrainCollider(XMFLOAT3 CenterPos, XMFLOAT3 m_scale)
{
	m_pTransformOBB->SetLocalPosition(CenterPos);

	m_pTransformOBB->SetScale(m_scale);

	this->shape = SHAPE_CAPSULE;

	return this;
}

void cCollider::Awake()
{
	m_pTransformOBB = new cTransform;
	m_pTransformOBB->SetParent(this->GetGameObject());
}

void cCollider::FixedUpdate()
{
	m_pTransformOBB->UpdateTransform();

	UpdateAABBBox();
}

void cCollider::Update(float deltatime)
{
	g_GizmoManager->AddLine(m_arrAABBboxVertex[0], m_arrAABBboxVertex[1], XMFLOAT4(0, 0, 1, 1));
	g_GizmoManager->AddLine(m_arrAABBboxVertex[1], m_arrAABBboxVertex[3], XMFLOAT4(0, 0, 1, 1));

	g_GizmoManager->AddLine(m_arrAABBboxVertex[3], m_arrAABBboxVertex[2], XMFLOAT4(0, 0, 1, 1));
	g_GizmoManager->AddLine(m_arrAABBboxVertex[2], m_arrAABBboxVertex[0], XMFLOAT4(0, 0, 1, 1));

	g_GizmoManager->AddLine(m_arrAABBboxVertex[4], m_arrAABBboxVertex[5], XMFLOAT4(0, 0, 1, 1));
	g_GizmoManager->AddLine(m_arrAABBboxVertex[5], m_arrAABBboxVertex[7], XMFLOAT4(0, 0, 1, 1));

	g_GizmoManager->AddLine(m_arrAABBboxVertex[7], m_arrAABBboxVertex[6], XMFLOAT4(0, 0, 1, 1));
	g_GizmoManager->AddLine(m_arrAABBboxVertex[6], m_arrAABBboxVertex[4], XMFLOAT4(0, 0, 1, 1));

	g_GizmoManager->AddLine(m_arrAABBboxVertex[0], m_arrAABBboxVertex[4], XMFLOAT4(0, 0, 1, 1));
	g_GizmoManager->AddLine(m_arrAABBboxVertex[1], m_arrAABBboxVertex[5], XMFLOAT4(0, 0, 1, 1));

	g_GizmoManager->AddLine(m_arrAABBboxVertex[2], m_arrAABBboxVertex[6], XMFLOAT4(0, 0, 1, 1));
	g_GizmoManager->AddLine(m_arrAABBboxVertex[3], m_arrAABBboxVertex[7], XMFLOAT4(0, 0, 1, 1));

	if (g_keyManager->IsOnceDown(VK_TAB))
	{
		LOG_MGR->AddLog("%f,%f,%f", m_arrAABBboxVertex[1].x, m_arrAABBboxVertex[1].y, m_arrAABBboxVertex[1].z);
		LOG_MGR->AddLog("%f,%f,%f", m_arrAABBboxVertex[6].x, m_arrAABBboxVertex[6].y, m_arrAABBboxVertex[6].z);
	}
}
*/
cCollider::cCollider()
{

}


cCollider::~cCollider()
{
	cBVHTree::DeleteCollider(this);
}
