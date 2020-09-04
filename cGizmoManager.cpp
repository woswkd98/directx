#include "stdafx.h"
#include "cGizmoManager.h"
#include "cTransform.h"


void cGizmoManager::Init(float sizeX, float sizeZ, int indexX, int indexZ)
{
	HRESULT hr;
	ID3DBlob * pShaderBlob;
	if (m_pVertexShader == nullptr) // ���ؽ� ���̴� ����
	{
	
		hr = CompileShaderFromFile("VertexColorShader.fx", "VertexColorVS", "vs_5_0", &pShaderBlob);
		if (hr == E_FAIL) LOG_MGR->AddLog("VertexColorShader Fail");

		hr = g_DeviceManager->GetDevice()->CreateVertexShader(pShaderBlob->GetBufferPointer(),
			pShaderBlob->GetBufferSize(), nullptr, &this->m_pVertexShader);
		if (hr == E_FAIL) LOG_MGR->AddLog("VertexColorVS Fail");


		// ��ǲ ���̾ƿ� ���� 

		if (m_pInputLayout == nullptr)
		{
			D3D11_INPUT_ELEMENT_DESC ied[] = // ���׸���� �⺻������ ��ġ �÷� ���� ������ȴ�  ���߿� �̸� �Ӽ��� ���̴��� �ֱ����� �Ӽ� desc 
			{
				{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }, 
			};

			hr = g_DeviceManager->GetDevice()->CreateInputLayout(ied, 2, pShaderBlob->GetBufferPointer(), pShaderBlob->GetBufferSize(), &m_pInputLayout);
			if (hr == E_FAIL) LOG_MGR->AddLog("InputLayout Fail");
		}

		SAFE_RELEASE(pShaderBlob);
	}
	if (m_pPixelShader == nullptr) // �ȼ� ���̴� ����
	{
		HRESULT hr = CompileShaderFromFile("VertexColorShader.fx", "VertexColorPS", "ps_5_0", &pShaderBlob);
		if (hr == E_FAIL) LOG_MGR->AddLog("VertexColorPS Fail");
		
		hr = g_DeviceManager->GetDevice()->CreatePixelShader(pShaderBlob->GetBufferPointer(),
			pShaderBlob->GetBufferSize(), nullptr, &this->m_pPixelShader);
		if (hr == E_FAIL) LOG_MGR->AddLog("VertexColorPS Fail");

		SAFE_RELEASE(pShaderBlob);
	}
	// vp�� �Ѱ���  ������Ʈ ���� ����

	if (m_pConstantBuf == nullptr)
	{	

		// ������Ʈ ���� ����
		D3D11_BUFFER_DESC cbDesc;
		ZeroMemory(&cbDesc, sizeof(cbDesc));
		cbDesc.Usage = D3D11_USAGE_DYNAMIC;
		cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		cbDesc.ByteWidth = sizeof(ST_GIZMO_CB);
		hr = g_DeviceManager->GetDevice()->CreateBuffer(&cbDesc, nullptr, &m_pConstantBuf);
		if (hr == E_FAIL) { LOG_MGR->AddLog("gizmoConstantbuf Fail"); }
	}

	vector<ST_GIZMO_VB_BUF> vecBuf;


	// �׸��� ����
	for (int x = -indexX / 2; x <= indexX / 2; ++x)
	{
		vecBuf.push_back(ST_GIZMO_VB_BUF(XMFLOAT3(x * sizeX, 0, -indexZ / 2 * sizeZ), XMFLOAT4(1, 1, 1, 1)));
		vecBuf.push_back(ST_GIZMO_VB_BUF(XMFLOAT3(x * sizeX, 0, +indexZ / 2 * sizeZ), XMFLOAT4(1, 1, 1, 1)));
	}

	for (int z = -indexZ / 2; z <= indexZ / 2; ++z)
	{
		vecBuf.push_back(ST_GIZMO_VB_BUF(XMFLOAT3(-indexX / 2 * sizeX , 0, z * sizeZ), XMFLOAT4(1, 1, 1, 1)));
		vecBuf.push_back(ST_GIZMO_VB_BUF(XMFLOAT3(indexX / 2 * sizeX ,0, z * sizeZ), XMFLOAT4(1, 1, 1, 1)));
	}

	m_gridSize = vecBuf.size();

	D3D11_BUFFER_DESC vb;
	vb.BindFlags = D3D11_BIND_VERTEX_BUFFER; // ��������, �ε��� ���������� �÷���
	vb.Usage = D3D11_USAGE_DEFAULT; // �⺻��
	vb.ByteWidth = m_gridSize * sizeof(ST_GIZMO_VB_BUF); // ���� ������ ũ�� ���� byte
	vb.CPUAccessFlags = 0; // cpu�� ���߿� ������ ���뿡 �մ���̳Ŀ� ���� �÷��� 
	vb.MiscFlags = 0; // �������ۿ����� ������
	vb.StructureByteStride = 0; // ������ ���ۿ� ����� ���� �ϳ��� ũ�� 

	D3D11_SUBRESOURCE_DATA vInitData;
	ZeroMemory(&vInitData, sizeof(vInitData));
	vInitData.pSysMem = vecBuf.data();// ���� ���۸� �ʱ�ȭ�� �ڷḦ ���� �Ú��� �޸� �迭 ������

	hr = g_DeviceManager->GetDevice()->CreateBuffer(&vb, &vInitData, &m_pGridBuf); // ���� ���� ����
	vecBuf.clear();


	vb.BindFlags = D3D11_BIND_VERTEX_BUFFER; // ��������, �ε��� ���������� �÷���
	vb.Usage = D3D11_USAGE_DYNAMIC; // �а� ���ǵ�
	vb.ByteWidth = 2 * sizeof(ST_GIZMO_VB_BUF); // ���� ������ ũ�� ���� byte
	vb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE; // cpu�� ���߿� ������ ���뿡 �մ���̳Ŀ� ���� �÷��� 
	vb.MiscFlags = 0; // �������ۿ����� ������
	vb.StructureByteStride = 0; // ������ ���ۿ� ����� ���� �ϳ��� ũ�� 

	hr = g_DeviceManager->GetDevice()->CreateBuffer(&vb, &vInitData, &m_pLineBuf); // ���� ���� ����

	if (hr == E_FAIL) LOG_MGR->AddLog("���ؽ� ���� �����ȵ�");

	this->CreateFigure();

	
}

