#include "stdafx.h"
#include "cTransform.h"

#include "cPhysicsComponent.h"
#include "cPhysicsInfo.h"
#include "cGameObject.h"
cTransform::cTransform() 
{
	/*
	cTransform * parent; // �θ�
	cTransform * root; // ��Ʈ

	// ��� 
	cTransform * nextSibling; // ���� ���� 
	cTransform * child; // ������ �ڽ�

	cTransform * FirstSibling; // ù��° ���� (next������ ������ )
	*/




	root = this;
	parent = nullptr;
	nextSibling = nullptr;
	child = nullptr;
	FirstSibling = this;

	m_localQuaternion = XMQuaternionIdentity();
	m_worldQuaternion = XMQuaternionIdentity();


	m_matRotationTranslation = XMMatrixIdentity();
	m_position = XMFLOAT3(0, 0, 0);
	m_localPos = XMFLOAT3(0, 0, 0);
	m_scale = XMFLOAT3(1, 1, 1);

}


cTransform::~cTransform()
{
}


void cTransform::Start()
{

}


cTransform * cTransform::FindLastSibling()
{
	if (this->nextSibling)
		return this->nextSibling->FindLastSibling();
	else return this;
}

void cTransform::InsertChild(cTransform * gameObject)
{
	// �θ�� ��
	gameObject->parent = this;
	
	// �ڽ��� �������� ã�Ƽ� �ű⿡ �ִ´�
	if (this->child)
	{
		cTransform * p = this->child->FindLastSibling();
		p->nextSibling = gameObject;
		gameObject->FirstSibling = p->FirstSibling; // ù�������� ����
	}
	else
	{
		this->child = gameObject; // �ڽ��̾����� ���� ù�ڽ��̵ǰ�
		gameObject->FirstSibling = gameObject; // �������� ���� ù���� ������ �ȴ�
	}
	// �̰��ӿ�����Ʈ�� ��Ʈ�� ���� ��Ʈ��
	gameObject->root = this->root;

}

void cTransform::InsertSibling(cTransform * gameObject)
{
	if (this == this->root) // ��Ʈ�� �ϳ���
	{
		// ���� �޼���
		return;
	}

	cTransform * p = this->FindLastSibling(); // ������ ���� ã�Ƹ�

	p->nextSibling = gameObject; // �־���

	// �θ�, ��Ʈ����
	gameObject->parent = this->parent; 
	gameObject->root = this->root;

	// ù���� ��������
	gameObject->FirstSibling = this->FirstSibling; 
	
}
// �ڽĻ��� 
void cTransform::DeleteChild(cTransform * gameObject)
{
	if (!this->child) return; // �ڽ��� ���°��

	bool IsFind = false;
	if (this->child->FirstSibling == gameObject)
	{
		cout << "ã�Ҵ�" << endl;
		IsFind = true;
		if (!this->child->FirstSibling->nextSibling)
		{
			cout << "������尡 ����" << endl;
			this->child = NULL;
		}
		else
		{
			cout << "������尡 �־�" << endl;

			this->child = this->child->FirstSibling->nextSibling;
			this->child->FirstSibling = this->child->FirstSibling->nextSibling; // ù���� �ú��� ������尡 ù���� �ú��� �ǰ� 
			this->child->FirstSibling->SetFirstSibling(this->child->FirstSibling); // ù���� ���� ����
		}
	}
	else
	{
		cout << "2���" << endl;
		cTransform * p = this->child->FirstSibling;

		while (1)
		{

			if (p->nextSibling == gameObject) // ������ Ȯ��
			{
				IsFind = true;
				p->nextSibling = p->nextSibling->nextSibling;
				break;
			}

			// ���� �ʾ�
			if (!p->nextSibling) // ������ ����
			{
				if (!p->child) // �ڽ��� ���� 
					break;
				p = p->child->FirstSibling; // ù���� ������ ���� �˻��Ѵ� 
			}
		}

	}


	// ã������ ����
	if (IsFind)
	{
		cMonoBehavior::DestroyGameObject(gameObject->GetGameObject());


		/*
		gameObject->parent = NULL;// �θ� ����
		gameObject->root = gameObject; // ��Ʈ�� ���ڽ��̰�
		gameObject->FirstSibling = NULL; // ���� ����
		gameObject->nextSibling = NULL;  // ���� ����
		gameObject->SetRoot(gameObject); // Ȥ�ó����� �̰��ӿ�����Ʈ�� �ڽĵ��� ��Ʈ ����
		*/

	
	}
	

}

void cTransform::SetRoot(cTransform * gameObject)
{
	if (this->child)
	{
		this->child->root = gameObject;
		this->child->SetRoot(gameObject);
	}

	if (this->nextSibling)
	{
		this->nextSibling->root = gameObject;
		this->nextSibling->SetRoot(gameObject);
	}

}
void cTransform::SetFirstSibling(cTransform * gameObject)
{
	if (this->nextSibling)
	{
		this->nextSibling->FirstSibling = gameObject;
		this->nextSibling->SetFirstSibling(gameObject);
	}
}



