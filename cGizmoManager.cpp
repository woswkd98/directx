#include "stdafx.h"
#include "cGizmoManager.h"
#include "cTransform.h"


void cGizmoManager::Init(float sizeX, float sizeZ, int indexX, int indexZ)
{
	HRESULT hr;
	ID3DBlob * pShaderBlob;
	if (m_pVertexShader == nullptr) // 버텍스 셰이더 생성
	{
	
		hr = CompileShaderFromFile("VertexColorShader.fx", "VertexColorVS", "vs_5_0", &pShaderBlob);
		if (hr == E_FAIL) LOG_MGR->AddLog("VertexColorShader Fail");

		hr = g_DeviceManager->GetDevice()->CreateVertexShader(pShaderBlob->GetBufferPointer(),
			pShaderBlob->GetBufferSize(), nullptr, &this->m_pVertexShader);
		if (hr == E_FAIL) LOG_MGR->AddLog("VertexColorVS Fail");


		// 인풋 레이아웃 설정 

		if (m_pInputLayout == nullptr)
		{
			D3D11_INPUT_ELEMENT_DESC ied[] = // 선그리기는 기본적으로 위치 컬러 값만 있으면된다  나중에 이름 속성을 셰이더에 넣기위한 속성 desc 
			{
				{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }, 
			};

			hr = g_DeviceManager->GetDevice()->CreateInputLayout(ied, 2, pShaderBlob->GetBufferPointer(), pShaderBlob->GetBufferSize(), &m_pInputLayout);
			if (hr == E_FAIL) LOG_MGR->AddLog("InputLayout Fail");
		}

		SAFE_RELEASE(pShaderBlob);
	}
	if (m_pPixelShader == nullptr) // 픽셀 셰이더 생성
	{
		HRESULT hr = CompileShaderFromFile("VertexColorShader.fx", "VertexColorPS", "ps_5_0", &pShaderBlob);
		if (hr == E_FAIL) LOG_MGR->AddLog("VertexColorPS Fail");
		
		hr = g_DeviceManager->GetDevice()->CreatePixelShader(pShaderBlob->GetBufferPointer(),
			pShaderBlob->GetBufferSize(), nullptr, &this->m_pPixelShader);
		if (hr == E_FAIL) LOG_MGR->AddLog("VertexColorPS Fail");

		SAFE_RELEASE(pShaderBlob);
	}
	// vp를 넘겨줄  컨스턴트 버퍼 생성

	if (m_pConstantBuf == nullptr)
	{	

		// 컨스턴트 버퍼 생성
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


	// 그리드 생성
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
	vb.BindFlags = D3D11_BIND_VERTEX_BUFFER; // 정점버퍼, 인덱스 버퍼인지의 플래그
	vb.Usage = D3D11_USAGE_DEFAULT; // 기본값
	vb.ByteWidth = m_gridSize * sizeof(ST_GIZMO_VB_BUF); // 정점 버퍼의 크기 단위 byte
	vb.CPUAccessFlags = 0; // cpu가 나중에 버퍼의 내용에 손댈것이냐에 대한 플래그 
	vb.MiscFlags = 0; // 정점버퍼에서는 무쓸모
	vb.StructureByteStride = 0; // 구조적 버퍼에 저장된 원소 하나의 크기 

	D3D11_SUBRESOURCE_DATA vInitData;
	ZeroMemory(&vInitData, sizeof(vInitData));
	vInitData.pSysMem = vecBuf.data();// 정점 버퍼를 초기화할 자료를 담은 시슽템 메모리 배열 포인터

	hr = g_DeviceManager->GetDevice()->CreateBuffer(&vb, &vInitData, &m_pGridBuf); // 실제 버퍼 생성
	vecBuf.clear();


	vb.BindFlags = D3D11_BIND_VERTEX_BUFFER; // 정점버퍼, 인덱스 버퍼인지의 플래그
	vb.Usage = D3D11_USAGE_DYNAMIC; // 읽고 쓸건데
	vb.ByteWidth = 2 * sizeof(ST_GIZMO_VB_BUF); // 정점 버퍼의 크기 단위 byte
	vb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE; // cpu가 나중에 버퍼의 내용에 손댈것이냐에 대한 플래그 
	vb.MiscFlags = 0; // 정점버퍼에서는 무쓸모
	vb.StructureByteStride = 0; // 구조적 버퍼에 저장된 원소 하나의 크기 

	hr = g_DeviceManager->GetDevice()->CreateBuffer(&vb, &vInitData, &m_pLineBuf); // 실제 버퍼 생성

	if (hr == E_FAIL) LOG_MGR->AddLog("버텍스 버퍼 생성안됨");

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
	// 컨스턴트 버퍼 업데이트 (vp 정보 들고 와야함)
	g_DeviceManager->GetDC()->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_LINELIST); // 나 선그릴껀데 
	
	g_DeviceManager->GetDC()->RSSetState(g_DeviceManager->GetRasterizeStateWire()); // 와이어 렌더링

	g_DeviceManager->GetDC()->IASetInputLayout(this->m_pInputLayout); // 인풋레이아웃 설정

	g_DeviceManager->GetDC()->VSSetShader(this->m_pVertexShader, nullptr, 0);

	g_DeviceManager->GetDC()->PSSetShader(this->m_pPixelShader, nullptr, 0);

	g_DeviceManager->GetDC()->VSSetConstantBuffers(0, 1, &m_pConstantBuf); // 넣어준다 

	XMMATRIX VP = XMMatrixMultiply(XMLoadFloat4x4(&pCamera->GetViewMatrix()), XMLoadFloat4x4(&pCamera->GetProjection()));

	this->RenderLine(VP);

	this->RenderGrid(VP);

	g_DeviceManager->GetDC()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST); // 삼각형 렌더링

	UINT stride = sizeof(ST_GIZMO_VB_BUF); 

	UINT offset = 0;

	// 도형 정점 인덱스 버퍼 불러옴
	g_DeviceManager->GetDC()->IASetVertexBuffers(
		0, // 버텍스 버퍼 슬롯의 개수
		1, // 사용할 버텍스 버퍼의 개수
		&m_pFigureBuf, // 버텍스 버퍼들의 포인터 ** 인데 포인터의 베열로 생각하면 편하다
		&stride, // 보폭 구조체의 포인터
		&offset // 시작지점의 포인터
	);

	g_DeviceManager->GetDC()->IASetIndexBuffer(
		m_pIndexBuf,
		DXGI_FORMAT_R32_UINT, //  unsigned int 를 쓰겠다
		0
	);

	this->RenderBox(VP);

	this->RenderCylinder(VP);

	this->RenderSphere(VP);
}

