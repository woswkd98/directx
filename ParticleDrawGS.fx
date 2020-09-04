
cbuffer CameraInfo : register(b0)
{
	float3 camWorldPos; // ī�޶��� ���� ��ġ 
	float4x4 VP; // ���� ���
};

cbuffer quadInfo : register(b1)
{
	float particleLifeTime;
	float3 size; // ���� ������
	//uint2 spriteXY;
};


struct GS_INPUT {
	float4 pos : SV_POSITION;
};


struct PS_INPUT {
	float4	pos		: SV_POSITION;
	float2  uv	: TEXCOORD0;
	float3	color	: COLOR;
};

[maxvertexcount(4)] // ����� ���� ������ 4���̹Ƿ� 
void ParticleDrawGS(point GS_INPUT p[1], inout TriangleStream<PS_INPUT> triStream) 
{
	PS_INPUT quadOut[4];
	
	// ī�޶󿡼� ���� ���� ���ʿ��� ī�޶�� ������ ������ ���´�
	float3 lookDir = normalize(camWorldPos - p[0].pos);
	
	// ���� �������� ���ο� right up ���� 

	// ������ y -> x -> z �̹Ƿ� �Ųٷ� ������ش�
	float3 right = normalize(cross(float3(0, 1, 0), lookDir));

	float3 up = normalize(cross(lookDir, right));

	// quad�� 4���� ������ش� 
	//	p0 �� ������ , p1�� ��������, p2���� �Ʒ� , p3 ������ �Ʒ� 
	// �ؽ��� ��ǥ�� 
	/*
		0,0	1,0
		0,1 1,1
	*/
	/*
	int spriteNum = spriteXY.y * spriteXY.x; // ��������Ʈ�� ��ü ���� ����

	float intervalU = 1 / spriteXY.x; // ���� ��������Ʈ�� ����
	float intervalV = 1 / spriteXY.y;

	int index = ceil(lerp(0, spriteNum - 1, p[0].currentTime / particleLifeTime)); // ���� �ش��ϴ� �ε����� ���� 
	float u = int(index % spriteXY.x) / spriteXY.x; // uv �������� ���� 
	float v = int(index / spriteXY.x) / spriteXY.y;
	*/
	quadOut[0].pos = float4(p[0].pos -right * size.x / 2 + up * size.y / 2, 1.0f);
	quadOut[0].pos = mul(quadOut[0].pos, VP); // �� �������� ������
	quadOut[0].uv = float2(0,0);
	triStream.Append(quadOut[0]); // �����ش� 
	quadOut[1].pos = float4(p[0].pos +right * size.x / 2 + up * size.y / 2, 1.0f);
	quadOut[1].pos = mul(quadOut[1].pos, VP);  // �� �������� ������
	quadOut[1].uv = float2(1, 0);
	triStream.Append(quadOut[1]); // �����ش�
	quadOut[2].pos = float4(p[0].pos -right * size.x / 2 - up * size.y / 2, 1.0f);
	quadOut[2].pos = mul(quadOut[2].pos, VP);  // �� �������� ������
	quadOut[2].uv = float2(0, 1);
	triStream.Append(quadOut[2]); // �����ش�
	quadOut[3].pos = float4(p[0].pos +right * size.x / 2 - up * size.y / 2, 1.0f);
	quadOut[3].pos = mul(quadOut[2].pos, VP); // �� �������� ������
	quadOut[3].uv = float2(1,1);
	triStream.Append(quadOut[3]); // �����ش�

}

