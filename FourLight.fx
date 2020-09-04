
#include "RenderSceneShader.fx"


/*
GPU �󿡼� �̷������ ���� ��κ��� ����ȭ�� ���·� �̷������.
�Ѱ��� ������ �ִ� 4�� ���� ���� �̷������.
���� ��� �Ѱ��� �� ( r 1   , g 1   , b 1   )   {\displaystyle (r_{1},g_{1},b_{1})}  (r_{1},g_{1},b_{1})�� �ٸ� ���� ( r 2   , g 2   , b 2   )   
{\displaystyle (r_{2},g_{2},b_{2})}  (r_{2},g_{2},b_{2}) �� ��ⷹ��Ʈ�ȴٸ� GPU�� ��� ������ ( r 1   r 2   , g 1   g 2   , b 1   b 2   )  
{\displaystyle (r_{1}r_{2},g_{1}g_{2},b_{1}b_{2})}  (r_{1}r_{2},g_{1}g_{2},b_{1}b_{2})�� �� �������� ����� �� �ִ�. 
�� ����� �׷��Ƚ����� �����ѵ� �� ����� ���� ��� �⺻ �ڷ����� (2, 3, 4����) �����̱� �����̴�. (��: ������, ����, ���� ����, ǥ�� ���� ��ǥ ��) ���� �ٸ� ���� ���α׷����� �̸� �����ϰ� ���� �� �ְ�,
���� ��� (SIMD)�� �� ����� �������� CPU�󿡼� �����Ǿ���.


�̿��������� x�� x ��� ���� y�� y��� ���� ����� ��µ� ������


  aX * b.xxxx + aY * b.yyyy + aZ * b.zzzz; // �̷����� ������ �ٷ� �Ҽ� �ֱ� �����̴�
  ���� �̷��� ����� �����ϱ⶧���� �������� ������ ���°��� ȿ�����̴� 

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
	
	float4 LightColorR			: packoffset(c10); // ��4���� �ݻ籤���� �� ������
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
	
	float4 ToCapsuleStartX = position.xxxx - LightPosX; // 4���� ToEye�� X ��ǥ
	float4 ToCapsuleStartY = position.yyyy - LightPosY; // 4���� ToEye�� Y ��ǥ
	float4 ToCapsuleStartZ = position.zzzz - LightPosZ; // 4���� ToEye�� Z ��ǥ
	float4 DistOnLine = dot4x4(ToCapsuleStartX, ToCapsuleStartY, ToCapsuleStartZ, LightDirX, LightDirY, LightDirZ); // ĸ�� ���� ���λ����� �Ÿ��˻�
	float4 CapsuleLenSafe = max(CapsuleLen, 1.e-6); // 1.e-6 �Ǽ��� ������ ����� �ִµ� �̿����� �������� �� 1 �� -6�°��� �������̴� 
	DistOnLine = CapsuleLen * saturate(DistOnLine / CapsuleLenSafe);
	float4 PointOnLineX = LightPosX + LightDirX * DistOnLine; //������ ���� �����
	float4 PointOnLineY = LightPosY + LightDirY * DistOnLine;
	float4 PointOnLineZ = LightPosZ + LightDirZ * DistOnLine;
	float4 ToLightX = PointOnLineX - position.xxxx; // ����Ʈ�� ���� ���⺤�� (��ֶ����� �ȵȻ���)
	float4 ToLightY = PointOnLineY - position.yyyy;
	float4 ToLightZ = PointOnLineZ - position.zzzz;
	float4 DistToLightSqr = dot4x4(ToLightX, ToLightY, ToLightZ, ToLightX, ToLightY, ToLightZ);// lenght ^ 2
	float4 DistToLight = sqrt(DistToLightSqr);// lenght 

	ToLightX /= DistToLight; // ToLight = dir * Length �� ���⿡ �ٽ� ���� ������ ��ֶ�����
	ToLightY /= DistToLight;
	ToLightZ /= DistToLight;

	// Phong ����� ���� 
	float4 NDotL = saturate(dot4x1(ToLightX, ToLightY, ToLightZ, material.normal));

	// �� ������
	ToEye = normalize(ToEye);
	float4 HalfWayX = ToEye.xxxx + ToLightX;
	float4 HalfWayY = ToEye.yyyy + ToLightY;
	float4 HalfWayZ = ToEye.zzzz + ToLightZ;
	float4 HalfWaySize = sqrt(dot4x4(HalfWayX, HalfWayY, HalfWayZ, HalfWayX, HalfWayY, HalfWayZ));
	float4 NDotH = saturate(dot4x1(HalfWayX / HalfWaySize, HalfWayY / HalfWaySize, HalfWayZ / HalfWaySize, material.normal));
	float4 SpecValue = pow(NDotH, material.specExp) * material.specIntensity; // ����ŧ�� ��

	// ���̶���Ʈ
	// ���԰��� ���� ���� ����
	float4 cosAng = dot4x4(LightDirX, LightDirY, LightDirZ, ToLightX, ToLightY, ToLightZ);
	float4 conAtt = saturate((cosAng - SpotCosOuterCone) * SpotCosInnerConeRcp);
	conAtt *= conAtt;

	// ���� ����
	float4 DistToLightNorm = 1.0 - saturate(DistToLight * LightRangeRcp);
	float4 Attn = DistToLightNorm * DistToLightNorm;
	Attn *= conAtt; 

	float4 pixelIntensity = (NDotL + SpecValue) * Attn; // (NDotL + SpecValue) * Attn;  attn �� ���̶���Ʈ
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