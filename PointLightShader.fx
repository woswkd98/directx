
#include "CommonShader.fx"


cbuffer cbPointLightDomain : register(b0)
{
	float4x4 LightProjection : packoffset(c0); //PointLight Matrix

	/*
		������� ����ϴ� ����� gameObject �� transform finalMatrix �̴�
		�ʿ��� ������
		���� ��ġ
		���� ����
		ī�޶� ��
		ī�޶� projection

		������ �ᱹ ���������°� �ᱹ ���� �����Ƿ� ���� ǥ���ϰ�
		������ ���� ũ�� �� ����� �����ϸ����̵ȴ�

		���� ����Ʈ ��Ʈ������ 
		(s * r * t) ����
		������ �����̶�� ������ �����Ƿ� r �� ���� ī�޶��� viewPos �� R ������ (�׻� ī�޶� �ٶ󺸰� �ִ�)
		projection ���� �����ش�
		Ls * Lt * Ct * Cprojection
	*/
}
// hull shader
/*
//http://vsts2010.tistory.com/520 �ڷ� ��ó
	hull shader�� �׼����̼� �۾��� �����̴�
	directx9������ VS�� �ֿ������ WVP ��ȯ�̿�����
	directx11������ VS�� ������ Ư���� �׼����̼� �۾��� �ʿ��Ѱ��
	HS���� ������ �����̴�
	���� �׼����̼� �۾��� VS �� World������ �����ش�
	HullShader���� �������� �����ϴ� ����� ������ �����Ѵ�
	�� �۾��� ���������� ����Ǹ� �����ӵ��� ����ȴ�


	���� �׸��� ������(Bezier) ���������� �̿��ؼ� ������ ����� ǥ���� ���Դϴ�.

	�ٺ������� �׼����̼��� ����� ������� ������Ű�� ����� �ſ� ����մϴ�.
	( ���� ����� ���� ������ ���������� ǥ���� �ʿ�� ���� �����̰���. )
	�׷��� ������, ���� ������� ���Ǵ� �˰������ ������ó�� ���� ���α׷��ӵ鿡�� ģ����
	������� ���˴ϴ�.

	Hull Shader �� �� �ϳ��� �߿��� ��Ȱ�� ���ʿ��� ������ ���̱� ����
	�׼����̼� �ܰ踦 ��ŵ������ ������ �� �ִٴ� ���Դϴ�.
	��, Hull Shader ���� Tessellation Factor �� 0 ������ ��쿡
	�� ��ġ�� �ø��Ǿ� ���� ������ ���ֵ˴ϴ�.
	( Tessellation Factor �� �󸶳� ���������� ��Ÿ���� ��ġ�� �����Դϴ�. )
	�̷����� �� �̻� ���������� ó���� �̷������ �������ν�,
	���� ����� ������ �� �ֽ��ϴ�.

	�׷��� ���� Hull Shader ������ '�������� ��� ������ ���ΰ�?' �� '�������� �󸶳� ������ ���ΰ�?'
	���α׷� �ڵ忡���� ��� ǥ���ؾ� �ұ��?

	���� MSDN �� �����ִ� Hull Shader �� ���� �ܼ��� ���´� ������ �����ϴ�.
	( ���� ������ �����ǰ� ���۵Ǵ� ������� ������ DirectX11 ���ÿ� �ֽ��ϴ�. )
*/

// �׼����̼�
// http://vsts2010.tistory.com/524
/*
	HS�� ������ �޾Ƽ� �۾��ϴ� �������� 
	�׼��������� ������ ���꿡���� ������ ���̴��� ���� �߽���ǥ���� �ְ� �˴ϴ�

	���� �߽���ǥ�� ���� ������ ��ó���� �����ְ�
	���� skinned animation�Ҷ��� �ʿ��� �����̴� (���������� �����߽���ǥ�� �༭ �󸶸�ŭ ������ �������� �����ش�)

	���� ��Ƣ�Ⱑ(������Ʈ�� ���̴����� ������ ���) �ƴ϶� �ִ������� ������ ������ �������� ����ȭ 

*/




cbuffer cbPointLightPixel : register(b1)
{
	float3 PointLightPos : packoffset(c0);
	float PointLightRangeRcp : packoffset(c0.w);
	float3 PointColor : packoffset(c1);
	float2 LightPerspectiveValues : packoffset(c2);
}
float4 PointLightVS() : SV_Position // ���⼭ ���ϵǴ� ���� ���� ���� ������ VS ���� �Ѿ�� �ּ� �Ѱ��� ������ �Ѱܾ� �ؼ� �ѱ�°��̴�
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
		Edge = left, Top, right, bottom�� ���� ����
		inside = uv �� �Ǵ� ���� ���� ���� ����
	
	*/
	float tessFactor = 18.0; // ���� ����
	Output.Edges[0] = Output.Edges[1] = Output.Edges[2] = Output.Edges[3] = tessFactor; //  ���� ũ��� ����
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

[domain("quad")] // ���� ����
[partitioning("integer")] // �׼����̼��� ������� �Ҽ������� �Ҽ������ϰ� ©�������Ƿ� ���������� �� ���Ͼ��
[outputtopology("triangle_ccw")] // �ﰢ���� �ݽð�
[outputcontrolpoints(4)] // �������� ���� 4
[patchconstantfunc("PointLightConstantHS")] // HS�� �����̸�
//[maxtessfactor(N)] �׼����̼� ����� �ִ밪�� �̸� ������ (�߰����� ����ȭ ������ �ִ�)
[maxtessfactor(18.0f)] // ���⼱ 18.0�ۿ� �Ⱦ��Ƿ� �Ϻη� �־���

HS_OUTPUT PointLightHS(uint PatchID : SV_PrimitiveID)
{
	HS_OUTPUT Output;

	Output.HemiDir = HemilDir[PatchID]; // �������� �޾Ƽ� ����

	return Output;
}


struct DS_OUTPUT
{
	float4 Position : SV_POSITION;
	float2 cpPos	: TEXCOORD0;
};

[domain("quad")]
DS_OUTPUT PointLightDS(HS_CONSTANT_DATA_OUTPUT input, float2 UV : SV_DomainLocation, const OutputPatch<HS_OUTPUT, 4> quad) // ���⼭ ���� ���� ��ȯ �۾����Ѵ�
{
	
	float2 posClipSpace = UV.xy * 2.0 - 1.0; // 0~ 1������ ���

	
	float2 posClipSpaceAbs = abs(posClipSpace.xy); // 0~1 ���� ���� ���밪 
	float maxLen = max(posClipSpaceAbs.x, posClipSpaceAbs.y); // ���⼭ ���� �Ÿ��� �� ���� ����

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