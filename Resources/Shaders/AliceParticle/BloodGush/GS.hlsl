#include<BloodGush.hlsli>

cbuffer objectData : register(b0)
{
    matrix matWorld;
    matrix matBillboard;
};

[maxvertexcount(4)]
void main(point VS_OUTPUT input[1], inout TriangleStream<GS_OUTPUT> outStream)
{
    GS_OUTPUT output;
    
	[unroll]
    for (int i = 0; i < 4; i++)
    {
        //中心からのオフセットをスケーリング
        float4 offset = offset_array[i] * input[0].size;
        //中心からのオフセットをビルボード回転
        offset = mul(matBillboard, offset);
        output.position = float4(input[0].position,1.0f) + offset;
        output.position = mul(matWorld, output.position);
        output.color = input[0].color;
        output.threshold = input[0].threshold;
        output.uv = uv_array[i];

        outStream.Append(output);
    }
}