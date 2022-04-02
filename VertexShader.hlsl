struct VS_INPUT
{
    float3 Pos : POSITION;
    float2 TexCoord: TEXCOORD;
};

struct VS_OUTPUT
{
    float4 Pos : SV_POSITION;
    float2 TexCoord : TEXCOORD;
};

VS_OUTPUT main(VS_INPUT input)
{
    VS_OUTPUT output;
    output.Pos = float4(input.Pos, 1.f);
    output.TexCoord = input.TexCoord;
    return output;
}