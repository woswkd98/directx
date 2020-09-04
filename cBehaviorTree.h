#pragma once
#include "cMonoBehavior.h"





typedef function<bool(bool)> FuncBehavior;

class cBehavior // 스크립트 앞으로의 모든 bt tree의 노드안에 있는 스크립트는 얘를 상속받는다 
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

	cGameObject * m_pGameObject; // behaviorTreeComponent의 게임오브젝트를 들고 온다 

	FuncBehavior m_function; // 

	vector<cBehavior*> m_pScripts; // 스크립트들 

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
		for (int i = 0; i < m_pScripts.size(); ++i) // 내가 넣은 스크립트 실행
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



