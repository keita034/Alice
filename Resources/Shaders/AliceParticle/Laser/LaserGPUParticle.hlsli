struct Particle
{
	float4 color;
	float3 position;
	float age;
	float3 velocity;
    float size;
    float alive;
    float threshold;
    float lifeTime;
    uint index;
};

struct EmitData
{
    float3 velocity;
    float lifeTime;

    float3 position;
    float maxParticles;

    float4 startColor;

    float4 endColor;

    float speed;
    float maxSize;
    float maxSizeTime;
    float pad;
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
    float threshold : THRESHOLD;
};

struct GS_OUTPUT
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
    float2 uv : TEXCOORD;
    float threshold : THRESHOLD;
};

static const uint RAND_MAX = 32767;

static const uint EMIT_DATA_MAX = 100;

//四角形の頂点
static const uint vnum = 4;

//センターからオフセット
static const float4 offset_array[vnum] =
{
    float4(-0.5f, -0.5f, 0, 0), //左下
    float4(-0.5f, +0.5f, 0, 0), //左上
    float4(+0.5f, -0.5f, 0, 0), //右下
    float4(+0.5f, +0.5f, 0, 0) //右上
};

//左上が0,0 右下が1,1

static const float2 uv_array[vnum] =
{
    float2(0, 1), //左下
    float2(0, 0), //左上
    float2(1, 1), //右下
    float2(1, 0), //右上
};

