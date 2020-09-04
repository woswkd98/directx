#pragma once
class cScene;
#include "cSingletonBase.h"
class cSceneManager :public cSingletonBase<cSceneManager>
{
public:

public:
	static map<string, cGameObject*> m_mapPrefabs; // ��� �̸� ����� ���� ���ӿ�����Ʈ ������ �� ���⿡ �����Ѵ�
private:
	vector<ID3D11Buffer*> m_vVB, m_vIB;

	map<string, POINT> m_mapMeshStore;

private:
	
public:
	vector<ID3D11Buffer*> & GetRefVecVB() { return m_vVB; }
	vector<ID3D11Buffer*> & GetRefVecIB() { return m_vIB; }

public:
	void InsertBuffer(char * nameSet, char * path);

	cScene * m_pCurrentScene = nullptr;

	cScene * m_pPreScene = nullptr;

	vector<cScene*> m_vecScene;
private:
	float backgroudColor[4];

	bool m_bShowHud = true;
	bool m_bVisualizeGBuffer = false;
	bool m_bVisualizeLightVolume = false;

	bool m_bRenderGizmo = true;

	ID3D11Buffer* m_pSceneVertexShaderCB;
	ID3D11Buffer* m_pScenePixelShaderCB;

	ID3D11VertexShader* m_pSceneVertexShader;
	ID3D11InputLayout* m_pSceneVSLayout;
	ID3D11PixelShader* m_pScenePixelShader;



private:
	void SetPrefabs();
public:

	void ChangeScene(int index);

	void SetUp(cScene * pScene);

	void InsertScene(int index, cScene * pScene)
	{

		m_vecScene.push_back(pScene);

	}

	cScene * GetCurrentScene()
	{
		return m_pCurrentScene;
	}

	void Update(float deltaTime);

	void Render();

	static cGameObject *GetPrefabs(string str)
	{
		
		/*
		map<string, cGameObject*>::iterator it = m_mapPrefabs.find(str);

		
		if(it != m_mapPrefabs.end())
		{
			return it->second;
		}
		*/
		return nullptr;
	}

	void Init(int index); // ùȭ�� �̴�
	void Release(); // �ʻ��� 

	cSceneManager();
	~cSceneManager();
};

#define g_SceneManager cSceneManager::GetInstance()