void cGizmoManager::RenderGrid(XMMATRIX & matVP)
{
	g_DeviceManager->GetDC()->Map(m_pConstantBuf, 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedResource); // 일단 연다 

	ST_GIZMO_CB* pConstant = static_cast<ST_GIZMO_CB*>(MappedResource.pData); // 데이터의 포인터를 넘겨준다

	XMStoreFloat4x4(&pConstant->matCB, XMMatrixTranspose(matVP));

	g_DeviceManager->GetDC()->Unmap(m_pConstantBuf, 0);  // 닫는다

	UINT stride = sizeof(ST_GIZMO_VB_BUF);

	UINT offset = 0;

	g_DeviceManager->GetDC()->IASetVertexBuffers(
		0, // 버텍스 버퍼 슬롯의 개수
		1, // 사용할 버텍스 버퍼의 개수
		&m_pGridBuf, // 버텍스 버퍼들의 포인터 ** 인데 포인터의 베열로 생각하면 편하다
		&stride, // 보폭들의 배열의 첫원소를 가리키는 포인터
		&offset // 시작지점 
	);

	g_DeviceManager->GetDC()->Draw(m_gridSize, 0); // 일반 드로우 함수 
}

void cGizmoManager::RenderLine(XMMATRIX & matVP)
{
	g_DeviceManager->GetDC()->Map(m_pConstantBuf, 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedResource); // 일단 연다 

	ST_GIZMO_CB* pConstant = (ST_GIZMO_CB*)(MappedResource.pData); // 데이터의 포인터를 넘겨준다

	XMStoreFloat4x4(&pConstant->matCB, XMMatrixTranspose(matVP));

	g_DeviceManager->GetDC()->Unmap(m_pConstantBuf, 0);  // 닫는다

	UINT stride = sizeof(ST_GIZMO_VB_BUF);

	UINT offset = 0;

	// 도형 정점 인덱스 버퍼 불러옴
	g_DeviceManager->GetDC()->IASetVertexBuffers(
		0, // 버텍스 버퍼 슬롯의 개수
		1, // 사용할 버텍스 버퍼의 개수
		&m_pLineBuf, // 버텍스 버퍼들의 포인터 ** 인데 포인터의 베열로 생각하면 편하다
		&stride, // 보폭 구조체의 포인터
		&offset // 시작지점의 포인터
	);

	list<ST_GIZMO_VB_BUF>::iterator it = m_listLines.begin();

	for (it; it != m_listLines.end();) // 박스 
	{
		g_DeviceManager->GetDC()->Map(m_pLineBuf, 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedResource); // 일단 연다 
		
		ST_GIZMO_VB_BUF * ppData = static_cast<ST_GIZMO_VB_BUF*>(MappedResource.pData);

		ppData[0].Position =it->Position;
		ppData[0].Color = it->Color;

		++it;

		ppData[1].Position = it->Position;
		ppData[1].Color = it->Color;
				 
		++it;

		g_DeviceManager->GetDC()->Unmap(m_pLineBuf, 0);  // 닫는다

		g_DeviceManager->GetDC()->Draw(2, 0); // draw
	}

	m_listLines.clear();
}

