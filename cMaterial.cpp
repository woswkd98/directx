#include "stdafx.h"
#include "cMaterial.h"

ID3D11ShaderResourceView * cMaterial::DefaultDiffuseSRV = nullptr;
ID3D11ShaderResourceView * cMaterial::DefaultEmissionSRV = nullptr;
ID3D11ShaderResourceView * cMaterial::DefaultNormalSRV = nullptr;
ID3D11ShaderResourceView * cMaterial::DefaultSpecularSRV = nullptr;

map<string, cMaterial::SRVInfo> cMaterial::g_mapMaterial;


void cMaterial::GetTextrueResource(
	wchar_t *path,
	wchar_t* ext,
	string textureName,
	eTextureType textureType,
	ID3D11Device * Dev
)
{
	ID3D11ShaderResourceView * shaderView = nullptr;
	HRESULT hr;

	SRVInfo temp;
	map<string, SRVInfo>::iterator it = g_mapMaterial.find(textureName);
	if (it == g_mapMaterial.end())
	{
		
		hr = CreateShaderResourceViewFromFile(Dev, path, ext, &temp.SRV );
		if (hr == E_FAIL)
		{
			LOG_MGR->AddLog("텍스쳐 파일에서 불러오는 것 실패");
			return;
		}

		temp.type = textureType;
		g_mapMaterial.insert(make_pair(textureName, temp));
		shaderView = temp.SRV;
	}
	else
	{
		shaderView = it->second.SRV;
	}

	switch (textureType)
	{
	case eTextureType::diffuse:
		m_vecSRV[0] = shaderView;
		break;
	case eTextureType::bump:
		m_vecSRV[1] = shaderView;

		break;
	case eTextureType::specular:
		m_vecSRV[2] = shaderView;

		break;
	case eTextureType::Emission:
		m_vecSRV[3] = shaderView;
		break;
	}

}



void cMaterial::SetShaderResources()
{
	g_DeviceManager->GetDC()->PSSetShaderResources(0, m_vecSRV.size(), m_vecSRV.data());
}


void cMaterial::DeleteDefaultSRV()
{
	for (map<string, SRVInfo>::iterator it = g_mapMaterial.begin(); it != g_mapMaterial.end(); ++it)
	{
		SAFE_RELEASE(it->second.SRV);
	}


	if(DefaultDiffuseSRV) cMaterial::DefaultDiffuseSRV->Release();
	if (DefaultDiffuseSRV) cMaterial::DefaultNormalSRV->Release();
	if (DefaultDiffuseSRV) cMaterial::DefaultSpecularSRV->Release();
	if (DefaultDiffuseSRV) cMaterial::DefaultEmissionSRV->Release();
}

void cMaterial::CreateDefaultSRV()
{
	HRESULT hr;
	hr = CreateShaderResourceViewFromFile(g_DeviceManager->GetDevice(), L"diffuseDefault.png", L"png", &cMaterial::DefaultDiffuseSRV);
	if (hr == E_FAIL) { LOG_MGR->AddLog("diffuseDefault fail"); }

	hr = CreateShaderResourceViewFromFile(g_DeviceManager->GetDevice(), L"normalDefault.png", L"png", &cMaterial::DefaultNormalSRV);
	if (hr == E_FAIL) { LOG_MGR->AddLog("normalDefault fail"); }

	hr = CreateShaderResourceViewFromFile(g_DeviceManager->GetDevice(), L"specularDefault.png", L"png", &cMaterial::DefaultSpecularSRV);
	if (hr == E_FAIL) { LOG_MGR->AddLog("specularDefault fail"); }

	hr = CreateShaderResourceViewFromFile(g_DeviceManager->GetDevice(), L"emissionDefault.png", L"png", &cMaterial::DefaultEmissionSRV);
	if (hr == E_FAIL) { LOG_MGR->AddLog("emissionDefault fail"); }
}





cMaterial::cMaterial()
{
	m_vecSRV.resize(4, nullptr);

	m_vecSRV[0] = DefaultDiffuseSRV;
	m_vecSRV[1] = DefaultNormalSRV;
	m_vecSRV[2] = DefaultSpecularSRV;
	m_vecSRV[3] = DefaultEmissionSRV;
}


cMaterial::~cMaterial()
{
}
/*
[reinterpret_cast]

비트열로만 보고 원하는 형으로 강제로 변환한다. 포인터를 바로 정수로 변환할 수도 있다.

가장 빠르고 가장 자유로운 형변환


*/