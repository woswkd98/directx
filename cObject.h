#pragma once
class cGameObject;
class cObject
{
private:
	static unsigned int count;
protected:
	bool m_bActive; // ��Ƽ�� �÷���
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

	static void ObjectsDelete(); // Ȥ�ó� ������ ������Ʈ Ŭ���� ����
public:
	cObject();

	virtual ~cObject();

};

