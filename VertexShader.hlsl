struct VS_INPUT
{
    float2 Pos : POSITION;
    float3 Color : COLOR;
};

struct VS_OUTPUT
{
    float4 Pos : SV_POSITION;
    float3 Color : COLOR;
};

VS_OUTPUT main(VS_INPUT input)
{
    VS_OUTPUT output;
    output.Pos = float4(input.Pos, 0.f, 1.f);
    output.Color = input.Color;
    return output;
}