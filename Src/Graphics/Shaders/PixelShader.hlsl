cbuffer LightBuffer : register(b0)
{
    float3 AmbientColor;
    float AmbientStrength;
    
    float3 DirectionalColor;
    float DirectionalStrength;
    
	float3 DirectionalPosition;
    float dummy3;
    
    float Attenuation_A;
    float Attenuation_B;
    float Attenuation_C;
    float dummy4;
}

struct PS_INPUT
{
    float4 Pos : SV_POSITION;
    float2 TexCoord : TEXCOORD;
    float3 Normal : NORMAL;
    float3 WorldPos : WORLD_POSITION;
};

Texture2D objTexture : TEXTURE : register(t0);
SamplerState objSamplerState : SAMPLER : register(s0);

float4 main(PS_INPUT input) : SV_TARGET
{
    float3 pixelColor = objTexture.Sample(objSamplerState, input.TexCoord);
    
    // Ambient Light
    float3 light = AmbientColor * AmbientStrength;
    
    // Directional Light
    float3 vectorToLight = normalize(DirectionalPosition - input.WorldPos);
    float facingToLight = clamp(dot(vectorToLight, input.Normal), 0.f, 1.f); 
    
    // Light Attenuation
    float d = distance(DirectionalPosition, input.WorldPos);
    float attenuation = max(1.0, Attenuation_A * d * (Attenuation_B * d + Attenuation_C));
    
    light += DirectionalColor * DirectionalStrength * facingToLight / attenuation;
    
    // Final Color
    float3 finalColor = pixelColor * light;
    return float4(finalColor, 1.f);
}