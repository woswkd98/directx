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
	if (!Success) LOG_MGR->AddLog("FbxImporter Initialize  안됨");
	Success = FbxScene::Create(m_pFbxSdkManager, "");
	if (!Success) LOG_MGR->AddLog("Create pFbxScene  안됨");

	Success = pImporter->Import(pFbxScene);
	if (!Success) LOG_MGR->AddLog("Import pFbxScene  안됨");

	pImporter->Destroy(); // 생성된거 삭제

	str.clear();

	str = nameSet;

	FbxNode * pRootNode = pFbxScene->GetRootNode(); // 루트노드 받아온다
	if (!pRootNode) LOG_MGR->AddLog("root가 없다 ");

	int vertex3Count = 0;
	int vertex4Count = 0;
	int uvCount = 0;
	int indexCount = 0;
	int vertexId = 0;
	m_mapModelRootNode.insert(make_pair(str, pRootNode)); // 루트노드 넣는곳에 넣음
	
	vector<ST_VERTEX> vecVertex;


	vector<XMFLOAT3> pOutModel; // 모델의 정점을 담을 정점배열
	vector<XMFLOAT3> pVecNormal; // 노멀 가져옴
	vector<XMFLOAT3> pVecBinormal; // 바이노말 가져옴
	vector<XMFLOAT3> pVecTangent; // 탄젠트 가져옴
	vector<XMFLOAT2> pVecUV; // 탄젠트 가져옴
	vector<WORD> pOutModelIndex; // 인덱스를 담을 인덱스 배열
	

	int tempSize = 0;

	LOG_MGR->AddLog(" childNum : %d ", (int)(pRootNode->GetChildCount()));
	for (int i = 0; i < pRootNode->GetChildCount(); ++i) // 자식개수만큼돌린다
	{
		string name = pRootNode->GetChild(i)->GetName(); // 이름 저장

		FbxNode* pFbxChildNode = pRootNode->GetChild(i); // 노드 포인터 들고옴

		if (pFbxChildNode->GetNodeAttribute() == NULL) // 속성값이 NULL 이면 리턴
			continue;

		FbxNodeAttribute::EType AttributeType =
			pFbxChildNode->GetNodeAttribute()->GetAttributeType();

		if (AttributeType != FbxNodeAttribute::eMesh) // 속성값이 메시가 아니면 리턴
			continue;

		FbxMesh* pMesh = static_cast<FbxMesh*>(pFbxChildNode->GetNodeAttribute()); // 속성을 메시포인터로 넘겨줌
		uvCount += pMesh->GetElementUVCount();
		int startVertexNum = pOutModel.size(); // 나중에 drawIndexed를 위한 정점 시작지점
		int startIndexNum = pOutModelIndex.size(); // 나중에 drawIndexed를 위한 인덱스 시작지점
		int indexCount = 0; //  나중에 drawIndexed를 위한 인덱스 카운트

							/*
							FbxVector4 * p = pMesh->GetControlPoints();

							for (int i = 0; i < pMesh->GetControlPointsCount(); ++i)
							{

							pOutModel.push_back(XMFLOAT3( // 정점 먼저 저장
							p[i].mData[0],
							p[i].mData[1],
							p[i].mData[2]));
							}
							*/

		for (int j = 0; j < pMesh->GetPolygonCount(); ++j) // 다각형의 개수만큼돌린다
		{
			int uVertexCount = pMesh->GetPolygonSize(j); // 다각형들 중 j 번쨰에 있는 다각형 안의 정점 개수를 구한다

			ST_VERTEX vertex;

			if (uVertexCount == 3) // 3개일경우 일반적인 저장
			{
				for (int k = 0; k < uVertexCount; ++k) // 정점개수 만큼 돌린다
				{
					int uVertexIndex = pMesh->GetPolygonVertex(j, k); // 정점 인덱스를 구한다
																	  //tempSize++;
					FbxVector4 p = pMesh->GetControlPointAt(uVertexIndex);

					vertex.pos = XMFLOAT3( // 정점 먼저 저장
						p.mData[0],
						p.mData[1],
						p.mData[2]);


					
					indexCount++;
				}
				vertex3Count++;

				// 여기서 3각형 정점을 인덱스로 함축시키지 않고 다넣는 이유는 
				// 삼각형의 기본 위치정보와는 별개로 uv값이나 normal값이 정점의 인덱스에 따라 다 다르기 때문에
				// 그것을 포함한 새로운 정점을 만들기 위해서 이다
			}
			else if (uVertexCount == 4)
			{
				FbxVector4 p[4];
				ZeroMemory(p, 4);
				int arraytemp[4];
				for (int k = 0; k < uVertexCount; ++k) // 정점개수 만큼 돌린다
				{
					tempSize++;
					arraytemp[k] = pMesh->GetPolygonVertex(j, k); // 정점 인덱스를 구한다
					p[k] = pMesh->GetControlPointAt(pMesh->GetPolygonVertex(j, k));

					vertex.pos = XMFLOAT3( // 정점 먼저 저장
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

			// 여기서부터 기타 정보들고 온다  (uv, tangent, binormal, normal)

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
제어 포인트는 꼭지점 동의어하는 XYZ 좌표이다.
 폴리곤 정점이 제어점에 대한 인덱스이고,는 정점 인덱스 동의어이다.
 다각형은 다각형의 정점의 그룹이다.


열거   ESplitObject { eBY_NORMAL }
관심의 대상 spliting. 더 많이 ...

타입 정의 KArrayTemplate
< KDuplicateVertex >   KArrayOfDuplicateVertex
관심의 개체 spliting 때.

빈   재설정 ()
기본값으로 메시를 재설정합니다.

빈   ComputeVertexNormals (부울 PCW = 거짓)
메쉬의 정점 법선을 계산합니다.

부울   CheckIfVertexNormalsCCW ()
다각형의 정점 사이에 정상 배열에 저장되어있는 사람에 의해 교차 제품을 수행하여 계산 된 법선을 비교합니다.

부울   CheckSamePointTwice ()
메시가 두 번 이상 동일한 지점에 정의 된 다각형이 있는지 확인합니다.

INT   RemoveBadPolygons ()
메쉬에서 나쁜 다각형을 제거합니다.



다각형 관리

빈   BeginPolygon (INT pMaterial = -1, INT pTexture = -1, INT pGroup = -1, 부울 pLegacy = 참)
다각형을 쓰기 시작합니다.

빈   BeginPolygonExt (INT pMaterial, INT * pTextures)
다각형을 쓰기 시작합니다.

빈   AddPolygon (INT pIndex, INT pTextureUVIndex = -1)
제어 포인트에 대한 인덱스를 추가 (즉,

빈   EndPolygon ()
다각형을 작성 종료합니다.

INT   GetPolygonCount () const를
이 메쉬의 폴리곤 수를 가져옵니다.

INT   GetPolygonSize (INT pPolygonIndex) const를
다각형에 다각형의 꼭지점의 수를 가져옵니다.

INT   GetPolygonGroup (INT pPolygonIndex) const를
다각형에 할당 된 그룹 인덱스를 가져옵니다.

INT   GetPolygonVertex (INT pPolygonIndex, INT pPositionInPolygon) const를
다각형의 정점을 가져옵니다 (예 :

빈   GetPolygonVertexNormal (INT pPolyIndex, INT pVertexIndex, KFbxVector4 & pNormal) const를
다각형 / 정점과 관련된 정상를 가져옵니다.

INT *   GetPolygonVertices () const를
다각형 정점의 배열을 가져옵니다 (예 :

INT   GetPolygonVertexCount () const를
메쉬의 폴리곤 정점의 수를 가져옵니다.

INT   GetPolygonVertexIndex (INT pPolygonIndex) const를
에 의해 반환 된 배열에 시작 인덱스를 가져옵니다 GetPolygonVertices () 주어진 다각형.

INT   RemovePolygon (INT pPolygonIndex)
메쉬에서이 다각형을 제거합니다.



*/