void cGizmoManager::Release()
{

	m_vecLines.clear();

	if (m_pLineBuf)	m_pLineBuf->Release();
	if (m_pConstantBuf) this->m_pConstantBuf->Release();
	if (m_pLineBuf) this->m_pLineBuf->Release();
	if (m_pGridBuf) this->m_pGridBuf->Release();
	if (m_pFigureBuf) this->m_pFigureBuf->Release();
	if (m_pIndexBuf) this->m_pIndexBuf->Release();
	
	if (m_pVertexShader) this->m_pVertexShader->Release();
	if (m_pPixelShader) this->m_pPixelShader->Release();
}

cGizmoManager::cGizmoManager()
{

}


cGizmoManager::~cGizmoManager()
{
}


void cGizmoManager::RenderGizmo(cCamera * pCamera)
{
	// ������Ʈ ���� ������Ʈ (vp ���� ��� �;���)
	g_DeviceManager->GetDC()->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_LINELIST); // �� ���׸����� 
	
	g_DeviceManager->GetDC()->RSSetState(g_DeviceManager->GetRasterizeStateWire()); // ���̾� ������

	g_DeviceManager->GetDC()->IASetInputLayout(this->m_pInputLayout); // ��ǲ���̾ƿ� ����

	g_DeviceManager->GetDC()->VSSetShader(this->m_pVertexShader, nullptr, 0);

	g_DeviceManager->GetDC()->PSSetShader(this->m_pPixelShader, nullptr, 0);

	g_DeviceManager->GetDC()->VSSetConstantBuffers(0, 1, &m_pConstantBuf); // �־��ش� 

	XMMATRIX VP = XMMatrixMultiply(XMLoadFloat4x4(&pCamera->GetViewMatrix()), XMLoadFloat4x4(&pCamera->GetProjection()));

	this->RenderLine(VP);

	this->RenderGrid(VP);

	g_DeviceManager->GetDC()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST); // �ﰢ�� ������

	UINT stride = sizeof(ST_GIZMO_VB_BUF); 

	UINT offset = 0;

	// ���� ���� �ε��� ���� �ҷ���
	g_DeviceManager->GetDC()->IASetVertexBuffers(
		0, // ���ؽ� ���� ������ ����
		1, // ����� ���ؽ� ������ ����
		&m_pFigureBuf, // ���ؽ� ���۵��� ������ ** �ε� �������� ������ �����ϸ� ���ϴ�
		&stride, // ���� ����ü�� ������
		&offset // ���������� ������
	);

	g_DeviceManager->GetDC()->IASetIndexBuffer(
		m_pIndexBuf,
		DXGI_FORMAT_R32_UINT, //  unsigned int �� ���ڴ�
		0
	);

	this->RenderBox(VP);

	this->RenderCylinder(VP);

	this->RenderSphere(VP);
}

