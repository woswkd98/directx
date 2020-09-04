#pragma once



class cMaterial 
{
	


private:

	static ID3D11ShaderResourceView * DefaultDiffuseSRV;

	static ID3D11ShaderResourceView * DefaultSpecularSRV;

	static ID3D11ShaderResourceView * DefaultEmissionSRV;

	static ID3D11ShaderResourceView * DefaultNormalSRV;

public:
	string m_strMaterialName;
public:
	// 나중에 지정할 텍스쳐 디멘션
	enum eTextureDimension
	{
		oneDim, TwoDim, ThrDim
	};

	// 지정할 텍스쳐 타입

	static enum eTextureType
	{
		diffuse, specular, normal, bump = normal, Emission
	};


	static enum eTextureDataType // 텍스쳐 설정할때 데이터 공간의 자료형
	{
		format_32bit_3_float = DXGI_FORMAT_R32G32B32_FLOAT,
		foramt_16bit_4_unorm = DXGI_FORMAT_R16G16B16A16_UNORM, // 0~ 1
		foramt_32bit_2_uint = DXGI_FORMAT_R32G32_UINT,
		foramt_8bit_4_unorm = DXGI_FORMAT_R8G8B8A8_UNORM,
		foramt_8bit_4_snorm = DXGI_FORMAT_R8G8B8A8_SNORM,  // -1 ~1
		foramt_8bit_4_sint = DXGI_FORMAT_R8G8B8A8_SINT, // -128 ~ 127
		foramt_8bit_4_uint = DXGI_FORMAT_R8G8B8A8_UINT, // 0~ 255
	};

private:
	struct SRVInfo
	{
		eTextureType type = diffuse;
		ID3D11ShaderResourceView * SRV = nullptr;

	};
	vector<ID3D11ShaderResourceView*> m_vecSRV;

private:
	static map<string, SRVInfo> g_mapMaterial;
public:

	void GetTextrueResource(
		wchar_t* path,
		wchar_t* ext,
		string textureName,
		eTextureType textureType,
		ID3D11Device * Dev);
	
	void SetShaderResources();

	static void DeleteDefaultSRV();
	static void CreateDefaultSRV();
public:
	cMaterial();
	~cMaterial();
};
