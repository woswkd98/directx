#pragma once
#include "cMainLoop.h"

class cGBuffer;
class cRigidbodyManager;
class cSceneManager;
class cLightComponent;
class cParticleEmitter;
class cScene
{

private:
	cRigidbodyManager * m_rigidbodyManager;
	list<cGameObject*>::iterator gameObject_Iterator;

	class cGameObjectHandle * m_pGameObjectHandle;
protected:

	cGameObject * MainCam = nullptr;
	cGameObject * pUICamObject = nullptr;
	cCamera * pMainCam = nullptr;
	cCamera * pUICam = nullptr;

	cLightComponent * pDirectionalLight = nullptr;

	
	//test 

	cParticleEmitter * p;

	cGameObject * player;

	cGBuffer * gBuffer;

	cCollider * test1;
	cCollider * test2;
private:
	float time = 0;
	float currentTime;
public:
	

	cCamera *GetMainCam()
	{
		if (pMainCam) return pMainCam;
		else
		{
			LOG_MGR->AddLog("카메라 없음");
			return nullptr;
		}
	}

	cCamera *GetUICam()
	{
		if (pUICam) return nullptr;
		else return nullptr;
	
	}

	void Render();

	void Release();

	void Init();
	void Update(float deltaTime);

	virtual void SceneInit();
	
	cScene();
	virtual ~cScene();
};

