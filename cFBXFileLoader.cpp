#include "stdafx.h"
#include "cFBXFileLoader.h"



void cFBXFileLoader::GetNormal(FbxMesh * pMesh, vector<XMFLOAT3>* vecNormal)
{
}

void cFBXFileLoader::GetBinorm(FbxMesh * pMesh, vector<XMFLOAT3>* vecBinorm)
{
}

void cFBXFileLoader::GetTangent(FbxMesh * pMesh, vector<XMFLOAT3>* vecTangent)
{
}

void cFBXFileLoader::Init()
{
	if (m_pFbxSdkManager == nullptr)
	{
		m_pFbxSdkManager = FbxManager::Create();

		FbxIOSettings* pIOsettings = FbxIOSettings::Create(m_pFbxSdkManager, IOSROOT);
		m_pFbxSdkManager->SetIOSettings(pIOsettings);
	}
	
}

vector<ST_VERTEX>  * cFBXFileLoader::GetStaticMesh(char * nameSet, char * path, vector<WORD> outIndex)
{
	FbxImporter* pImporter = FbxImporter::Create(m_pFbxSdkManager, "");
	FbxScene* pFbxScene = FbxScene::Create(m_pFbxSdkManager, "");

	string str = path;
	LOG_MGR->AddLog("%s", str.c_str());
	bool Success = pImporter->Initialize(str.c_str(), -1, m_pFbxSdkManager->GetIOSettings());
	if (!Success) LOG_MGR->AddLog("FbxImporter Initialize  �ȵ�");
	Success = FbxScene::Create(m_pFbxSdkManager, "");
	if (!Success) LOG_MGR->AddLog("Create pFbxScene  �ȵ�");

	Success = pImporter->Import(pFbxScene);
	if (!Success) LOG_MGR->AddLog("Import pFbxScene  �ȵ�");

	pImporter->Destroy(); // �����Ȱ� ����

	str.clear();

	str = nameSet;

	FbxNode * pRootNode = pFbxScene->GetRootNode(); // ��Ʈ��� �޾ƿ´�
	if (!pRootNode) LOG_MGR->AddLog("root�� ���� ");

	int vertex3Count = 0;
	int vertex4Count = 0;
	int uvCount = 0;
	int indexCount = 0;
	int vertexId = 0;
	m_mapModelRootNode.insert(make_pair(str, pRootNode)); // ��Ʈ��� �ִ°��� ����
	
	vector<ST_VERTEX> vecVertex;


	vector<XMFLOAT3> pOutModel; // ���� ������ ���� �����迭
	vector<XMFLOAT3> pVecNormal; // ��� ������
	vector<XMFLOAT3> pVecBinormal; // ���̳븻 ������
	vector<XMFLOAT3> pVecTangent; // ź��Ʈ ������
	vector<XMFLOAT2> pVecUV; // ź��Ʈ ������
	vector<WORD> pOutModelIndex; // �ε����� ���� �ε��� �迭
	

	int tempSize = 0;

	LOG_MGR->AddLog(" childNum : %d ", (int)(pRootNode->GetChildCount()));
	for (int i = 0; i < pRootNode->GetChildCount(); ++i) // �ڽİ�����ŭ������
	{
		string name = pRootNode->GetChild(i)->GetName(); // �̸� ����

		FbxNode* pFbxChildNode = pRootNode->GetChild(i); // ��� ������ ����

		if (pFbxChildNode->GetNodeAttribute() == NULL) // �Ӽ����� NULL �̸� ����
			continue;

		FbxNodeAttribute::EType AttributeType =
			pFbxChildNode->GetNodeAttribute()->GetAttributeType();

		if (AttributeType != FbxNodeAttribute::eMesh) // �Ӽ����� �޽ð� �ƴϸ� ����
			continue;

		FbxMesh* pMesh = static_cast<FbxMesh*>(pFbxChildNode->GetNodeAttribute()); // �Ӽ��� �޽������ͷ� �Ѱ���
		uvCount += pMesh->GetElementUVCount();
		int startVertexNum = pOutModel.size(); // ���߿� drawIndexed�� ���� ���� ��������
		int startIndexNum = pOutModelIndex.size(); // ���߿� drawIndexed�� ���� �ε��� ��������
		int indexCount = 0; //  ���߿� drawIndexed�� ���� �ε��� ī��Ʈ

							/*
							FbxVector4 * p = pMesh->GetControlPoints();

							for (int i = 0; i < pMesh->GetControlPointsCount(); ++i)
							{

							pOutModel.push_back(XMFLOAT3( // ���� ���� ����
							p[i].mData[0],
							p[i].mData[1],
							p[i].mData[2]));
							}
							*/

		for (int j = 0; j < pMesh->GetPolygonCount(); ++j) // �ٰ����� ������ŭ������
		{
			int uVertexCount = pMesh->GetPolygonSize(j); // �ٰ����� �� j ������ �ִ� �ٰ��� ���� ���� ������ ���Ѵ�

			ST_VERTEX vertex;

			if (uVertexCount == 3) // 3���ϰ�� �Ϲ����� ����
			{
				for (int k = 0; k < uVertexCount; ++k) // �������� ��ŭ ������
				{
					int uVertexIndex = pMesh->GetPolygonVertex(j, k); // ���� �ε����� ���Ѵ�
																	  //tempSize++;
					FbxVector4 p = pMesh->GetControlPointAt(uVertexIndex);

					vertex.pos = XMFLOAT3( // ���� ���� ����
						p.mData[0],
						p.mData[1],
						p.mData[2]);


					
					indexCount++;
				}
				vertex3Count++;

				// ���⼭ 3���� ������ �ε����� �����Ű�� �ʰ� �ٳִ� ������ 
				// �ﰢ���� �⺻ ��ġ�����ʹ� ������ uv���̳� normal���� ������ �ε����� ���� �� �ٸ��� ������
				// �װ��� ������ ���ο� ������ ����� ���ؼ� �̴�
			}
			else if (uVertexCount == 4)
			{
				FbxVector4 p[4];
				ZeroMemory(p, 4);
				int arraytemp[4];
				for (int k = 0; k < uVertexCount; ++k) // �������� ��ŭ ������
				{
					tempSize++;
					arraytemp[k] = pMesh->GetPolygonVertex(j, k); // ���� �ε����� ���Ѵ�
					p[k] = pMesh->GetControlPointAt(pMesh->GetPolygonVertex(j, k));

					vertex.pos = XMFLOAT3( // ���� ���� ����
						p[k].mData[0],
						p[k].mData[1],
						p[k].mData[2]);

				}
				arraytemp[0] = indexCount++;
				arraytemp[1] = indexCount++;
				arraytemp[2] = indexCount++;
				arraytemp[3] = indexCount++;


				outIndex.push_back(arraytemp[0]);
				outIndex.push_back(arraytemp[1]);
				outIndex.push_back(arraytemp[2]);

				outIndex.push_back(arraytemp[0]);
				outIndex.push_back(arraytemp[2]);
				outIndex.push_back(arraytemp[3]);
				vertex4Count++;
			}

			// ���⼭���� ��Ÿ ������� �´�  (uv, tangent, binormal, normal)

			// child Count i , polygonCount j , lPolygonSize(j) k 
			for (int k = 0; k < uVertexCount; ++k)
			{

				int lControlPointIndex = pMesh->GetPolygonVertex(j, k);


				for (UINT l = 0; l < pMesh->GetElementUVCount(); ++l)
				{
					FbxGeometryElementUV* leUV = pMesh->GetElementUV(l);

					FbxVector2 temp;
					switch (leUV->GetMappingMode())
					{
					default:
						break;
					case FbxGeometryElement::eByControlPoint:
						switch (leUV->GetReferenceMode())
						{
						case FbxGeometryElement::eDirect:
							temp = leUV->GetDirectArray().GetAt(lControlPointIndex);
							vertex.UV = XMFLOAT2(
								temp.mData[0],
								temp.mData[1]
							);
							break;
						case FbxGeometryElement::eIndexToDirect:
						{
							int id = leUV->GetIndexArray().GetAt(lControlPointIndex);
							temp = leUV->GetDirectArray().GetAt(id);
							vertex.UV = XMFLOAT2(
								temp.mData[0],
								temp.mData[1]
							);
						}
						break;
						default:
							break;
						}
						break;

					case FbxGeometryElement::eByPolygonVertex:
					{
						int lTextureUVIndex = pMesh->GetTextureUVIndex(j, k);
						switch (leUV->GetReferenceMode())
						{
						case FbxGeometryElement::eDirect:
						case FbxGeometryElement::eIndexToDirect:
						{
							temp = leUV->GetDirectArray().GetAt(lTextureUVIndex);
							vertex.UV = XMFLOAT2(
								temp.mData[0],
								temp.mData[1]
							);

						}
						break;
						default:
							break;
						}
					}
					break;

					case FbxGeometryElement::eByPolygon:
					case FbxGeometryElement::eAllSame:
					case FbxGeometryElement::eNone:
						break;
					}


				}

				for (UINT l = 0; l < pMesh->GetElementNormalCount(); ++l)
				{
					FbxGeometryElementNormal* leNormal = pMesh->GetElementNormal(l);

					FbxVector4 temp = FbxVector4(0, 0, 0, 0);

					if (leNormal->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
					{
						switch (leNormal->GetReferenceMode())
						{
						case FbxGeometryElement::eDirect:
							temp = leNormal->GetDirectArray().GetAt(vertexId);
							vertex.normal = XMFLOAT3(temp.mData[0], temp.mData[1], temp.mData[2]);
							break;
						case FbxGeometryElement::eIndexToDirect:
						{
							int id = leNormal->GetIndexArray().GetAt(vertexId);
							temp = leNormal->GetDirectArray().GetAt(id);
							vertex.normal = XMFLOAT3(temp.mData[0], temp.mData[1], temp.mData[2]);
						}
						break;
						default:
							break; // other reference modes not shown here!
						}
					}



				}

				for (UINT l = 0; l < pMesh->GetElementTangentCount(); ++l)
				{
					FbxGeometryElementTangent* leTangent = pMesh->GetElementTangent(l);

					FbxVector4 temp = FbxVector4(0, 0, 0, 0);
					if (leTangent->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
					{
						switch (leTangent->GetReferenceMode())
						{
						case FbxGeometryElement::eDirect:
							temp = leTangent->GetDirectArray().GetAt(vertexId);
							vertex.tangent = XMFLOAT3(temp.mData[0], temp.mData[1], temp.mData[2]);
							break;
						case FbxGeometryElement::eIndexToDirect:
						{
							int id = leTangent->GetIndexArray().GetAt(vertexId);
							temp = leTangent->GetDirectArray().GetAt(id);
							vertex.tangent = XMFLOAT3(temp.mData[0], temp.mData[1], temp.mData[2]);
						}
						break;
						default:
							break;
						}
					}



				}
				for (UINT l = 0; l < pMesh->GetElementBinormalCount(); ++l)
				{

					FbxGeometryElementBinormal* leBinormal = pMesh->GetElementBinormal(l);

					FbxVector4 temp = FbxVector4(0, 0, 0, 0);

					if (leBinormal->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
					{
						switch (leBinormal->GetReferenceMode())
						{
						case FbxGeometryElement::eDirect:
							temp = leBinormal->GetDirectArray().GetAt(vertexId);

							vertex.binormal = XMFLOAT3(temp.mData[0], temp.mData[1], temp.mData[2]);
							break;
						case FbxGeometryElement::eIndexToDirect:
						{
							int id = leBinormal->GetIndexArray().GetAt(vertexId);
							temp = leBinormal->GetDirectArray().GetAt(id);

							vertex.binormal = XMFLOAT3(temp.mData[0], temp.mData[1], temp.mData[2]);
						}
						break;
						default:
							break; // other reference modes not shown here!
						}
					}


				}
				vertexId++;
			}
			vecVertex.push_back(vertex);
		}
		
	}
	LOG_MGR->AddLog("%d ,v3", vertex3Count);
	LOG_MGR->AddLog("%d, v4", vertex4Count);
	LOG_MGR->AddLog("%d, tempsize", tempSize);
	LOG_MGR->AddLog("%d", pOutModel.size());
	LOG_MGR->AddLog("%d", uvCount);

	return &vecVertex;


}


void cFBXFileLoader::Release()
{
	map<string, FbxNode*>::iterator it = m_mapModelRootNode.begin();
	for (it; it != m_mapModelRootNode.end(); ++it)
	{
		it->second->Destroy();
	}

	m_mapModelRootNode.clear();
	
	m_pFbxSdkManager->Destroy();

}

cFBXFileLoader::cFBXFileLoader()
{
}


cFBXFileLoader::~cFBXFileLoader()
{
}


/*
���� ����Ʈ�� ������ ���Ǿ��ϴ� XYZ ��ǥ�̴�.
 ������ ������ �������� ���� �ε����̰�,�� ���� �ε��� ���Ǿ��̴�.
 �ٰ����� �ٰ����� ������ �׷��̴�.


����   ESplitObject { eBY_NORMAL }
������ ��� spliting. �� ���� ...

Ÿ�� ���� KArrayTemplate
< KDuplicateVertex >   KArrayOfDuplicateVertex
������ ��ü spliting ��.

��   �缳�� ()
�⺻������ �޽ø� �缳���մϴ�.

��   ComputeVertexNormals (�ο� PCW = ����)
�޽��� ���� ������ ����մϴ�.

�ο�   CheckIfVertexNormalsCCW ()
�ٰ����� ���� ���̿� ���� �迭�� ����Ǿ��ִ� ����� ���� ���� ��ǰ�� �����Ͽ� ��� �� ������ ���մϴ�.

�ο�   CheckSamePointTwice ()
�޽ð� �� �� �̻� ������ ������ ���� �� �ٰ����� �ִ��� Ȯ���մϴ�.

INT   RemoveBadPolygons ()
�޽����� ���� �ٰ����� �����մϴ�.



�ٰ��� ����

��   BeginPolygon (INT pMaterial = -1, INT pTexture = -1, INT pGroup = -1, �ο� pLegacy = ��)
�ٰ����� ���� �����մϴ�.

��   BeginPolygonExt (INT pMaterial, INT * pTextures)
�ٰ����� ���� �����մϴ�.

��   AddPolygon (INT pIndex, INT pTextureUVIndex = -1)
���� ����Ʈ�� ���� �ε����� �߰� (��,

��   EndPolygon ()
�ٰ����� �ۼ� �����մϴ�.

INT   GetPolygonCount () const��
�� �޽��� ������ ���� �����ɴϴ�.

INT   GetPolygonSize (INT pPolygonIndex) const��
�ٰ����� �ٰ����� �������� ���� �����ɴϴ�.

INT   GetPolygonGroup (INT pPolygonIndex) const��
�ٰ����� �Ҵ� �� �׷� �ε����� �����ɴϴ�.

INT   GetPolygonVertex (INT pPolygonIndex, INT pPositionInPolygon) const��
�ٰ����� ������ �����ɴϴ� (�� :

��   GetPolygonVertexNormal (INT pPolyIndex, INT pVertexIndex, KFbxVector4 & pNormal) const��
�ٰ��� / ������ ���õ� ���� �����ɴϴ�.

INT *   GetPolygonVertices () const��
�ٰ��� ������ �迭�� �����ɴϴ� (�� :

INT   GetPolygonVertexCount () const��
�޽��� ������ ������ ���� �����ɴϴ�.

INT   GetPolygonVertexIndex (INT pPolygonIndex) const��
�� ���� ��ȯ �� �迭�� ���� �ε����� �����ɴϴ� GetPolygonVertices () �־��� �ٰ���.

INT   RemovePolygon (INT pPolygonIndex)
�޽������� �ٰ����� �����մϴ�.



*/


