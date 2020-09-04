#include "stdafx.h"
#include "cGameObject.h"
#include "cMonoBehavior.h"
#include "cMesh_Node.h"
#include "cScene.h"
#include "cPhysicsInfo.h"
#include "cGameObjectHandle.h"
ULONG cGameObject::Count = 0;

cGameObject::cGameObject()
{
	Count++;
	LOG_MGR->AddLog("%d", Count);
	pTransform = new cTransform();
	pTransform->SetGameObject(this);
}


cGameObject::~cGameObject()
{
	Count--;
	LOG_MGR->AddLog("%d", Count);
	delete this->pTransform;
}
void cGameObject::Awake()
{
	for (int i = 0; i < pTransform->GetListComponents().size(); ++i)
	{
		pTransform->GetListComponents()[i]->Awake();
	}
	if (pTransform->child) pTransform->child->GetGameObject()->Awake();
	if (pTransform->nextSibling) pTransform->nextSibling->GetGameObject()->Awake();

}
void cGameObject::SetActive(bool active)
{
	if (m_bActive == false && active == true)
	{
		// 게임오브젝트가 켜지면 cMonoBehavior 를 찾아서 Enable시킨다
		for (int i = 0; i < pTransform->GetListComponents().size(); ++i)
		{
			pTransform->GetListComponents()[i]->SetActive(active);
			
		}
		if (pTransform->child) pTransform->child->GetGameObject()->SetActive(active);
		if (pTransform->nextSibling) pTransform->nextSibling->GetGameObject()->SetActive(active);

		m_bActive = active;
		
		// 자식이 있으면 자식의 게임오브젝트 안에 있는 컴포넌트의 active를 다 true 
	}
	else if (m_bActive == true && active == false)
	{
		// 게임오브젝트가 꺼지면 cMonoBehavior 를 찾아서 Disable시킨다
		for (int i = 0; i < pTransform->GetListComponents().size(); ++i)
		{
			pTransform->GetListComponents()[i]->SetActive(active);

		}
		if (pTransform->child) pTransform->child->GetGameObject()->SetActive(active);
		if (pTransform->nextSibling) pTransform->nextSibling->GetGameObject()->SetActive(active);

		m_bActive = active;
	
		// 자식이 있으면 자식의 게임오브젝트 안에 있는 컴포넌트의 active를 다 false 
	}
}

void cGameObject::BroadCastFunction(string str)
{
	for (int i = 0; i < pTransform->GetListComponents().size(); ++i)
	{
		map<string, Event> & ref = pTransform->GetListComponents()[i]->GetFunctions();

		map<string, Event>::iterator it = ref.find(str);
		if (it != ref.end())
		{
			it->second();
			pTransform->GetListComponents()[i]->BroadCastFunction(str);
		}
	}

	if (pTransform->child) pTransform->child->GetGameObject()->BroadCastFunction(str);
	if (pTransform->nextSibling) pTransform->nextSibling->GetGameObject()->BroadCastFunction(str);
}


void cGameObject::Start()
{
	pTransform->Start();

	for (int i = 0; i < pTransform->GetListComponents().size(); ++i)
	{
		pTransform->GetListComponents()[i]->Start();
	}
	if (pTransform->child) pTransform->child->GetGameObject()->Start();
	if (pTransform->nextSibling) pTransform->nextSibling->GetGameObject()->Start();
}



void cGameObject::Update(float deltaSec)
{
	if (!pTransform->GetActive()) return;

	for (int i = 0; i < pTransform->GetListComponents().size(); ++i)
	{
		pTransform->GetListComponents()[i]->Update(deltaSec);
	}

	if (pTransform->child) pTransform->child->GetGameObject()->Update(deltaSec);
	if (pTransform->nextSibling) pTransform->nextSibling->GetGameObject()->Update(deltaSec);

}


void cGameObject::FixedUpdate()
{
	if (!pTransform->GetActive()) return;

	for (int i = 0; i < pTransform->GetListComponents().size(); ++i)
	{
		pTransform->GetListComponents()[i]->FixedUpdate( );
	}

	if (pTransform->child) pTransform->child->GetGameObject()->FixedUpdate();
	if (pTransform->nextSibling) pTransform->nextSibling->GetGameObject()->FixedUpdate();

}

void cGameObject::LateUpdate() 
{
	if (!pTransform->GetActive()) return;
	for (int i = 0; i < pTransform->GetListComponents().size(); ++i)
	{
		pTransform->GetListComponents()[i]->LateUpdate();
	}

	if (pTransform->child) pTransform->child->GetGameObject()->LateUpdate();
	if (pTransform->nextSibling) pTransform->nextSibling->GetGameObject()->LateUpdate();
}

void cGameObject::OnEnable()
{
	for (int i = 0; i < pTransform->GetListComponents().size(); ++i)
	{
		pTransform->GetListComponents()[i]->OnEnable();
	}

	if (pTransform->child) pTransform->child->GetGameObject()->OnEnable();
	if (pTransform->nextSibling) pTransform->nextSibling->GetGameObject()->OnEnable();
}

void cGameObject::OnDisable()
{
	for (int i = 0; i < pTransform->GetListComponents().size(); ++i)
	{
		pTransform->GetListComponents()[i]->OnDisable();
	}
	if (pTransform->child) pTransform->child->GetGameObject()->OnDisable();
	if (pTransform->nextSibling) pTransform->nextSibling->GetGameObject()->OnDisable();

	//if (root != this)
	//{
	//	static_cast<cGameObject*>(root)->GetPhysicsInfo()->Init(); // 만약 게임오브젝트가 삭제되면 질량, 무게중심등을 재 계산
	//}
}

void cGameObject::RenderFrame(ID3D11DeviceContext * pDC, vector<ID3D11Buffer*>* VertexInfo, vector<ID3D11Buffer*>* indexInfo)
{
	if (pMesh_Node)
	{
		pMesh_Node->RenderFrame(pDC, VertexInfo, indexInfo);
	}

	if (pTransform->child) pTransform->GetGameObject()->RenderFrame(pDC, VertexInfo, indexInfo);
	if (pTransform->nextSibling) pTransform->GetGameObject()->RenderFrame(pDC, VertexInfo, indexInfo);
}
void cGameObject::Destroy()
{
	LOG_MGR->AddLog("%s", this->GetName().c_str());
	cGameObjectHandle * p = new cGameObjectHandle();
	p->DestroyObject(this);
	delete p;
}