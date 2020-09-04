#pragma once

// ����� Ŭ���� ���ø��� ���Ͽ� static ������ 
// �����ص� ������ Ŭ������ ���� ������ ������ 

//http://www.gilgil.net/3079

// �Լ� ���� ����
/*
Init()
{
	Awake();
}

Update()
{
	// ���ο� ���ӿ�����Ʈ ����

	virtual void Start() {} // ù �������� ���� ���۵Ǵ� ������

	virtual void OnEnable() {} // ���ӿ�����Ʈ�� �������� ����Ǵ� �޼��� 

	virtual void Update(float deltaSce) {} // �Ϲ� ������Ʈ��

	virtual void LateUpdate() {} // ������Ʈ ����� ������ ������Ʈ

	virtual void OnDisable() {} // ���ӿ�����Ʈ�� �������� ����Ǵ� �޼���

	virtual void Release() {} // ���� ������Ʈ ���� �� �� ����

	// ���ӿ�����Ʈ ����
}

	 





*/

class cGameObject;
class cMonoBehavior : public cObject
{
private:
	cGameObject * gameObject = nullptr;
protected:
	map<string, Event> m_mapFunctionStore; // ���� ���ѳ��ٱ� �Լ��� �ҷ��� 
public:
	cGameObject * GetGameObject() { return gameObject; }
	virtual void Awake();

	virtual void Start() {} // ù �������� ���� ���۵Ǵ� ������

	virtual void Update(float dt) {} // �Ϲ� ������Ʈ��

	virtual void LateUpdate() {} // ������Ʈ ����� ������ ������Ʈ

	virtual void FixedUpdate() {} // ������ ������ ���ư��� ������Ʈ ���� Ÿ�̸�
	
	virtual void BackGroundUpdate() {}  // ���ӿ�����Ʈ�� ������� �ʾƵ� ���� ������Ʈ

	virtual void OnEnable() {} // ���ӿ�����Ʈ�� �������� ����Ǵ� �޼��� 

	virtual void OnDisable() {} // ���ӿ�����Ʈ�� �������� ����Ǵ� �޼���  

	virtual void Release() {} // ���� ������Ʈ ���� �� �� ����

	virtual void Destroy() {}
public:
	virtual void SetActive(bool active);
	
	static void DestroyGameObject(cGameObject * pGAameObject);// ���ӿ�����Ʈ ���� 

	void SetGameObject(cGameObject * pGAameObject) {
		gameObject = pGAameObject;
	}

	cGameObject * CopyGameObject(cGameObject * pInGameObject, cGameObject * pOutGameObject = nullptr); // ���� ������Ʈ ���� ��ü �����ؼ� ���Ͻ����� 
private: 
	template<class T>
	T * newComponent(T * t)
	{
		T * newT = new T;
		*newT = *t; // ���� ����
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
