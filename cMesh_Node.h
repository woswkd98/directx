#pragma once
class cMesh_Node : public cMonoBehavior
{
private:
	cMaterial * material;
	
public:
	void Awake() override;

	string name; // �̸�

	UINT bufferIndex; // ���ؽ� ������ ���� �ε���

	// draw Indexed �� �ʿ��� �δ콺 ���� 
	UINT indexCount = 0;
	UINT locateIndex = 0;
	UINT locateVertex = 0;

	void RenderFrame(ID3D11DeviceContext * pDC, vector<ID3D11Buffer*> * VertexInfo, vector<ID3D11Buffer*> *indexInfo);

public:
	cMesh_Node();
	~cMesh_Node();
};

