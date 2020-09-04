
#include "RenderSceneShader.fx"

cbuffer cbDirLightPS  : register(b1) // 컨스턴트 슬롯 1번쨰
{
	float4 AmbientDown		: packoffset(c0);
	float4 AmbientUpper		: packoffset(c1);
	float4 LightDirection	: packoffset(c2);
	float4 DirLightColor	: packoffset(c3);
};		


/*
	반구형 엠비언트 라이트 
	엠비언트 라이트는 물체를 균일하게 비추는데 

	반구형 엠비언트 라이트는 

	위아래의 색을 나누어서 렌더링하고 

	계산할때는 물체의 노멀을 기준으로 혼합한다
*/


/*
struct Material
{
float3 normal; // 재질의 노멀
float4 diffuseColor; // 재질 색상
float specExp; // 스펙큘러 지수 (이 값이 높을수록 더 적게 퍼진다)
float specIntensity; // 스펙큘러 세기
};

*/
float3 CalcAmbient(float3 normal, float3 color)
{
	float3 LinearAmbientDowm = GammaToLinear(float3(AmbientDown.xyz)); // 선형공간으로 넘김

	float3 LinearAmbientRange = GammaToLinear(float3(AmbientUpper.xyz)) - LinearAmbientDowm; // 길이 구함

	float up = normal.y * 0.5 + 0.5; // -> 0 ~ 1 로만든다 (-1 ~ 1)n 

	float3 ambient = LinearAmbientDowm + up * LinearAmbientRange; // 업

	return ambient * color; 
}

float3 CalcDirectional(float3 position, Material material) 
{
	float3 DirToLight = -LightDirection.xyz;

	float3 LinearDirLightColor = GammaToLinear(DirLightColor.rgb); // 방향광 색상도 선형공간으로 넘김

	float NDotL = dot(DirToLight, material.normal);
	float3 finalColor = LinearDirLightColor.rgb * saturate(NDotL);

	/*
		1 물체의 중심에서 빛의 근원으로 가는방향,
		2 눈의 위치로 가는방향 
		을 더해서 노멀라이즈 시키면 1, 2번의 중간 방향벡터가 나오고 (3)
		이거를 내 노말 벡터와 dotproduct 시키면 cosΘ 나오고 이 각도 차이만큼 세기를 조절해준다 
		(두벡터 다 노멀라이즈 됬다는 가정하에)
	*/

	float3 ToEye = EyePosition.xyz - position; 
	ToEye = normalize(ToEye);
	float3 HalfWay = normalize(ToEye + DirToLight);
	float NDotH = saturate(dot(HalfWay, material.normal));
	finalColor += LinearDirLightColor.rgb * pow(NDotH, material.specExp) * material.specIntensity;

	return finalColor * material.diffuseColor.rgb;
}

float4 DirectionalLightPS(VS_OUTPUT In) : SV_TARGET0
{
	//return float4(1,0,0,1);
	
	Material material = PrepareMaterial(In, In.UV); 

	float3 finalColor = CalcAmbient(material.normal, material.diffuseColor.rgb); 

	
	finalColor += CalcDirectional(In.WorldPos, material);

	 //업데이트 시키기 전에 맵핑정보 텍스쳐들을 컬러에 업데이트 시켜준다

	/*SpecularTexture*/
	finalColor *= SpecularTexture.Sample(LinearSampler, In.UV).rgb;
	/*EmissionTexture*/
	finalColor *= EmissionTexture.Sample(LinearSampler, In.UV).rgb;

	

	return float4(finalColor,1.0f);
}


/*
반구형 엠비언트 라이트

빛(주변광)을 위 아래 두공간으로 쪼개서 다른색으로 구별한후 섞어서 넣어줌



float up = normal.y * 0.5 + 0.5; // 엠비언트  윗방향를 구하고

float3 ambient = AmbientDown + up * AmbientRange; // 길이 곱해줘서 위방향의 엠비언트 구해서 아래랑 섞어준다

return ambient * color; // 컬러에 적용


directional

float NDotL = dot(DirToLight, material.normal);
// 이부분에서 라이트와 노멀 둘다 단위행렬이라 할때 길이는 1이므로 빛과 노멀의 사이각이 \
// 나오고 (디렉셔널은 모든위치에서 동일한 빛이다 즉 위치에 관계없다) 이 사이각은 반사했을때의 세기와도 같다 saturate 하면 0~1 사이값인데
// 각도가 음수면 0이므로 빛이 없다 그리고 light의 반사방향(DirToLight)이 노멀과 같을수록 (1에 가까울수록 세다)
float3 finalColor = DirLightColor.rgb * saturate(NDotL); // 적용


float3 ToEye = EyePosition.xyz - position; // 내위치에서 눈으로 들어오는 빛의 방향
ToEye = normalize(ToEye); // 방향성만 남김
float3 HalfWay = normalize(ToEye + DirToLight); // 내 눈으로 들어오는 빛과 반사각의 중간 값을 구한다 이러면 근사치로 반사벡터가 나온다
float NDotH = saturate(dot(HalfWay, material.normal)); // 반사벡터가 반대방향이면 적용안하고 1일수록 세다
finalColor += DirLightColor.rgb * pow(NDotH, material.specExp) * material.specIntensity; // 적용

http://hoidu.tistory.com/entry/HLSL-%EB%B8%94%EB%A6%B0-%ED%90%81-%EB%B0%98%EC%98%81-%EB%B0%98%EC%82%AC%EA%B4%91

return finalColor * material.diffuseColor.rgb;

*/