void cGizmoManager::RenderGrid(XMMATRIX & matVP)
{
	g_DeviceManager->GetDC()->Map(m_pConstantBuf, 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedResource); // �ϴ� ���� 

	ST_GIZMO_CB* pConstant = static_cast<ST_GIZMO_CB*>(MappedResource.pData); // �������� �����͸� �Ѱ��ش�

	XMStoreFloat4x4(&pConstant->matCB, XMMatrixTranspose(matVP));

	g_DeviceManager->GetDC()->Unmap(m_pConstantBuf, 0);  // �ݴ´�

	UINT stride = sizeof(ST_GIZMO_VB_BUF);

	UINT offset = 0;

	g_DeviceManager->GetDC()->IASetVertexBuffers(
		0, // ���ؽ� ���� ������ ����
		1, // ����� ���ؽ� ������ ����
		&m_pGridBuf, // ���ؽ� ���۵��� ������ ** �ε� �������� ������ �����ϸ� ���ϴ�
		&stride, // �������� �迭�� ù���Ҹ� ����Ű�� ������
		&offset // �������� 
	);

	g_DeviceManager->GetDC()->Draw(m_gridSize, 0); // �Ϲ� ��ο� �Լ� 
}

void cGizmoManager::RenderLine(XMMATRIX & matVP)
{
	g_DeviceManager->GetDC()->Map(m_pConstantBuf, 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedResource); // �ϴ� ���� 

	ST_GIZMO_CB* pConstant = (ST_GIZMO_CB*)(MappedResource.pData); // �������� �����͸� �Ѱ��ش�

	XMStoreFloat4x4(&pConstant->matCB, XMMatrixTranspose(matVP));

	g_DeviceManager->GetDC()->Unmap(m_pConstantBuf, 0);  // �ݴ´�

	UINT stride = sizeof(ST_GIZMO_VB_BUF);

	UINT offset = 0;

	// ���� ���� �ε��� ���� �ҷ���
	g_DeviceManager->GetDC()->IASetVertexBuffers(
		0, // ���ؽ� ���� ������ ����
		1, // ����� ���ؽ� ������ ����
		&m_pLineBuf, // ���ؽ� ���۵��� ������ ** �ε� �������� ������ �����ϸ� ���ϴ�
		&stride, // ���� ����ü�� ������
		&offset // ���������� ������
	);

	list<ST_GIZMO_VB_BUF>::iterator it = m_listLines.begin();

	for (it; it != m_listLines.end();) // �ڽ� 
	{
		g_DeviceManager->GetDC()->Map(m_pLineBuf, 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedResource); // �ϴ� ���� 
		
		ST_GIZMO_VB_BUF * ppData = static_cast<ST_GIZMO_VB_BUF*>(MappedResource.pData);

		ppData[0].Position =it->Position;
		ppData[0].Color = it->Color;

		++it;

		ppData[1].Position = it->Position;
		ppData[1].Color = it->Color;
				 
		++it;

		g_DeviceManager->GetDC()->Unmap(m_pLineBuf, 0);  // �ݴ´�

		g_DeviceManager->GetDC()->Draw(2, 0); // draw
	}

	m_listLines.clear();
}

