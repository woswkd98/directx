
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



// �ؽ��� ���ø�
Texture2D DiffuseTexture	: register(t0); // �ؽ��� 
Texture2D NormalTexture		: register(t1); // �ؽ���
Texture2D SpecularTexture	: register(t2); // �ؽ���
Texture2D EmissionTexture	: register(t3); // �ؽ��� 

SamplerState LinearSampler	: register(s0); // ���÷��� ����

// �ø�ƽ�� �Ķ������ ��� �ǵ��� �����Ѵ� 
//https://translate.google.co.kr/translate?hl=ko&sl=en&u=https://msdn.microsoft.com/en-us/library/windows/desktop/bb509647(v%3Dvs.85).aspx&prev=search ���۷��� 

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

	// ���⿣ ��¥ World * view * projection�� �������� 
	Output.Position = mul(float4(input.Position,1.0f), WVP);


	Output.WorldPos = mul(input.Position, World);

	Output.UV = input.UV;

	// ����� �������·� �ٲ۴� (��ġ������ ����) ��Ȯ���� ���� ��ȯ�� �ƴ϶� ������ ��ȯ�̹Ƿ� 3x3���ε� ����ϴ�
	Output.Normal = mul(input.Normal, (float3x3)World);
	
	// tangent
	Output.Tangent = mul(input.Tangent, (float3x3)World);

	// binormal
	Output.Binormal = mul(input.Binormal, (float3x3)World);

	return Output;
}

struct Material
{
	float3 normal; // ��ָ�
	float4 diffuseColor; // ��ǻ���
	float specExp;		// emission
	float specIntensity; // ����ŧ�� ��
};

Material PrepareMaterial(VS_OUTPUT OutIN, float2 UV)
{
	
	Material material;

	/*
	��ü���� ���������ιٲٴ����
	3x3		Tx		Bx		Nx
			Ty		By		Ny
			Tz		Bz		Nz
	*/
	float3x3 TBN = float3x3(OutIN.Tangent, OutIN.Binormal, OutIN.Normal); // ���� TBN																		  
	material.diffuseColor = DiffuseTexture.Sample(LinearSampler, UV);// ���÷��� ��ǻ�� �ؽ��� ����

	material.diffuseColor.rgb = GammaToLinear(material.diffuseColor.rgb); // ���� ���� ����
	material.diffuseColor.a = 1.0f;
	// ��ó http://www.rastertek.com/dx10tut21.html
	// d3dx11�� �̿��� 3d���� ���α׷��� �Թ� 18.6 �κ�
	
	float4 bumpMap = NormalTexture.Sample(LinearSampler, UV);
	bumpMap = (bumpMap * 2.0f) - 1.0f; // 0~ 1 , -1 ~1 ����
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