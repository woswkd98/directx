#pragma once


#include "cSingletonBase.h"


class cFBXFileLoader : public  cSingletonBase<cFBXFileLoader>
{
private:
	FbxManager * m_pFbxSdkManager = nullptr;

	map<string, FbxNode*> m_mapModelRootNode; // ��Ʈ�� �����Ѵ� 
	map<string, FbxSurfaceMaterial*> m_mapMaterial;

private:
	void GetVertexUV(FbxMesh *pMesh, vector<XMFLOAT2> * vecUV, vector<XMFLOAT3> * Vertex,vector<WORD> * index, int indexCount);
	void GetNormal(FbxMesh *pMesh, vector<XMFLOAT3> * vecNormal);
	void GetBinorm(FbxMesh *pMesh, vector<XMFLOAT3> * vecBinorm);
	void GetTangent(FbxMesh *pMesh, vector<XMFLOAT3> * vecTangent);
public:
	void Init();
	FbxManager * GetFBXManagerPointer() { return m_pFbxSdkManager; }

	// ���ϳ��� ������ �̸�, nameSet ���� �ٿ��� �̸�, ��� 
	vector<ST_VERTEX> * GetStaticMesh(char * nameSet , char * path, vector<WORD> outIndex);


	void Release();
public:
	cFBXFileLoader();
	~cFBXFileLoader();
};
#define g_FBXFiles cFBXFileLoader::GetInstance()