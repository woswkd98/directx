#include "stdafx.h"
#include "cTransform.h"

#include "cPhysicsComponent.h"
#include "cPhysicsInfo.h"
#include "cGameObject.h"
cTransform::cTransform() 
{
	/*
	cTransform * parent; // 부모
	cTransform * root; // 루트

	// 노드 
	cTransform * nextSibling; // 다음 형제 
	cTransform * child; // 내밑의 자식

	cTransform * FirstSibling; // 첫번째 형제 (next루프의 시작점 )
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
	// 부모는 나
	gameObject->parent = this;
	
	// 자식의 끝형제를 찾아서 거기에 넣는다
	if (this->child)
	{
		cTransform * p = this->child->FindLastSibling();
		p->nextSibling = gameObject;
		gameObject->FirstSibling = p->FirstSibling; // 첫번쨰형제 지정
	}
	else
	{
		this->child = gameObject; // 자식이없으면 내가 첫자식이되고
		gameObject->FirstSibling = gameObject; // 내형제중 내가 첫번쨰 형제가 된다
	}
	// 이게임오브젝트의 루트는 나의 루트다
	gameObject->root = this->root;

}

void cTransform::InsertSibling(cTransform * gameObject)
{
	if (this == this->root) // 루트는 하나다
	{
		// 에러 메세지
		return;
	}

	cTransform * p = this->FindLastSibling(); // 마지막 형제 찾아리

	p->nextSibling = gameObject; // 넣어줌

	// 부모, 루트설정
	gameObject->parent = this->parent; 
	gameObject->root = this->root;

	// 첫번쨰 형제지정
	gameObject->FirstSibling = this->FirstSibling; 
	
}
// 자식삭제 
void cTransform::DeleteChild(cTransform * gameObject)
{
	if (!this->child) return; // 자식이 없는경우

	bool IsFind = false;
	if (this->child->FirstSibling == gameObject)
	{
		cout << "찾았다" << endl;
		IsFind = true;
		if (!this->child->FirstSibling->nextSibling)
		{
			cout << "다음노드가 없어" << endl;
			this->child = NULL;
		}
		else
		{
			cout << "다음노드가 있어" << endl;

			this->child = this->child->FirstSibling->nextSibling;
			this->child->FirstSibling = this->child->FirstSibling->nextSibling; // 첫번쨰 시블링에 다음노드가 첫번쨰 시블링이 되고 
			this->child->FirstSibling->SetFirstSibling(this->child->FirstSibling); // 첫번쨰 형제 갱신
		}
	}
	else
	{
		cout << "2경우" << endl;
		cTransform * p = this->child->FirstSibling;

		while (1)
		{

			if (p->nextSibling == gameObject) // 같은지 확인
			{
				IsFind = true;
				p->nextSibling = p->nextSibling->nextSibling;
				break;
			}

			// 같지 않아
			if (!p->nextSibling) // 형제가 없다
			{
				if (!p->child) // 자식이 없어 
					break;
				p = p->child->FirstSibling; // 첫번쨰 형제를 이제 검사한다 
			}
		}

	}


	// 찾았을때 삭제
	if (IsFind)
	{
		cMonoBehavior::DestroyGameObject(gameObject->GetGameObject());


		/*
		gameObject->parent = NULL;// 부모 없고
		gameObject->root = gameObject; // 루트는 내자신이고
		gameObject->FirstSibling = NULL; // 형제 없다
		gameObject->nextSibling = NULL;  // 형제 없다
		gameObject->SetRoot(gameObject); // 혹시나있을 이게임오브젝트의 자식들의 루트 변경
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
	m_localQuaternion = m_worldQuaternion; // 먼저 월드부터 넣고

     // 로컬은 부모가 없다면 월드랑 같다 부모가 있으면 부모의 역쿼터니언을 곱한다  (부모가 있을때 월드는 로컬에 부모 누적시킨거)

	// 추가 코드 

	if (parent)
	{
		m_localQuaternion *= XMQuaternionInverse(parent->GetWorldRotationRef());
	}
}

void cTransform::SetLocalRotate(XMVECTOR quat)
{
	m_localQuaternion = quat; // 로컬 설정

}

void cTransform::SetWorldRotate(float x, float y, float z)
{
	m_worldQuaternion = XMQuaternionRotationRollPitchYaw(XMConvertToRadians(x), XMConvertToRadians(y), XMConvertToRadians(z)); // 쿼터니언 만들고 
	m_localQuaternion = m_worldQuaternion;
	
	// 로컬은 부모가 없다면 월드랑 같다 부모가 있으면 부모의 역쿼터니언을 곱한다  (부모가 있을때 월드는 로컬에 부모 누적시킨거)
	if (parent)
	{
		m_localQuaternion = XMQuaternionMultiply(m_worldQuaternion, XMQuaternionInverse(parent->GetWorldRotationRef()));
	}


}

void cTransform::SetLocalRotate(float x, float y, float z)
{
	m_localQuaternion = XMQuaternionRotationRollPitchYaw(XMConvertToRadians(x), XMConvertToRadians(y), XMConvertToRadians(z)); // 쿼터니언 만들고 

}

void cTransform::LookPosition(XMVECTOR pos, XMVECTOR Up)
{
	XMMATRIX matRT = XMMatrixLookAtLH(XMLoadFloat3(&m_position), pos, Up); // 바라보는 회전 위치 행렬 만들어줌 (world)

	// 회전 쿼터니언 만듬

	this->SetWorldRotate(XMQuaternionRotationMatrix(matRT)); // 쿼터니언 업데이트
}

void cTransform::LookDirection(XMVECTOR dir, XMVECTOR Up)
{
	LookPosition(XMLoadFloat3(&m_position) + dir, Up); // 바라보는 위치를 넣어주므로 현재위치 + dir 넣어줌 dir은 노멀라이즈 안되있어도 상관없다
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
		this->m_worldQuaternion = XMQuaternionRotationMatrix(m_matRotationTranslation); // 월드 회전 업데이트
	}
	else
	{
		this->m_worldQuaternion = m_localQuaternion;
	}


	XMStoreFloat4x4(&m_matFinal, m_matRotationTranslation);

	// 축 업데이트
	m_arrayDirection[0] = XMVectorSet(m_matFinal._11, m_matFinal._12, m_matFinal._13, 0); // x
	m_arrayDirection[1] = XMVectorSet(m_matFinal._21, m_matFinal._22, m_matFinal._23, 0); // y
	m_arrayDirection[2] = XMVectorSet(m_matFinal._31, m_matFinal._32, m_matFinal._33, 0); // z

	// 위치 업데이트

	m_position = XMFLOAT3(m_matFinal._41, m_matFinal._42, m_matFinal._43);

	// 크기업데이트
	m_matFinal._11 *= m_scale.x;
	m_matFinal._22 *= m_scale.y;
	m_matFinal._33 *= m_scale.z;
	

	if (this->child) child->UpdateTransform();
	if (this->nextSibling) nextSibling->UpdateTransform();
}

XMVECTOR cTransform::LerpTranslate(XMVECTOR from, XMVECTOR to, float t)
{
	// 이동 회전 크기에 대한 보간

	return XMVectorLerp(from, to, t);
}

XMVECTOR cTransform::LerpRotate(XMVECTOR from, XMVECTOR to, float t) // 쿼터니언 보간
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
	// 일단 월드 변경
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
	if (parent) // 부모가 있으면 월드는 로컬 * parent RT
	{
		XMStoreFloat3(&m_position, XMVector3TransformCoord(XMLoadFloat3(&m_localPos),  parent->GetRotationTranslation()));
	}
	*/
}

