#include "stdafx.h"
#include "cSceneManager.h"
#include "cScene.h"
#include "cGizmoManager.h"

void cSceneManager::ChangeScene(int index)
{
	if (m_vecScene.size() <= index) return; // error

	SetUp(m_vecScene[index]);

}

void cSceneManager::SetUp(cScene * pScene)
{
	if (m_pPreScene != nullptr)
		m_pPreScene->Release();

	m_pCurrentScene = pScene;

	pScene->Init();
}

void cSceneManager::Update(float deltaTime)
{
	m_pCurrentScene->Update(deltaTime);
	
}

void cSceneManager::Render()
{
	g_DeviceManager->GetDC()->ClearRenderTargetView(g_DeviceManager->GetTargetView(), backgroudColor);

	g_DeviceManager->GetDC()->ClearDepthStencilView(g_DeviceManager->GetDepthStencilView(), D3D11_CLEAR_DEPTH, 1.0, 0);
	ID3D11DepthStencilState* pPrevDepthState; //깊이상태
	UINT nPrevStencil;
	g_DeviceManager->GetDC()->OMGetDepthStencilState(&pPrevDepthState, &nPrevStencil);

	g_DeviceManager->SetPSSamplerStateLinear(0, 1);

	g_DeviceManager->GetGBuffer()->PreRender();

	// renderScene


	g_DeviceManager->GetGBuffer()->PostRender();

	ID3D11RenderTargetView * pRTV = g_DeviceManager->GetTargetView();

	g_DeviceManager->GetDC()->OMSetRenderTargets(1, &pRTV, g_DeviceManager->GetGBuffer()->GetDepthReadOnlyDSV());

	g_DeviceManager->GetGBuffer()->PrePareForUnpack(m_pCurrentScene->GetMainCam());

	//g_LightManager->DoLighting(g_DeviceManager->GetGBuffer()->GetArraySRV());


	// 여기서 부턴 부가적 옵션의 라인 렌더러들
	if (m_bVisualizeLightVolume) // 빛의 거리를 선으로 나타넨 렌더러
	{
		//g_LightManager->DoDebugLightVolume();
	}

	if (m_bVisualizeGBuffer) // gBuffer의 모든 렌더 타겟 뷰를 작은 화면으로 보여줌
	{
		g_DeviceManager->GetDC()->OMSetRenderTargets(1, &pRTV, NULL);
	
		g_DeviceManager->GetGBuffer()->VisualizeGBuffer();

		g_DeviceManager->GetDC()->OMSetRenderTargets(1, &pRTV, g_DeviceManager->GetGBuffer()->GetDepthDSV());
	}
	

	if (m_bRenderGizmo) // 기즈모 렌더 및 컬라이더 렌더 
	{
		g_GizmoManager->RenderGizmo(m_pCurrentScene->GetMainCam());
	}

	m_pCurrentScene->Render();


	g_DeviceManager->GetDC()->OMSetDepthStencilState(pPrevDepthState, nPrevStencil);
	SAFE_RELEASE(pPrevDepthState);

	HRESULT hr = g_DeviceManager->GetSwapChain()->Present(0, 0);
	if (hr == E_FAIL) { LOG_MGR->AddLog("failrender"); };

}



void cSceneManager::Init(int index)
{
	this->m_vecScene.resize(5,nullptr);

	backgroudColor[0] = 0.0f;
	backgroudColor[1] = 0.0f;
	backgroudColor[2] = 0;
	backgroudColor[3] = 1;

	

	// Create constant buffers
	D3D11_BUFFER_DESC cbDesc;
	ZeroMemory(&cbDesc, sizeof(cbDesc));
	cbDesc.Usage = D3D11_USAGE_DYNAMIC;
	cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cbDesc.ByteWidth = sizeof(CB_VS_PER_OBJECT);
	g_DeviceManager->GetDevice()->CreateBuffer(&cbDesc, NULL, &m_pSceneVertexShaderCB);

	cbDesc.ByteWidth = sizeof(CB_PS_PER_OBJECT);
	g_DeviceManager->GetDevice()->CreateBuffer(&cbDesc, NULL, &m_pScenePixelShaderCB);

	// Load the prepass light shader
	ID3DBlob* pShaderBlob = NULL;
	CompileShaderFromFile("DeferredShader.fx", "RenderSceneVS", "vs_5_0", &pShaderBlob);
	g_DeviceManager->GetDevice()->CreateVertexShader(pShaderBlob->GetBufferPointer(),
		pShaderBlob->GetBufferSize(), NULL, &m_pSceneVertexShader);

	// Create a layout for the object data
	const D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	g_DeviceManager->GetDevice()->CreateInputLayout(layout, ARRAYSIZE(layout), pShaderBlob->GetBufferPointer(),
		pShaderBlob->GetBufferSize(), &m_pSceneVSLayout);
	SAFE_RELEASE(pShaderBlob);

	CompileShaderFromFile("DeferredShader.fx", "RenderScenePS", "ps_5_0", &pShaderBlob);
	g_DeviceManager->GetDevice()->CreatePixelShader(pShaderBlob->GetBufferPointer(),
		pShaderBlob->GetBufferSize(), NULL, &m_pScenePixelShader);
	SAFE_RELEASE(pShaderBlob);



	SetPrefabs();

	ChangeScene(index);


}

