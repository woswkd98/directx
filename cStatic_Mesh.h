#pragma once

class cCamera;
class cGameObject;
class cStatic_Mesh : public cMonoBehavior
{
private:

private:

	cMaterial * material;

	ID3D11VertexShader * m_pVertexShader = nullptr;

	ID3D11PixelShader * m_pPixelShader = nullptr;

	ID3D11HullShader * m_pHullShader = nullptr;

	ID3D11DomainShader * m_pDomainShader = nullptr;

	ID3D11GeometryShader * m_pGeometryShader = nullptr;

	ID3D11Buffer * m_pVertexCB = nullptr;

	ID3D11Buffer * m_pPixelCB = nullptr;

	ID3D11Buffer * m_pHullCB = nullptr;
	
	ID3D11Buffer * m_pDomainCB = nullptr;
	
	ID3D11Buffer * m_pGeometryCB = nullptr;

	//vector<cGameObject*> m_listInstancedDatas;

public:
	void Awake();
	// 스테틱 메시는 동적인 애니메이션등을 안한다 그래서 나무나 집 같은 같은오브젝트등을 
	// 한개를 가지고 이동 회전등을 버퍼에 미리 저장시켜놓고(회전이나 이동 텍스쳐 등이 실시간으로 바뀌지 않는다는 가정하에 )
	// 여러번 드로우 콜을 할일이 있는데 이럴때 그 기능을 대신해주는 렌더기능이 인스턴싱이다 

	void SetInstancingRenderer()
	{
	
	}

	void RenderFrame(XMMATRIX & VP);

	void SetVertexShader(ID3D11VertexShader * vertexShader = nullptr, ID3D11Buffer * buf = nullptr)
	{
		m_pVertexShader = vertexShader;
		m_pVertexCB = buf;

	}

	void SetPixelShader(ID3D11PixelShader * pixelShader = nullptr, ID3D11Buffer * buf = nullptr)
	{
		m_pPixelShader = pixelShader;
		m_pPixelCB = buf;
	}

	void SetHullShader(ID3D11HullShader * HullShader = nullptr, ID3D11Buffer * buf = nullptr)
	{
		m_pHullShader = HullShader;
		m_pHullCB = buf;
	}

	void SetDomainShader(ID3D11DomainShader * domainShader = nullptr, ID3D11Buffer * buf = nullptr)
	{
		m_pDomainShader = domainShader;
		m_pDomainCB = buf;
	}

	void SetGeometryShader(ID3D11GeometryShader * geometryShader = nullptr, ID3D11Buffer * buf = nullptr)
	{
		m_pGeometryShader = geometryShader;
		m_pGeometryCB = buf;
	}


public:
	cStatic_Mesh();
	~cStatic_Mesh();
};

