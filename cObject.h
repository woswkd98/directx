#pragma once
class cGameObject;
class cObject
{
private:
	static unsigned int count;
protected:
	bool m_bActive; // 액티브 플래그
	string name;
private:
	 static list<cObject*> pObject;
private:

protected:
	
public:			


	// active Get, Set
	virtual bool GetActive() { return m_bActive; }
	virtual void SetActive(bool active); 
	virtual string GetName() { return name; }
	virtual void SetName(string str) { name = str; }

	static void ObjectsDelete(); // 혹시나 남았을 오브젝트 클래스 삭제
public:
	cObject();

	virtual ~cObject();

};

