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
	virtual void SetActive(bool active); // 내위치서부터 내 자식들의 스위치를 온오프 시킨다

	virtual bool GetActive() { return m_bActive; }

	cTransform * GetTransform() {

		return pTransform; }
public:
	void BroadCastFunction(string str);
	

	template <typename T> // 컴포넌트 넣기 (클래스 타입으로 생성시켜서)
	T * AddComponent()
	{
		return this->GetTransform()->AddComponent<T>();
	}

	template <typename T> //  컴포넌드 찾아서 가져오기 (기준은 내게임 오브젝트
	T * GetComponentInMyGameObject()
	{
		return	this->GetTransform()->GetComponentInMyGameObject<T>();
	}

	template <typename T> //  컴포넌드 찾아서 가져오기 (기준은 내 기준 형제들 포함)
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

	template <typename T> //  컴포넌트들 (여러개 있으면 다) 찾아서 가져오기 (내위치자식들 뒤진다) 
	vector<T*> & GetComponentsInChild()
	{
		return this->GetTransform()->GetComponentsInChild<T>();
	}

	template <typename T> // 컴포넌트 삭제 (내안에 있는 컴포넌트중 현제 타입의 컴포넌트만 삭제)
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



