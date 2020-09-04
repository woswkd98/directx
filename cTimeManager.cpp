#include "stdafx.h"
#include "cTimeManager.h"
#include <stdio.h>


//timeGetTime �� �̿��ϱ� ���� ����� ���̺귯��
#include <MMSystem.h>
#pragma comment(lib, "winmm.lib")


cTimeManager::cTimeManager()
{
}


cTimeManager::~cTimeManager()
{
}

HRESULT cTimeManager::Init(void)
{
	//���� Ÿ�̸� ���� ����
	//QueryPerformanceFrequency �Լ��� �ʴ� ī��Ʈ ������ ���� ��´�
	//���� Ÿ�̸� ���� �ȴٸ� TRUE �ȵȴٸ� FALSE
	if (QueryPerformanceFrequency((LARGE_INTEGER*)&m_nPriodTime))
	{
		m_bHardWare = true;

		//1 ī��Ʈ�� �ʰ� �󸶳� �ǳ�?
		m_TimeScaleSec = 1.0 / m_nPriodTime;

		//�ʱ�ȭ ������ �ð��� LastTime����...
		QueryPerformanceCounter((LARGE_INTEGER*)&m_nLastTime);
	}
	else
	{
		//���� Ÿ�̸� �������� ������...
		m_bHardWare = false;

		//1000 �и������� ������ �ð����� ��´�
		//timeGetTime �� GetTickCount ���� ���� �����ϴ�
		m_nLastTime = timeGetTime();

		//1 ī��Ʈ�� �ʰ� �󸶳� �ǳ�?
		m_TimeScaleSec = 1.0 / 1000.0;
	}

	m_TotalSec = 0.0f;
	m_FrameCountSec = 0.0f;
	m_FramePerSec = 0;
	m_FrameCount = 0;

	return S_OK;
}

void cTimeManager::Release(void)
{

}

//�� ������Ʈ �����ش�
void cTimeManager::UpdateTime(float frameLock /*= 0.0f*/)
{
	//���� �ð��� ��´�
	if (m_bHardWare)
		QueryPerformanceCounter((LARGE_INTEGER*)&m_nCurTime);
	else
		m_nCurTime = timeGetTime();

	//���� �ð����� ������ ���
	m_nDeltaTime = m_nCurTime - m_nLastTime;

	//�������� ������ ���� �ʽð� ���
	m_FrameDeltaSec = m_nDeltaTime * m_TimeScaleSec;

	//������ ���� �����Ѵٸ�...
	if (frameLock > 0.0f)
	{
		float fLockTime = (1.0f / frameLock);

		while (m_FrameDeltaSec < fLockTime)
		{
			//������ �ð�
			if (m_bHardWare)
				QueryPerformanceCounter((LARGE_INTEGER*)&m_nCurTime);
			else
				m_nCurTime = timeGetTime();

			//���� �ð����� ������ ���
			m_nDeltaTime = m_nCurTime - m_nLastTime;

			//�������� ������ ���� �ʽð� ���
			m_FrameDeltaSec = m_nDeltaTime * m_TimeScaleSec;
		}
	}

	//���� �ð� ����
	m_nLastTime = m_nCurTime;

	//�� ���� �ð� ����
	m_TotalSec += m_FrameDeltaSec;

	//������ �� ����
	m_FrameCountSec += m_FrameDeltaSec;

	//1�ʰ� ��������...
	if (m_FrameCountSec >= 1.0)
	{
		m_FramePerSec = m_FrameCount;
		m_FrameCount = 0;
		m_FrameCountSec -= 1.0;
	}

	m_FrameCount++;

}

//Ÿ�� ������ ����Ѵ�
void cTimeManager::DrawTimeInfo(HDC hdc)
{
	char szTemp[128];
	wsprintf(szTemp, "FPS : %d", m_FramePerSec);
	DrawShadowText(hdc, 0, 0, szTemp, 0x0000ffff);

	sprintf_s(szTemp, "FrameDeltaSec : %.2f", m_FrameDeltaSec);
	DrawShadowText(hdc, 0, 20, szTemp, 0x0000ffff);

	sprintf_s(szTemp, "TotalDeltaSec : %.2f", m_TotalSec);
	DrawShadowText(hdc, 0, 40, szTemp, 0x0000ffff);
}

void cTimeManager::DrawShadowText(HDC hdc, int x, int y, char* szText, COLORREF textColor)
{
	//Text ����� �������� 
	SetBkMode(hdc, TRANSPARENT);

	SetTextColor(hdc, 0x00000000);
	TextOut(hdc, x + 1, y + 1, szText, strlen(szText));

	SetTextColor(hdc, textColor);
	TextOut(hdc, x, y, szText, strlen(szText));

	SetTextColor(hdc, 0x00000000);

	//Text ����������
	SetBkMode(hdc, OPAQUE);
}