void cGizmoManager::RenderBox(XMMATRIX & matVP)
{
	transform_iterator = m_listBoxTransform.begin();

	for (transform_iterator; transform_iterator != m_listBoxTransform.end(); ++transform_iterator) // �ڽ� 
	{
		g_DeviceManager->GetDC()->Map(m_pConstantBuf, 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedResource); // �ϴ� ���� 
		
		ST_GIZMO_CB* pConstant = static_cast<ST_GIZMO_CB*>(MappedResource.pData); // �������� �����͸� �Ѱ��ش�

		XMStoreFloat4x4(&pConstant->matCB, XMMatrixTranspose((*transform_iterator)->GetFinalMatrix() * matVP));

		g_DeviceManager->GetDC()->Unmap(m_pConstantBuf, 0);  // �ݴ´�

		g_DeviceManager->GetDC()->DrawIndexed(m_boxIndexSize, m_boxlocateIndics, m_boxlocateVertics); // drawindexed
	}

}

void cGizmoManager::RenderCylinder(XMMATRIX & matVP)
{
	transform_iterator = m_listCylinderTransform.begin();

	for (transform_iterator; transform_iterator != m_listCylinderTransform.end(); ++transform_iterator) // �ڽ� 
	{
		g_DeviceManager->GetDC()->Map(m_pConstantBuf, 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedResource); // �ϴ� ���� 
		
		ST_GIZMO_CB* pConstant = static_cast<ST_GIZMO_CB*>(MappedResource.pData); // �������� �����͸� �Ѱ��ش�

		XMStoreFloat4x4(&pConstant->matCB, XMMatrixTranspose((*transform_iterator)->GetFinalMatrix() * matVP));

		g_DeviceManager->GetDC()->Unmap(m_pConstantBuf, 0);  // �ݴ´�

		g_DeviceManager->GetDC()->DrawIndexed(m_cylinderIndexSize, m_cylinderlocateIndics,m_cylinderlocateVertics ); // drawindexed
	}
}

void cGizmoManager::RenderSphere(XMMATRIX & matVP)
{
	transform_iterator = m_listSsphereTransform.begin();

	for (transform_iterator; transform_iterator != m_listSsphereTransform.end(); ++transform_iterator) // �ڽ� 
	{
		g_DeviceManager->GetDC()->Map(m_pConstantBuf, 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedResource); // �ϴ� ���� 
		
		ST_GIZMO_CB* pConstant = static_cast<ST_GIZMO_CB*>(MappedResource.pData); // �������� �����͸� �Ѱ��ش�

		XMStoreFloat4x4(&pConstant->matCB, XMMatrixTranspose((*transform_iterator)->GetFinalMatrix() * matVP));

		g_DeviceManager->GetDC()->Unmap(m_pConstantBuf, 0);  // �ݴ´�

		g_DeviceManager->GetDC()->DrawIndexed(m_sphereIndexSize, m_spherelocateIndics, m_spherelocateVertics); // drawindexed
	}
}

void cGizmoManager::RenderTerrain(XMMATRIX & matVP)
{
}

