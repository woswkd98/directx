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
	samDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR; // ���� ���� ���⼱ ���� 
	samDesc.AddressU = samDesc.AddressV = samDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP; //  uvw�� ���� 0~1 �̳Ѿ ���� ���� ����Ǵ� ����
	samDesc.MaxAnisotropy = 1; // D3D11_FILTER_ANISOTROPIC �Ǵ� D3D11_FILTER_COMPARISON_ANISOTROPIC�� ���Ϳ� ������ ��쿡 ���Ǵ� ���� Ŭ����.  ��ȿ�� ���� 1�� 16 �����Դϴ�. 
	samDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS; // ������ ���ø� �����Ϳ� ���� �����͸� ���ø� ���ϴ� ���
	samDesc.MaxLOD = D3D11_FLOAT32_MAX; // �ҹ������ ��ܿ� ���� �� �̰��� ����ū���̴�
	hr = md3dDevice->CreateSamplerState(&samDesc, &m_pSampLinear);
	if (hr == E_FAIL) LOG_MGR->AddLog("���� ���÷� ���þȵ�"); 
	samDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	hr = md3dDevice->CreateSamplerState(&samDesc, &m_pSampPoint);
	if (hr == E_FAIL) LOG_MGR->AddLog("����Ʈ ���÷� ���þȵ�");
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

	//rasterize ����
	

	D3D11_RASTERIZER_DESC wireframeDesc;
	ZeroMemory(&wireframeDesc, sizeof(D3D11_RASTERIZER_DESC));
	wireframeDesc.FillMode = D3D11_FILL_WIREFRAME; // ä��� ���
	wireframeDesc.CullMode = D3D11_CULL_BACK; // �����̽� �ø� 
	wireframeDesc.FrontCounterClockwise = false; // �ð���� ���
	wireframeDesc.DepthClipEnable = true;

	this->GetDevice()->CreateRasterizerState(&wireframeDesc, &pRSStateWire);

	D3D11_RASTERIZER_DESC solidFrameDesc;
	ZeroMemory(&solidFrameDesc, sizeof(D3D11_RASTERIZER_DESC));
	solidFrameDesc.FillMode = D3D11_FILL_SOLID; // ä��� ���
	solidFrameDesc.CullMode = D3D11_CULL_BACK; // �����̽� �ø� 
	solidFrameDesc.FrontCounterClockwise = false; // �ð���� ���
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
	* ���� �Ҵ��, �ʱ�ȭ���� ���� �޸𸮴� 0xCDCDCDCD�� �����ȴ�.

	* ���ÿ� �Ҵܵ�, �ʱ�ȭ���� ���� �޸𸮴� 0xCCCCCCCC�� �����ȴ�.

	* ������ �� �޸𸮴� 0xFEEEFEEE�� �����Ѵ�. (���� ���Ҵ�Ǹ� �ٽ� 0xCDCDCDCD�� �ȴ�)

	�޸𸮰� �����Ǵ� ��Ŀ� ���󼭴� ������ �޸𸮰� 0xDDDDDDDD�� ������ ���� �ִ�.

	* ���� �Ҵ�� �޸��� ù �κа� �� �κ��� 0xFDFDFDFD�� �����ȴ�.

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


	m_gBuffer->Init(this); // ����� �ٲ�� ���� ����
}

