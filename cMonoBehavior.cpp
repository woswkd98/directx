#include "stdafx.h"
#include "cMonoBehavior.h"
#include "cGizmoManager.h"
#include "cGameObject.h"
#include "cTransform.h"
void cMonoBehavior::Awake()
{
	
}

void cMonoBehavior::SetActive(bool active)
{
	m_bActive = active;
}

void cMonoBehavior::DestroyGameObject(cGameObject * pGAameObject) // 게임 오브젝트 삭제 (내용까지 전부 삭제)
{
	if (!pGAameObject) return;

	cTransform * tp = pGAameObject->GetTransform()->FirstSibling; // 시작은 자식부터
	
	// 먼저 본인 컴포넌트와 오브젝트 삭제
	for (int i = 0; i < pGAameObject->GetTransform()->GetListComponents().size(); ++i)
	{
		delete  pGAameObject->GetTransform()->GetListComponents()[i];
	}

	SAFE_DELETE(pGAameObject);

	if (tp == nullptr)
	{
		return;
	}

	while (1) // 부모 루프
	{

		while (1) // 형제 루프
		{
			for (int i = 0; i < tp->GetListComponents().size(); ++i) // 돌면서 컴포넌트 삭제
			{
				delete  tp->GetListComponents()[i];
			}

			if (tp->nextSibling)  
			{
				cTransform * prePointer = tp;
				tp = tp->nextSibling; // 있으면 다음노드로 넘겨주고
				DestroyGameObject(tp->GetGameObject()); // 함수 재호출
				delete prePointer; // 이전노드 삭제
			
			}
			else
			{
				break;
			}
		}

		if (tp->child != nullptr) 
		{
			cTransform * prePointer = tp; // 이전 (컴포넌트는 다 삭제시킨 상태)
			tp = tp->child->FirstSibling; 
			DestroyGameObject(tp->GetGameObject());
			delete prePointer; // 이전노드 삭제 
		}
		else
		{
			delete tp; // 끝났으므로 현제 포인터 삭제 후 return
			break;
		}

	}

}

cGameObject * cMonoBehavior::CopyGameObject(cGameObject * pInGameObject, cGameObject * pOutGameObject)
{
	/*
	if (!pInGameObject) return pOutGameObject = nullptr;

	cTransform * InObjectChild = pInGameObject->GetTransform()->child; // 자식부터 시작


	// 먼저 루트 부터 복사
	cGameObject * newRoot = new cGameObject(); // 
	
	for (int i = 0; i < pInGameObject->GetTransform()->GetListComponents().size(); ++i)
	{
		newRoot->AddComponent(ne);
	}

	if (InObjectChild == nullptr)
	{
		return pOutGameObject = newRoot; // 자식이 없으면 루트만 
	}

	cTransform * copyObject = newRoot->GetTransform(); // InObjectChild의 위치의 부모 위치 노드 

	while (1) // 부모 루프
	{
		while (1) // 형제 루프
		{
			cGameObject * newTp = new cGameObject(); // 새로운 노드를 생성시킨다 

			copyObject->InsertChild(newTp->GetTransform()); // 자식에 넣어준다

			for (int i = 0; i < InObjectChild->GetListComponents().size(); ++i) // 컴포넌트를 돌면서 복사
			{
				newTp->AddComponent<T>();
			}

			if (InObjectChild->nextSibling)
			{
				InObjectChild = InObjectChild->nextSibling;
			}
			else
			{
				break;
			}
		}

		if (InObjectChild->child != nullptr)
		{
			// 여기서 copyObject의 위치를 내린다 

			InObjectChild = InObjectChild->child->FirstSibling; //InObjectChild의 자식들중 첫번쨰 형제
		
			copyObject = copyObject->child->FirstSibling; //copyObject 첫번쨰 자식
		}
		else
		{
			break;
		}

	}
	*/
	//return pOutGameObject = newRoot;

	return nullptr;
}

cMonoBehavior::cMonoBehavior()
{
	
	Awake();
}


cMonoBehavior::~cMonoBehavior()
{
	
}
