#pragma once







class cGameObject;

class cPhysicsComponent;
class cPhysicsInfo;
#include "cMonoBehavior.h"
class cTransform : public cMonoBehavior
{
public:
	static enum eDirection
	{
		forward,
		right,
		up,
		back,
		left,
		down
	};

private:

	cPhysicsComponent * m_pPhysicsComponent = nullptr;
	cPhysicsInfo * m_pPhysicsinfo = nullptr;
	// tree �� ���ӿ�����Ʈ ����
protected:
	cTransform * parent; // �θ�
	cTransform * root; // ��Ʈ
public:
	// ��� 
	cTransform * nextSibling; // ���� ���� 
	cTransform * child; // ������ �ڽ�
	
	cTransform * FirstSibling; // ù��° ���� (next������ ������ )
public:
	cPhysicsInfo * GetPhysicsInfo() { return m_pPhysicsinfo; }
	virtual cTransform * FindLastSibling();
	virtual void InsertChild(cTransform * gameObject); // �ڽ��߰�
	virtual void InsertSibling(cTransform * gameObject); // ������ �߰�
	virtual void DeleteChild(cTransform * gameObject); // �� �ڽ��߿� �̰��ӿ�����Ʈ�� �ִ��� Ȯ���� ������ ������ ������´�
	
	void Start(); // �ڴ��� �ʱ�ȭ������ ������� gameObject�� Start���� ����Ǳ� ������ �Լ� �̸��� Start��� �ߴ�

protected:
	virtual void SetRoot(cTransform * gameObject); // �ڽ��� ��Ʈ ��
	virtual void SetFirstSibling(cTransform * gameObject); // ������ ù���� ���� ��

private: 
	
	template<typename T>
	T * Find()
	{
		T * p = this->GetComponentInMyGameObject<T>(); // �� ������Ʈ�� ������ 

		if (p != nullptr) return p; // ���� �����µ� ������ ����

		if (this->nextSibling) 
			nextSibling->Find<T>(); // ���
		

		if (this->child)
			nextSibling->Find<T>(); // ���
	}

	template<typename T>
	T * FindSibling()
	{
		T * p = this->GetComponentInMyGameObject<T>(); // �� ������Ʈ�� ������ 

		if (p != nullptr) return p; // ���� �����µ� ������ ����

		if (this->nextSibling) {
			return nextSibling->FindSibling<T>(); // ���
		}
	}
	template<typename T>
	void FindSiblingArr(vector<T*> In)
	{
		T * p = this->GetComponentInMyGameObject<T>(); // �� ������Ʈ�� ������ 

		if (p != nullptr) In.push_back(p); // ���� �����µ� ������ ����

		if (this->nextSibling) {
			nextSibling->FindSiblingArr<T>(In); // ���
		}

	}

	template<typename T>
	void FindArr(vector<T*> In)
	{
		T * p = this->GetComponentInMyGameObject<T>(); // �� ������Ʈ�� ������ 

		if (p != nullptr)
		{
			In.push_back(p);
		}
		if (this->nextSibling) 
		{
			nextSibling->FindArr<T>(In); // ���
		}
		if (this->child)
			nextSibling->FindArr<T>(In); // ���
	}

	// ������Ʈ ����
protected:
	vector<cMonoBehavior*> m_listComponents; // ������Ʈ ������
private:

public:

	virtual void SetParent(cTransform * parent) // �θ� ����
	{

		this->parent = parent;
		this->root = parent->GetRoot();
		this->SetRoot(parent->GetRoot());
	}

	cTransform * GetRoot()
	{
		return root;
	}

	// ������Ʈ ã�� �ְ� �����
	virtual vector<cMonoBehavior*> & GetListComponents() { return m_listComponents; } // Ȥ�ø� ������Ʈ ����Ʈ�� ���۷��� �������� �Լ�

	template <typename T> // ������Ʈ �ֱ� (Ŭ���� Ÿ������ �������Ѽ�)
	T * AddComponent() 
	{

		T * p;

		for (int i = 0; i < this->GetListComponents().size(); ++i)
		{
			p = dynamic_cast<T*>(this->GetListComponents()[i]);
	
			if (p)
			{
				return p;
			}
		}



		p = new T();

		LOG_MGR->AddLog("%d", this->GetGameObject());
		p->SetGameObject(this->GetGameObject());
		m_listComponents.push_back(p);
		return p;
	}

	template <typename T> //  �����͵� ã�Ƽ� �������� (������ ������ ������Ʈ
	T * GetComponentInMyGameObject()
	{
		T * p;

		for (int i = 0; i < this->GetListComponents().size(); ++i)
		{
			p = dynamic_cast<T*>(this->GetListComponents()[i]);
			if (p)
			{	
				return p;
			}
		}
		return nullptr;
	}


	template <typename T> //  �����͵� ã�Ƽ� �������� (������ �� ���� ������ ����)
	T * GetComponent()
	{
		if (is_base_of< cMonoBehavior, T>::value == false) return nullptr; // ��� Ŭ���� �ȹ޾����� ����

		
		
		if (!FirstSibling) {
		
			return nullptr;
		}
		return this->FirstSibling->FindSibling<T>();
	}


	template <typename T> //  ������Ʈ�� ã�Ƽ� �������� (������ �� ���� ������ ����)
	vector<T*> GetComponents()
	{
		if (!is_base_of<T, cMonoBehavior>::value) return nullptr; // ��� Ŭ���� �ȹ޾����� ����

		vector<T*> m_vT; // ������Ʈ���� ���� �׸�

		this->FirstSibling->FindSiblingArr<T>(m_vT);
		
		return m_vT;
	}

