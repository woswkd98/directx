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
	void Init(); // ��ũ��Ʈ �����̹Ƿ� �ǵ帱�� �ִ°� �ʱ�ȭ���� ��ü�����ÿ��� �̴�
	void Update();
	void Render();


	
	
	cMainLoop();
	virtual ~cMainLoop();


};

/*
resterizing

D3D11_RASTERIZER_DESC wireframeDesc;
ZeroMemory(&wireframeDesc, sizeof(D3D11_RASTERIZER_DESC));
wireframeDesc.FillMode = D3D11_FILL_WIREFRAME; // ä��� ���
wireframeDesc.CullMode = D3D11_CULL_BACK; // �ø����
wireframeDesc.FrontCounterClockwise = false; // �ð���� �ݽð���� ���ϴ� �÷���

wireframeDesc.DepthClipEnable = true;

HR(md3dDevice->CreateRasterizerState(&wireframeDesc, &mWireframeRS));

*/