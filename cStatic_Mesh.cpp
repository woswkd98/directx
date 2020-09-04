#include "stdafx.h"
#include "cStatic_Mesh.h"
#include "cCamera.h"


void cStatic_Mesh::Awake()
{

}

void cStatic_Mesh::RenderFrame(XMMATRIX & VP)
{
	XMMATRIX viewProjection;
	
	D3D11_MAPPED_SUBRESOURCE MappedResource;
	g_DeviceManager->GetDC()->Map(m_pVertexCB, 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedResource);
	CB_VS_PER_OBJECT* pVSPerObject = (CB_VS_PER_OBJECT*)MappedResource.pData;
	pVSPerObject->m_matWVP = XMMatrixTranspose(VP);
	pVSPerObject->m_matWorld = XMMatrixTranspose(this->GetGameObject()->GetTransform()->GetFinalMatrix());
	g_DeviceManager->GetDC()->Unmap(m_pVertexCB, 0);
	g_DeviceManager->GetDC()->VSSetConstantBuffers(0, 1, &m_pVertexCB);

	g_DeviceManager->GetDC()->VSSetShader(m_pVertexShader, nullptr, 0);

	material->SetShaderResources();

	g_DeviceManager->GetDC()->PSSetShader(m_pPixelShader, nullptr, NULL);


	//pMesh->RenderFrame(pDC, VertexInfo, indexInfo);
}

cStatic_Mesh::cStatic_Mesh()
{
	//this->m_bUnique = true;
}


cStatic_Mesh::~cStatic_Mesh()
{
}
