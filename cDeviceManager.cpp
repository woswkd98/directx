#include "stdafx.h"
#include "cDeviceManager.h"
#include "cGBuffer.h"


cDeviceManager::cDeviceManager()
{

}


cDeviceManager::~cDeviceManager()
{
}
void cDeviceManager::SetUpSamplerState()
{
	

	HRESULT hr;

	// Create the two samplers
	D3D11_SAMPLER_DESC samDesc;
	ZeroMemory(&samDesc, sizeof(samDesc));
	samDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR; // 필터 형태 여기선 선형 
	samDesc.AddressU = samDesc.AddressV = samDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP; //  uvw의 값이 0~1 이넘어간 값이 들어갈때 적용되는 설정
	samDesc.MaxAnisotropy = 1; // D3D11_FILTER_ANISOTROPIC 또는 D3D11_FILTER_COMPARISON_ANISOTROPIC가 필터에 지정된 경우에 사용되는 값을 클램핑.  유효한 값은 1과 16 사이입니다. 
	samDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS; // 기존의 샘플링 데이터에 대해 데이터를 샘플링 비교하는 기능
	samDesc.MaxLOD = D3D11_FLOAT32_MAX; // 밈뱁범위의 상단에 대한 값 이값은 가장큰값이다
	hr = md3dDevice->CreateSamplerState(&samDesc, &m_pSampLinear);
	if (hr == E_FAIL) LOG_MGR->AddLog("선형 샘플러 세팅안됨"); 
	samDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	hr = md3dDevice->CreateSamplerState(&samDesc, &m_pSampPoint);
	if (hr == E_FAIL) LOG_MGR->AddLog("포인트 샘플러 세팅안됨");
}

void cDeviceManager::SetPSSamplerStateLinear(UINT startSlot, UINT samplersNum)
{
	md3dImmediateContext->PSSetSamplers(startSlot, samplersNum, &m_pSampLinear);
}

void cDeviceManager::SetPSSamplerStatePoint(UINT startSlot, UINT samplersNum)
{
	md3dImmediateContext->PSSetSamplers(startSlot, samplersNum, &m_pSampPoint);
}

HRESULT cDeviceManager::Init()
{
	WINDOW_SIZEX = 800;
	WINDOW_SIZEY = 600;
	WINDOW_X = 0;
	WINDOW_Y = 0;

	UINT createDeviceFlags = 0;
#if defined(DEBUG) || defined(_DEBUG)  
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
	HRESULT hr = D3D11CreateDevice(
		0,                
		D3D_DRIVER_TYPE_HARDWARE,
		0,                
		createDeviceFlags,
		0, 0,             
		D3D11_SDK_VERSION,
		&md3dDevice,
		&featureLevel,
		&md3dImmediateContext);
	if (FAILED(hr))
	{
		MessageBox(0, "D3D11CreateDevice Failed.", 0, 0);
		return E_FAIL;
	}

	if (featureLevel != D3D_FEATURE_LEVEL_11_0)
	{
		MessageBox(0, ", 0", 0,0);
		return E_FAIL;
	}
	
	md3dDevice->CheckMultisampleQualityLevels(
		DXGI_FORMAT_R8G8B8A8_UNORM, 4, &m4xMsaaQuality);

	DXGI_SWAP_CHAIN_DESC sd;
	sd.BufferDesc.Width = WINDOW_SIZEX;
	sd.BufferDesc.Height = WINDOW_SIZEY;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.BufferCount = 1;
	sd.OutputWindow = g_hWnd;
	sd.Windowed = true;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	sd.Flags = 0;

	IDXGIDevice* dxgiDevice = 0;
	md3dDevice->QueryInterface(__uuidof(IDXGIDevice), (void**)&dxgiDevice);

	IDXGIAdapter* dxgiAdapter = 0;
	dxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&dxgiAdapter);

	IDXGIFactory* dxgiFactory = 0;
	dxgiAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&dxgiFactory);

	dxgiFactory->CreateSwapChain(md3dDevice, &sd, &mSwapChain);
	
	SAFE_RELEASE(dxgiDevice);
	SAFE_RELEASE(dxgiAdapter);
	SAFE_RELEASE(dxgiFactory);
	m_gBuffer = new cGBuffer();
	ReSize(WINDOW_SIZEX, WINDOW_SIZEY);

	//rasterize 세팅
	

	D3D11_RASTERIZER_DESC wireframeDesc;
	ZeroMemory(&wireframeDesc, sizeof(D3D11_RASTERIZER_DESC));
	wireframeDesc.FillMode = D3D11_FILL_WIREFRAME; // 채우는 모드
	wireframeDesc.CullMode = D3D11_CULL_BACK; // 백페이스 컬링 
	wireframeDesc.FrontCounterClockwise = false; // 시계방향 계산
	wireframeDesc.DepthClipEnable = true;

	this->GetDevice()->CreateRasterizerState(&wireframeDesc, &pRSStateWire);

	D3D11_RASTERIZER_DESC solidFrameDesc;
	ZeroMemory(&solidFrameDesc, sizeof(D3D11_RASTERIZER_DESC));
	solidFrameDesc.FillMode = D3D11_FILL_SOLID; // 채우는 모드
	solidFrameDesc.CullMode = D3D11_CULL_BACK; // 백페이스 컬링 
	solidFrameDesc.FrontCounterClockwise = false; // 시계방향 계산
	solidFrameDesc.DepthClipEnable = true;

	this->GetDevice()->CreateRasterizerState(&solidFrameDesc, &pRSStateSolid);

	return S_OK;
}
void cDeviceManager::Release()
{
	/*
	
	ID3D11Device* md3dDevice = nullptr;
	ID3D11DeviceContext* md3dImmediateContext = nullptr;

	IDXGISwapChain* mSwapChain = nullptr;
	ID3D11Texture2D* mDepthStencilBuffer = nullptr;
	ID3D11RenderTargetView* mRenderTargetView = nullptr;
	ID3D11DepthStencilView* mDepthStencilView = nullptr;
	
	ID3D11SamplerState* m_pSampPoint = nullptr;
	ID3D11SamplerState* m_pSampLinear = nullptr;

	ID3D11RasterizerState* pRSStateWire = nullptr;
	ID3D11RasterizerState* pRSStateSolid = nullptr;
	*/

	m_gBuffer->Relase();
	
	delete m_gBuffer;

	SAFE_RELEASE(mRenderTargetView);
	
	SAFE_RELEASE(mSwapChain);
	SAFE_RELEASE(mDepthStencilBuffer);
	SAFE_RELEASE(mDepthStencilView);
	
	SAFE_RELEASE(m_pSampLinear);
	SAFE_RELEASE(m_pSampPoint);

	SAFE_RELEASE(pRSStateWire);
	SAFE_RELEASE(pRSStateSolid);

	SAFE_RELEASE(md3dImmediateContext);
	SAFE_RELEASE(md3dDevice);
}

