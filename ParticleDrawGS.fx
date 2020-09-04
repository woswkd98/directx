
cbuffer CameraInfo : register(b0)
{
	float3 camWorldPos; // 카메라의 월드 위치 
	float4x4 VP; // 투영 행렬
};

cbuffer quadInfo : register(b1)
{
	float particleLifeTime;
	float3 size; // 쿼드 사이즈
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

[maxvertexcount(4)] // 쿼드는 점의 개수가 4개이므로 
void ParticleDrawGS(point GS_INPUT p[1], inout TriangleStream<PS_INPUT> triStream) 
{
	PS_INPUT quadOut[4];
	
	// 카메라에서 나를 빼야 내쪽에서 카메라로 나가는 방향이 나온다
	float3 lookDir = normalize(camWorldPos - p[0].pos);
	
	// 룩을 기준으로 새로운 right up 재계산 

	// 순서는 y -> x -> z 이므로 거꾸로 만들어준다
	float3 right = normalize(cross(float3(0, 1, 0), lookDir));

	float3 up = normalize(cross(lookDir, right));

	// quad의 4점을 만들어준다 
	//	p0 는 왼쪽위 , p1은 오른쪽위, p2왼쪽 아래 , p3 오른쪽 아래 
	// 텍스쳐 좌표계 
	/*
		0,0	1,0
		0,1 1,1
	*/
	/*
	int spriteNum = spriteXY.y * spriteXY.x; // 스프라이트의 전체 개수 구함

	float intervalU = 1 / spriteXY.x; // 나눈 스프라이트의 간격
	float intervalV = 1 / spriteXY.y;

	int index = ceil(lerp(0, spriteNum - 1, p[0].currentTime / particleLifeTime)); // 현재 해당하는 인덱스를 구함 
	float u = int(index % spriteXY.x) / spriteXY.x; // uv 시작지점 구함 
	float v = int(index / spriteXY.x) / spriteXY.y;
	*/
	quadOut[0].pos = float4(p[0].pos -right * size.x / 2 + up * size.y / 2, 1.0f);
	quadOut[0].pos = mul(quadOut[0].pos, VP); // 뷰 프로젝션 곱해줌
	quadOut[0].uv = float2(0,0);
	triStream.Append(quadOut[0]); // 더해준다 
	quadOut[1].pos = float4(p[0].pos +right * size.x / 2 + up * size.y / 2, 1.0f);
	quadOut[1].pos = mul(quadOut[1].pos, VP);  // 뷰 프로젝션 곱해줌
	quadOut[1].uv = float2(1, 0);
	triStream.Append(quadOut[1]); // 더해준다
	quadOut[2].pos = float4(p[0].pos -right * size.x / 2 - up * size.y / 2, 1.0f);
	quadOut[2].pos = mul(quadOut[2].pos, VP);  // 뷰 프로젝션 곱해줌
	quadOut[2].uv = float2(0, 1);
	triStream.Append(quadOut[2]); // 더해준다
	quadOut[3].pos = float4(p[0].pos +right * size.x / 2 - up * size.y / 2, 1.0f);
	quadOut[3].pos = mul(quadOut[2].pos, VP); // 뷰 프로젝션 곱해줌
	quadOut[3].uv = float2(1,1);
	triStream.Append(quadOut[3]); // 더해준다

}

