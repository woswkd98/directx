#include "stdafx.h"
#include "cMeshManager.h"


void cMeshManager::InsertBuffer(vector<XMFLOAT3> & pos,
	vector<XMFLOAT3> & normal,
	vector<XMFLOAT2> & uv,
	string * name,
	vector<WORD>* indices)
{
	ID3D11Buffer * pVB;
	ID3D11Buffer * pIB;

	cMesh_Node * newMesh = new cMesh_Node;

	newMesh->name = *name;


	vector<Vertex1> vVertex;
	vVertex.resize(pos.size()); // 생성 시킨다

	vector<XMFLOAT3> tempNormal;
	vector<XMFLOAT3> binormal;
	vector<XMFLOAT4> vtangent;
	ComputeTangent(pos, normal, uv, &vtangent, &binormal);

	for (int i = 0; i < vVertex.size(); ++i)
	{
		vVertex[i].pos = pos[i];
		vVertex[i].normal = normal[i];
		vVertex[i].UV = uv.begin()[i];
		vVertex[i].tangent = XMFLOAT3(vtangent[i].x, vtangent[i].y, vtangent[i].z);
		vVertex[i].binormal = binormal[i];
	}


	D3D11_BUFFER_DESC vb;
	vb.BindFlags = D3D11_BIND_VERTEX_BUFFER; // 정점버퍼, 인덱스 버퍼인지의 플래그
	vb.Usage = D3D11_USAGE_DEFAULT; // 버퍼가 쓰이는 용도 대체로 readonly형식을 선호하겠지 (gpu전용)
	vb.ByteWidth = sizeof(Vertex1) * vVertex.size(); // 정점 버퍼의 크기 단위 byte
	vb.CPUAccessFlags = 0; // cpu가 나중에 버퍼의 내용에 손댈것이냐에 대한 플래그 
	vb.MiscFlags = 0; // 정점버퍼에서는 무쓸모
	vb.StructureByteStride = 0; // 구조적 버퍼에 저장된 원소 하나의 크기 

	D3D11_SUBRESOURCE_DATA vInitData;
	ZeroMemory(&vInitData, sizeof(vInitData));
	vInitData.pSysMem = vVertex.data();// 정점 버퍼를 초기화할 자료를 담은 시슽템 메모리 배열 포인터

	HRESULT hr = m_pDevice->CreateBuffer(&vb, &vInitData, &pVB); // 실제 버퍼 생성
	m_vIB.push_back(pVB);
	if (hr == E_FAIL) LOG_MGR->AddLog("버텍스 버퍼 생성안됨");

	if (indices == nullptr) return;



	D3D11_BUFFER_DESC ib;

	ib.BindFlags = D3D11_BIND_INDEX_BUFFER; // 정점버퍼, 인덱스 버퍼인지의 플래그
	ib.Usage = D3D11_USAGE_DEFAULT; // 버퍼가 쓰이는 용도 대체로 readonly형식을 선호하겠지 (gpu전용)
	ib.ByteWidth = sizeof(WORD) * indices->size(); // 버퍼의 크기 단위 byte
	ib.CPUAccessFlags = 0; // cpu가 나중에 버퍼의 내용에 손댈것이냐에 대한 플래그 
	ib.MiscFlags = 0; // 정점버퍼에서는 무쓸모
	ib.StructureByteStride = 0; // 구조적 버퍼에 저장된 원소 하나의 크기 


	ZeroMemory(&vInitData, sizeof(vInitData));
	vInitData.pSysMem = indices->data();// 인덱스 버퍼를 초기화할 자료를 담은 시슽템 메모리 배열 포인터

	//AllIndexNum = indices->size();

	//indexNum = AllIndexNum;

	hr = m_pDevice->CreateBuffer(&ib, &vInitData, &pIB); // 실제 버퍼 생성

	m_vIB.push_back(pIB);

	if (hr == E_FAIL) LOG_MGR->AddLog("인덱스 버퍼 생성안됨");
}