void cDeviceManager::ReSize(float sizeX, float sizeY)
{

	/*
	* 힙에 할당된, 초기화되지 않은 메모리는 0xCDCDCDCD로 설정된다.

	* 스택에 할단된, 초기화되지 않은 메모리는 0xCCCCCCCC로 설정된다.

	* 해제된 힙 메모리는 0xFEEEFEEE로 설정한다. (이후 재할당되면 다시 0xCDCDCDCD가 된다)

	메모리가 해제되는 방식에 따라서는 해제된 메모리가 0xDDDDDDDD로 설정될 수도 있다.

	* 힙에 할당된 메모리의 첫 부분과 끝 부분은 0xFDFDFDFD로 설정된다.

	*/


	//SAFE_RELEASE(mRenderTargetView);
	//SAFE_RELEASE(mDepthStencilView);
	//SAFE_RELEASE(mDepthStencilBuffer);

	WINDOW_SIZEX = sizeX; 
	WINDOW_SIZEY = sizeY;
	mSwapChain->ResizeBuffers(1, WINDOW_SIZEX, WINDOW_SIZEY, DXGI_FORMAT_R8G8B8A8_UNORM, 0);
	ID3D11Texture2D* backBuffer;
	mSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer));
	md3dDevice->CreateRenderTargetView(backBuffer, 0, &mRenderTargetView);
	SAFE_RELEASE(backBuffer);

	
	// Create depth stencil texture
	D3D11_TEXTURE2D_DESC descDepth;
	ZeroMemory(&descDepth, sizeof(descDepth));
	descDepth.Width = WINDOW_SIZEX;
	descDepth.Height = WINDOW_SIZEY;
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;
	HRESULT hr;
	hr = md3dDevice->CreateTexture2D(&descDepth, nullptr, &mDepthStencilBuffer);

	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
	ZeroMemory(&descDSV, sizeof(descDSV));
	descDSV.Format = descDepth.Format;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0;
	hr = md3dDevice->CreateDepthStencilView(mDepthStencilBuffer, &descDSV, &mDepthStencilView);


	
	md3dImmediateContext->OMSetRenderTargets(1, &mRenderTargetView, NULL);

	mScreenViewport.TopLeftX = 0;
	mScreenViewport.TopLeftY = 0;
	mScreenViewport.Width = WINDOW_SIZEX;
	mScreenViewport.Height = WINDOW_SIZEY;
	mScreenViewport.MinDepth = 0.0f;
	mScreenViewport.MaxDepth = 1.0f;

	md3dImmediateContext->RSSetViewports(1, &mScreenViewport);


	m_gBuffer->Init(this); // 사이즈가 바뀌면 버퍼 갱신
}