void cGizmoManager::CreateBox(vector<ST_GIZMO_VB_BUF> & VB, vector<DWORD> & IB)
{
	static const XMFLOAT4 color = XMFLOAT4(1, 0, 0, 1);

	m_boxlocateVertics = VB.size();
	m_boxlocateIndics = IB.size();

	// �簢��
	VB.push_back(ST_GIZMO_VB_BUF(XMFLOAT3(-0.5f, -0.5f, -0.5f), color));
	VB.push_back(ST_GIZMO_VB_BUF(XMFLOAT3(-0.5f, 0.5f, -0.5f), color));
	VB.push_back(ST_GIZMO_VB_BUF(XMFLOAT3(0.5f, 0.5f, -0.5f), color));
	VB.push_back(ST_GIZMO_VB_BUF(XMFLOAT3(0.5f, -0.5f, -0.5f), color));
	
	VB.push_back(ST_GIZMO_VB_BUF(XMFLOAT3(-0.5f, -0.5f, 0.5f), color));
	VB.push_back(ST_GIZMO_VB_BUF(XMFLOAT3(-0.5f, 0.5f, 0.5f), color));
	VB.push_back(ST_GIZMO_VB_BUF(XMFLOAT3(0.5f, 0.5f, 0.5f), color));
	VB.push_back(ST_GIZMO_VB_BUF(XMFLOAT3(0.5f, -0.5f, 0.5f), color));

	IB.push_back(0); IB.push_back(1); IB.push_back(2);
	IB.push_back(0); IB.push_back(2); IB.push_back(3);

	IB.push_back(4); IB.push_back(6); IB.push_back(5);
	IB.push_back(4); IB.push_back(7); IB.push_back(6);

	IB.push_back(4); IB.push_back(5); IB.push_back(1);
	IB.push_back(4); IB.push_back(1); IB.push_back(0);

	IB.push_back(3); IB.push_back(2); IB.push_back(6);
	IB.push_back(3); IB.push_back(6); IB.push_back(7);
	
	IB.push_back(1); IB.push_back(5); IB.push_back(6);
	IB.push_back(1); IB.push_back(6); IB.push_back(2);
	
	IB.push_back(4); IB.push_back(0); IB.push_back(3);
	IB.push_back(4); IB.push_back(3); IB.push_back(7);

	m_boxIndexSize = IB.size() - m_boxlocateIndics;
}

void cGizmoManager::CreateCylinder(vector<ST_GIZMO_VB_BUF> & VB, vector<DWORD> & IB)
{
	// �����

	// �Ǹ��� ���� ���θ� �� ������

	// ���� �ε��� 

	m_cylinderlocateVertics = VB.size();
	m_cylinderlocateIndics = IB.size();



	static const unsigned int sliceTop = 30;

	static const XMFLOAT4 color = XMFLOAT4(0, 1, 0, 1);

	XMFLOAT3 topCircleCenter = XMFLOAT3(0, 0.5f, 0);

	VB.push_back(ST_GIZMO_VB_BUF(topCircleCenter, color)); // ������ ����

	float deltaRad = XMConvertToRadians(static_cast<float>(360 / sliceTop));

	for (int i = 0; i < sliceTop; ++i)
	{
		topCircleCenter.x = cosf(deltaRad * i) * 0.5f;
		topCircleCenter.z = sinf(deltaRad * i) * 0.5f;

		LOG_MGR->AddLog("%f,%f,%f",topCircleCenter.x, topCircleCenter.y, topCircleCenter.z);

		VB.push_back(ST_GIZMO_VB_BUF(topCircleCenter, color)); // ž �����ְ�
	}

	// ���� ���� ����
	topCircleCenter = XMFLOAT3(0, -0.5f, 0);
	VB.push_back(ST_GIZMO_VB_BUF(topCircleCenter, color)); // ������ ����
	for (int i = 0; i < sliceTop; ++i)
	{
		topCircleCenter.z = sinf(deltaRad * i) * 0.5f;
		topCircleCenter.x = cosf(deltaRad * i) * 0.5f;

		VB.push_back(ST_GIZMO_VB_BUF(topCircleCenter, color));
	}

	// �ε��� ����

	/*	0 ,i + 1 ,i
		
		1
	6		2
		0
	5		3
		4
	*/

	float bottomStartIndex = sliceTop + 1;
	for (int i = 1; i < sliceTop; ++i)
	{	
		IB.push_back(0); // ��������
		IB.push_back(0 + i + 1);
		IB.push_back(0 + i);

		// ���ҿ� ���� �ﰢ�� n - 1��
		IB.push_back(bottomStartIndex); // ��������
		IB.push_back(bottomStartIndex + i);
		IB.push_back(bottomStartIndex + i + 1);

		// �簢�� �׸���

		// ���� �����
		
		/*
		�ϳ��� �ܸ�
		i					i + 1

		i + sliceTop + 1  i + sliceTop + 2
		*/

		IB.push_back(i);
		IB.push_back(i + 1);
		IB.push_back(i + sliceTop + 1);
					
		IB.push_back(i + 1); // ��������
		IB.push_back(i + sliceTop + 2);
		IB.push_back(i + sliceTop + 1);
	}

	//// ž�� ���� �ﰢ�� n��
	IB.push_back(0); // ��������
	IB.push_back(1);
	IB.push_back(sliceTop);
	
	// ���ҿ� ���� �ﰢ�� n��
	IB.push_back(sliceTop + 1); // ��������
	IB.push_back(sliceTop * 2 + 1);
	IB.push_back(sliceTop + 2);

	/*
		�ܸ� ��
		sliceTop		1

		sliceTop * 2 + 2  sliceTop + 2
	*/

	// ���� �簢�� n ��
	IB.push_back(1);
	IB.push_back(sliceTop + 2);
	IB.push_back(sliceTop);

	IB.push_back(sliceTop); // ��������
	IB.push_back(sliceTop + 2);
	IB.push_back(sliceTop * 2 + 1);
	m_cylinderIndexSize = IB.size() - m_cylinderlocateIndics;
}

