#pragma once
#include "cCollider.h"
class cCapsuleCollider :
	public cCollider
{
private:
public:
	XMVECTOR & GetPosition() override;
	XMFLOAT3 GetMOI(float mass) override;
	void Calculate() override;
	cCapsuleCollider();
	~cCapsuleCollider();
};

