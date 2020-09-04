#include "stdafx.h"
#include "cKeyController.h"




cKeyController *cKeyController::g_pController;
vector<unsigned char> cKeyController::keySet;
vector<bool> cKeyController::downClick;
vector<bool> cKeyController::upClick;

void cKeyController::Awake()
{
	keySet.push_back('W');
	keySet.push_back('A');
	keySet.push_back('D');
	keySet.push_back('S');
	
	downClick.resize(keySet.size(), 0);
	upClick.resize(keySet.size(), 0);
}

void cKeyController::ControllerChange()
{
	if (g_pController->GetActive()) { LOG_MGR->AddLog("%s, %s input controller2개", g_pController->GetGameObject()->GetName(), this->GetGameObject()->GetName()); }
	g_pController = this;
}

void cKeyController::KeyUpdate()
{
	//if(IsStayDown(keySet[0])) this->gameObject->TranslationDir(speed, cTransform::forward);
	//if (IsStayDown(keySet[1])) this->gameObject->TranslationDir(-speed, cTransform::back);
	// 나머지 핸들링도 추가
}

void cKeyController::AddKey(int key)
{
	keySet.push_back(key);
	downClick.push_back(0);
	upClick.push_back(0);
}

void cKeyController::SetAccelScale(float speed)
{
	this->speed = speed;
}

void cKeyController::HandlingScale(float angle)
{
	this->angle = angle;
}



cKeyController::cKeyController()
{
}


cKeyController::~cKeyController()
{
}
bool cKeyController::IsStayDown(int nVKey)
{
	return (GetAsyncKeyState(nVKey) & 0x8000);
}

bool cKeyController::IsOnceDown(int nVKey)
{
	if (GetAsyncKeyState(nVKey) & 0x8000)
	{
		if (downClick[nVKey] == false)
		{
			downClick[nVKey] = true;
			return true;
		}
	}
	else downClick[nVKey] = false;

	return false;
}

bool cKeyController::IsOnceUp(int nVKey)
{
	if (GetAsyncKeyState(nVKey) & 0x8000)
	{
		if (upClick[nVKey] == false)
		{
			upClick[nVKey] = true;
		}
	}
	else
	{
		if (upClick[nVKey])
		{
			upClick[nVKey] = false;
			return true;
		}
	}

	return false;
}