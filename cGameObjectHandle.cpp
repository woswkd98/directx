#include "stdafx.h"
#include "cGameObjectHandle.h"

map<string, list<Event>*> cGameObjectHandle::g_mapEvents;


list<cGameObject*> cGameObjectHandle::g_rootGameObjects;
list<cGameObject*> cGameObjectHandle::g_listStoredEnableGameObjects;
list<cGameObject*> cGameObjectHandle::g_listStoredDisableGameObjects;
list<cGameObject*> cGameObjectHandle::g_listStoredCreateGameObjects;
list<cGameObject*> cGameObjectHandle::g_listStoredDeleteGameObjects;

void cGameObjectHandle::AddEvent(string str, Event event)
{
	map<string, list<Event>*>::iterator it = g_mapEvents.find(str);

	if (it == g_mapEvents.end())
	{
		list<Event> * p = new list<Event>();

		g_mapEvents.insert(make_pair(str, p));

		p->push_back(event);
		return;
	}
	else
	{
		it->second->push_back(event);
	}
}

void cGameObjectHandle::AddGameObject(cGameObject* p)
{
	g_rootGameObjects.push_back(p);
	g_listStoredEnableGameObjects.push_back(p);
	g_listStoredCreateGameObjects.push_back(p);
}
cGameObjectHandle::cGameObjectHandle()
{

}


cGameObjectHandle::~cGameObjectHandle()
{
}
void cGameObjectHandle::Start()
{
	it = g_listStoredCreateGameObjects.begin();
	for (it; it != g_listStoredCreateGameObjects.end(); ++it)
	{
		(*it)->Awake();
	}

	it = g_listStoredCreateGameObjects.begin();
	for (it; it != g_listStoredCreateGameObjects.end(); ++it)
	{
		(*it)->Start();
		(*it)->SetActive(true);
	}
	g_listStoredCreateGameObjects.clear();
}

void cGameObjectHandle::OnEnable()
{

	it = g_listStoredEnableGameObjects.begin();
	for (it; it != g_listStoredEnableGameObjects.end(); ++it)
	{

		(*it)->OnEnable();
	}
	g_listStoredEnableGameObjects.clear();

}
void cGameObjectHandle::FixedUpdate()
{

	
	it = g_rootGameObjects.begin();
	for (it; it != g_rootGameObjects.end(); ++it)
	{
		(*it)->FixedUpdate();

	}

	it = g_rootGameObjects.begin();
	for (it; it != g_rootGameObjects.end(); ++it)
	{
		(*it)->GetTransform()->UpdateTransform();
	}
}

void cGameObjectHandle::LateUpdate()
{

	it = g_rootGameObjects.begin();
	for (it; it != g_rootGameObjects.end(); ++it)
	{
		(*it)->LateUpdate();
	
	}
}
void cGameObjectHandle::Update(float dt)
{

	
	it = g_rootGameObjects.begin();
	for (it; it != g_rootGameObjects.end(); ++it)
	{
		(*it)->Update(dt);

	}


}
void  cGameObjectHandle::Disable()
{

	it = g_listStoredDisableGameObjects.begin();
	for (it; it != g_listStoredDisableGameObjects.end(); ++it)
	{
		(*it)->OnDisable();
	
	}
	g_listStoredDisableGameObjects.clear();
}
void  cGameObjectHandle::Destroy()
{
	it = g_listStoredDeleteGameObjects.begin();
	
	for (it; it != g_listStoredDeleteGameObjects.end(); ++it)
	{
		
		find(this->g_rootGameObjects.begin(), this->g_rootGameObjects.end(), (*it));
		cMonoBehavior::DestroyGameObject((*it));
	}
	g_listStoredDeleteGameObjects.clear();
}

void  cGameObjectHandle::Release()
{
	g_listStoredDisableGameObjects = g_rootGameObjects;
	g_listStoredDeleteGameObjects = g_rootGameObjects;

	Destroy();

	g_rootGameObjects.clear();
	g_listStoredEnableGameObjects.clear();
	g_listStoredDisableGameObjects.clear();
	g_listStoredCreateGameObjects.clear();
	g_listStoredDeleteGameObjects.clear();

}

list<cGameObject*> &   cGameObjectHandle::GetCurrentGameObjects() { return g_rootGameObjects; }

void   cGameObjectHandle::DestroyObject(cGameObject * p)
{
	this->g_listStoredDisableGameObjects.push_back(p);
	this->g_listStoredDeleteGameObjects.push_back(p);
}