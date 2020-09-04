#include "stdafx.h"
#include "cWorldShader.h"



void cWorldShader::Init()
{
	HRESULT hr;

	D3D11_BUFFER_DESC cb;
	ZeroMemory(&cb, sizeof(cb));
	cb.Usage = D3D11_USAGE_DEFAULT;
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.CPUAccessFlags = 0;

	cb.ByteWidth = sizeof(WVP);

	hr = DeviceManagerDev->CreateBuffer(&cb, NULL, &pVSConstantBuffer);

	if (hr == E_FAIL) LOG_MGR->AddLog("WVP constant ���� �����ȵ�");

	ID3DBlob * compiledVertex = nullptr; // �����ϵ� ���̴��� ���� ������

	hr = CompileShaderFromFile(
		"RenderSceneShader.fx", // ���̴� �̸�
		"VS_prepass", // ���̴� �����Լ� �̸�( �������Լ� )
		"vs_5_0", // ����
		&compiledVertex // ���̴� ������
	);

	if (hr == E_FAIL) LOG_MGR->AddLog("���� ���� �����ȵ�");

	hr = DeviceManagerDev->CreateVertexShader(compiledVertex->GetBufferPointer(), compiledVertex->GetBufferSize(), nullptr, &pVS);

	D3D11_INPUT_ELEMENT_DESC vertexDsc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	UINT numElements = ARRAYSIZE(vertexDsc);
	DeviceManagerDev->CreateInputLayout(
		vertexDsc,
		numElements,
		compiledVertex->GetBufferPointer(),
		compiledVertex->GetBufferSize(),
		&m_vsInputlayout);

	SAFE_RELEASE(compiledVertex);

	this->InitRenderSceneShader();
}

void cWorldShader::InitRenderSceneShader()
{
	D3D11_BUFFER_DESC cb;
	ZeroMemory(&cb, sizeof(cb));
	cb.Usage = D3D11_USAGE_DEFAULT;
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.CPUAccessFlags = 0;

	cb.ByteWidth = sizeof(WVP);

	DeviceManagerDev->CreateBuffer(&cb, NULL, &pVSConstantBuffer);

	ID3D10Blob * compiledVertex = nullptr; // �����ϵ� ���̴��� ���� ������

	HRESULT hr = CompileShaderFromFile(
		"RenderSceneShader.fx", // ���̴� �̸�
		"RenderSceneVS", // ���̴� �����Լ� �̸�( �������Լ� )
		"vs_5_0", // ����
		&compiledVertex // ���̴� ������
	);


	if (hr == E_FAIL) LOG_MGR->AddLog("���� ���� �����ȵ�");

	hr = DeviceManagerDev->CreateVertexShader(compiledVertex->GetBufferPointer(), compiledVertex->GetBufferSize(), nullptr, &pVS_RenderScene);

	D3D11_INPUT_ELEMENT_DESC vertexDsc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TANGENT",  0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "BINORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 44, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	UINT numElements = ARRAYSIZE(vertexDsc);
	DeviceManagerDev->CreateInputLayout(
		vertexDsc,
		numElements,
		compiledVertex->GetBufferPointer(),
		compiledVertex->GetBufferSize(),
		&m_vsRenderSceneInputLayOut);
	SAFE_RELEASE(compiledVertex);


}

void cWorldShader::WorldShaderUpdate()
{
	DeviceManagerContext->IASetInputLayout(m_vsInputlayout);

	XMMATRIX vp = this->wvp.VP;
	XMMATRIX world = this->wvp.World;
	world = XMMatrixTranspose(world);
	vp = XMMatrixTranspose(vp);

	WVP VSConstant;
	VSConstant.VP = vp;
	VSConstant.World = world;

	DeviceManagerContext->UpdateSubresource(this->pVSConstantBuffer, 0, nullptr, &VSConstant, 0, 0);
}

void cWorldShader::RenderSceneUpdate()
{
	DeviceManagerContext->IASetInputLayout(m_vsRenderSceneInputLayOut);
	XMMATRIX vp = this->wvp.VP;
	XMMATRIX world = this->wvp.World;
	world = XMMatrixTranspose(world);
	vp = XMMatrixTranspose(vp);

	WVP VSConstant;
	VSConstant.VP = vp;
	VSConstant.World = world;

	DeviceManagerContext->UpdateSubresource(this->pVSConstantBuffer, 0, nullptr, &VSConstant, 0, 0);
}


cWorldShader::cWorldShader()
{
}


cWorldShader::~cWorldShader()
{
}