void cGizmoManager::RenderBox(XMMATRIX & matVP)
{
	transform_iterator = m_listBoxTransform.begin();

	for (transform_iterator; transform_iterator != m_listBoxTransform.end(); ++transform_iterator) // 박스 
	{
		g_DeviceManager->GetDC()->Map(m_pConstantBuf, 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedResource); // 일단 연다 
		
		ST_GIZMO_CB* pConstant = static_cast<ST_GIZMO_CB*>(MappedResource.pData); // 데이터의 포인터를 넘겨준다

		XMStoreFloat4x4(&pConstant->matCB, XMMatrixTranspose((*transform_iterator)->GetFinalMatrix() * matVP));

		g_DeviceManager->GetDC()->Unmap(m_pConstantBuf, 0);  // 닫는다

		g_DeviceManager->GetDC()->DrawIndexed(m_boxIndexSize, m_boxlocateIndics, m_boxlocateVertics); // drawindexed
	}

}

void cGizmoManager::RenderCylinder(XMMATRIX & matVP)
{
	transform_iterator = m_listCylinderTransform.begin();

	for (transform_iterator; transform_iterator != m_listCylinderTransform.end(); ++transform_iterator) // 박스 
	{
		g_DeviceManager->GetDC()->Map(m_pConstantBuf, 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedResource); // 일단 연다 
		
		ST_GIZMO_CB* pConstant = static_cast<ST_GIZMO_CB*>(MappedResource.pData); // 데이터의 포인터를 넘겨준다

		XMStoreFloat4x4(&pConstant->matCB, XMMatrixTranspose((*transform_iterator)->GetFinalMatrix() * matVP));

		g_DeviceManager->GetDC()->Unmap(m_pConstantBuf, 0);  // 닫는다

		g_DeviceManager->GetDC()->DrawIndexed(m_cylinderIndexSize, m_cylinderlocateIndics,m_cylinderlocateVertics ); // drawindexed
	}
}

