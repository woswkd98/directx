#pragma once
#include "cMonoBehavior.h"


class cKeyController :
	public cMonoBehavior
{
private:
	static cKeyController * g_pController; // 1��Ī �ڵ������ӿ����� ��Ʈ�ѷ��� �ϳ��̴� ���� ��Ʈ�ѷ� �������� �ϳ��� �������
private:
	static vector<unsigned char> keySet;
	static vector<bool> downClick, upClick;

	float speed; // �ڵ����� �ӵ��ø����� ����
	float steering; // ȸ�� ������
	float angle; // �ڵ����� �ڵ鸵 ����
public:
	void Awake() override;
	void ControllerChange(); // ��Ʈ�ѷ� �ٲٱ� 

	void KeyUpdate(); // Ű������Ʈ

	void AddKey(int key); // Ű �߰�
	 
	void SetAccelScale(float speed); // ���� ����
	void HandlingScale(float angle); // �ڵ鸵 ���� ����

private:
	bool IsStayDown(int nVKey); // Ű�Է� �Լ�
	bool IsOnceDown(int nVKey);
	bool IsOnceUp(int nVKey);

	cKeyController();
	~cKeyController();
};

