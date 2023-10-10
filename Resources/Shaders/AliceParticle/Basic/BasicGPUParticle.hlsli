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
	//���W
    float3 startPosition;
    //�ő吔
    int emitCount;
	//���x
    float3 velocity;
    //���C�t
    float lifeTime;
	//�����x
    float3 accel;
    float pad;
	//�X�P�[��(�����l,�ŏI�l)
    float2 scale;
	//�����l(�����l,�ŏI�l)
    float2 rotation;
    //�J���[�����l
    float4 startColor;
    //�J���[�ŏI�l
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

