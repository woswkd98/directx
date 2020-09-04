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

void cMonoBehavior::DestroyGameObject(cGameObject * pGAameObject) // ���� ������Ʈ ���� (������� ���� ����)
{
	if (!pGAameObject) return;

	cTransform * tp = pGAameObject->GetTransform()->FirstSibling; // ������ �ڽĺ���
	
	// ���� ���� ������Ʈ�� ������Ʈ ����
	for (int i = 0; i < pGAameObject->GetTransform()->GetListComponents().size(); ++i)
	{
		delete  pGAameObject->GetTransform()->GetListComponents()[i];
	}

	SAFE_DELETE(pGAameObject);

	if (tp == nullptr)
	{
		return;
	}

	while (1) // �θ� ����
	{

		while (1) // ���� ����
		{
			for (int i = 0; i < tp->GetListComponents().size(); ++i) // ���鼭 ������Ʈ ����
			{
				delete  tp->GetListComponents()[i];
			}

			if (tp->nextSibling)  
			{
				cTransform * prePointer = tp;
				tp = tp->nextSibling; // ������ �������� �Ѱ��ְ�
				DestroyGameObject(tp->GetGameObject()); // �Լ� ��ȣ��
				delete prePointer; // ������� ����
			
			}
			else
			{
				break;
			}
		}

		if (tp->child != nullptr) 
		{
			cTransform * prePointer = tp; // ���� (������Ʈ�� �� ������Ų ����)
			tp = tp->child->FirstSibling; 
			DestroyGameObject(tp->GetGameObject());
			delete prePointer; // ������� ���� 
		}
		else
		{
			delete tp; // �������Ƿ� ���� ������ ���� �� return
			break;
		}

	}

}

cGameObject * cMonoBehavior::CopyGameObject(cGameObject * pInGameObject, cGameObject * pOutGameObject)
{
	/*
	if (!pInGameObject) return pOutGameObject = nullptr;

	cTransform * InObjectChild = pInGameObject->GetTransform()->child; // �ڽĺ��� ����


	// ���� ��Ʈ ���� ����
	cGameObject * newRoot = new cGameObject(); // 
	
	for (int i = 0; i < pInGameObject->GetTransform()->GetListComponents().size(); ++i)
	{
		newRoot->AddComponent(ne);
	}

	if (InObjectChild == nullptr)
	{
		return pOutGameObject = newRoot; // �ڽ��� ������ ��Ʈ�� 
	}

	cTransform * copyObject = newRoot->GetTransform(); // InObjectChild�� ��ġ�� �θ� ��ġ ��� 

	while (1) // �θ� ����
	{
		while (1) // ���� ����
		{
			cGameObject * newTp = new cGameObject(); // ���ο� ��带 ������Ų�� 

			copyObject->InsertChild(newTp->GetTransform()); // �ڽĿ� �־��ش�

			for (int i = 0; i < InObjectChild->GetListComponents().size(); ++i) // ������Ʈ�� ���鼭 ����
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
			// ���⼭ copyObject�� ��ġ�� ������ 

			InObjectChild = InObjectChild->child->FirstSibling; //InObjectChild�� �ڽĵ��� ù���� ����
		
			copyObject = copyObject->child->FirstSibling; //copyObject ù���� �ڽ�
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
