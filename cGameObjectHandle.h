#pragma once
#include "cGameObject.h"
class cGameObjectHandle
{
public:


private: 

	static map<string, list<Event>*> g_mapEvents;

	static list<cGameObject*> g_rootGameObjects;
	static list<cGameObject*> g_listStoredEnableGameObjects;
	static list<cGameObject*> g_listStoredDisableGameObjects;
	static list<cGameObject*> g_listStoredCreateGameObjects;
	static list<cGameObject*> g_listStoredDeleteGameObjects;

	list<cGameObject*>::iterator it;
public:

	void AddEvent(string str, Event event);

	void AddGameObject(cGameObject* p);

	void Start();

	void OnEnable();

	void FixedUpdate();

	void LateUpdate();

	void Update(float dt);

	void Disable();

	void Destroy();

	void Release();

	list<cGameObject*> & GetCurrentGameObjects();

	void DestroyObject(cGameObject * p);

	cGameObjectHandle();
	~cGameObjectHandle();


};
