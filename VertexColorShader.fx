




struct VertexColorIn
{
	float3 position : POSITION;
	float4 color : COLOR;
};

struct VertexColorOut
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
};

cbuffer WVPBUF : register(b0)  // constantbuffer 
{
	float4x4 ViewProjection;
};

VertexColorOut VertexColorVS(VertexColorIn In)
{
	VertexColorOut output;
	output.color = In.color;
	output.position = mul(float4(In.position, 1.0f), ViewProjection);
	return output;
}


VertexColorOut LineRenderVS(VertexColorIn In)
{
	VertexColorOut output;
	output.color = In.color;
	output.position = mul(float4(In.position, 1.0f), ViewProjection);
	return output;
}


float4 VertexColorPS(VertexColorOut Out) : SV_Target
{
	return Out.color;
}