void cGizmoManager::RenderSphere(XMMATRIX & matVP)
{
	transform_iterator = m_listSsphereTransform.begin();

	for (transform_iterator; transform_iterator != m_listSsphereTransform.end(); ++transform_iterator) // 박스 
	{
		g_DeviceManager->GetDC()->Map(m_pConstantBuf, 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedResource); // 일단 연다 
		
		ST_GIZMO_CB* pConstant = static_cast<ST_GIZMO_CB*>(MappedResource.pData); // 데이터의 포인터를 넘겨준다

		XMStoreFloat4x4(&pConstant->matCB, XMMatrixTranspose((*transform_iterator)->GetFinalMatrix() * matVP));

		g_DeviceManager->GetDC()->Unmap(m_pConstantBuf, 0);  // 닫는다

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

	// 사각형
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
	// 원기둥

	// 실린더 생성 가로를 다 나눈다

	// 시작 인덱스 

	m_cylinderlocateVertics = VB.size();
	m_cylinderlocateIndics = IB.size();



	static const unsigned int sliceTop = 30;

	static const XMFLOAT4 color = XMFLOAT4(0, 1, 0, 1);

	XMFLOAT3 topCircleCenter = XMFLOAT3(0, 0.5f, 0);

	VB.push_back(ST_GIZMO_VB_BUF(topCircleCenter, color)); // 시작점 저장

	float deltaRad = XMConvertToRadians(static_cast<float>(360 / sliceTop));

	for (int i = 0; i < sliceTop; ++i)
	{
		topCircleCenter.x = cosf(deltaRad * i) * 0.5f;
		topCircleCenter.z = sinf(deltaRad * i) * 0.5f;

		LOG_MGR->AddLog("%f,%f,%f",topCircleCenter.x, topCircleCenter.y, topCircleCenter.z);

		VB.push_back(ST_GIZMO_VB_BUF(topCircleCenter, color)); // 탑 정점넣고
	}

	// 바텀 정점 생성
	topCircleCenter = XMFLOAT3(0, -0.5f, 0);
	VB.push_back(ST_GIZMO_VB_BUF(topCircleCenter, color)); // 시작점 저장
	for (int i = 0; i < sliceTop; ++i)
	{
		topCircleCenter.z = sinf(deltaRad * i) * 0.5f;
		topCircleCenter.x = cosf(deltaRad * i) * 0.5f;

		VB.push_back(ST_GIZMO_VB_BUF(topCircleCenter, color));
	}

	// 인덱스 순서

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
		IB.push_back(0); // 시작지점
		IB.push_back(0 + i + 1);
		IB.push_back(0 + i);

		// 바텀에 원의 삼각형 n - 1개
		IB.push_back(bottomStartIndex); // 시작지점
		IB.push_back(bottomStartIndex + i);
		IB.push_back(bottomStartIndex + i + 1);

		// 사각형 그리기

		// 옆면 만들기
		
		/*
		하나의 단면
		i					i + 1

		i + sliceTop + 1  i + sliceTop + 2
		*/

		IB.push_back(i);
		IB.push_back(i + 1);
		IB.push_back(i + sliceTop + 1);
					
		IB.push_back(i + 1); // 시작지점
		IB.push_back(i + sliceTop + 2);
		IB.push_back(i + sliceTop + 1);
	}

	//// 탑에 원의 삼각형 n개
	IB.push_back(0); // 시작지점
	IB.push_back(1);
	IB.push_back(sliceTop);
	
	// 바텀에 원의 삼각형 n개
	IB.push_back(sliceTop + 1); // 시작지점
	IB.push_back(sliceTop * 2 + 1);
	IB.push_back(sliceTop + 2);

	/*
		단면 끝
		sliceTop		1

		sliceTop * 2 + 2  sliceTop + 2
	*/

	// 옆면 사각형 n 개
	IB.push_back(1);
	IB.push_back(sliceTop + 2);
	IB.push_back(sliceTop);

	IB.push_back(sliceTop); // 시작지점
	IB.push_back(sliceTop + 2);
	IB.push_back(sliceTop * 2 + 1);
	m_cylinderIndexSize = IB.size() - m_cylinderlocateIndics;
}

