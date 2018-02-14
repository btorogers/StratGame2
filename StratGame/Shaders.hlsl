struct VOut {
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

VOut VShader(float4 position : POSITION, float4 color : COLOR, float3 normal : NORMAL) {
	VOut output;

	position.w = 1.0f;
	output.position = mul(position, rotationMatrix);
	output.position = mul(output.position, positionMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);
	output.color = color;

	output.normal = mul(normal, (float3x3)positionMatrix);
	output.normal = normalize(output.normal);

	return output;
}

float4 PShader(float4 position : SV_POSITION, float4 color : COLOR, float3 normal : NORMAL) : SV_TARGET{
	float3 lightDir = -lightDirection;
	float lightIntensity = (smoothstep(-0.5, 1, dot(normal, lightDir)) * 0.5) + 0.5;

	color = color * saturate(diffuseColor * lightIntensity);

	return color;
}