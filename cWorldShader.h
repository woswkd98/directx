#pragma once
class cWorldShader 
{
private:
	struct WVP
	{
		XMMATRIX VP;
		XMMATRIX World;
	};

	WVP wvp;

	ID3D11VertexShader * pVS = nullptr; // depth prepass 

	ID3D11VertexShader * pVS_RenderScene = nullptr; // SceneRender

	ID3D11InputLayout * m_vsInputlayout = nullptr; // depth prepass 

	ID3D11InputLayout * m_vsRenderSceneInputLayOut = nullptr;// SceneRender

	ID3D11Buffer * pVSConstantBuffer = nullptr; 
public:
	void SetVP(XMMATRIX & mat) { wvp.VP = mat; };
	void SetWorldMat(XMMATRIX & mat) { wvp.World = mat; };
	void WorldShaderUpdate();
	void RenderSceneUpdate();
	void Init();
	void InitRenderSceneShader();
	cWorldShader();
	~cWorldShader();
};