void cTransform::LocalRotate(float x, float y, float z) // 회전량 추가 
{
	m_localQuaternion =	XMQuaternionMultiply(m_localQuaternion, XMQuaternionRotationRollPitchYaw(XMConvertToRadians(x), XMConvertToRadians(y), XMConvertToRadians(z))); // 쿼터니언 만들고
}

void cTransform::WorldRatate(float x, float y, float z)
{
	XMVECTOR quat = XMQuaternionRotationRollPitchYaw(XMConvertToRadians(x), XMConvertToRadians(y), XMConvertToRadians(z)); // 쿼터니언 만들고 
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
	// 일단 월드 변경
	m_position = XMFLOAT3(x, y, z);
	m_localPos = m_position;
	if (parent)
	{
		XMStoreFloat3(&m_localPos, XMVector3TransformCoord(XMLoadFloat3(&m_position), XMMatrixInverse(nullptr, parent->GetRotationTranslation())));
	}

}

void cTransform::WorldTranslation(float x, float y, float z)
{
	
	// 일단 월드 변경
	m_position.x += x;
	m_position.y += y;
	m_position.z += z;
	m_localPos = m_position;

	// 월드가 변경됬으니 로컬을 변경시켜야하는데 월드는  R * T * parentRT;
	// 부모가 있을시 부모의 RT의 역행렬을 곱해나간다

	if (parent)
	{
		XMStoreFloat3(&m_localPos, XMVector3TransformCoord(XMLoadFloat3(&m_position), XMMatrixInverse(nullptr, parent->GetRotationTranslation())));
	}

}







