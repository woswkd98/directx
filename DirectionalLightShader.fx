
#include "RenderSceneShader.fx"

cbuffer cbDirLightPS  : register(b1) // ������Ʈ ���� 1����
{
	float4 AmbientDown		: packoffset(c0);
	float4 AmbientUpper		: packoffset(c1);
	float4 LightDirection	: packoffset(c2);
	float4 DirLightColor	: packoffset(c3);
};		


/*
	�ݱ��� �����Ʈ ����Ʈ 
	�����Ʈ ����Ʈ�� ��ü�� �����ϰ� ���ߴµ� 

	�ݱ��� �����Ʈ ����Ʈ�� 

	���Ʒ��� ���� ����� �������ϰ� 

	����Ҷ��� ��ü�� ����� �������� ȥ���Ѵ�
*/


/*
struct Material
{
float3 normal; // ������ ���
float4 diffuseColor; // ���� ����
float specExp; // ����ŧ�� ���� (�� ���� �������� �� ���� ������)
float specIntensity; // ����ŧ�� ����
};

*/
float3 CalcAmbient(float3 normal, float3 color)
{
	float3 LinearAmbientDowm = GammaToLinear(float3(AmbientDown.xyz)); // ������������ �ѱ�

	float3 LinearAmbientRange = GammaToLinear(float3(AmbientUpper.xyz)) - LinearAmbientDowm; // ���� ����

	float up = normal.y * 0.5 + 0.5; // -> 0 ~ 1 �θ���� (-1 ~ 1)n 

	float3 ambient = LinearAmbientDowm + up * LinearAmbientRange; // ��

	return ambient * color; 
}

float3 CalcDirectional(float3 position, Material material) 
{
	float3 DirToLight = -LightDirection.xyz;

	float3 LinearDirLightColor = GammaToLinear(DirLightColor.rgb); // ���Ɽ ���� ������������ �ѱ�

	float NDotL = dot(DirToLight, material.normal);
	float3 finalColor = LinearDirLightColor.rgb * saturate(NDotL);

	/*
		1 ��ü�� �߽ɿ��� ���� �ٿ����� ���¹���,
		2 ���� ��ġ�� ���¹��� 
		�� ���ؼ� ��ֶ����� ��Ű�� 1, 2���� �߰� ���⺤�Ͱ� ������ (3)
		�̰Ÿ� �� �븻 ���Ϳ� dotproduct ��Ű�� cos�� ������ �� ���� ���̸�ŭ ���⸦ �������ش� 
		(�κ��� �� ��ֶ����� ��ٴ� �����Ͽ�)
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

	 //������Ʈ ��Ű�� ���� �������� �ؽ��ĵ��� �÷��� ������Ʈ �����ش�

	/*SpecularTexture*/
	finalColor *= SpecularTexture.Sample(LinearSampler, In.UV).rgb;
	/*EmissionTexture*/
	finalColor *= EmissionTexture.Sample(LinearSampler, In.UV).rgb;

	

	return float4(finalColor,1.0f);
}


/*
�ݱ��� �����Ʈ ����Ʈ

��(�ֺ���)�� �� �Ʒ� �ΰ������� �ɰ��� �ٸ������� �������� ��� �־���



float up = normal.y * 0.5 + 0.5; // �����Ʈ  �����⸦ ���ϰ�

float3 ambient = AmbientDown + up * AmbientRange; // ���� �����༭ �������� �����Ʈ ���ؼ� �Ʒ��� �����ش�

return ambient * color; // �÷��� ����


directional

float NDotL = dot(DirToLight, material.normal);
// �̺κп��� ����Ʈ�� ��� �Ѵ� ��������̶� �Ҷ� ���̴� 1�̹Ƿ� ���� ����� ���̰��� \
// ������ (�𷺼ų��� �����ġ���� ������ ���̴� �� ��ġ�� �������) �� ���̰��� �ݻ��������� ����͵� ���� saturate �ϸ� 0~1 ���̰��ε�
// ������ ������ 0�̹Ƿ� ���� ���� �׸��� light�� �ݻ����(DirToLight)�� ��ְ� �������� (1�� �������� ����)
float3 finalColor = DirLightColor.rgb * saturate(NDotL); // ����


float3 ToEye = EyePosition.xyz - position; // ����ġ���� ������ ������ ���� ����
ToEye = normalize(ToEye); // ���⼺�� ����
float3 HalfWay = normalize(ToEye + DirToLight); // �� ������ ������ ���� �ݻ簢�� �߰� ���� ���Ѵ� �̷��� �ٻ�ġ�� �ݻ纤�Ͱ� ���´�
float NDotH = saturate(dot(HalfWay, material.normal)); // �ݻ纤�Ͱ� �ݴ�����̸� ������ϰ� 1�ϼ��� ����
finalColor += DirLightColor.rgb * pow(NDotH, material.specExp) * material.specIntensity; // ����

http://hoidu.tistory.com/entry/HLSL-%EB%B8%94%EB%A6%B0-%ED%90%81-%EB%B0%98%EC%98%81-%EB%B0%98%EC%82%AC%EA%B4%91

return finalColor * material.diffuseColor.rgb;

*/