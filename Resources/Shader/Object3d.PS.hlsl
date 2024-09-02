#include "Object3d.hlsli"

struct PixelShaderOutput
{
    float32_t4 color : SV_TARGET0;
};

struct Material
{
    float32_t4 color;
    int32_t enableLighting;
    float32_t4x4 uvTransform;
    int32_t eneblePhong;
    float32_t shininess;
    int32_t enableEnvTexture;
};

struct DirectionalLight
{
    float32_t4 color;
    float32_t3 direction;
    float intensity;
};
	
struct Camera
{
    float32_t3 worldPosition;
};

struct SpotLight
{
    float32_t4 color;
    float32_t3 position;
    float32_t intensity;
    float32_t3 direction;
    float32_t distance;
    float32_t decay;
    float32_t cosAngle;
};

ConstantBuffer<Material> gMaterial : register(b0);
ConstantBuffer<Camera> gCamera : register(b1);
ConstantBuffer<DirectionalLight> gDirectionalLight : register(b2);
ConstantBuffer<SpotLight> gSpotLight : register(b3);

Texture2D<float32_t4> gTexture : register(t0);
TextureCube<float32_t4> gEnviromentTexture : register(t1);

SamplerState gSampler : register(s0);

PixelShaderOutput main(VertexShaderOutput input)
{
    PixelShaderOutput output;

    float4 transformedUV = mul(float32_t4(input.texcoord, 0.0f, 1.0f), gMaterial.uvTransform);
    float32_t4 textureColor = gTexture.Sample(gSampler, transformedUV.xy);

    output.color = gMaterial.color * textureColor;
	
    if (gMaterial.enableLighting != 0)
    {
        float NdotL = dot(normalize(input.normal), -gDirectionalLight.direction);
        float cos = pow(NdotL * 0.5f + 0.5f, 2.0f);

        output.color = gMaterial.color * textureColor * gDirectionalLight.color * cos * gDirectionalLight.intensity;
        output.color.a = 0.0f;
    }
    else
    {
        output.color = gMaterial.color * textureColor;
    }
    
    return output;
}

PixelShaderOutput main(VertexShaderOutput input)
{
    PixelShaderOutput output;

    float4 transformedUV = mul(float32_t4(input.texcoord, 0.0f, 1.0f), gMaterial.uvTransform);
    float32_t4 textureColor = gTexture.Sample(gSampler, transformedUV.xy);

    output.color = gMaterial.color * textureColor;
	
    if (gMaterial.enableLighting != 0)
    {
        
        float32_t3 spotLightDirectionOnSurface = normalize(input.worldPosition - gSpotLight.position);
        
        //float NdotL = dot(normalize(input.normal), -gDirectionalLight.direction);
        //float cos = pow(NdotL * 0.5f + 0.5f, 2.0f);
        float cos = saturate(dot(normalize(input.normal), -spotLightDirectionOnSurface));
        
        //output.color.rgb = gSpotLight.color.rgb * gSpotLight.intensity;
        
         //カメラへの方向を算出
        float32_t3 toEye = normalize(gCamera.worldPosition - input.worldPosition);
         //内積
        float32_t3 halfVector = normalize(-spotLightDirectionOnSurface + toEye);
        float NDotH = dot(normalize(input.normal), halfVector);
        //反射強度
        float specularPow = pow(saturate(NDotH), gMaterial.shininess);
        //
        float32_t cosAngle = dot(spotLightDirectionOnSurface, gSpotLight.direction);
        float32_t falloffFactor = saturate((cosAngle - gSpotLight.cosAngle) / 1.0f - gSpotLight.cosAngle);
        
        //
        float32_t distance = length(gSpotLight.position - input.worldPosition);
        //float32_t attenuationFactor = pow(saturate(-distance / gSpotLight.distance + 1.0f), gSpotLight.decay);
        float32_t attenuationFactor = saturate(pow(-distance / gSpotLight.distance + 1.0f, gSpotLight.decay));
        
         //拡散反射
        float32_t3 diffuse = gMaterial.color.rgb * textureColor.rgb * gSpotLight.color.rgb * cos * gSpotLight.intensity * attenuationFactor * falloffFactor;
        
        output.color.rgb += diffuse;
        output.color.a = gMaterial.color.a * textureColor.a;
        //gSpotLight.color.rgb * gSpotLight.intensity * attenuationFactor * falloffFactor; //減衰後の光の色
    }
    else
    {
        output.color = gMaterial.color * textureColor;
    }

    return output;
}