void cTransform::SetWorldRotate(XMVECTOR quat)
{
	m_worldQuaternion = quat;
	m_localQuaternion = m_worldQuaternion; // ���� ������� �ְ�

     // ������ �θ� ���ٸ� ����� ���� �θ� ������ �θ��� �����ʹϾ��� ���Ѵ�  (�θ� ������ ����� ���ÿ� �θ� ������Ų��)

	// �߰� �ڵ� 

	if (parent)
	{
		m_localQuaternion *= XMQuaternionInverse(parent->GetWorldRotationRef());
	}
}

void cTransform::SetLocalRotate(XMVECTOR quat)
{
	m_localQuaternion = quat; // ���� ����

}

void cTransform::SetWorldRotate(float x, float y, float z)
{
	m_worldQuaternion = XMQuaternionRotationRollPitchYaw(XMConvertToRadians(x), XMConvertToRadians(y), XMConvertToRadians(z)); // ���ʹϾ� ����� 
	m_localQuaternion = m_worldQuaternion;
	
	// ������ �θ� ���ٸ� ����� ���� �θ� ������ �θ��� �����ʹϾ��� ���Ѵ�  (�θ� ������ ����� ���ÿ� �θ� ������Ų��)
	if (parent)
	{
		m_localQuaternion = XMQuaternionMultiply(m_worldQuaternion, XMQuaternionInverse(parent->GetWorldRotationRef()));
	}


}

void cTransform::SetLocalRotate(float x, float y, float z)
{
	m_localQuaternion = XMQuaternionRotationRollPitchYaw(XMConvertToRadians(x), XMConvertToRadians(y), XMConvertToRadians(z)); // ���ʹϾ� ����� 

}

void cTransform::LookPosition(XMVECTOR pos, XMVECTOR Up)
{
	XMMATRIX matRT = XMMatrixLookAtLH(XMLoadFloat3(&m_position), pos, Up); // �ٶ󺸴� ȸ�� ��ġ ��� ������� (world)

	// ȸ�� ���ʹϾ� ����

	this->SetWorldRotate(XMQuaternionRotationMatrix(matRT)); // ���ʹϾ� ������Ʈ
}

void cTransform::LookDirection(XMVECTOR dir, XMVECTOR Up)
{
	LookPosition(XMLoadFloat3(&m_position) + dir, Up); // �ٶ󺸴� ��ġ�� �־��ֹǷ� ������ġ + dir �־��� dir�� ��ֶ����� �ȵ��־ �������
}

void cTransform::UpdateTransform()
{

	//  R
	XMMATRIX matR = XMMatrixRotationQuaternion(m_localQuaternion);
	
	// T
	XMMATRIX matT = XMMatrixTranslation(this->m_localPos .x, this->m_localPos.y, this->m_localPos.z);
	
	m_matRotationTranslation = matR * matT;

	if (parent)
	{
		m_matRotationTranslation *= parent->GetRotationTranslation();
		this->m_worldQuaternion = XMQuaternionRotationMatrix(m_matRotationTranslation); // ���� ȸ�� ������Ʈ
	}
	else
	{
		this->m_worldQuaternion = m_localQuaternion;
	}


	XMStoreFloat4x4(&m_matFinal, m_matRotationTranslation);

	// �� ������Ʈ
	m_arrayDirection[0] = XMVectorSet(m_matFinal._11, m_matFinal._12, m_matFinal._13, 0); // x
	m_arrayDirection[1] = XMVectorSet(m_matFinal._21, m_matFinal._22, m_matFinal._23, 0); // y
	m_arrayDirection[2] = XMVectorSet(m_matFinal._31, m_matFinal._32, m_matFinal._33, 0); // z

	// ��ġ ������Ʈ

	m_position = XMFLOAT3(m_matFinal._41, m_matFinal._42, m_matFinal._43);

	// ũ�������Ʈ
	m_matFinal._11 *= m_scale.x;
	m_matFinal._22 *= m_scale.y;
	m_matFinal._33 *= m_scale.z;
	

	if (this->child) child->UpdateTransform();
	if (this->nextSibling) nextSibling->UpdateTransform();
}

XMVECTOR cTransform::LerpTranslate(XMVECTOR from, XMVECTOR to, float t)
{
	// �̵� ȸ�� ũ�⿡ ���� ����

	return XMVectorLerp(from, to, t);
}

XMVECTOR cTransform::LerpRotate(XMVECTOR from, XMVECTOR to, float t) // ���ʹϾ� ����
{
	return XMQuaternionSlerp(from, to, t);
}

void cTransform::interpolation2(XMFLOAT3 start, XMFLOAT3 end, cTransform * out, XMFLOAT3 BezierPoint[])
{


}

void cTransform::SetScale(float x, float y, float z)
{
	m_scale.x = x;
	m_scale.y = y;
	m_scale.z = z;

}

