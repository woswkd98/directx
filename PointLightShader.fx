
#include "CommonShader.fx"


cbuffer cbPointLightDomain : register(b0)
{
	float4x4 LightProjection : packoffset(c0); //PointLight Matrix

	/*
		이행렬을 대신하는 행렬이 gameObject 의 transform finalMatrix 이다
		필요한 정보는
		조명 위치
		조명 범위
		카메라 뷰
		카메라 projection

		점광은 결국 퍼져나가는게 결국 구와 같으므로 구로 표시하고
		범위는 구의 크기 즉 행렬의 스케일링값이된다

		따라서 라이트 메트릭스는 
		(s * r * t) 에서
		점광은 방향이라는 개념이 없으므로 r 을 뺴고 카메라의 viewPos 에 R 생략후 (항상 카메라를 바라보고 있다)
		projection 까지 시켜준다
		Ls * Lt * Ct * Cprojection
	*/
}
// hull shader
/*
//http://vsts2010.tistory.com/520 자료 출처
	hull shader는 테셀레이션 작업의 시작이다
	directx9에서의 VS의 주요목적은 WVP 변환이였지만
	directx11에서의 VS의 목적은 특히나 테셀레이션 작업이 필요한경우
	HS로의 데이터 전달이다
	따라서 테셀레이션 작업시 VS 는 World까지만 곱해준다
	HullShader에선 폴리곤을 분할하는 방법과 개수를 결정한다
	이 작업은 병렬적으로 실행되며 빠른속도로 진행된다


	위의 그림은 베지어(Bezier) 제어점들을 이용해서 베지어 곡면을 표현한 것입니다.

	근본적으로 테셀레이션은 평면을 곡면으로 생성시키는 개념과 매우 비슷합니다.
	( 굳이 평면을 많은 갯수의 폴리곤으로 표현할 필요는 없기 때문이겠죠. )
	그렇기 때문에, 분할 방법으로 사용되는 알고리즘들은 베지어처럼 게임 프로그래머들에게 친숙한
	개념들이 사용됩니다.

	Hull Shader 의 또 하나의 중요한 역활은 불필요한 연산을 줄이기 위해
	테셀레이션 단계를 스킵할지를 결정할 수 있다는 것입니다.
	즉, Hull Shader 에서 Tessellation Factor 가 0 이하인 경우에
	이 패치는 컬링되어 버린 것으로 간주됩니다.
	( Tessellation Factor 는 얼마나 분할할지를 나타내는 수치적 비율입니다. )
	이로인해 더 이상 파이프라인 처리가 이루어지지 않음으로써,
	성능 향상을 도모할 수 있습니다.

	그러면 과연 Hull Shader 에서의 '폴리곤을 어떻게 분할할 것인가?' 와 '폴리곤을 얼마나 분할할 것인가?'
	프로그램 코드에서는 어떻게 표현해야 할까요?

	현재 MSDN 에 나와있는 Hull Shader 의 가장 단순한 형태는 다음과 같습니다.
	( 물론 실제로 구현되고 동작되는 내용들의 예들은 DirectX11 샘플에 있습니다. )
*/

// 테셀레이션
// http://vsts2010.tistory.com/524
/*
	HS의 내용을 받아서 작업하는 스테이지 
	테셀레이터의 정해진 연산에의해 도메인 쉐이더에 무게 중심좌표등을 주게 됩니다

	무게 중심좌표에 대한 설명은 출처에도 나와있고
	또한 skinned animation할때도 필요한 개념이다 (정점사이의 무게중심좌표를 줘서 얼마만큼 정점이 움직일지 정해준다)

	정점 뻥튀기가(지오메트리 쉐이더에서 가능한 기능) 아니라 있는정점에 나누는 정점을 기준으로 세분화 

*/




cbuffer cbPointLightPixel : register(b1)
{
	float3 PointLightPos : packoffset(c0);
	float PointLightRangeRcp : packoffset(c0.w);
	float3 PointColor : packoffset(c1);
	float2 LightPerspectiveValues : packoffset(c2);
}
float4 PointLightVS() : SV_Position // 여기서 리턴되는 값은 쓸모가 없다 하지만 VS 에서 넘어갈때 최소 한개의 정점은 넘겨야 해서 넘기는것이다
{
	return float4(0.0, 0.0, 0.0, 1.0);
}


struct HS_CONSTANT_DATA_OUTPUT
{
	float Edges[4] : SV_TessFactor;
	float Inside[2] : SV_InsideTessFactor;
};

