#pragma once
#include "cMonoBehavior.h"

#define MAX_KEY_NUM 256
class cInputController : // 
	public cMonoBehavior
{


private:
	// 키 체킹 플래그
	bool m_bDownCheck[MAX_KEY_NUM];
	bool m_bUpCheck[MAX_KEY_NUM];

public:
	/*template<class T> //클래스 형식
	
	void IsOnceKeyDown(int key, T * pClass, void T::*Func)
	{
		if (GetAsyncKeyState(nVKey) & 0x8000)
		{
			if (m_bDownCheck[nVKey] == false)
			{
				m_bDownCheck[nVKey] = true;
				(pClass->*Func)(); // 함수 호출
			}
		}
		else m_bDownCheck[nVKey] = false;
	}

	template<class T> //클래스 형식 
	void IsOnceKeyUp(int key, T * pClass, void T::*Func)
	{
		if (GetAsyncKeyState(nVKey) & 0x8000)
		{
			if (m_bUpCheck[nVKey] == false)
			{
				m_bUpCheck[nVKey] = true;
			}
		}
		else
		{
			if (m_bUpCheck[nVKey])
			{
				m_bUpCheck[nVKey] = false;
				(pClass->*Func)(); // 함수 호출
				return true;
			}
		}
		return false;
	}
	*/


public:
	cInputController();
	~cInputController();
};

