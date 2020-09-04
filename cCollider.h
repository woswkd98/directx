#pragma once






class cColliderShape;
class cCollider : public cMonoBehavior // 바운딩 마찰 등을 할 클래스
{

	
public:

	
	list<cColliderShape*> m_listShapes;

public:
	bool m_bIsTrigger = true; // 트리거 함수를 만들어서 쓸꺼냐?




public:

private:

	

	ST_AABB m_aabbBox;
	XMFLOAT3 m_arrAABBboxVertex[8];
	XMFLOAT3 m_minBox, m_maxBox;
private:
	typedef void(*FuncTrigger)(cCollider * pMyCollider, cCollider * pCollionCol);
	function<void()> m_pTriggerFunc;
public:

	ST_AABB & GetAABBBox()
	{
		ST_AABB m_aabbBox;
	}

	/*

	void UpdateAABBBox();

	cCollider * SetBoundBox(XMFLOAT3 CenterPos, XMFLOAT3 scale);

	cCollider * SetBoundSphere(XMFLOAT3 CenterPos, float radius);

	cCollider * SetBoundCapsule(XMFLOAT3 CenterPos, float radius, float Length);

	cCollider * SetTerrainCollider(XMFLOAT3 CenterPos, XMFLOAT3 m_scale);

	int GetShapeIndex()
	{
		return static_cast<int>(shape);
	}

	void SetCenterPos(float x, float y, float z)
	{
		m_pTransformOBB->SetWorldPosition(XMFLOAT3(x, y, z));
	}

	virtual void SetRadius(float rad)
	{
		m_fRadius = rad;
	}

	float GetRadius()
	{	
		return m_fRadius;
	}

	virtual XMVECTOR & GetPosition()
	{
		return XMLoadFloat3(&m_pTransformOBB->GetPositionRef());
	}


	void Awake() override;

	void FixedUpdate() override;

	void SetTriggerFunc(FuncTrigger func, cCollider * col)
	{
		m_pTriggerFunc = bind(func, this, col);
		m_bIsTrigger = true;
	}
	
	void Update(float deltatime) override;

	virtual float GetProjLength() { return m_fRadius; }

	*/
	cCollider();
	~cCollider();
};

