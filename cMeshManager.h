#pragma once
#include "cMesh_Node.h"

struct Vertex1
{
	XMFLOAT3 pos = XMFLOAT3(0, 0, 0);
	XMFLOAT3 normal = XMFLOAT3(0, 0, 0);
	XMFLOAT2 UV;
	XMFLOAT3 tangent;
	XMFLOAT3 binormal;
};


#include "cSingletonBase.h"
class cMeshManager :public cSingletonBase<cMeshManager>
{
private:
	vector<ID3D11Buffer*> m_vVB, m_vIB;

	vector<cMesh_Node*> m_ListMeshData;
	map<string, POINT> m_mapMeshStore;

	ID3D11Device *m_pDevice = nullptr;
public:
	vector<ID3D11Buffer*> & GetRefVecVB() { return m_vVB; }
	vector<ID3D11Buffer*> & GetRefVecIB() { return m_vIB; }

public:
	void InsertBuffer(vector<XMFLOAT3> & pos,
		vector<XMFLOAT3> & normal,
		vector<XMFLOAT2> & uv,
		string * name,
		vector<WORD>* indices);

	void Init(ID3D11Device * pDevice);

	void QuadGenerator();

	void Release();
private:
	cMeshManager();
	~cMeshManager();
};

#define g_meshManager cMeshManager::GetInstance()