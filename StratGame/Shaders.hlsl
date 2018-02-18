struct VertexOutput {
	float4 position : SV_POSITION;
	float4 color : COLOR;
	float3 normal : NORMAL;
};

cbuffer MatrixBuffer {
	matrix rotationMatrix;
	matrix positionMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
};

cbuffer LightBuffer {
	float4 diffuseColor;
	float3 lightDirection;
	float padding;
};

VertexOutput VShader(float4 position : POSITION, float4 color : COLOR, float3 normal : NORMAL) {
	VertexOutput output;

	position.w = 1.0f;
	output.position = mul(position, rotationMatrix);
	output.position = mul(output.position, positionMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);
	output.color = color;

	output.normal = mul(normal, (float3x3)rotationMatrix);
	output.normal = normalize(output.normal);

	return output;
}

float4 PShader(VertexOutput input) : SV_TARGET{
	float3 lightDir = -lightDirection;
	float lightIntensity = (smoothstep(-0.5, 1, dot(input.normal, lightDir)) * 0.5) + 0.5;

	float4 color = input.color * saturate(diffuseColor * lightIntensity);

	return color;
}