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
	ID3D11DepthStencilState* pPrevDepthState; //���̻���
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


	// ���⼭ ���� �ΰ��� �ɼ��� ���� ��������
	if (m_bVisualizeLightVolume) // ���� �Ÿ��� ������ ��Ÿ�� ������
	{
		//g_LightManager->DoDebugLightVolume();
	}

	if (m_bVisualizeGBuffer) // gBuffer�� ��� ���� Ÿ�� �並 ���� ȭ������ ������
	{
		g_DeviceManager->GetDC()->OMSetRenderTargets(1, &pRTV, NULL);
	
		g_DeviceManager->GetGBuffer()->VisualizeGBuffer();

		g_DeviceManager->GetDC()->OMSetRenderTargets(1, &pRTV, g_DeviceManager->GetGBuffer()->GetDepthDSV());
	}
	

	if (m_bRenderGizmo) // ����� ���� �� �ö��̴� ���� 
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
	vb.BindFlags = D3D11_BIND_VERTEX_BUFFER; // ��������, �ε��� ���������� �÷���
	vb.Usage = D3D11_USAGE_DEFAULT; // ���۰� ���̴� �뵵 ��ü�� readonly������ ��ȣ�ϰ��� (gpu����)
	vb.ByteWidth = sizeof(ST_VERTEX) * temp->size(); // ���� ������ ũ�� ���� byte
	vb.CPUAccessFlags = 0; // cpu�� ���߿� ������ ���뿡 �մ���̳Ŀ� ���� �÷��� 
	vb.MiscFlags = 0; // �������ۿ����� ������
	vb.StructureByteStride = 0; // ������ ���ۿ� ����� ���� �ϳ��� ũ�� 

	D3D11_SUBRESOURCE_DATA vInitData;
	ZeroMemory(&vInitData, sizeof(vInitData));
	vInitData.pSysMem = temp->data();// ���� ���۸� �ʱ�ȭ�� �ڷḦ ���� �Ú��� �޸� �迭 ������

	HRESULT hr = g_DeviceManager->GetDevice()->CreateBuffer(&vb, &vInitData, &pVB); // ���� ���� ����
	m_vIB.push_back(pVB);
	if (hr == E_FAIL) LOG_MGR->AddLog("���ؽ� ���� �����ȵ�");

	D3D11_BUFFER_DESC ib;

	ib.BindFlags = D3D11_BIND_INDEX_BUFFER; // ��������, �ε��� ���������� �÷���
	ib.Usage = D3D11_USAGE_DEFAULT; // ���۰� ���̴� �뵵 ��ü�� readonly������ ��ȣ�ϰ��� (gpu����)
	ib.ByteWidth = sizeof(WORD) * index.size(); // ������ ũ�� ���� byte
	ib.CPUAccessFlags = 0; // cpu�� ���߿� ������ ���뿡 �մ���̳Ŀ� ���� �÷��� 
	ib.MiscFlags = 0; // �������ۿ����� ������
	ib.StructureByteStride = 0; // ������ ���ۿ� ����� ���� �ϳ��� ũ�� 

	ZeroMemory(&vInitData, sizeof(vInitData));
	vInitData.pSysMem = index.data();// �ε��� ���۸� �ʱ�ȭ�� �ڷḦ ���� �ý��� �޸� �迭 ������

	hr = g_DeviceManager->GetDevice()->CreateBuffer(&ib, &vInitData, &pIB); // ���� ���� ����

	m_vIB.push_back(pIB);

	if (hr == E_FAIL) LOG_MGR->AddLog("�ε��� ���� �����ȵ�");
}


cSceneManager::cSceneManager()
{

	

}


cSceneManager::~cSceneManager()
{
}
void cSceneManager::SetPrefabs() // ���⼭ �̸� ������Ű�� ���� �������� ������ ����
{
	

	//this->InsertBuffer("Robot","Robot Kyle");
}
