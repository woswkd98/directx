#pragma once
#include "stdafx.h"




static inline void GramSchmidtOrt(XMVECTOR tan, XMVECTOR norm, XMVECTOR binorm, XMFLOAT4 * out)
{
	XMFLOAT3 v;
	XMStoreFloat3(&v, XMVector3Normalize(tan - norm * XMVector3Dot(norm, tan)));
	XMFLOAT3 dotValue;
	XMStoreFloat3(&dotValue, XMVector4Dot(XMVector3Cross(norm, tan), binorm));

	*out = XMFLOAT4(v.x, v.y, v.z, (dotValue.x < 0) ? -1.0f : 1.0f);
}

static inline void ComputeNormal(vector<XMFLOAT3>* vecVector, vector<WORD>* vecIndices, vector<XMFLOAT3>* outNormal) // 노멀 계산 참고로 cross dot하면 2개 나가는 방향백터에 수직인 벡터
{
	for (int i = 0; i < vecIndices->size(); i += 3)
	{

		// 삼각형 한개
		/*
		// v1
		vecVector[vecIndices[i]];
		// v2
		vecVector[vecIndices[i + 1]];
		// v3
		vecVector[vecIndices[i + 2]];
		*/
		XMVECTOR newNormal = XMVector3Normalize(XMVector3Cross(XMLoadFloat3(&vecVector->begin()[vecIndices->begin()[i]]) - XMLoadFloat3(&vecVector->begin()[vecIndices->begin()[i+ 1]]),  // 1- 2
			XMLoadFloat3(&vecVector->begin()[vecIndices->begin()[i + 2]]) - XMLoadFloat3(&vecVector->begin()[vecIndices->begin()[i + 1]])) // 3- 2
		);
		XMStoreFloat3(&outNormal->begin()[i], XMLoadFloat3(&outNormal->begin()[i]) += newNormal);
		XMStoreFloat3(&outNormal->begin()[i + 1], XMLoadFloat3(&outNormal->begin()[i + 1]) += newNormal);
		XMStoreFloat3(&outNormal->begin()[i + 2], XMLoadFloat3(&outNormal->begin()[i + 2]) += newNormal);
	}
}

// 탄젠트 벡터 계산법 directx11을 이용한 게임 프로그래밍 책 655 쪽의 텍스쳐 접공간

static inline XMMATRIX & inverseTranseTranspose(XMMATRIX & m)
{
	XMMATRIX a = m;
	a.r[3] = XMVectorSet(0, 0, 0, 1);
	XMVECTOR det = XMMatrixDeterminant(a);
	return XMMatrixTranspose(XMMatrixInverse(&det,a));
}
/*
static inline HRESULT CompileShader(PWCHAR strPath, D3D10_SHADER_MACRO* pMacros, char* strEntryPoint, char* strProfile, DWORD dwShaderFlags, ID3DBlob** ppVertexShaderBuffer)
{
	HRESULT hr;
	ID3DBlob* pErrorBlob;
		hr = D3DX11CompileFromFile(strPath, pMacros, NULL, strEntryPoint, strProfile, dwShaderFlags, 0, NULL,
		ppVertexShaderBuffer, &pErrorBlob, NULL)))
	{
		int buffSize = pErrorBlob->GetBufferSize() + 1;
		LPWSTR gah = new wchar_t[buffSize];
		MultiByteToWideChar(CP_ACP, 0, (char*)pErrorBlob->GetBufferPointer(), buffSize, gah, buffSize);
		OutputDebugString(gah);
		delete gah;
		OutputDebugString(L"\n");
	}
	return hr;
}
*/

static inline HRESULT CompileShaderFromFile(CHAR * szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob ** ppBlobOut)
{
	HRESULT hr = S_OK;

	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#ifdef _DEBUG

	dwShaderFlags |= D3DCOMPILE_DEBUG;

	dwShaderFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

	ID3DBlob* pErrorBlob = nullptr;
	hr = D3DX11CompileFromFile(szFileName, nullptr, nullptr, szEntryPoint, szShaderModel,
		dwShaderFlags, 0,NULL,ppBlobOut, &pErrorBlob, NULL);

	
	if (FAILED(hr))
	{
		if (pErrorBlob)
		{
			OutputDebugStringA(reinterpret_cast<const char*>(pErrorBlob->GetBufferPointer()));
			pErrorBlob->Release();
		}
		return hr;
	}

	return S_OK;
}

// 선형 공간
static inline XMFLOAT3 GammaToLinear(XMFLOAT3 & color)
{
	return XMFLOAT3(color.x * color.x, color.y * color.y, color.z * color.z);
}

