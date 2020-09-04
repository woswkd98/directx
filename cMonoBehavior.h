#pragma once

// 참고로 클래스 템플릿에 대하여 static 변수를 
// 선언해도 각각의 클래스에 대한 변수가 생성됨 

//http://www.gilgil.net/3079

// 함수 도는 순서
/*
Init()
{
	Awake();
}

Update()
{
	// 새로운 게임오브젝트 생성

	virtual void Start() {} // 첫 프레임이 돌때 시작되는 생성자

	virtual void OnEnable() {} // 게임오브젝트가 켜졌을때 실행되는 메서드 

	virtual void Update(float deltaSce) {} // 일반 업데이트용

	virtual void LateUpdate() {} // 업데이트 실행된 이후의 업데이트

	virtual void OnDisable() {} // 게임오브젝트가 꺼졌을때 실행되는 메서드

	virtual void Release() {} // 게임 오브젝트 삭제 될 때 실행

	// 게임오브젝트 삭제
}

	 





*/

class cGameObject;
class cMonoBehavior : public cObject
{
private:
	cGameObject * gameObject = nullptr;
protected:
	map<string, Event> m_mapFunctionStore; // 저장 시켜놨다기 함수를 불러옴 
public:
	cGameObject * GetGameObject() { return gameObject; }
	virtual void Awake();

	virtual void Start() {} // 첫 프레임이 돌때 시작되는 생성자

	virtual void Update(float dt) {} // 일반 업데이트용

	virtual void LateUpdate() {} // 업데이트 실행된 이후의 업데이트

	virtual void FixedUpdate() {} // 피직스 내에서 돌아가는 업데이트 개별 타이머
	
	virtual void BackGroundUpdate() {}  // 게임오브젝트가 실행되지 않아도 도는 업데이트

	virtual void OnEnable() {} // 게임오브젝트가 켜졌을때 실행되는 메서드 

	virtual void OnDisable() {} // 게임오브젝트가 꺼졌을때 실행되는 메서드  

	virtual void Release() {} // 게임 오브젝트 삭제 될 때 실행

	virtual void Destroy() {}
public:
	virtual void SetActive(bool active);
	
	static void DestroyGameObject(cGameObject * pGAameObject);// 게임오브젝트 삭제 

	void SetGameObject(cGameObject * pGAameObject) {
		gameObject = pGAameObject;
	}

	cGameObject * CopyGameObject(cGameObject * pInGameObject, cGameObject * pOutGameObject = nullptr); // 게임 오브젝트 내용 전체 복사해서 리턴시켜줌 
private: 
	template<class T>
	T * newComponent(T * t)
	{
		T * newT = new T;
		*newT = *t; // 내용 복사
		return newT;
	}
public:

	map<string, Event> & GetFunctions()
	{
		return m_mapFunctionStore;
	}

	void BroadCastFunction(string str)
	{
		map<string, Event> ref = this->m_mapFunctionStore;

		map<string, Event>::iterator it = ref.find(str);

		if (it != ref.end())
		{
			it->second();
		}
	}


public:

	cMonoBehavior(); 

	virtual ~cMonoBehavior();
};
