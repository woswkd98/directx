#include "stdafx.h"
#include "cMainLoop.h"



cMainLoop::cMainLoop()
{
	

}

cMainLoop::~cMainLoop()
{
	cMaterial::DeleteDefaultSRV();

	g_LightManager->Release();
	g_LightManager->ReleaseInstance();

	g_GizmoManager->Release();
	g_GizmoManager->ReleaseInstance();

	g_keyManager->ReleaseInstance();

	LOG_MGR->Release();
	LOG_MGR->ReleaseInstance();

	g_DeviceManager->Release();
}



void cMainLoop::RenderScene()
{
	
}


void cMainLoop::Init()
{
	LOG_MGR->Init(LOG_WINDOW | LOG_FILE, g_hWnd, "Dump");
	g_DeviceManager->Init();
	m_Timer.Init();
	//g_lightManager->Init();

	g_FBXFiles->Init();
	LOG_MGR->AddLog("½ÃÀÛ");

	cMaterial::CreateDefaultSRV();

	g_GizmoManager->Init(1, 1, 20, 20);

	g_SceneManager->InsertScene(0, new cScene());

	g_SceneManager->Init(0);

	g_keyManager->Init();

}
void cMainLoop::Update() 
{
	m_Timer.UpdateTime();
	g_SceneManager->Update(m_Timer.GetFrameDeltaSec());

}
void cMainLoop::Render() 
{
	g_SceneManager->Render();
}





