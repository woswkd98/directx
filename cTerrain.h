#pragma once
#include "cMonoBehavior.h"
class cTerrain : // ���� ������ ������ �ﰢ�� ���� ��� (���� , �Ϲ� ������ ����)
	public cMonoBehavior
{
private:
	int sizeX = 10;
	int sizeY = 10;

	cMaterial * m_pMaterial;

	

	UINT terrainSmootingNum = 5; // �ͷ��� �������� ����� ���ļ� �Ұ��̳�
public:
	void TerrainSmoothing(); // �ͷ����� ���̸� �ε巴�� ���ִ� �˰���
	
	cTerrain();
	~cTerrain();
};

