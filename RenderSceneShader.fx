
cbuffer cbPerObjectPS : register(b0) // Model constants
{
	float3 EyePosition			: packoffset(c0);
	float specExp				: packoffset(c0.w);
	float specIntensity			: packoffset(c1);
}

cbuffer WVP : register(b0) 
{
	float4x4 WVP	: packoffset(c0);
	float4x4 World	: packoffset(c4);
};



// 텍스쳐 샘플링
Texture2D DiffuseTexture	: register(t0); // 텍스쳐 
Texture2D NormalTexture		: register(t1); // 텍스쳐
Texture2D SpecularTexture	: register(t2); // 텍스쳐
Texture2D EmissionTexture	: register(t3); // 텍스쳐 

SamplerState LinearSampler	: register(s0); // 샘플러는 같다

// 시멘틱은 파라미터의 사용 의도를 지정한다 
//https://translate.google.co.kr/translate?hl=ko&sl=en&u=https://msdn.microsoft.com/en-us/library/windows/desktop/bb509647(v%3Dvs.85).aspx&prev=search 레퍼런스 

struct VS_INPUT
{
	float3 Position	: POSITION;	
	float3 Normal	: NORMAL;
	float2 UV		: TEXCOORD0;
	float3 Tangent	: TANGENT;
	float3 Binormal : BINORMAL;
};

struct VS_OUTPUT
{
	float4 Position : SV_POSITION;
	float3 Normal	: NORMAL;
	float2 UV		: TEXCOORD0;
	float3 Tangent	: TANGENT;
	float3 Binormal : BINORMAL;
	float3 WorldPos : TEXCOORD1;
};

float3 GammaToLinear(float3 color)
{
	return float3(pow(color.r, 2.2), pow(color.g, 2.2), pow(color.b, 2.2));
}

VS_OUTPUT RenderSceneVS(VS_INPUT input) 
{
	VS_OUTPUT Output;

	// 여기엔 진짜 World * view * projection이 곱해진다 
	Output.Position = mul(float4(input.Position,1.0f), WVP);


	Output.WorldPos = mul(input.Position, World);

	Output.UV = input.UV;

	// 노멀을 월드형태로 바꾼다 (위치정보를 뺀다) 정확히는 점의 변환이 아니라 벡터의 변환이므로 3x3으로도 충분하다
	Output.Normal = mul(input.Normal, (float3x3)World);
	
	// tangent
	Output.Tangent = mul(input.Tangent, (float3x3)World);

	// binormal
	Output.Binormal = mul(input.Binormal, (float3x3)World);

	return Output;
}

struct Material
{
	float3 normal; // 노멀맵
	float4 diffuseColor; // 디퓨즈맵
	float specExp;		// emission
	float specIntensity; // 스펙큘러 맵
};

Material PrepareMaterial(VS_OUTPUT OutIN, float2 UV)
{
	
	Material material;

	/*
	물체에서 접공간으로바꾸는행렬
	3x3		Tx		Bx		Nx
			Ty		By		Ny
			Tz		Bz		Nz
	*/
	float3x3 TBN = float3x3(OutIN.Tangent, OutIN.Binormal, OutIN.Normal); // 모델의 TBN																		  
	material.diffuseColor = DiffuseTexture.Sample(LinearSampler, UV);// 샘플러된 디퓨즈 텍스쳐 가져

	material.diffuseColor.rgb = GammaToLinear(material.diffuseColor.rgb); // 선형 공간 변경
	material.diffuseColor.a = 1.0f;
	// 출처 http://www.rastertek.com/dx10tut21.html
	// d3dx11을 이용한 3d게임 프로그래밍 입문 18.6 부분
	
	float4 bumpMap = NormalTexture.Sample(LinearSampler, UV);
	bumpMap = (bumpMap * 2.0f) - 1.0f; // 0~ 1 , -1 ~1 사이
	material.normal = normalize(mul(bumpMap.xyz, TBN));
	material.normal = OutIN.Normal;
	material.specIntensity = specIntensity;

	material.specExp = specExp;

	return material;
}

float4 VS_prepass(float3 Pos : POSITION) : SV_POSITION
{
	float4 outPos;
	outPos = float4(Pos, 1.0f);
	outPos = mul(outPos, WVP);
	return outPos;
}