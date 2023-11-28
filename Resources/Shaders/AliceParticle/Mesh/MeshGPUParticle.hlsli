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

struct Mesh
{
    float4 pos : POSITION; // ���W
    float3 normal : NORMAL; // �@��
    float2 uv : TEXCOORD; // uv���W
    float3 tangent :TANGENT; // �ڋ��
    float4 color :COLOR; // ���_�F
    uint4 boneIndex : BONEINDICES; // �{�[���̔ԍ�
    float4 boneWeight : BONEWEIGHTS; // �{�[���̏d��
};

struct EmitData
{
    matrix matWorld;

    float3 velocity;
    float maxParticles;

    float4 startColor;

    float4 endColor;

    float lifeTime;
    float size;
    float speed;
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

static const uint BONE_MAX = 128;

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

