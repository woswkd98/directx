#include "stdafx.h"
#include "cMesh_Node.h"
#include "cMaterial.h"


void cMesh_Node::Awake()
{
	this->GetGameObject()->pMesh_Node = this;
}

void cMesh_Node::RenderFrame(ID3D11DeviceContext * pDC, vector<ID3D11Buffer*> * VertexInfo, vector<ID3D11Buffer*>* indexInfo)
{
	// 렌더 코드 나중에 줘야함

	//pCB->m_matWorld = this->gameObject->GetFinalMatrix(); 

	UINT stride = sizeof(ST_VERTEX);
	UINT offset = 0;

	pDC->IASetVertexBuffers(
		0, // 정점 버퍼들을 붙이기 시작할 입력슬롯의 색인
		1, // 버퍼의 개수
		&VertexInfo->begin()[this->bufferIndex], // 첫번쨰 포인터
		&stride, // 보폭들의 배열의 첫원소를 가리키는 포인터
		&offset // 시작지점 
	);

	// shader resourceUpdate


	pDC->IASetIndexBuffer(
		VertexInfo[1][this->bufferIndex], // 버퍼
		DXGI_FORMAT_R16_UINT, // 포멧 형식
		0 // 시작인덱스 위치
	);

	// shaderResource
	
	// 언제 리소스를 넘겨줘야할지 알아봐야한다
	//pDC->PSSetShaderResources(0, 4, this->material->GetArraySRV().data());

	//drawindexed

	pDC->DrawIndexed(this->indexCount, this->locateIndex, this->locateVertex);
}

cMesh_Node::cMesh_Node()
{
}


cMesh_Node::~cMesh_Node()
{
}
