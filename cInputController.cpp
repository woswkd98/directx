#include "stdafx.h"
#include "cInputController.h"


cInputController::cInputController()
{
	for (int i = 0; i < MAX_KEY_NUM; i++)
	{
		m_bDownCheck[i] = false;
		m_bUpCheck[i] = false;
	}

}


cInputController::~cInputController()
{
}
