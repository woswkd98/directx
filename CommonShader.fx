


// 렌더 타겟 뷰 공간 할당 
/*
24bit 			8bit		32bit // 1
depth			stencil

24bit			8bit		32bit // 2
Color			specPower

32bit						32bit // 3
normal

8bit			empty		32bit	  // 4
specPowor
*/
Texture2D<float>  DepthTexture			: register(t0);  // 깊이부분을 가져오는 작업이므로 float 형태로 들고 온다
Texture2D<float4> ColorSpecIntTexture	: register(t1);  // r,g,b,specPower float4 형태
Texture2D<float3> NormalTexture			: register(t2); // x,y,z (r11,g11,b10) float3형태
Texture2D<float4> SpecPowTexture		: register(t3); // specPower, ?,?,? ? 임시 저장소 각각 8비트씩 첫번쨰 인자만 쓰면됨

// 샘플러 상태 는 세가지정도 있는데 
// point가 가장 빠르고 (상수 보간)
// linear 중간 (선형 보간)
// anisotropic이 가장 퀄리티가 좋으며 느리다
// 여기서 포인트 샘플러를 쓰는 이유는 미리 선형보간 시키지 않고
// perspective값을 적용한 깊이를 만들고 싶기 때문이다

SamplerState PointSampler               : register(s0); // 포인트 샘플러를 쓴다

cbuffer BufferUnpack : register(b0) // 언패킹시 필요한 값
{
	float4 PerspectiveValues : packoffset(c0); 
	float4x4 ViewInv		: packoffset(c1);
}

// 뷰의 역행렬은 
/*
	v = w ^ -1  뷰의 역행렬은 카메라의 월드 행렬이 된다 
	따리서 뷰의 역행렬의 
	41,42,43  부분은 눈의 위치 벡터가 된다
*/

struct HS_CONSTANT_DATA_OUTPUT_TRI
{
	float Edges[3] : SV_TessFactor;
	float Inside : SV_InsideTessFactor;
};



float3 GammaToLinear(float3 color)
{
	return float3(pow(color.r, 2.2f), pow(color.g, 2.2f), pow(color.b, 2.2f));
}


cbuffer cbFog : register(b2)
{	
	float3 FogColor				: packoffset(c0);
	float  FogStartDepth		: packoffset(c0.w);
	float3 FogHighlightColor	: packoffset(c1);
	float  FogGlobalDensity		: packoffset(c1.w);
	float3 FogSunDir			: packoffset(c2);
	float  FogStartHeight		: packoffset(c2.w);
}

#define EyePosition (ViewInv[3].xyz) 

static const float2 g_SpecPowerRange = { 10.0, 250.0 }; // 스펙큘러 파워 의 범위

float3 DecodeNormal(float2 encodedNormal)
{
	float4 decodedNormal = encodedNormal.xyyy * float4(2, 2, 0, 0) + float4(-1, -1, 1, -1);
	decodedNormal.z = dot(decodedNormal.xyz, -decodedNormal.xyw);
	decodedNormal.xy *= sqrt(decodedNormal.z);
	return decodedNormal.xyz * 2.0 + float3(0.0, 0.0, -1.0);
}
/*
// 저장 PerspectiveValues
x	0	0	0
0	y	0	0
0	0	-w	1
0	0	z   0

z = w * zf;
*/
float ConvertZToLinearDepth(float depth)
{
	float linearDepth = PerspectiveValues.z / (depth + PerspectiveValues.w); // 
	return linearDepth;
}

float3 CalcWorldPos(float2 csPos, float depth)
{
	float4 position;

	position.xy = csPos.xy * PerspectiveValues.xy * depth;
	position.z = depth;
	position.w = 1.0;

	return mul(position, ViewInv).xyz;
}

struct SURFACE_DATA
{
	float LinearDepth;
	float3 Color;
	float3 Normal;
	float SpecPow;
	float SpecIntensity;
};

SURFACE_DATA UnpackGBuffer(float2 UV)
{
	SURFACE_DATA Out;

	float depth = DepthTexture.Sample(PointSampler, UV.xy).x;
	Out.LinearDepth = ConvertZToLinearDepth(depth);
	float4 baseColorSpecInt = ColorSpecIntTexture.Sample(PointSampler, UV.xy);
	Out.Color = baseColorSpecInt.xyz;
	Out.SpecIntensity = baseColorSpecInt.w;
	Out.Normal = NormalTexture.Sample(PointSampler, UV.xy).xyz;
	Out.Normal = normalize(Out.Normal * 2.0 - 1.0);
	Out.SpecPow = SpecPowTexture.Sample(PointSampler, UV.xy).x;

	return Out;
}

SURFACE_DATA UnpackGBuffer_Loc(int2 location)
{
	SURFACE_DATA Out;
	int3 location3 = int3(location, 0);

	float depth = DepthTexture.Load(location3).x;
	Out.LinearDepth = ConvertZToLinearDepth(depth);
	float4 baseColorSpecInt = ColorSpecIntTexture.Load(location3);
	Out.Color = baseColorSpecInt.xyz;
	Out.SpecIntensity = baseColorSpecInt.w;
	Out.Normal = NormalTexture.Load(location3).xyz;
	Out.Normal = normalize(Out.Normal * 2.0 - 1.0);
	Out.SpecPow = SpecPowTexture.Load(location3).x;

	return Out;
}

struct Material
{
	float3 normal;
	float4 diffuseColor;
	float specPow;
	float specIntensity;
};

void MaterialFromGBuffer(SURFACE_DATA gbd, inout Material mat)
{
	mat.normal = gbd.Normal;
	mat.diffuseColor.xyz = gbd.Color;
	mat.diffuseColor.w = 1.0; // Fully opaque
	mat.specPow = g_SpecPowerRange.x + g_SpecPowerRange.y * gbd.SpecPow;
	mat.specIntensity = gbd.SpecIntensity;
}

float4 DebugLightPS() : SV_TARGET
{
	return float4(1.0, 1.0, 1.0, 1.0);
}