	template <typename T> //  ������Ʈ ã�Ƽ� �������� (����ġ, ������ �θ� ��ģ��) 
	T * GetComponentInParent()
	{
		if (!is_base_of<T, cMonoBehavior>::value) return nullptr; // ��� Ŭ���� �ȹ޾����� ����

		T * p = this->FirstSibling;

		while (1)
		{
			if (p->FindSibling() == nullptr)
			{
				if (this->parent)
					p = parent->FirstSibling;
				else
					return nullptr;
			}
			else return p ;
		}
	}

	template <typename T> //  ������Ʈ�� (������ ������ ��)������Ʈ ã�Ƽ� ��������  (����ġ, ������ �θ� ��ģ��) 
	vector<T*> & GetComponentsInParent()
	{
		if (!is_base_of<T, cMonoBehavior>::value) return nullptr; // ��� Ŭ���� �ȹ޾����� ����

		vector<T*> list;

		T * p = this->FirstSibling;

		while (1)
		{
			p->FindSiblingArr(p);

			if (this->parent)
				p = parent->FirstSibling;
			else
				return list;
		}
	}

	template <typename T> //  ������Ʈ ã�Ƽ� �������� (����ġ,�ڽĵ� ������) 
	T * GetComponentInChild()
	{
		if (!is_base_of<T, cMonoBehavior>::value) return nullptr; // ��� Ŭ���� �ȹ޾����� ����
	
		return this->Find<T>();
	}
	template <typename T> //  ������Ʈ�� (������ ������ ��) ã�Ƽ� �������� (����ġ�ڽĵ� ������) 
	vector<T*> & GetComponentsInChild()
	{
		vector<T*> list;

		this->child->FirstSibling->FindArr(list);

		return list;
	}

	template <typename T> // ������Ʈ ���� (���ȿ� �ִ� ������Ʈ�� ���� Ÿ���� ������Ʈ�� ����)
	void DeleteComponent()
	{
		for (int i = 0; i < m_listComponents.size(); ++i)
		{

			T * p = dynamic_cast<T*>(m_listComponents[i]);
			if (p)
			{
				delete p;
				m_listComponents.erase(m_listComponents.begin() + i);
				break;
			}
		}
	}


	void DeleteAllComponent() // ������ ��� ������Ʈ ����
	{
		for (int i = 0; i < m_listComponents.size(); ++i)
		{
			delete m_listComponents[i];
		}
		m_listComponents.clear();

	}
public :

	// ����Ʈ�������� Ȯ��
protected:
	// ���� ��Ʈ����
	//����� xmvector,xmmatrix, ����� xmfloatXXX��

	union
	{
		struct
		{
			XMVECTOR m_directionUp;
			XMVECTOR m_directionRight;
			XMVECTOR m_directionForward;
		};

		struct
		{
			XMVECTOR m_arrayDirection[3];
		};
	};

	XMMATRIX m_matRotationTranslation; // ��ȯ��� 

	XMFLOAT4X4 m_matFinal; // �������

	XMFLOAT3 m_position; // worldPos

	XMFLOAT3 m_localPos; //

	XMFLOAT3 m_scale; // ũ��

	XMVECTOR m_localQuaternion; // ���ʹϾ�� 
	XMVECTOR m_worldQuaternion;
	
public:

	virtual void SetScale(float x, float y, float z);
	virtual void SetScale(XMFLOAT3 v);
	
	virtual void Scaling(float x, float y, float z);
	virtual void Scaling(XMFLOAT3 v);

	virtual void SetLocalPosition(float x, float y, float z);
	virtual void SetWorldPosition(float x, float y, float z);

	virtual void SetLocalPosition(XMFLOAT3 pos);
	virtual void SetWorldPosition(XMFLOAT3 pos);

	virtual void LocalRotate(float x, float y, float z);
	virtual void WorldRatate(float x, float y, float z);
	
	virtual void TranslationDir(float speed, eDirection dir);
	
	virtual void WorldTranslation(float x, float y, float z); 
	virtual void LocalTranaslation(float x, float y, float z);
	
	virtual void SetWorldRotate(XMVECTOR quat);
	virtual void SetLocalRotate(XMVECTOR quat);

	virtual void SetWorldRotate(float x, float y, float z);
	virtual void SetLocalRotate(float x, float y, float z);
	// �ٶ󺸱�
	void LookPosition(XMVECTOR pos, XMVECTOR Up = XMVectorSet(0, 1, 0, 0));
	void LookDirection(XMVECTOR dir, XMVECTOR Up = XMVectorSet(0, 1, 0, 0));

	virtual void UpdateTransform(); // ��ĵ��� ������Ʈ ���⼭ ���ɺ�ȯ�õ�
	virtual XMMATRIX GetRotationTranslation() { return m_matRotationTranslation; }
	virtual XMMATRIX GetFinalMatrix() { return XMLoadFloat4x4(&m_matFinal); }
	virtual XMFLOAT3 & GetPositionRef() { return m_position; }
	virtual XMFLOAT3 & GetLocalPositionRef() { return m_localPos; }
	static XMVECTOR LerpTranslate(XMVECTOR from, XMVECTOR to, float t);
	static XMVECTOR LerpRotate(XMVECTOR from, XMVECTOR to, float t);

	static void interpolation2(XMFLOAT3 start, XMFLOAT3  end, cTransform * out, XMFLOAT3 BezierPoint[]);
	virtual XMVECTOR GetForward() { return m_directionForward; }
	virtual XMVECTOR GetRight() { return m_directionRight; }
	virtual XMVECTOR GetUp() { return m_directionUp; }

	virtual XMVECTOR & GetWorldRotationRef() { return m_worldQuaternion; }
	virtual XMVECTOR & GetLocalRotationRef() { return m_localQuaternion; }
	virtual XMFLOAT3 & GetScale() { return m_scale; }



public:
	cTransform();
	virtual ~cTransform();
};

