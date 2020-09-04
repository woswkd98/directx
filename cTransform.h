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
	// tree 및 게임오브젝트 관련
protected:
	cTransform * parent; // 부모
	cTransform * root; // 루트
public:
	// 노드 
	cTransform * nextSibling; // 다음 형제 
	cTransform * child; // 내밑의 자식
	
	cTransform * FirstSibling; // 첫번째 형제 (next루프의 시작점 )
public:
	cPhysicsInfo * GetPhysicsInfo() { return m_pPhysicsinfo; }
	virtual cTransform * FindLastSibling();
	virtual void InsertChild(cTransform * gameObject); // 자식추가
	virtual void InsertSibling(cTransform * gameObject); // 형제에 추가
	virtual void DeleteChild(cTransform * gameObject); // 내 자식중에 이게임오브젝트가 있는지 확인후 있으면 포인터 연결끊는다
	
	void Start(); // 뒤늦은 초기화때문에 만들었다 gameObject의 Start에서 실행되기 때문에 함수 이름을 Start라고 했다

protected:
	virtual void SetRoot(cTransform * gameObject); // 자식의 루트 셋
	virtual void SetFirstSibling(cTransform * gameObject); // 형제의 첫번쨰 형제 셋

private: 
	
	template<typename T>
	T * Find()
	{
		T * p = this->GetComponentInMyGameObject<T>(); // 내 컴포넌트들 뒤지고 

		if (p != nullptr) return p; // 만약 뒤졌는데 있으면 리턴

		if (this->nextSibling) 
			nextSibling->Find<T>(); // 재귀
		

		if (this->child)
			nextSibling->Find<T>(); // 재귀
	}

	template<typename T>
	T * FindSibling()
	{
		T * p = this->GetComponentInMyGameObject<T>(); // 내 컴포넌트들 뒤지고 

		if (p != nullptr) return p; // 만약 뒤졌는데 있으면 리턴

		if (this->nextSibling) {
			return nextSibling->FindSibling<T>(); // 재귀
		}
	}
	template<typename T>
	void FindSiblingArr(vector<T*> In)
	{
		T * p = this->GetComponentInMyGameObject<T>(); // 내 컴포넌트들 뒤지고 

		if (p != nullptr) In.push_back(p); // 만약 뒤졌는데 있으면 리턴

		if (this->nextSibling) {
			nextSibling->FindSiblingArr<T>(In); // 재귀
		}

	}

	template<typename T>
	void FindArr(vector<T*> In)
	{
		T * p = this->GetComponentInMyGameObject<T>(); // 내 컴포넌트들 뒤지고 

		if (p != nullptr)
		{
			In.push_back(p);
		}
		if (this->nextSibling) 
		{
			nextSibling->FindArr<T>(In); // 재귀
		}
		if (this->child)
			nextSibling->FindArr<T>(In); // 재귀
	}

	// 컴포넌트 관련
protected:
	vector<cMonoBehavior*> m_listComponents; // 컴포넌트 연결목록
private:

public:

	virtual void SetParent(cTransform * parent) // 부모 설정
	{

		this->parent = parent;
		this->root = parent->GetRoot();
		this->SetRoot(parent->GetRoot());
	}

	cTransform * GetRoot()
	{
		return root;
	}

	// 컴포넌트 찾고 넣고 지우고
	virtual vector<cMonoBehavior*> & GetListComponents() { return m_listComponents; } // 혹시모를 컴포넌트 리스트의 레퍼런스 가져오는 함수

	template <typename T> // 컴포넌트 넣기 (클래스 타입으로 생성시켜서)
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

	template <typename T> //  컴포넌드 찾아서 가져오기 (기준은 내게임 오브젝트
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


	template <typename T> //  컴포넌드 찾아서 가져오기 (기준은 내 기준 형제들 포함)
	T * GetComponent()
	{
		if (is_base_of< cMonoBehavior, T>::value == false) return nullptr; // 상속 클래스 안받았으면 리턴

		
		
		if (!FirstSibling) {
		
			return nullptr;
		}
		return this->FirstSibling->FindSibling<T>();
	}


	template <typename T> //  컴포넌트들 찾아서 가져오기 (기준은 내 기준 형제들 포함)
	vector<T*> GetComponents()
	{
		if (!is_base_of<T, cMonoBehavior>::value) return nullptr; // 상속 클래스 안받았으면 리턴

		vector<T*> m_vT; // 컴포넌트들을 담을 그릇

		this->FirstSibling->FindSiblingArr<T>(m_vT);
		
		return m_vT;
	}

	template <typename T> //  컴포넌트 찾아서 가져오기 (내위치, 형제들 부모 뒤친다) 
	T * GetComponentInParent()
	{
		if (!is_base_of<T, cMonoBehavior>::value) return nullptr; // 상속 클래스 안받았으면 리턴

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

	template <typename T> //  컴포넌트들 (여러개 있으면 다)컴포넌트 찾아서 가져오기  (내위치, 형제들 부모 뒤친다) 
	vector<T*> & GetComponentsInParent()
	{
		if (!is_base_of<T, cMonoBehavior>::value) return nullptr; // 상속 클래스 안받았으면 리턴

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

	template <typename T> //  컴포넌트 찾아서 가져오기 (내위치,자식들 뒤진다) 
	T * GetComponentInChild()
	{
		if (!is_base_of<T, cMonoBehavior>::value) return nullptr; // 상속 클래스 안받았으면 리턴
	
		return this->Find<T>();
	}
	template <typename T> //  컴포넌트들 (여러개 있으면 다) 찾아서 가져오기 (내위치자식들 뒤진다) 
	vector<T*> & GetComponentsInChild()
	{
		vector<T*> list;

		this->child->FirstSibling->FindArr(list);

		return list;
	}

	template <typename T> // 컴포넌트 삭제 (내안에 있는 컴포넌트중 현제 타입의 컴포넌트만 삭제)
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


	void DeleteAllComponent() // 내안의 모든 컴포넌트 삭제
	{
		for (int i = 0; i < m_listComponents.size(); ++i)
		{
			delete m_listComponents[i];
		}
		m_listComponents.clear();

	}
public :

	// 현재트랜스폼의 확인
protected:
	// 벡터 메트릭스
	//계산은 xmvector,xmmatrix, 적재는 xmfloatXXX로

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

	XMMATRIX m_matRotationTranslation; // 변환행렬 

	XMFLOAT4X4 m_matFinal; // 최종행렬

	XMFLOAT3 m_position; // worldPos

	XMFLOAT3 m_localPos; //

	XMFLOAT3 m_scale; // 크기

	XMVECTOR m_localQuaternion; // 쿼터니언용 
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
	// 바라보기
	void LookPosition(XMVECTOR pos, XMVECTOR Up = XMVectorSet(0, 1, 0, 0));
	void LookDirection(XMVECTOR dir, XMVECTOR Up = XMVectorSet(0, 1, 0, 0));

	virtual void UpdateTransform(); // 행렬들의 업데이트 여기서 아핀변환시도
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

