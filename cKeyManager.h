#pragma once



#define MAX_KEY_NUM 256
#include "cSingletonBase.h"
class cKeyManager : public cSingletonBase<cKeyManager>
{
private:
	bool m_bDownCheck[MAX_KEY_NUM];
	bool m_bUpCheck[MAX_KEY_NUM];

public:
	cKeyManager(void);

public:

	~cKeyManager();

	HRESULT Init(void);
	void Release(void);

	bool IsStayDown(int nVKey);
	bool IsOnceDown(int nVKey);
	bool IsOnceUp(int nVKey);
};

#define g_keyManager cKeyManager::GetInstance()