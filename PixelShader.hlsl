cbuffer LightBuffer : register(b0)
{
    float3 AmbientColor;
    float AmbientStrength;
}

struct PS_INPUT
{
    float4 Pos : SV_POSITION;
    float2 TexCoord : TEXCOORD;
    float3 Normal : NORMAL;
};

Texture2D objTexture : TEXTURE : register(t0);
SamplerState objSamplerState : SAMPLER : register(s0);

float4 main(PS_INPUT input) : SV_TARGET
{
    //float3 pixelColor = objTexture.Sample(objSamplerState, input.TexCoord);
    float3 pixelColor = input.Normal;
    float3 ambientLight = AmbientColor * AmbientStrength;
    float3 finalColor = pixelColor * ambientLight;
    return float4(finalColor, 1.f);
}