#pragma once
#include "cSingletonBase.h"
class cGameObject;
class cTransform;
class cGizmoManager :public cSingletonBase<cGizmoManager>
{
private:

private:
	ID3D11VertexShader * m_pVertexShader = nullptr; 
	ID3D11PixelShader * m_pPixelShader = nullptr;
	ID3D11Buffer * m_pConstantBuf = nullptr; // 셰이더쪽에서 필요한 컨스턴트 버퍼
	ID3D11Buffer * m_pLineBuf = nullptr; // 버텍스 버퍼
	ID3D11Buffer * m_pGridBuf = nullptr;
	ID3D11Buffer * m_pFigureBuf = nullptr; // 박스 구 캡슐 원기둥 등을 그릴때 쓸 저장버퍼 
	ID3D11Buffer * m_pIndexBuf = nullptr; // 인덱스 버퍼


	// 도형들의 게임 오브젝트들 나중에 여기서 트랜스폼 가져와서 
	// 그트랜스폼으로 이동 회전 크기 변경 매트릭스 가져옴 여기서 저장한 오브젝트는 기본적으로 움직이는 오브젝트이므로
	//드로우 콜을 여러번한다 (레스터 라이즈를 wire로 해서 충돌표시 용으로 쓸 예정)
	list<cTransform*> m_listBoxTransform;
	list<cTransform*> m_listSsphereTransform;
	list<cTransform*> m_listCapsuleTransform;
	list<cTransform*> m_listCylinderTransform;
	list<cTransform*> m_listTerrainTransform;

	list<cTransform*>::iterator transform_iterator;

	struct ST_GIZMO_VB_BUF
	{
		XMFLOAT3 Position;
		XMFLOAT4 Color;

		ST_GIZMO_VB_BUF(XMFLOAT3 pos, XMFLOAT4 color) : Position(pos), Color(color) {}

		ST_GIZMO_VB_BUF() {
			ZeroMemory(this, sizeof(ST_GIZMO_VB_BUF));
		}
	};

	struct ST_GIZMO_CB
	{
		XMFLOAT4X4 matCB;

		ST_GIZMO_CB() {
			ZeroMemory(this, sizeof(ST_GIZMO_CB));
		}
	};
	D3D11_MAPPED_SUBRESOURCE MappedResource;

	ID3D11InputLayout * m_pInputLayout = nullptr;

private:
	vector<ST_GIZMO_VB_BUF> m_vecLines;
	list<ST_GIZMO_VB_BUF> m_listLines;
	int m_gridSize = 0;
private:
	unsigned int m_boxlocateVertics;
	unsigned int m_boxlocateIndics;
	unsigned int m_boxIndexSize;

	unsigned int m_spherelocateVertics;
	unsigned int m_spherelocateIndics;
	unsigned int m_sphereIndexSize;

	unsigned int m_cylinderlocateVertics;
	unsigned int m_cylinderlocateIndics;
	unsigned int m_cylinderIndexSize;
public:

public:
	ID3D11Buffer * GetFigureVB() { return m_pFigureBuf; }

	ID3D11Buffer * GetFigureIB() { return m_pIndexBuf; }

	unsigned int GetBoxLocVertices() { return m_boxlocateVertics; }

	unsigned int GetBoxLocIndics() { return m_boxlocateIndics; }

	unsigned int GetBoxLocIdxSize() { return m_boxIndexSize; }

	unsigned int GetSphereLocVertices() { return m_spherelocateVertics; }

	unsigned int GetSphereLocIndics() { return m_spherelocateIndics; }

	unsigned int GetSphereLocIdxSize() { return m_sphereIndexSize; }

	unsigned int GetCylinderLocVertices() { return m_cylinderlocateVertics; }
					
	unsigned int GetCylinderLocIndics() { return m_cylinderlocateIndics; }
					
	unsigned int GetCylinderLocIdxSize() { return m_cylinderIndexSize; }
public:
	void Init(float sizeX, float sizeZ, int indexX , int indexZ); // 기본적인 셰이더 초기화 및 그리드 버퍼 생성 그리드는 말그대로 보이기위한 그리드 이다 
	//나중엔 따로 트라이 엥글 그리드를 만들어 놓고 쓸것이다 (terrain)
	 


	void Release();

	void RenderGizmo(cCamera * pMainCamera);


private:
	

	void RenderGrid(XMMATRIX & matVP);

	void RenderLine(XMMATRIX & matVP);

	void RenderBox(XMMATRIX & matVP);

	void RenderCylinder(XMMATRIX & matVP);

	void RenderSphere(XMMATRIX & matVP);

	void RenderTerrain(XMMATRIX & matVP);

	void CreateFigure();

	void CreateBox(vector<ST_GIZMO_VB_BUF> & VB, vector<DWORD> & IB);

	void CreateCylinder(vector<ST_GIZMO_VB_BUF> & VB, vector<DWORD> & IB);

	void CreateSphere(vector<ST_GIZMO_VB_BUF> & VB, vector<DWORD> & IB);
public: 
	void AddBox(cTransform * transform);

	void AddSphere(cTransform * transform);

	void AddCylinder(cTransform * transform);

	void AddTerrain(cTransform * transform);

	void AddLine(XMFLOAT3 v1, XMFLOAT3 v2, XMFLOAT4 color);


	cGizmoManager();
	~cGizmoManager();
};

#define g_GizmoManager cGizmoManager::GetInstance()