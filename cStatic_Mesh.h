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
	// ����ƽ �޽ô� ������ �ִϸ��̼ǵ��� ���Ѵ� �׷��� ������ �� ���� ����������Ʈ���� 
	// �Ѱ��� ������ �̵� ȸ������ ���ۿ� �̸� ������ѳ���(ȸ���̳� �̵� �ؽ��� ���� �ǽð����� �ٲ��� �ʴ´ٴ� �����Ͽ� )
	// ������ ��ο� ���� ������ �ִµ� �̷��� �� ����� ������ִ� ��������� �ν��Ͻ��̴� 

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