void cGizmoManager::CreateSphere(vector<ST_GIZMO_VB_BUF>& VB, vector<DWORD>& IB)
{
	m_spherelocateVertics = VB.size();
	m_spherelocateIndics = IB.size();
	// ������ ����

	const UINT SubDivideNum = 5; // ����ȭ ��ȣ 5�� �̻� ���� �ʵ����Ѵ�
	const  float x = 0.525731f; // �����̸�ü �׼����̼� �ؼ� ���� �ٻ�
	const  float z = 0.850651f;
	XMFLOAT4 color = XMFLOAT4(0, 1, 1, 1);

	XMFLOAT3 posArray[12] = {
		XMFLOAT3(-x,0.0f, z),XMFLOAT3(x,0.0f, z),
		XMFLOAT3(-x,0.0f, -z),XMFLOAT3(x,0.0f, -z),
		XMFLOAT3(0.0f,z, x),XMFLOAT3(0.0f, z,-x),
		XMFLOAT3(0.0f,-z, z),XMFLOAT3(0.0f, -z,-x),
		XMFLOAT3(z, x,0.0f),XMFLOAT3(-z ,x,0.0f),
		XMFLOAT3(z, -x,0.0f),XMFLOAT3(-z, -x,0.0f)
	};

	

	DWORD k[60] = {
		1,4,0, 4,9,0, 4,5,9,
		8,5,4, 1,8,4, 1,10,8,
		10,3,8, 8,3,5, 3,2,5,
		3,7,2, 3,10,7, 10, 6,7,
		6,11,7, 6,0, 11, 6,1,0,
		10, 1,6, 11, 0, 9, 2,11, 9,
		5,2,9,11,2,7
	};

	float radius = 0.5f;


	for (int i = 0; i < 12; ++i)
	{
		VB.push_back(ST_GIZMO_VB_BUF(posArray[i], color));
	}

	for (int i = 0; i < 60; ++i)
	{
		IB.push_back(k[i]);
	}

	m_sphereIndexSize = IB.size() - m_spherelocateIndics;
}

