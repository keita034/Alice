#include "RainParticle.hlsli"

float4 rotate(float4 pos, float angle)
{
    return float4(
    pos.x * cos(angle) + pos.y * -sin(angle),
    pos.x * sin(angle) + pos.y * cos(angle),
    pos.z,
    pos.w);
}

//四角形の頂点
static const uint vnum = 2;

//センターからオフセット
static const float4 offset_array[vnum] =
{
    float4(-0.5f, -0.5f, 0, 0), //左下
    float4(-0.5f, +0.5f, 0, 0), //左上
};

[maxvertexcount(vnum)]

void main(point VSOutput input[1] : SV_POSITION,
	inout LineStream<GSOutput> output)
{
    GSOutput element;
    for (uint i = 0; i < vnum; i++)
    {
        //中心からのオフセットをスケーリング
        float4 offset = offset_array[i] * input[0].scale;

       //中心からのオフセットをローテーション
        offset = rotate(offset, input[0].rotation);

        //中心からのオフセットをビルボード回転
        offset = mul(matBillboard, offset);

        //オフセット分ずらす
        element.svpos = input[0].svpos + offset;

        //ビュープロジェクション変換
        element.svpos = mul(matWorld, element.svpos);
        element.color = input[0].color;
        output.Append(element);
    }
	
 
}