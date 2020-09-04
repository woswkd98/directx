#pragma once

class cTimeManager 
{
private:
	bool	m_bHardWare;	   //고성능 타이머 지원 여부
							   
	__int64 m_nPriodTime;	   //1초에 몇번 카운트 하는가?
	__int64 m_nLastTime;	   //마지막 프레임 시간
	__int64 m_nCurTime;		   //현재 시간
	__int64 m_nDeltaTime;	   //이전 프레임과 지난 시간
							   
	double	m_TimeScaleSec;	   //1카운트 당초.
	double	m_FrameDeltaSec;   //한프레임 경과 초.
	double	m_TotalSec;		   //프로그램 시작후 경과 시간
							   
	double	m_FrameCountSec;   //프레임 갱신 시간
	DWORD	m_FrameCount;	   //프레임 카운트
	DWORD	m_FramePerSec;	   //초당 프레임

private:
	ID3D11Buffer * m_timeCB; // 시간 CB
public:
	cTimeManager();
	~cTimeManager();

	HRESULT Init(void);
	void Release(void);

	//매 업데이트 물려준다
	void UpdateTime(float frameLock = 0.0f);

	//타임 정보를 출력한다
	void DrawTimeInfo(HDC hdc);

	//프레임간의 지난 초시간을 얻는다
	double GetFrameDeltaSec(void) { return m_FrameDeltaSec; }

	void DrawShadowText(HDC hdc, int x, int y, char* szText, COLORREF textColor);
};

