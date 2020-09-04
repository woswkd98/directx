#pragma once
class cColliderShape
{
protected:


	UINT shape;

	float m_fRadius = 3.f;

	cTransform * m_pTransformOBB = nullptr;

public:

	cTransform * GetOBBTransform()
	{
		return m_pTransformOBB;
	}
	cColliderShape();
	~cColliderShape();
};

