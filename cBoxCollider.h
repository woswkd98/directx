#pragma once
#include "cCollider.h"


class cBoxCollider :
	public cCollider
{
	
private:
	float m_fLength = 0;
public:

public:
	void Awake() override;

	void FixedUpdate() override;

	void Calculate() override;

	XMFLOAT3 GetMOI(float mass) override;

	float GetProjLength() { return m_fLength; }

public:
	cBoxCollider();
	~cBoxCollider();
};

