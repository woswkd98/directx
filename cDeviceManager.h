#pragma once

#include "cSingletonBase.h"
class cGBuffer;
class cDeviceManager :public cSingletonBase<cDeviceManager>
{
private:
	
private:

	// 와면의 위치, 크기 
	float WINDOW_SIZEX;
	float WINDOW_SIZEY;
	float WINDOW_X;
	float WINDOW_Y;

	ID3D11Device* md3dDevice = nullptr;
	ID3D11DeviceContext* md3dImmediateContext = nullptr;

	IDXGISwapChain* mSwapChain = nullptr;
	ID3D11Texture2D* mDepthStencilBuffer = nullptr;
	ID3D11RenderTargetView* mRenderTargetView = nullptr;
	ID3D11DepthStencilView* mDepthStencilView = nullptr;
	D3D11_VIEWPORT mScreenViewport;
	D3D_DRIVER_TYPE md3dDriverType;
	bool mEnable4xMsaa;
	D3D_FEATURE_LEVEL featureLevel;
	UINT m4xMsaaQuality;

	ID3D11SamplerState* m_pSampPoint = nullptr;
	ID3D11SamplerState* m_pSampLinear = nullptr;

	ID3D11RasterizerState* pRSStateWire = nullptr;
	ID3D11RasterizerState* pRSStateSolid = nullptr;
private:
	cGBuffer * m_gBuffer = nullptr;
public:
	void SetUpSamplerState();
	void SetPSSamplerStateLinear(UINT startSlot, UINT samplersNum);
	void SetPSSamplerStatePoint(UINT startSlot, UINT samplersNum);
public:
	HRESULT Init();
	void Release();
	void ReSize(float sizeX, float sizeY);
	ID3D11Device* GetDevice() { return md3dDevice; }
	ID3D11DeviceContext* GetDC() { return md3dImmediateContext; }
	ID3D11RenderTargetView * GetTargetView() { return mRenderTargetView; }
	ID3D11DepthStencilView * GetDepthStencilView() { return mDepthStencilView; }
	IDXGISwapChain * GetSwapChain() { return mSwapChain; }

	ID3D11SamplerState* GetSamplePoint() { return m_pSampPoint; }
	ID3D11SamplerState* GetSampleLinear() { return m_pSampLinear; }

	ID3D11RasterizerState * GetRasterizeStateWire() { return pRSStateWire; }
	ID3D11RasterizerState * GetRasterizeStateSolid() { return pRSStateSolid; }
public:
	float GetSizeX() { return WINDOW_SIZEX; }
	float GetSizeY() { return WINDOW_SIZEY; }
public:
	void test() { md3dImmediateContext->OMSetRenderTargets(1, &mRenderTargetView, NULL); }
	
	cGBuffer * GetGBuffer() { return m_gBuffer; } // 버퍼포인터 가져옴
	
	cDeviceManager();
	~cDeviceManager();


};

#define g_DeviceManager cDeviceManager::GetInstance()