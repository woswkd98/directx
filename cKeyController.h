#pragma once
#include "cMonoBehavior.h"


class cKeyController :
	public cMonoBehavior
{
private:
	static cKeyController * g_pController; // 1인칭 자동차게임에서는 컨트롤러가 하나이다 따라서 컨트롤러 전역으로 하나만 만들었다
private:
	static vector<unsigned char> keySet;
	static vector<bool> downClick, upClick;

	float speed; // 자동차가 속도올릴때의 엑셀
	float steering; // 회전 구심점
	float angle; // 자동차의 핸들링 엥글
public:
	void Awake() override;
	void ControllerChange(); // 컨트롤러 바꾸기 

	void KeyUpdate(); // 키업데이트

	void AddKey(int key); // 키 추가
	 
	void SetAccelScale(float speed); // 엑셀 설정
	void HandlingScale(float angle); // 핸들링 각도 설정

private:
	bool IsStayDown(int nVKey); // 키입력 함수
	bool IsOnceDown(int nVKey);
	bool IsOnceUp(int nVKey);

	cKeyController();
	~cKeyController();
};

