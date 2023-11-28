// 頂点データ
static const int MAX_BONE = 128;

struct PosNormSkin
{
    float4 pos : POSITION; // 座標
    float3 normal : NORMAL; // 法線
    float2 uv : TEXCOORD; // uv座標
    float3 tangent :TANGENT; // 接空間
    float4 color :COLOR; // 頂点色
    uint4 boneIndex : BONEINDICES; // ボーンの番号
    float4 boneWeight : BONEWEIGHTS; // ボーンの重み

};

cbuffer SkinData : register(b0)
{
    matrix bones[MAX_BONE];
};

//出力データ構造体
struct OutputData
{
    float4 pos; // 座標
    float3 normal; // 法線
    float2 uv; // uv座標
    float3 tangent; // 接空間
    float4 color; // 頂点色
};

//入力データにアクセスするための変数
StructuredBuffer<PosNormSkin> input : register(t0);

//出力データにアクセスするための変数
RWStructuredBuffer<OutputData> output : register(u0);

//エントリーポイント
[numthreads(256, 1, 1)]
void main(uint3 DTid : SV_DispatchThreadID)
{
     // ゼロクリア
    OutputData outputData = (OutputData) 0;

    uint iBone; // 計算するボーンの番号
    float boneWeight; // ボーンウェイト(重み)
    matrix m; // スキニング行列

    // ボーン0
    iBone = input[DTid.x].boneIndex.x;
    boneWeight = input[DTid.x].boneWeight.x;
    m = bones[iBone];
    outputData.pos += boneWeight * mul(m, input[DTid.x].pos);
    outputData.normal += boneWeight * mul((float3x3) m, input[DTid.x].normal);

    // ボーン1
    iBone = input[DTid.x].boneIndex.y;
    boneWeight = input[DTid.x].boneWeight.y;
    m = bones[iBone];
    outputData.pos += boneWeight * mul(m, input[DTid.x].pos);
    outputData.normal += boneWeight * mul((float3x3) m, input[DTid.x].normal);

    // ボーン2
    iBone = input[DTid.x].boneIndex.z;
    boneWeight = input[DTid.x].boneWeight.z;
    m = bones[iBone];
    outputData.pos += boneWeight * mul(m, input[DTid.x].pos);
    outputData.normal += boneWeight * mul((float3x3) m, input[DTid.x].normal);

    // ボーン3
    iBone = input[DTid.x].boneIndex.w;
    boneWeight = input[DTid.x].boneWeight.w;
    m = bones[iBone];
    outputData.pos += boneWeight * mul(m, input[DTid.x].pos);
    outputData.normal += boneWeight * mul((float3x3) m, input[DTid.x].normal);
 
    //outputData.uv = input[DTid.x].uv;
    //outputData.tangent = input[DTid.x].tangent;
    //outputData.color = input[DTid.x].color;
    
    output[DTid.x].pos = outputData.pos;
    output[DTid.x].normal = outputData.normal;

}