static inline void ComputeTangent( 
	vector<XMFLOAT3> Point, 
	vector<XMFLOAT3> normal, 
	vector<XMFLOAT2> uv,
	vector<XMFLOAT4> * outTangent, 
	vector<XMFLOAT3> * outBinormal)
{
	
	vector<XMFLOAT3> tangent;
	vector<XMFLOAT3> binormal;
	int PointSize = Point.size();
	tangent.clear();
	tangent.resize(PointSize);
	binormal.clear();
	binormal.resize(PointSize);
	outTangent->resize(PointSize);
	outBinormal->resize(PointSize);
	for (int i = 0; i < PointSize - 3; i += 3)
	{
		 // z 축이 u, t
		// x 축이 v, b
	
		
		XMFLOAT2 deltaUV0 = XMFLOAT2(uv[i + 1].x - uv[i].x, uv[i + 1].y - uv[i].y);
		XMFLOAT2 deltaUV1 = XMFLOAT2(uv[i + 2].x - uv[i].x, uv[i + 2].y - uv[i].y);

		/*
			v1 - v0 = du0 * T + dv0 * B 
			v2 - v0 = du1 * T + dv1 * B
			
			이걸 행렬로 만들고 T와 B를 구하기 위해 uv를 넘겨준다 
			넘겨주므로써 역행렬이된다

		*/

		float R = 1 / (deltaUV0.x * deltaUV1.y - deltaUV1.x * deltaUV0.y); // 역행렬 수

		XMFLOAT3 v0 = XMFLOAT3(Point[i + 1].x - Point[i].x, Point[i + 1].y - Point[i].y, Point[i + 1].z - Point[i].z);
		XMFLOAT3 v1 = XMFLOAT3(Point[i + 2].x - Point[i].x, Point[i + 2].y - Point[i].y, Point[i + 2].z - Point[i].z);
		
		tangent[i] = tangent[i + 1] = tangent[i + 2] = XMFLOAT3(
			R * (deltaUV1.y * v0.x + -deltaUV0.y * v1.x),
			R * (deltaUV1.y * v0.y + -deltaUV0.y * v1.y),
			R * (deltaUV1.y * v0.z + -deltaUV0.y * v1.z)
		);

		binormal[i] = binormal[i + 1] = binormal[i + 2] = XMFLOAT3(
			R * (-deltaUV1.x * v0.x + deltaUV0.x * v1.x),
			R * (-deltaUV1.x * v0.y + deltaUV0.x * v1.y),
			R * (-deltaUV1.x * v0.z + deltaUV0.x * v1.z)
		);
		XMVECTOR tan1;
		XMVECTOR norm1;
		XMVECTOR binorm1;


		for (int j = i; j < i + 3; ++j)
		{
			tan1 = XMVectorSet(tangent[j].x, tangent[j].y, tangent[j].z, 0.0f);
			norm1 = XMVectorSet(normal[j].x, normal[j].y, normal[j].z, 0.0f);
			binorm1 = XMVectorSet(binormal[j].x, binormal[j].y, binormal[j].z, 0.0f);
			binorm1 = XMVector3Normalize(binorm1);
			// 끝내기 전에 그람 슈미트 직교화를 한번더 시킨다 (미리 해놓고 세이더 맵핑에선 안할꺼다)
			GramSchmidtOrt(tan1, norm1, binorm1, &outTangent->begin()[j]);
		}
		//
		
	}
	*outBinormal = binormal;
	LOG_MGR->AddLog("%d", outBinormal->size());
}

// 이코드 출처 http://m.blog.daum.net/maru09/3034719 
static inline HRESULT CreateShaderResourceViewFromFile(
	ID3D11Device* device,
	LPCWSTR filename,
	LPCWSTR ext,
	ID3D11ShaderResourceView** pSRV
	)
{
	TexMetadata imageMetadata;

	ScratchImage* pScratchImage = new ScratchImage();

	HRESULT hr;

	if (_wcsicmp(ext, L".dds") == 0)
	{
		hr = LoadFromDDSFile(filename, DDS_FLAGS_NONE, &imageMetadata, *pScratchImage);
	}

	else if (_wcsicmp(ext, L".tga") == 0)

	{
		LOG_MGR->AddLog("tga 들어옴");
		hr = LoadFromTGAFile(filename, &imageMetadata, *pScratchImage);
	}
	else
	{
		hr = LoadFromWICFile(filename, WIC_FLAGS_NONE, &imageMetadata, *pScratchImage);
	}
	if (SUCCEEDED(hr))
	{
		hr = CreateShaderResourceView(
			device,
			pScratchImage->GetImages(),
			pScratchImage->GetImageCount(),
			imageMetadata,
			pSRV);
	}

	delete pScratchImage;

	return hr;
}

static float GetDistance(XMVECTOR * p1, XMVECTOR * p2)
{
	XMVECTOR dirVec = *p1 - *p2;
	XMVECTOR length;
	length = XMVector3Length(dirVec);

	return XMVectorGetX(length); // 길이 구한다
}

static bool IsFloatNull(float val) // 부동소수점 상으로 0 값이냐
{
	if (val < EPSILON && val > -EPSILON) return true;
	return false;
}

static float clampO1(float val)
{ 
	if (val < 0)
		return 0;
	else if (val > 1)
		return 1;
	else
		return val;
}

struct ST_AABB
{
	XMFLOAT3 minXYZ;
	XMFLOAT3 maxXYZ;

public:
	static bool AABBCollision(ST_AABB aabb1, ST_AABB aabb2)
	{
		if (aabb1.maxXYZ.x < aabb2.minXYZ.x || aabb1.minXYZ.x > aabb2.maxXYZ.x) // 2d 충돌과 비슷함 보통 2d 충돌은 여기에 점충돌 추가 나중에 나도 할예정 (충돌후 뒤처리를 위해)
			return false;
	
		if (aabb1.maxXYZ.y < aabb2.minXYZ.y || aabb1.minXYZ.y > aabb2.maxXYZ.y)
			return false;
	
		if (aabb1.maxXYZ.z < aabb2.minXYZ.z || aabb1.minXYZ.z > aabb2.maxXYZ.z)
			return false;
	
		return true;
	}

};


static struct OBB
{
	union
	{
		struct
		{
			XMVECTOR dirForward;
			XMVECTOR dirRight;
			XMVECTOR dirUp;
		};

		XMVECTOR dirArray[3];
	};

	XMFLOAT3 position;

	XMFLOAT3 halfScale;

	OBB()
	{
		ZeroMemory(this, sizeof(OBB));
	}
};
