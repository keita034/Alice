#include<BasicGPUParticle.hlsli>

cbuffer WorldMatrix : register(b0)
{
    matrix world;
};

cbuffer objectData : register(b1)
{
    matrix view;
    matrix projection;
};

[maxvertexcount(4)]
void main(point VS_OUTPUT input[1], inout TriangleStream<GS_OUTPUT> outStream)
{
    GS_OUTPUT output;

    matrix mvp = mul(mul(world, view), projection);

	[unroll]
    for (int i = 0; i < 4; i++)
    {
        output.position = mul(float4(input[0].position, 1.0f), mvp);

        float depthChange = output.position.z / output.position.w;

        output.position.xy += gsOffsets[i] * depthChange * input[0].size;
        output.color = input[0].color;
        output.uv = saturate(gsOffsets[i]);

        outStream.Append(output);
    }
}