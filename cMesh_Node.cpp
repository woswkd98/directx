#include "stdafx.h"
#include "cMesh_Node.h"
#include "cMaterial.h"


void cMesh_Node::Awake()
{
	this->GetGameObject()->pMesh_Node = this;
}

void cMesh_Node::RenderFrame(ID3D11DeviceContext * pDC, vector<ID3D11Buffer*> * VertexInfo, vector<ID3D11Buffer*>* indexInfo)
{
	// ���� �ڵ� ���߿� �����

	//pCB->m_matWorld = this->gameObject->GetFinalMatrix(); 

	UINT stride = sizeof(ST_VERTEX);
	UINT offset = 0;

	pDC->IASetVertexBuffers(
		0, // ���� ���۵��� ���̱� ������ �Է½����� ����
		1, // ������ ����
		&VertexInfo->begin()[this->bufferIndex], // ù���� ������
		&stride, // �������� �迭�� ù���Ҹ� ����Ű�� ������
		&offset // �������� 
	);

	// shader resourceUpdate


	pDC->IASetIndexBuffer(
		VertexInfo[1][this->bufferIndex], // ����
		DXGI_FORMAT_R16_UINT, // ���� ����
		0 // �����ε��� ��ġ
	);

	// shaderResource
	
	// ���� ���ҽ��� �Ѱ�������� �˾ƺ����Ѵ�
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