HS_CONSTANT_DATA_OUTPUT PointLightConstantHS()
{
	HS_CONSTANT_DATA_OUTPUT Output;

	/*
		Edge = left, Top, right, bottom의 분할 개수
		inside = uv 값 또는 수평 수직 분할 개수
	
	*/
	float tessFactor = 18.0; // 분할 단위
	Output.Edges[0] = Output.Edges[1] = Output.Edges[2] = Output.Edges[3] = tessFactor; //  같은 크기로 분할
	Output.Inside[0] = Output.Inside[1] = tessFactor; 

	return Output;
}

struct HS_OUTPUT
{
	float3 HemiDir : POSITION;
};

static const float3 HemilDir[2] = {
	float3(1.0, 1.0,1.0), 
	float3(-1.0, 1.0, -1.0)
};

[domain("quad")] // 쿼드 형태
[partitioning("integer")] // 테셀레이션의 계수값의 소수점무시 소수점이하가 짤려나가므로 파핑현상이 더 잘일어난다
[outputtopology("triangle_ccw")] // 삼각형에 반시계
[outputcontrolpoints(4)] // 제어점의 개수 4
[patchconstantfunc("PointLightConstantHS")] // HS로 쓰일이름
//[maxtessfactor(N)] 테셀레이션 계수의 최대값을 미리 보내줌 (추가적인 최적화 여지가 있다)
[maxtessfactor(18.0f)] // 여기선 18.0밖에 안쓰므로 일부러 넣었다

HS_OUTPUT PointLightHS(uint PatchID : SV_PrimitiveID)
{
	HS_OUTPUT Output;

	Output.HemiDir = HemilDir[PatchID]; // 분할정보 받아서 리턴

	return Output;
}


struct DS_OUTPUT
{
	float4 Position : SV_POSITION;
	float2 cpPos	: TEXCOORD0;
};

[domain("quad")]
DS_OUTPUT PointLightDS(HS_CONSTANT_DATA_OUTPUT input, float2 UV : SV_DomainLocation, const OutputPatch<HS_OUTPUT, 4> quad) // 여기서 분할 이후 변환 작업을한다
{
	
	float2 posClipSpace = UV.xy * 2.0 - 1.0; // 0~ 1값으로 축소

	
	float2 posClipSpaceAbs = abs(posClipSpace.xy); // 0~1 만든 값의 절대값 
	float maxLen = max(posClipSpaceAbs.x, posClipSpaceAbs.y); // 여기서 가장 거리가 먼 값을 구함

	float3 normDir = normalize(float3(posClipSpace.xy, (maxLen - 1.0)) * quad[0].HemiDir);
	float4 posLS = float4(normDir.xyz, 1.0);


	DS_OUTPUT Output;
	Output.Position = mul(posLS, LightProjection);


	Output.cpPos = Output.Position.xy / Output.Position.w;

	return Output;
}

float3 CalcPoint(float3 position, Material material, bool bUseShadow)
{
	float3 ToLight = PointLightPos - position;
	float3 ToEye = EyePosition - position;
	float DistToLight = length(ToLight);

	
	ToLight /= DistToLight;
	float NDotL = saturate(dot(ToLight, material.normal));
	float3 finalColor = material.diffuseColor.rgb * NDotL;


	ToEye = normalize(ToEye);
	float3 HalfWay = normalize(ToEye + ToLight);
	float NDotH = saturate(dot(HalfWay, material.normal));
	finalColor += pow(NDotH, material.specPow) * material.specIntensity;


	float DistToLightNorm = 1.0 - saturate(DistToLight * PointLightRangeRcp);
	float Attn = DistToLightNorm * DistToLightNorm;

	float3 LinearPointLIght = GammaToLinear(PointColor.rgb);
	finalColor *= LinearPointLIght.rgb * Attn;

	return finalColor;
}

float4 PointLightCommonPS(DS_OUTPUT In, bool bUseShadow) : SV_TARGET
{
	SURFACE_DATA gbd = UnpackGBuffer_Loc(In.Position.xy);

	Material mat;
	MaterialFromGBuffer(gbd, mat);

	float3 position = CalcWorldPos(In.cpPos, gbd.LinearDepth);

	float3 finalColor = CalcPoint(position, mat, bUseShadow);

	return float4(finalColor, 1.0);
}

float4 PointLightPS(DS_OUTPUT In) : SV_TARGET
{
	return PointLightCommonPS(In, false);
}

float4 PointLightShadowPS(DS_OUTPUT In) : SV_TARGET
{
	return PointLightCommonPS(In, true);
}