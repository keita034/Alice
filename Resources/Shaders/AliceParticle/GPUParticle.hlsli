struct Particle
{
	float4 color;

	float3 position;
	float age;

	float3 velocity;
    float alive;
    
    float3 accel;
    uint index;

    float2 size;
    float threshold;
    float lifeTime;
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
    float4 color : COLOR;
    float2 size : SIZE;
    float threshold : THRESHOLD;
};

struct GS_OUTPUT
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
    float2 uv : TEXCOORD;
    float threshold : THRESHOLD;
};

//�l�p�`�̒��_
static const uint vnum = 4;

//�Z���^�[����I�t�Z�b�g
static const float4 offset_array[vnum] =
{
    float4(-0.5f, -0.5f, 0, 0), //����
    float4(-0.5f, +0.5f, 0, 0), //����
    float4(+0.5f, -0.5f, 0, 0), //�E��
    float4(+0.5f, +0.5f, 0, 0) //�E��
};

//���オ0,0 �E����1,1

static const float2 uv_array[vnum] =
{
    float2(0, 1), //����
    float2(0, 0), //����
    float2(1, 1), //�E��
    float2(1, 0), //�E��
};

