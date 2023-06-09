#include "Particle.hlsli"

float4 rotate(float4 pos, float angle)
{
    return float4(
    pos.x * cos(angle) + pos.y * -sin(angle),
    pos.x * sin(angle) + pos.y * cos(angle),
    pos.z,
    pos.w);
}

//四角形の頂点
static const uint vnum = 60;


static const float4 vertexs[vnum] =
{
    float4(0, -1, -0 ,0),
    float4(0.7236, -0.447215, -0.52572, 0),
    float4(-0.276385, -0.447215, -0.85064, 0),
    float4(0.7236, -0.447215, -0.52572, 0),
    float4(0, -1, -0, 0),
    float4(0.7236, -0.447215, 0.52572, 0),
    float4(0, -1, -0, 0),
    float4(-0.276385, -0.447215, -0.85064, 0),
    float4(-0.894425, -0.447215, -0, 0),
    float4(0, -1, -0, 0),
    float4(-0.894425, -0.447215, -0, 0),
    float4(-0.276385, -0.447215, 0.85064, 0),
    float4(0, -1, -0, 0),
    float4(-0.276385, -0.447215, 0.85064, 0),
    float4(0.7236, -0.447215, 0.52572, 0),
    float4(0.7236, -0.447215, -0.52572, 0),
    float4(0.7236, -0.447215, 0.52572, 0),
    float4(0.894425, 0.447215, -0, 0),
    float4(-0.276385, -0.447215, -0.85064, 0),
    float4(0.7236, -0.447215, -0.52572, 0),
    float4(0.276385, 0.447215, -0.85064, 0),
    float4(-0.894425, -0.447215, -0, 0),
    float4(-0.276385, -0.447215, -0.85064, 0),
    float4(-0.7236, 0.447215, -0.52572, 0),
    float4(-0.276385, -0.447215, 0.85064, 0),
    float4(-0.894425, -0.447215, -0, 0),
    float4(-0.7236, 0.447215, 0.52572, 0),
    float4(0.7236, -0.447215, 0.52572, 0),
    float4(-0.276385, -0.447215, 0.85064, 0),
    float4(0.276385, 0.447215, 0.85064, 0),
    float4(0.7236, -0.447215, -0.52572, 0),
    float4(0.894425, 0.447215, -0, 0),
    float4(0.276385, 0.447215, -0.85064, 0),
    float4(-0.276385, -0.447215, -0.85064, 0),
    float4(0.276385, 0.447215, -0.85064, 0),
    float4(-0.7236, 0.447215, -0.52572, 0),
    float4(-0.894425, -0.447215, -0, 0),
    float4(-0.7236, 0.447215, -0.52572, 0),
    float4(-0.7236, 0.447215, 0.52572, 0),
    float4(-0.276385, -0.447215, 0.85064, 0),
    float4(-0.7236, 0.447215, 0.52572, 0),
    float4(0.276385, 0.447215, 0.85064, 0),
    float4(0.7236, -0.447215, 0.52572, 0),
    float4(0.276385, 0.447215, 0.85064, 0),
    float4(0.894425, 0.447215, -0, 0),
    float4(0.276385, 0.447215, -0.85064, 0),
    float4(0.894425, 0.447215, -0, 0),
    float4(0, 1, -0, 0),
    float4(-0.7236, 0.447215, -0.52572, 0),
    float4(0.276385, 0.447215, -0.85064, 0),
    float4(0, 1, -0, 0),
    float4(-0.7236, 0.447215, 0.52572, 0),
    float4(-0.7236, 0.447215, -0.52572, 0),
    float4(0, 1, -0, 0),
    float4(0.276385, 0.447215, 0.85064, 0),
    float4(-0.7236, 0.447215, 0.52572, 0),
    float4(0, 1, -0, 0),
    float4(0.894425, 0.447215, -0, 0),
    float4(0.276385, 0.447215, 0.85064, 0),
    float4(0, 1, -0, 0)
};

static const float2 uvs[vnum] =
{
    float2(0.181819, 1),
    float2(0.272728, 0.842539),
    float2(0.09091, 0.842539),
    float2(0.272728, 0.842539),
    float2(0.363637, 1),
    float2(0.454546, 0.842539),
    float2(0.909091, 1),
    float2(1, 0.842539),
    float2(0.818182, 0.842539),
    float2(0.727273, 1),
    float2(0.818182, 0.842539),
    float2(0.636364, 0.842539),
    float2(0.545455, 1),
    float2(0.636364, 0.842539),
    float2(0.454546, 0.842539),
    float2(0.272728, 0.842539),
    float2(0.454546, 0.842539),
    float2(0.363637, 0.685079),
    float2(0.09091, 0.842539),
    float2(0.272728, 0.842539),
    float2(0.181819, 0.685079),
    float2(0.818182, 0.842539),
    float2(1, 0.842539),
    float2(0.909091, 0.685079),
    float2(0.636364, 0.842539),
    float2(0.818182, 0.842539),
    float2(0.727273, 0.685079),
    float2(0.454546, 0.842539),
    float2(0.636364, 0.842539),
    float2(0.545455, 0.685079),
    float2(0.272728, 0.842539),
    float2(0.363637, 0.685079),
    float2(0.181819, 0.685079),
    float2(0.09091, 0.842539),
    float2(0.181819, 0.685079),
    float2(0, 0.685079),
    float2(0.818182, 0.842539),
    float2(0.909091, 0.685079),
    float2(0.727273, 0.685079),
    float2(0.636364, 0.842539),
    float2(0.727273, 0.685079),
    float2(0.545455, 0.685079),
    float2(0.454546, 0.842539),
    float2(0.545455, 0.685079),
    float2(0.363637, 0.685079),
    float2(0.181819, 0.685079),
    float2(0.363637, 0.685079),
    float2(0.272728, 0.527618),
    float2(0, 0.685079),
    float2(0.181819, 0.685079),
    float2(0.09091, 0.527618),
    float2(0.727273, 0.685079),
    float2(0.909091, 0.685079),
    float2(0.818182, 0.527618),
    float2(0.545455, 0.685079),
    float2(0.727273, 0.685079),
    float2(0.636364, 0.527618),
    float2(0.363637, 0.685079),
    float2(0.545455, 0.685079),
    float2(0.454546, 0.527618)
};

[maxvertexcount(vnum)]

void main(point VSOutput input[1] : SV_POSITION,
	inout TriangleStream<GSOutput> output)
{
    GSOutput element;
    
    for (uint i = 0; i < 20; i++)
    {
        for (uint j = 0; j < 3; j++)
        {
            const uint vid = i * 3 + j;
            
        //中心からのオフセットをスケーリング
            float4 offset = vertexs[vid] * input[0].scale;

       //中心からのオフセットをローテーション
            offset = rotate(offset, input[0].rotation);

        //中心からのオフセットをビルボード回転
            offset = mul(matBillboard, offset);

        //オフセット分ずらす
            element.svpos = input[0].svpos + offset;

        //ビュープロジェクション変換
            element.svpos = mul(matWorld, element.svpos);
            element.uv = uvs[vid];
            element.color = input[0].color;
            output.Append(element);
        }
    }
}