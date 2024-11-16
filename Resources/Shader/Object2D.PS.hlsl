#include "Object2d.hlsli"


static const int kPointLightNum = 4;

struct PixelShaderOutput
{
    float32_t4 color : SV_TARGET0;
};

struct Material
{
    float32_t4 color;
    float32_t4x4 uvTransform;
};

ConstantBuffer<Material> gMaterial : register(b0);

Texture2D<float32_t4> gTexture : register(t0);
Texture2D<float32_t4> gMaskTexture : register(t2);


SamplerState gSampler : register(s0);


PixelShaderOutput main(VertexShaderOutput input)
{
    PixelShaderOutput output;

    output.color.rgb = 0;
    float4 transformedUV = mul(float32_t4(input.texcoord, 0.0f, 1.0f), gMaterial.uvTransform);
    float32_t4 textureColor = gTexture.Sample(gSampler, transformedUV.xy);
    
    output.color = gMaterial.color * textureColor;
    
    //float32_t mask = gMaskTexture.Sample(gSampler, input.texcoord);
    //if (mask <= 0.5f)
    //{
    //    discard;
    //}
    //output.color = gTexture.Sample(gSampler, input.texcoord);

    return output;
}