cbuffer CBUFFER : register(b0)
{
    row_major float4x4 MVP;
    row_major float4x4 World;
};

struct VS_INPUT
{
    float3 Pos : POSITION;
    float2 TexCoord: TEXCOORD;
    float3 Normal : NORMAL;
};

struct VS_OUTPUT
{
    float4 Pos : SV_POSITION;
    float2 TexCoord : TEXCOORD;
    float3 Normal : NORMAL;
};

VS_OUTPUT main(VS_INPUT input)
{
    VS_OUTPUT output;
    output.Pos = mul(float4(input.Pos, 1.f), MVP);
    output.TexCoord = input.TexCoord;
    output.Normal = normalize(mul(float4(input.Normal, 0.f), World)); 
    return output;
}