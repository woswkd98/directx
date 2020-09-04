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
	ID3D11Buffer * m_pConstantBuf = nullptr; // ���̴��ʿ��� �ʿ��� ������Ʈ ����
	ID3D11Buffer * m_pLineBuf = nullptr; // ���ؽ� ����
	ID3D11Buffer * m_pGridBuf = nullptr;
	ID3D11Buffer * m_pFigureBuf = nullptr; // �ڽ� �� ĸ�� ����� ���� �׸��� �� ������� 
	ID3D11Buffer * m_pIndexBuf = nullptr; // �ε��� ����


	// �������� ���� ������Ʈ�� ���߿� ���⼭ Ʈ������ �����ͼ� 
	// ��Ʈ���������� �̵� ȸ�� ũ�� ���� ��Ʈ���� ������ ���⼭ ������ ������Ʈ�� �⺻������ �����̴� ������Ʈ�̹Ƿ�
	//��ο� ���� �������Ѵ� (������ ����� wire�� �ؼ� �浹ǥ�� ������ �� ����)
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
	void Init(float sizeX, float sizeZ, int indexX , int indexZ); // �⺻���� ���̴� �ʱ�ȭ �� �׸��� ���� ���� �׸���� ���״�� ���̱����� �׸��� �̴� 
	//���߿� ���� Ʈ���� ���� �׸��带 ����� ���� �����̴� (terrain)
	 


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