#pragma once



class cMonoBehavior;
class cMesh_Node;
class cScene;
#include "cTransform.h"
class cGameObject : public cObject
{
private:

private:
	 
public:
	cMesh_Node * pMesh_Node = nullptr;
	cTransform * pTransform = nullptr;
private:
	static ULONG Count;

	ULONG m_id;

public:
	virtual void SetActive(bool active); // ����ġ������ �� �ڽĵ��� ����ġ�� �¿��� ��Ų��

	virtual bool GetActive() { return m_bActive; }

	cTransform * GetTransform() {

		return pTransform; }
public:
	void BroadCastFunction(string str);
	

	template <typename T> // ������Ʈ �ֱ� (Ŭ���� Ÿ������ �������Ѽ�)
	T * AddComponent()
	{
		return this->GetTransform()->AddComponent<T>();
	}

	template <typename T> //  �����͵� ã�Ƽ� �������� (������ ������ ������Ʈ
	T * GetComponentInMyGameObject()
	{
		return	this->GetTransform()->GetComponentInMyGameObject<T>();
	}

	template <typename T> //  �����͵� ã�Ƽ� �������� (������ �� ���� ������ ����)
	T * GetComponent()
	{
		return	this->GetTransform()->GetComponent<T>();
	}

	template <typename T>
	vector<T*> & GetComponentsInParent()
	{
		return this->GetTransform()->GetComponentsInParent<T>();
	}

	template <typename T>
	T * GetComponentInChild()
	{
		return this->GetTransform()->GetComponentInChild<T>();
	}

	template <typename T> //  ������Ʈ�� (������ ������ ��) ã�Ƽ� �������� (����ġ�ڽĵ� ������) 
	vector<T*> & GetComponentsInChild()
	{
		return this->GetTransform()->GetComponentsInChild<T>();
	}

	template <typename T> // ������Ʈ ���� (���ȿ� �ִ� ������Ʈ�� ���� Ÿ���� ������Ʈ�� ����)
	void DeleteComponent()
	{
		this->GetTransform()->DeleteComponent<T>();
	}

	void DeleteAllComponent()
	{
		this->GetTransform()->DeleteAllComponent();
	}
public:


public:
	void Awake();

	void Start();

	void Update(float deltaSec);

	void FixedUpdate();
	void LateUpdate();

	void OnEnable();
	void OnDisable();

	void Destroy();
public:
	void RenderFrame(
		ID3D11DeviceContext * pDC, 
		vector<ID3D11Buffer*> * VertexInfo, 
		vector<ID3D11Buffer*> *indexInfo
		);
public:

public:
	cGameObject();
	virtual ~cGameObject();
};



