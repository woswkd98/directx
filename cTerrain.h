#pragma once
#include "cMonoBehavior.h"
class cTerrain : // 높이 변경이 가능한 삼각형 격자 평면 (물결 , 일반 지형에 쓰임)
	public cMonoBehavior
{
private:
	int sizeX = 10;
	int sizeY = 10;

	cMaterial * m_pMaterial;

	

	UINT terrainSmootingNum = 5; // 터레인 스무딩을 몇번에 걸쳐서 할것이냐
public:
	void TerrainSmoothing(); // 터레인의 높이를 부드럽게 해주는 알고리즘
	
	cTerrain();
	~cTerrain();
};