void cSceneManager::Release()
{
	SAFE_RELEASE(m_pSceneVertexShaderCB);
	SAFE_RELEASE(m_pScenePixelShaderCB);
	SAFE_RELEASE(m_pSceneVertexShader);
	SAFE_RELEASE(m_pSceneVSLayout);
	SAFE_RELEASE(m_pScenePixelShader);

	for (int i = 0; i < m_vecScene.size(); ++i)
	{
		m_vecScene[i]->Release();
		delete m_vecScene[i];
	}
	m_vecScene.clear();
	
}

void cSceneManager::InsertBuffer(char * name, char * path)
{
	vector<WORD> index;
	vector<ST_VERTEX> * temp = g_FBXFiles->GetStaticMesh(name, path, index);

	ID3D11Buffer * pVB;
	ID3D11Buffer * pIB;

	D3D11_BUFFER_DESC vb;
	vb.BindFlags = D3D11_BIND_VERTEX_BUFFER; // 정점버퍼, 인덱스 버퍼인지의 플래그
	vb.Usage = D3D11_USAGE_DEFAULT; // 버퍼가 쓰이는 용도 대체로 readonly형식을 선호하겠지 (gpu전용)
	vb.ByteWidth = sizeof(ST_VERTEX) * temp->size(); // 정점 버퍼의 크기 단위 byte
	vb.CPUAccessFlags = 0; // cpu가 나중에 버퍼의 내용에 손댈것이냐에 대한 플래그 
	vb.MiscFlags = 0; // 정점버퍼에서는 무쓸모
	vb.StructureByteStride = 0; // 구조적 버퍼에 저장된 원소 하나의 크기 

	D3D11_SUBRESOURCE_DATA vInitData;
	ZeroMemory(&vInitData, sizeof(vInitData));
	vInitData.pSysMem = temp->data();// 정점 버퍼를 초기화할 자료를 담은 시슽템 메모리 배열 포인터

	HRESULT hr = g_DeviceManager->GetDevice()->CreateBuffer(&vb, &vInitData, &pVB); // 실제 버퍼 생성
	m_vIB.push_back(pVB);
	if (hr == E_FAIL) LOG_MGR->AddLog("버텍스 버퍼 생성안됨");

	D3D11_BUFFER_DESC ib;

	ib.BindFlags = D3D11_BIND_INDEX_BUFFER; // 정점버퍼, 인덱스 버퍼인지의 플래그
	ib.Usage = D3D11_USAGE_DEFAULT; // 버퍼가 쓰이는 용도 대체로 readonly형식을 선호하겠지 (gpu전용)
	ib.ByteWidth = sizeof(WORD) * index.size(); // 버퍼의 크기 단위 byte
	ib.CPUAccessFlags = 0; // cpu가 나중에 버퍼의 내용에 손댈것이냐에 대한 플래그 
	ib.MiscFlags = 0; // 정점버퍼에서는 무쓸모
	ib.StructureByteStride = 0; // 구조적 버퍼에 저장된 원소 하나의 크기 

	ZeroMemory(&vInitData, sizeof(vInitData));
	vInitData.pSysMem = index.data();// 인덱스 버퍼를 초기화할 자료를 담은 시스템 메모리 배열 포인터

	hr = g_DeviceManager->GetDevice()->CreateBuffer(&ib, &vInitData, &pIB); // 실제 버퍼 생성

	m_vIB.push_back(pIB);

	if (hr == E_FAIL) LOG_MGR->AddLog("인덱스 버퍼 생성안됨");
}


cSceneManager::cSceneManager()
{

	

}


cSceneManager::~cSceneManager()
{
}
void cSceneManager::SetPrefabs() // 여기서 미리 생성시키고 싶은 프리펩이 있으면 생성
{
	

	//this->InsertBuffer("Robot","Robot Kyle");
}
