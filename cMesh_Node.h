#pragma once
class cMesh_Node : public cMonoBehavior
{
private:
	cMaterial * material;
	
public:
	void Awake() override;

	string name; // 이름

	UINT bufferIndex; // 버텍스 정보를 담은 인덱스

	// draw Indexed 에 필요한 인댁스 버퍼 
	UINT indexCount = 0;
	UINT locateIndex = 0;
	UINT locateVertex = 0;

	void RenderFrame(ID3D11DeviceContext * pDC, vector<ID3D11Buffer*> * VertexInfo, vector<ID3D11Buffer*> *indexInfo);

public:
	cMesh_Node();
	~cMesh_Node();
};

