#pragma once
#include "cSceneManager.h"


class cMainLoop
{
private:
	
private:
	cTimeManager m_Timer;
protected:
	// managers

protected:
	cScene * Scene1;

private :


public:


public:
	
	void RenderScene();
	void Init(); // 스크립트 형식이므로 건드릴수 있는건 초기화때의 객체생성시에만 이다
	void Update();
	void Render();


	
	
	cMainLoop();
	virtual ~cMainLoop();


};

/*
resterizing

D3D11_RASTERIZER_DESC wireframeDesc;
ZeroMemory(&wireframeDesc, sizeof(D3D11_RASTERIZER_DESC));
wireframeDesc.FillMode = D3D11_FILL_WIREFRAME; // 채우는 모드
wireframeDesc.CullMode = D3D11_CULL_BACK; // 컬링모드
wireframeDesc.FrontCounterClockwise = false; // 시계방향 반시계방향 정하는 플래그

wireframeDesc.DepthClipEnable = true;

HR(md3dDevice->CreateRasterizerState(&wireframeDesc, &mWireframeRS));

*/