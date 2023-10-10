struct Particle
{
    float4 Color;
    float3 Position;
    float Age;
    float3 Velocity;
    float Size;
    float Alive;
    float3 accel;
    uint index;
};

struct ParticleData
{
	//座標
    float3 startPosition;
    //最大数
    int emitCount;
	//速度
    float3 velocity;
    //ライフ
    float lifeTime;
	//加速度
    float3 accel;
    float pad;
	//スケール(初期値,最終値)
    float2 scale;
	//初期値(初期値,最終値)
    float2 rotation;
    //カラー初期値
    float4 startColor;
    //カラー最終値
    float4 endColor;
};

struct IndirectCommand
{
    uint vertexCountPerInstance;
    uint instanceCount;
    uint startVertexLocation;
    uint startInstanceLocation;
};

struct VS_OUTPUT
{
    float3 position : POSITION;
    float size : SIZE;
    float4 color : COLOR;
};

struct GS_OUTPUT
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
    float2 uv : TEXCOORD;
};

static const uint RAND_MAX = 32767;

static const float2 gsOffsets[4] =
{
    float2(-1.0f, -1.0f),
    float2(-1.0f, +1.0f),
    float2(+1.0f, -1.0f),
    float2(+1.0f, +1.0f)
};