void cMeshManager::Init(ID3D11Device * pDevice)
{
	m_pDevice = pDevice;
}





/*
cMeshFilter * cMeshManager::BoxGenerator(vector<XMFLOAT3>* outRefVecMesh, vector<WORD>* outIndices, string * outMeshName)
{
	if (outMeshName)
		*outMeshName = "cube";
	cMeshFilter * p = this->GetMeshFilterFromName("cube");

	if (p != nullptr) return p;

	vector<XMFLOAT3> mesh;

	mesh.push_back(XMFLOAT3(-1.0f, 1.0f, -1.0f));
	mesh.push_back(XMFLOAT3(1.0f, 1.0f, -1.0f));
	mesh.push_back(XMFLOAT3(1.0f, 1.0f, 1.0f));
	mesh.push_back(XMFLOAT3(-1.0f, 1.0f, 1.0f));
	mesh.push_back(XMFLOAT3(-1.0f, -1.0f, -1.0f));
	mesh.push_back(XMFLOAT3(1.0f, -1.0f, -1.0f));
	mesh.push_back(XMFLOAT3(1.0f, -1.0f, 1.0f));
	mesh.push_back(XMFLOAT3(-1.0f, -1.0f, 1.0f));
	if (outRefVecMesh)
		outRefVecMesh = &mesh;
	vector<WORD> indices;

	indices.push_back(3); 	indices.push_back(1); 	indices.push_back(0);
	indices.push_back(2);	indices.push_back(1); 	indices.push_back(3);

	indices.push_back(0); 	indices.push_back(5); 	indices.push_back(4);
	indices.push_back(1); 	indices.push_back(5);  	indices.push_back(0);

	indices.push_back(3); 	indices.push_back(4); 	indices.push_back(7);
	indices.push_back(0); 	indices.push_back(4); 	indices.push_back(3);

	indices.push_back(1); 	indices.push_back(6); 	indices.push_back(5);
	indices.push_back(2); 	indices.push_back(6); 	indices.push_back(1);

	indices.push_back(2); 	indices.push_back(7); 	indices.push_back(6);
	indices.push_back(3); 	indices.push_back(7); 	indices.push_back(2);

	indices.push_back(6); 	indices.push_back(4); 	indices.push_back(5);
	indices.push_back(7); 	indices.push_back(4); 	indices.push_back(6);

	if (outIndices)
		outIndices = &indices;
	string name = "cube";
	p = new cMeshFilter();
	//p->Init(&mesh, NULL, NULL, NULL, NULL, &name, &indices);
	this->insertMeshMap("cube", p);

	return p;

	
}

*/

void cMeshManager::QuadGenerator()
{
	vector<XMFLOAT3> mesh;
	mesh.push_back(XMFLOAT3(-0.5f, 0, -0.5f)); // 왼쪽 하단
	mesh.push_back(XMFLOAT3(0.5f, 0, -0.5f)); // 오른쪽하단
	mesh.push_back(XMFLOAT3(-0.5f, 0, 0.5f)); // 왼쪽상단
	mesh.push_back(XMFLOAT3(0.5f, 0, 0.5f)); // 오른쪽 상단

	vector<WORD> indices;
	indices.push_back(3); 	indices.push_back(1); 	indices.push_back(0);
	indices.push_back(2);	indices.push_back(1); 	indices.push_back(3);
	

}

void cMeshManager::Release()
{
	for (int i = 0; i < m_vVB.size(); ++i)
	{
		m_vVB[i]->Release();
		SAFE_DELETE(m_vVB[i]);
	}

	for (int i = 0; i < m_vIB.size(); ++i)
	{
		m_vIB[i]->Release();
		SAFE_DELETE(m_vIB[i]);
	}
}

cMeshManager::cMeshManager()
{
	Release();
}


cMeshManager::~cMeshManager()
{
}
