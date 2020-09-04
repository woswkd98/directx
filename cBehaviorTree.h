#pragma once
#include "cMonoBehavior.h"





typedef function<bool(bool)> FuncBehavior;

class cBehavior // ��ũ��Ʈ �������� ��� bt tree�� ���ȿ� �ִ� ��ũ��Ʈ�� �긦 ��ӹ޴´� 
{

public:
	virtual void Update() = 0;
	



};

class iBehaviorTreeNode
{



};




class cBehaviorNode 
{
private:
	string m_name;

	cGameObject * m_pGameObject; // behaviorTreeComponent�� ���ӿ�����Ʈ�� ��� �´� 

	FuncBehavior m_function; // 

	vector<cBehavior*> m_pScripts; // ��ũ��Ʈ�� 

public:
	void SetName(string name)
	{
		m_name = name;
	}

	void SetFunction(FuncBehavior Func)
	{
		m_function = Func;
	}
	
	void SetGameObject(cGameObject * p)
	{
		m_pGameObject = p;
	}

	virtual bool Run()
	{
		for (int i = 0; i < m_pScripts.size(); ++i) // ���� ���� ��ũ��Ʈ ����
		{
			m_pScripts[i]->Update();
		}

		if (m_function(m_pGameObject))
		{

		}

		return true;
	}
public:

};

class cBehaviorTree :
	public cMonoBehavior
{
private:
private:


	float m_fSimulateTime;

public:
	cBehaviorTree();
	~cBehaviorTree();
};



