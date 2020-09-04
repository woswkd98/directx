#pragma once
#include "cMonoBehavior.h"

#define MAX_KEY_NUM 256
class cInputController : // 
	public cMonoBehavior
{


private:
	// Ű üŷ �÷���
	bool m_bDownCheck[MAX_KEY_NUM];
	bool m_bUpCheck[MAX_KEY_NUM];

public:
	/*template<class T> //Ŭ���� ����
	
	void IsOnceKeyDown(int key, T * pClass, void T::*Func)
	{
		if (GetAsyncKeyState(nVKey) & 0x8000)
		{
			if (m_bDownCheck[nVKey] == false)
			{
				m_bDownCheck[nVKey] = true;
				(pClass->*Func)(); // �Լ� ȣ��
			}
		}
		else m_bDownCheck[nVKey] = false;
	}

	template<class T> //Ŭ���� ���� 
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
				(pClass->*Func)(); // �Լ� ȣ��
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

