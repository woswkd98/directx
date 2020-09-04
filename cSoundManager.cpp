#include "stdafx.h"
#include "cSoundManager.h"


void cSoundManager::Init()
{
	//사운드 시스템을 생성한다
	System_Create(&m_pSystem);
}

cSoundManager::cSoundManager()
{
}


cSoundManager::~cSoundManager()
{
}