void cTransform::SetScale(XMFLOAT3 v)
{
	m_scale = v;
}

void cTransform::Scaling(float x, float y, float z) {

	m_scale.x *= x;
	m_scale.y *= y;
	m_scale.z *= z;

}
 void cTransform::Scaling(XMFLOAT3 v)
{
	m_scale.x *= v.x;
	m_scale.y *= v.y;
	m_scale.z *= v.z;

}

void cTransform::SetLocalPosition(XMFLOAT3 pos)
{
	this->m_localPos = pos;
	m_position = m_localPos;


}
void cTransform::SetWorldPosition(XMFLOAT3 pos)
{
	// �ϴ� ���� ����
	m_position = pos;
	m_localPos = m_position;
	if (parent)
	{
		XMStoreFloat3(&m_localPos, XMVector3TransformCoord(XMLoadFloat3(&m_position), XMMatrixInverse(nullptr, parent->GetRotationTranslation())));
	}


}
void cTransform::SetLocalPosition(float x, float y, float z)
{
	this->m_localPos.x = x;
	this->m_localPos.y = y;
	this->m_localPos.z = z;
	/*
	m_position = m_localPos;
	if (parent) // �θ� ������ ����� ���� * parent RT
	{
		XMStoreFloat3(&m_position, XMVector3TransformCoord(XMLoadFloat3(&m_localPos),  parent->GetRotationTranslation()));
	}
	*/
}

void cTransform::LocalRotate(float x, float y, float z) // ȸ���� �߰� 
{
	m_localQuaternion =	XMQuaternionMultiply(m_localQuaternion, XMQuaternionRotationRollPitchYaw(XMConvertToRadians(x), XMConvertToRadians(y), XMConvertToRadians(z))); // ���ʹϾ� �����
}

void cTransform::WorldRatate(float x, float y, float z)
{
	XMVECTOR quat = XMQuaternionRotationRollPitchYaw(XMConvertToRadians(x), XMConvertToRadians(y), XMConvertToRadians(z)); // ���ʹϾ� ����� 
	m_worldQuaternion = XMQuaternionMultiply(m_worldQuaternion, quat);
	m_localQuaternion = m_worldQuaternion;
	if (parent)
	{
		m_localQuaternion = XMQuaternionMultiply(m_worldQuaternion, XMQuaternionInverse(parent->GetWorldRotationRef()));
	}
}

void cTransform::TranslationDir(float speed, eDirection dir)
{
	XMFLOAT3 temp;
	
	if (dir == eDirection::forward)
	{
		XMStoreFloat3(&temp, m_directionForward * speed + XMLoadFloat3(&m_position));

		this->SetWorldPosition(temp);
	}
	else if (dir == eDirection::back)
	{
		XMStoreFloat3(&temp, -m_directionForward * speed + XMLoadFloat3(&m_position));

		this->SetWorldPosition(temp);
	}
	else if (dir == eDirection::up)
	{
		XMStoreFloat3(&temp, m_arrayDirection[1] * speed + XMLoadFloat3(&m_position));

		this->SetWorldPosition(temp);
	}
	else if (dir == eDirection::down)
	{
		XMStoreFloat3(&temp, -m_arrayDirection[1] * speed + XMLoadFloat3(&m_position));

		this->SetWorldPosition(temp);
	}
	else if (dir == eDirection::left)
	{
		XMStoreFloat3(&temp,- m_arrayDirection[0] * speed + XMLoadFloat3(&m_position));

		this->SetWorldPosition(temp);

	}
	else if (dir == eDirection::right)
	{
		XMStoreFloat3(&temp, m_arrayDirection[0] * speed + XMLoadFloat3(&m_position));

		this->SetWorldPosition(temp);
	}
}

void cTransform::LocalTranaslation(float x, float y, float z)
{
	this->m_localPos.x += x;
	this->m_localPos.y += y;
	this->m_localPos.z += z;
}

void cTransform::SetWorldPosition(float x , float y, float z)
{
	// �ϴ� ���� ����
	m_position = XMFLOAT3(x, y, z);
	m_localPos = m_position;
	if (parent)
	{
		XMStoreFloat3(&m_localPos, XMVector3TransformCoord(XMLoadFloat3(&m_position), XMMatrixInverse(nullptr, parent->GetRotationTranslation())));
	}

}

void cTransform::WorldTranslation(float x, float y, float z)
{
	
	// �ϴ� ���� ����
	m_position.x += x;
	m_position.y += y;
	m_position.z += z;
	m_localPos = m_position;

	// ���尡 ��������� ������ ������Ѿ��ϴµ� �����  R * T * parentRT;
	// �θ� ������ �θ��� RT�� ������� ���س�����

	if (parent)
	{
		XMStoreFloat3(&m_localPos, XMVector3TransformCoord(XMLoadFloat3(&m_position), XMMatrixInverse(nullptr, parent->GetRotationTranslation())));
	}

}







