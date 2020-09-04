#include "stdafx.h"
#include "cCapsuleCollider.h"


XMVECTOR & cCapsuleCollider::GetPosition()
{
	XMVECTOR capsuleStartPos;
	XMVECTOR capsuleDir = this->GetOBBTransform()->GetForward(); // ĸ���� ����
	XMVECTOR colpos = XMLoadFloat3(&m_pDetectCollider->GetOBBTransform()->GetPositionRef()); // �浹�˻��� �ö��̴� ��ġ
	float length = this->GetOBBTransform()->GetScale().z; // ĸ���� ���� 
	capsuleStartPos = XMLoadFloat3(&this->GetOBBTransform()->GetPositionRef()) - capsuleDir *  length / 2; // ĸ���� �������� (����ġ���� -z �������� ������ �ݸ�ŭ )

	XMVECTOR v1 = colpos - capsuleStartPos; // ����ġ���� �ö��̴� �������� ������ ����

	float DistOnLIne = XMVectorGetX(XMVector3Dot(v1, capsuleDir)) / length; // ���̺� 

	return clampO1(DistOnLIne) * length * capsuleDir + capsuleStartPos;// �������� �������� ��ġ���� (�� ��ġ�� �ö��̴� �����̶� �� ������ �ִܰŸ�)
}

XMFLOAT3 cCapsuleCollider::GetMOI(float mass)
{


	
	return XMFLOAT3();
}

void cCapsuleCollider::Calculate()
{
	
}

cCapsuleCollider::cCapsuleCollider()
{
}


cCapsuleCollider::~cCapsuleCollider()
{
}
