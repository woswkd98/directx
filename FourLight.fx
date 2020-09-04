
#include "RenderSceneShader.fx"


/*
GPU 상에서 이루어지는 연산 대부분은 벡터화된 형태로 이루어진다.
한가지 연산이 최대 4개 값에 대해 이루어진다.
예를 들어 한가지 색 ( r 1   , g 1   , b 1   )   {\displaystyle (r_{1},g_{1},b_{1})}  (r_{1},g_{1},b_{1})가 다른 색상 ( r 2   , g 2   , b 2   )   
{\displaystyle (r_{2},g_{2},b_{2})}  (r_{2},g_{2},b_{2}) 로 모듈레이트된다면 GPU는 결과 색상인 ( r 1   r 2   , g 1   g 2   , b 1   b 2   )  
{\displaystyle (r_{1}r_{2},g_{1}g_{2},b_{1}b_{2})}  (r_{1}r_{2},g_{1}g_{2},b_{1}b_{2})를 한 연산으로 계산할 수 있다. 
이 기능은 그래픽스에서 유용한데 그 까닭은 거의 모든 기본 자료형이 (2, 3, 4차원) 벡터이기 때문이다. (예: 꼭짓점, 색상, 수직 벡터, 표면 패턴 좌표 등) 많은 다른 응용 프로그램에서 이를 유용하게 사용될 수 있고,
벡터 명령 (SIMD)이 그 우수한 성능으로 CPU상에서 지원되었다.


이예제에서도 x는 x 대로 묶고 y는 y대로 묶는 방식을 썼는데 이유는


  aX * b.xxxx + aY * b.yyyy + aZ * b.zzzz; // 이런식의 연산을 바로 할수 있기 때문이다
  또한 이러한 계산이 가능하기때문에 여러개의 광원을 묶는것이 효율적이다 

*/

cbuffer FourLightsConstants : register(b1)
{
	float4 LightPosX			: packoffset(c0); 
	float4 LightPosY			: packoffset(c1);
	float4 LightPosZ			: packoffset(c2);

	float4 LightDirX			: packoffset(c3);
	float4 LightDirY			: packoffset(c4);
	float4 LightDirZ			: packoffset(c5);

	float4 LightRangeRcp		: packoffset(c6);

	float4 SpotCosOuterCone		: packoffset(c7);
	
	float4 SpotCosInnerConeRcp	: packoffset(c8);
	
	float4 CapsuleLen			: packoffset(c9);
	
	float4 LightColorR			: packoffset(c10); // 빛4개의 반사광색깔도 다 묶었다
	float4 LightColorG			: packoffset(c11);
	float4 LightColorB			: packoffset(c12);
}
/*
struct VS_INPUT
{
	float4 Position	: POSITION;		
	float3 Normal	: NORMAL;		
	float2 UV		: TEXCOORD0;	
	float3 Tangent	: TANGENT;
	float3 Binormal : BINORMAL;
};
*/


float4 dot4x4(float4 aX, float4 aY, float4 aZ, float4 bX, float4 bY, float4 bZ)
{
	return aX * bX + aY * bY + aZ * bZ;
}


float4 dot4x1(float4 aX, float4 aY, float4 aZ, float3 b)
{
	return aX * b.xxxx + aY * b.yyyy + aZ * b.zzzz;
}


float3 CalcFourLights(float3 position, Material material)
{
	float3 ToEye = EyePosition.xyz - position;
	
	float4 ToCapsuleStartX = position.xxxx - LightPosX; // 4개의 ToEye의 X 좌표
	float4 ToCapsuleStartY = position.yyyy - LightPosY; // 4개의 ToEye의 Y 좌표
	float4 ToCapsuleStartZ = position.zzzz - LightPosZ; // 4개의 ToEye의 Z 좌표
	float4 DistOnLine = dot4x4(ToCapsuleStartX, ToCapsuleStartY, ToCapsuleStartZ, LightDirX, LightDirY, LightDirZ); // 캡슐 광원 라인사이의 거리검사
	float4 CapsuleLenSafe = max(CapsuleLen, 1.e-6); // 1.e-6 실수는 오차가 생길수 있는데 이에대한 오차범위 값 1 의 -6승값이 오차값이다 
	DistOnLine = CapsuleLen * saturate(DistOnLine / CapsuleLenSafe);
	float4 PointOnLineX = LightPosX + LightDirX * DistOnLine; //실제로 점을 만든다
	float4 PointOnLineY = LightPosY + LightDirY * DistOnLine;
	float4 PointOnLineZ = LightPosZ + LightDirZ * DistOnLine;
	float4 ToLightX = PointOnLineX - position.xxxx; // 라이트로 가는 방향벡터 (노멀라이즈 안된상태)
	float4 ToLightY = PointOnLineY - position.yyyy;
	float4 ToLightZ = PointOnLineZ - position.zzzz;
	float4 DistToLightSqr = dot4x4(ToLightX, ToLightY, ToLightZ, ToLightX, ToLightY, ToLightZ);// lenght ^ 2
	float4 DistToLight = sqrt(DistToLightSqr);// lenght 

	ToLightX /= DistToLight; // ToLight = dir * Length 고 여기에 다시 길이 나누면 노멀라이즈
	ToLightY /= DistToLight;
	ToLightZ /= DistToLight;

	// Phong 계산을 위한 
	float4 NDotL = saturate(dot4x1(ToLightX, ToLightY, ToLightZ, material.normal));

	// 블린 스펙쿨러
	ToEye = normalize(ToEye);
	float4 HalfWayX = ToEye.xxxx + ToLightX;
	float4 HalfWayY = ToEye.yyyy + ToLightY;
	float4 HalfWayZ = ToEye.zzzz + ToLightZ;
	float4 HalfWaySize = sqrt(dot4x4(HalfWayX, HalfWayY, HalfWayZ, HalfWayX, HalfWayY, HalfWayZ));
	float4 NDotH = saturate(dot4x1(HalfWayX / HalfWaySize, HalfWayY / HalfWaySize, HalfWayZ / HalfWaySize, material.normal));
	float4 SpecValue = pow(NDotH, material.specExp) * material.specIntensity; // 스펙큘러 값

	// 스팟라이트
	// 원뿔각에 따른 빛의 세기
	float4 cosAng = dot4x4(LightDirX, LightDirY, LightDirZ, ToLightX, ToLightY, ToLightZ);
	float4 conAtt = saturate((cosAng - SpotCosOuterCone) * SpotCosInnerConeRcp);
	conAtt *= conAtt;

	// 빛의 세기
	float4 DistToLightNorm = 1.0 - saturate(DistToLight * LightRangeRcp);
	float4 Attn = DistToLightNorm * DistToLightNorm;
	Attn *= conAtt; 

	float4 pixelIntensity = (NDotL + SpecValue) * Attn; // (NDotL + SpecValue) * Attn;  attn 은 스팟라이트
	float3 finalColor = float3(dot(LightColorR, pixelIntensity), dot(LightColorG, pixelIntensity), dot(LightColorB, pixelIntensity));
	finalColor *= material.diffuseColor.rgb;



	return finalColor;
}

float4 FourLightPS(VS_OUTPUT In) : SV_TARGET0
{

	Material material = PrepareMaterial(In, In.UV);

	float3 finalColor = CalcFourLights(In.WorldPos, material);

	/*SpecularTexture*/
	finalColor *= SpecularTexture.Sample(LinearSampler, In.UV).rgb;
	/*EmissionTexture*/
	finalColor *= EmissionTexture.Sample(LinearSampler, In.UV).rgb;

	// Return the final color
	return float4(finalColor, 1.0);
}