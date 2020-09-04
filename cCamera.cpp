#include "stdafx.h"
#include "cCamera.h"


void cCamera::Awake() 
{

	// 카메라 의사코드 
	/*

	Xaxis.x             Yaxis.x            Zaxis.x       0

	Xaxis.y             Yaxis.y            Zaxis.y       0

	Xaxis.z             Yaxis.z            Zaxis.z       0

	- dot(Xaxis, eye) - dot(Yaxis, eye) - dot(Zaxis, eye)  1
	*/

	LOG_MGR->AddLog(__FUNCTION__);

	LOG_MGR->AddLog("카메라 생성");


	//기본화각 60 도
	this->m_fov = 60.0f;

	//기본 Near
	this->m_camNear = 0.3f;

	//기본 Far
	this->m_camFar = 1000.0f;

	this->m_sizeX = 800;
	this->m_sizeY = 600;

	this->name = "cCamera";
	LOG_MGR->AddLog("%f", XMConvertToRadians(m_fov));
	LOG_MGR->AddLog("%f,%f", m_sizeX, m_sizeY);
	// 기본적인 설정
	XMStoreFloat4x4(
		&m_matPerspective,
		XMMatrixPerspectiveFovLH(
			XMConvertToRadians(m_fov),
			static_cast<float>(m_sizeX / m_sizeY)
			, m_camNear
			, m_camFar));
	m_matProjection = m_matPerspective; // 기본적으로 perspective로 설정했다
	XMStoreFloat4x4(&m_matView, XMMatrixIdentity());

	XMStoreFloat4x4(&m_matView, XMMatrixInverse(nullptr, this->GetGameObject()->GetTransform()->GetFinalMatrix())); // 월드 행렬(스케일 없는)의 역행렬은 뷰행렬이다 
	XMStoreFloat4x4(&m_matVP, XMLoadFloat4x4(&m_matView) *XMLoadFloat4x4(&m_matProjection));
}



void cCamera::Update(float deltaSce)
{
	LOG_MGR->AddLog("awetawetawetawe");

	XMStoreFloat4x4(&m_matView, XMMatrixInverse(nullptr, this->GetGameObject()->GetTransform()->GetFinalMatrix())); // 월드 행렬(스케일 없는)의 역행렬은 뷰행렬이다 
	XMStoreFloat4x4(&m_matVP, XMLoadFloat4x4(&m_matView) *XMLoadFloat4x4(&m_matProjection));
}



cCamera::~cCamera()
{



}