void cGizmoManager::CreateFigure()
{
	// ���� ���� �� �ð� ����

	vector<DWORD> vecIndex;
	vector<ST_GIZMO_VB_BUF> vecBuf;

	this->CreateBox(vecBuf, vecIndex);
	this->CreateCylinder(vecBuf, vecIndex);
	this->CreateSphere(vecBuf, vecIndex);

	//  ���� �׸��� ���� ���� �ΰ� �߰� 

	UINT lineVertexLocate, lineIndexLocate;
	lineVertexLocate = vecBuf.size();
	lineIndexLocate = vecIndex.size();
	
	vecIndex.push_back(vecBuf.size());
	vecIndex.push_back(vecBuf.size() + 1);
	
	// z������ ���̰� 1 �� ���� �̸� �߰�

	vecBuf.push_back(ST_GIZMO_VB_BUF(XMFLOAT3(0, 0, -0.5f), XMFLOAT4(0, 0, 1, 0)));
	vecBuf.push_back(ST_GIZMO_VB_BUF(XMFLOAT3(0, 0, 0.5f), XMFLOAT4(0, 0, 1, 0)));

	D3D11_BUFFER_DESC vb;
	vb.BindFlags = D3D11_BIND_VERTEX_BUFFER; // ��������, �ε��� ���������� �÷���
	vb.Usage = D3D11_USAGE_DEFAULT; // �⺻��
	vb.ByteWidth = vecBuf.size() * sizeof(ST_GIZMO_VB_BUF); // ���� ������ ũ�� ���� byte
	vb.CPUAccessFlags = 0; // cpu�� ���߿� ������ ���뿡 �մ���̳Ŀ� ���� �÷��� 
	vb.MiscFlags = 0; // �������ۿ����� ������
	vb.StructureByteStride = 0; // ������ ���ۿ� ����� ���� �ϳ��� ũ�� 

	D3D11_SUBRESOURCE_DATA vInitData;
	ZeroMemory(&vInitData, sizeof(vInitData));
	vInitData.pSysMem = vecBuf.data();// ���� ���۸� �ʱ�ȭ�� �ڷḦ ���� �Ú��� �޸� �迭 ������

	HRESULT hr = g_DeviceManager->GetDevice()->CreateBuffer(&vb, &vInitData, &m_pFigureBuf); // ���� ���� ����
	if (hr == E_FAIL) { LOG_MGR->AddLog("Figure vertex Buffer Fail"); }
	D3D11_BUFFER_DESC ib;

	ib.BindFlags = D3D11_BIND_INDEX_BUFFER; // ��������, �ε��� ���������� �÷���
	ib.Usage = D3D11_USAGE_IMMUTABLE; // �⺻��
	ib.ByteWidth = vecIndex.size() * sizeof(DWORD); // ���� ������ ũ�� ���� byte
	ib.CPUAccessFlags = 0; // cpu�� ���߿� ������ ���뿡 �մ���̳Ŀ� ���� �÷��� 
	ib.MiscFlags = 0; // �������ۿ����� ������
	ib.StructureByteStride = 0; // ������ ���ۿ� ����� ���� �ϳ��� ũ�� 

	ZeroMemory(&vInitData, sizeof(vInitData));
	vInitData.pSysMem = vecIndex.data();// ���� ���۸� �ʱ�ȭ�� �ڷḦ ���� �Ú��� �޸� �迭 ������

	hr = g_DeviceManager->GetDevice()->CreateBuffer(&ib, &vInitData, &m_pIndexBuf); // ���� ���� ����
	if (hr == E_FAIL) { LOG_MGR->AddLog("Figure Index Buffer Fail"); }

}

void cGizmoManager::AddBox(cTransform * transform)
{
	m_listBoxTransform.push_back(transform);
}

void cGizmoManager::AddSphere(cTransform * transform)
{
	m_listSsphereTransform.push_back(transform);
}

void cGizmoManager::AddCylinder(cTransform * transform)
{
	m_listCylinderTransform.push_back(transform);
}

void cGizmoManager::AddTerrain(cTransform * transform)
{
	m_listTerrainTransform.push_back(transform);
}

void cGizmoManager::AddLine(XMFLOAT3 v1, XMFLOAT3 v2, XMFLOAT4 color)
{
	m_listLines.push_back(ST_GIZMO_VB_BUF(v1, color));
	m_listLines.push_back(ST_GIZMO_VB_BUF(v2, color));
}



