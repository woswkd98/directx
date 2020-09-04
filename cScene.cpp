#include "stdafx.h"
#include "cScene.h"
#include "cGBuffer.h"
#include "cTransform.h"
#include "cGameObjectHandle.h"
#include "cRigidbodyManager.h"
#include "cParticleEmitter.h"

void cScene::Init()
{
	
	m_pGameObjectHandle = new cGameObjectHandle();
	
	g_GizmoManager->Init(1,1,40,40);

	//m_pGameObjectHandle->AddEvent(__FUNCTION__, bind(&cScene::Init, this));
	//m_rigidbodyManager = new cRigidbodyManager();
	//m_rootGameObjects.reserve(10);
	
	MainCam = new cGameObject();
	
	MainCam->SetName("MainCamGameObject");

	pMainCam = MainCam->AddComponent<cCamera>();


	m_pGameObjectHandle->AddGameObject(MainCam);

	MainCam->AddComponent<cCubeMove>();

	if (!pMainCam) LOG_MGR->AddLog("카메라 없네");

	MainCam->SetName("MainCam");
	
	MainCam->GetTransform()->SetWorldPosition(0, 5, 0);
	MainCam->GetTransform()->SetWorldRotate(-30, 70, 0);

	//p = new cParticleEmitter(nullptr);

	//p->InitBuffer();
	//p->Start();
	//p->SetTextures(L"texture/Particle.png", L"png", "Point", cMaterial::diffuse);



	g_GizmoManager->AddBox(new cTransform());
}

void cScene::Update(float deltaTime)
{
	time += deltaTime;

	m_pGameObjectHandle->Start();

	m_pGameObjectHandle->OnEnable();

	m_pGameObjectHandle->FixedUpdate();
	
	m_pGameObjectHandle->Update(deltaTime);

	if (g_keyManager->IsOnceDown(VK_SPACE))
	{
		LOG_MGR->AddLog("%f,%f,%f", pMainCam->GetEyePos().x,
			pMainCam->GetEyePos().y, pMainCam->GetEyePos().z);
	}

	

	m_pGameObjectHandle->LateUpdate();
	
	m_pGameObjectHandle->Disable();
	
	m_pGameObjectHandle->Destroy();

	//p->Update(deltaTime);

}

void cScene::SceneInit()
{
}


void cScene::Render()
{

	//p->Render(pMainCam);


}

void cScene::Release()
{
	
	this->m_pGameObjectHandle->Release();
	delete m_pGameObjectHandle;
}

cScene::cScene()
{
}


cScene::~cScene()
{
}
