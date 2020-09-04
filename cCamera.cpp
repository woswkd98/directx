#include "stdafx.h"
#include "cCamera.h"


void cCamera::Awake() 
{

	// ī�޶� �ǻ��ڵ� 
	/*

	Xaxis.x             Yaxis.x            Zaxis.x       0

	Xaxis.y             Yaxis.y            Zaxis.y       0

	Xaxis.z             Yaxis.z            Zaxis.z       0

	- dot(Xaxis, eye) - dot(Yaxis, eye) - dot(Zaxis, eye)  1
	*/

	LOG_MGR->AddLog(__FUNCTION__);

	LOG_MGR->AddLog("ī�޶� ����");


	//�⺻ȭ�� 60 ��
	this->m_fov = 60.0f;

	//�⺻ Near
	this->m_camNear = 0.3f;

	//�⺻ Far
	this->m_camFar = 1000.0f;

	this->m_sizeX = 800;
	this->m_sizeY = 600;

	this->name = "cCamera";
	LOG_MGR->AddLog("%f", XMConvertToRadians(m_fov));
	LOG_MGR->AddLog("%f,%f", m_sizeX, m_sizeY);
	// �⺻���� ����
	XMStoreFloat4x4(
		&m_matPerspective,
		XMMatrixPerspectiveFovLH(
			XMConvertToRadians(m_fov),
			static_cast<float>(m_sizeX / m_sizeY)
			, m_camNear
			, m_camFar));
	m_matProjection = m_matPerspective; // �⺻������ perspective�� �����ߴ�
	XMStoreFloat4x4(&m_matView, XMMatrixIdentity());

	XMStoreFloat4x4(&m_matView, XMMatrixInverse(nullptr, this->GetGameObject()->GetTransform()->GetFinalMatrix())); // ���� ���(������ ����)�� ������� ������̴� 
	XMStoreFloat4x4(&m_matVP, XMLoadFloat4x4(&m_matView) *XMLoadFloat4x4(&m_matProjection));
}



void cCamera::Update(float deltaSce)
{
	LOG_MGR->AddLog("awetawetawetawe");

	XMStoreFloat4x4(&m_matView, XMMatrixInverse(nullptr, this->GetGameObject()->GetTransform()->GetFinalMatrix())); // ���� ���(������ ����)�� ������� ������̴� 
	XMStoreFloat4x4(&m_matVP, XMLoadFloat4x4(&m_matView) *XMLoadFloat4x4(&m_matProjection));
}



cCamera::~cCamera()
{



}
