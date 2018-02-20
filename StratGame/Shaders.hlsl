struct VertexOutput {
	float4 position : SV_POSITION;
	float4 color : COLOR;
	float3 normal : NORMAL;
};

struct VertexInput {
	float4 position : POSITION;
	float4 color : COLOR;
	float3 normal : NORMAL;
	float4x4 scale : SCALE;
	float4x4 rotation : ROTATION;
	float4x4 location : LOCATION;
};
struct UninstancedVertexInput {
	float4 position : POSITION;
	float4 color : COLOR;
	float3 normal : NORMAL;
};

cbuffer MatrixBuffer {
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
};

cbuffer LightBuffer {
	float4 diffuseColor;
	float3 lightDirection;
	float padding;
};

VertexOutput InstancedVertexShader(VertexInput input) {
	VertexOutput output;

	input.position.w = 1.0f;
	output.position = mul(input.position, input.scale);
	output.position = mul(output.position, input.rotation);
	output.position = mul(output.position, input.location);
	output.position = mul(output.position, worldMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);
	output.color = input.color;

	output.normal = mul(input.normal, (float3x3)input.rotation);
	output.normal = mul(output.normal, (float3x3)worldMatrix);
	output.normal = normalize(output.normal);

	return output;
}

VertexOutput UninstancedVertexShader(UninstancedVertexInput input) {
	VertexOutput output;

	input.position.w = 1.0f;
	output.position = mul(input.position, worldMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);
	output.color = input.color;

	output.normal = mul(input.normal, (float3x3)worldMatrix);
	output.normal = normalize(output.normal);

	return output;
}

float4 PShader(VertexOutput input) : SV_TARGET {
	float3 lightDir = -lightDirection;
	float lightIntensity = (smoothstep(-0.5, 1, dot(input.normal, lightDir)) * 0.5) + 0.5;

	float4 color = input.color * saturate(diffuseColor * lightIntensity);

	return color;
}