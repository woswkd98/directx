#pragma once
#include "cCollider.h"
class cSphereCollider :
	public cCollider
{
private:
	
public:

	void Awake() override; 

	void FixedUpdate() override;
	
	void Calculate() override;

	void LateUpdate() override;
	
	void SetRadius(float rad) override;

	XMFLOAT3 GetMOI(float mass) override; // ���� ���� ���Ʈ

	cSphereCollider();
	~cSphereCollider();
};

