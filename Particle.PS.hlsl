#include "Particle.hlsli"

ConstantBuffer<Material> gMaterial : register(b0);
ConstantBuffer<DirectionalLight> gDirectionalLight : register(b1);

Texture2D<float32_t4> gTexture : register(t0);
SamplerState gSampler : register(s0);

PixelShaderOutput main(VertexShaderOutput input) {
	PixelShaderOutput output;
	float4 transformdUV = mul(float32_t4(input.texcoord,0.0f,1.0f),gMaterial.uvTransform);
	float32_t4 textureColor = gTexture.Sample(gSampler, transformdUV.xy);
	output.color = gMaterial.color * textureColor;

	if(output.color.a == 0.0) {
		discard;
	}

	output.color = gMaterial.color * textureColor;
	return output;
}