void cGizmoManager::CreateSphere(vector<ST_GIZMO_VB_BUF>& VB, vector<DWORD>& IB)
{
	m_spherelocateVertics = VB.size();
	m_spherelocateIndics = IB.size();
	// 측지구 생성

	const UINT SubDivideNum = 5; // 세분화 번호 5번 이상 넘지 않도록한다
	const  float x = 0.525731f; // 정십이면체 테셀레이션 해서 구를 근사
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
	// 도형 생성 반 시계 방향

	vector<DWORD> vecIndex;
	vector<ST_GIZMO_VB_BUF> vecBuf;

	this->CreateBox(vecBuf, vecIndex);
	this->CreateCylinder(vecBuf, vecIndex);
	this->CreateSphere(vecBuf, vecIndex);

	//  선을 그리기 위한 정점 두개 추가 

	UINT lineVertexLocate, lineIndexLocate;
	lineVertexLocate = vecBuf.size();
	lineIndexLocate = vecIndex.size();
	
	vecIndex.push_back(vecBuf.size());
	vecIndex.push_back(vecBuf.size() + 1);
	
	// z축으로 길이가 1 인 선을 미리 추가

	vecBuf.push_back(ST_GIZMO_VB_BUF(XMFLOAT3(0, 0, -0.5f), XMFLOAT4(0, 0, 1, 0)));
	vecBuf.push_back(ST_GIZMO_VB_BUF(XMFLOAT3(0, 0, 0.5f), XMFLOAT4(0, 0, 1, 0)));

	D3D11_BUFFER_DESC vb;
	vb.BindFlags = D3D11_BIND_VERTEX_BUFFER; // 정점버퍼, 인덱스 버퍼인지의 플래그
	vb.Usage = D3D11_USAGE_DEFAULT; // 기본값
	vb.ByteWidth = vecBuf.size() * sizeof(ST_GIZMO_VB_BUF); // 정점 버퍼의 크기 단위 byte
	vb.CPUAccessFlags = 0; // cpu가 나중에 버퍼의 내용에 손댈것이냐에 대한 플래그 
	vb.MiscFlags = 0; // 정점버퍼에서는 무쓸모
	vb.StructureByteStride = 0; // 구조적 버퍼에 저장된 원소 하나의 크기 

	D3D11_SUBRESOURCE_DATA vInitData;
	ZeroMemory(&vInitData, sizeof(vInitData));
	vInitData.pSysMem = vecBuf.data();// 정점 버퍼를 초기화할 자료를 담은 시슽템 메모리 배열 포인터

	HRESULT hr = g_DeviceManager->GetDevice()->CreateBuffer(&vb, &vInitData, &m_pFigureBuf); // 실제 버퍼 생성
	if (hr == E_FAIL) { LOG_MGR->AddLog("Figure vertex Buffer Fail"); }
	D3D11_BUFFER_DESC ib;

	ib.BindFlags = D3D11_BIND_INDEX_BUFFER; // 정점버퍼, 인덱스 버퍼인지의 플래그
	ib.Usage = D3D11_USAGE_IMMUTABLE; // 기본값
	ib.ByteWidth = vecIndex.size() * sizeof(DWORD); // 정점 버퍼의 크기 단위 byte
	ib.CPUAccessFlags = 0; // cpu가 나중에 버퍼의 내용에 손댈것이냐에 대한 플래그 
	ib.MiscFlags = 0; // 정점버퍼에서는 무쓸모
	ib.StructureByteStride = 0; // 구조적 버퍼에 저장된 원소 하나의 크기 

	ZeroMemory(&vInitData, sizeof(vInitData));
	vInitData.pSysMem = vecIndex.data();// 정점 버퍼를 초기화할 자료를 담은 시슽템 메모리 배열 포인터

	hr = g_DeviceManager->GetDevice()->CreateBuffer(&ib, &vInitData, &m_pIndexBuf); // 실제 버퍼 생성
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



