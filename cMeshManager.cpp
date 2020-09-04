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
	vVertex.resize(pos.size()); // ���� ��Ų��

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
	vb.BindFlags = D3D11_BIND_VERTEX_BUFFER; // ��������, �ε��� ���������� �÷���
	vb.Usage = D3D11_USAGE_DEFAULT; // ���۰� ���̴� �뵵 ��ü�� readonly������ ��ȣ�ϰ��� (gpu����)
	vb.ByteWidth = sizeof(Vertex1) * vVertex.size(); // ���� ������ ũ�� ���� byte
	vb.CPUAccessFlags = 0; // cpu�� ���߿� ������ ���뿡 �մ���̳Ŀ� ���� �÷��� 
	vb.MiscFlags = 0; // �������ۿ����� ������
	vb.StructureByteStride = 0; // ������ ���ۿ� ����� ���� �ϳ��� ũ�� 

	D3D11_SUBRESOURCE_DATA vInitData;
	ZeroMemory(&vInitData, sizeof(vInitData));
	vInitData.pSysMem = vVertex.data();// ���� ���۸� �ʱ�ȭ�� �ڷḦ ���� �Ú��� �޸� �迭 ������

	HRESULT hr = m_pDevice->CreateBuffer(&vb, &vInitData, &pVB); // ���� ���� ����
	m_vIB.push_back(pVB);
	if (hr == E_FAIL) LOG_MGR->AddLog("���ؽ� ���� �����ȵ�");

	if (indices == nullptr) return;



	D3D11_BUFFER_DESC ib;

	ib.BindFlags = D3D11_BIND_INDEX_BUFFER; // ��������, �ε��� ���������� �÷���
	ib.Usage = D3D11_USAGE_DEFAULT; // ���۰� ���̴� �뵵 ��ü�� readonly������ ��ȣ�ϰ��� (gpu����)
	ib.ByteWidth = sizeof(WORD) * indices->size(); // ������ ũ�� ���� byte
	ib.CPUAccessFlags = 0; // cpu�� ���߿� ������ ���뿡 �մ���̳Ŀ� ���� �÷��� 
	ib.MiscFlags = 0; // �������ۿ����� ������
	ib.StructureByteStride = 0; // ������ ���ۿ� ����� ���� �ϳ��� ũ�� 


	ZeroMemory(&vInitData, sizeof(vInitData));
	vInitData.pSysMem = indices->data();// �ε��� ���۸� �ʱ�ȭ�� �ڷḦ ���� �Ú��� �޸� �迭 ������

	//AllIndexNum = indices->size();

	//indexNum = AllIndexNum;

	hr = m_pDevice->CreateBuffer(&ib, &vInitData, &pIB); // ���� ���� ����

	m_vIB.push_back(pIB);

	if (hr == E_FAIL) LOG_MGR->AddLog("�ε��� ���� �����ȵ�");
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
	mesh.push_back(XMFLOAT3(-0.5f, 0, -0.5f)); // ���� �ϴ�
	mesh.push_back(XMFLOAT3(0.5f, 0, -0.5f)); // �������ϴ�
	mesh.push_back(XMFLOAT3(-0.5f, 0, 0.5f)); // ���ʻ��
	mesh.push_back(XMFLOAT3(0.5f, 0, 0.5f)); // ������ ���

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
