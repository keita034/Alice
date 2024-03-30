//struct IndirectCommand
//{
//    uint vertexCountPerInstance;
//    uint instanceCount;
//    uint startVertexLocation;
//    uint startInstanceLocation;
//};

//struct VS_OUTPUT
//{
//    float3 position : POSITION;
//    float4 color : COLOR;
//    float2 size : SIZE;
//    float threshold : THRESHOLD;
//};

//struct GS_OUTPUT
//{
//    float4 position : SV_POSITION;
//    float4 color : COLOR;
//    float2 uv : TEXCOORD;
//    float threshold : THRESHOLD;
//};

////�l�p�`�̒��_
//static const uint vnum = 4;

////�Z���^�[����I�t�Z�b�g
//static const float4 offset_array[vnum] =
//{
//    float4(-0.5f, -0.5f, 0, 0), //����
//    float4(-0.5f, +0.5f, 0, 0), //����
//    float4(+0.5f, -0.5f, 0, 0), //�E��
//    float4(+0.5f, +0.5f, 0, 0) //�E��
//};

////���オ0,0 �E����1,1

//static const float2 uv_array[vnum] =
//{
//    float2(0, 1), //����
//    float2(0, 0), //����
//    float2(1, 1), //�E��
//    float2(1, 0), //�E��
//};

//struct ModelParticle
//{
//    float4 color;

//    float3 position;
//    float age;

//    float3 movePos;
//    float alive;

//    float3 meshPos;
//    float invTime;
    
//    float moveTime;
//    float totalTime;
//    float2 pad;
    
//    float2 size;
//    float threshold;
//    float lifeTime;
//};

//struct Mesh
//{
//    float4 pos : POSITION; // ���W
//    float3 normal : NORMAL; // �@��
//    float2 uv : TEXCOORD; // uv���W
//    float3 tangent :TANGENT; // �ڋ��
//    float4 color :COLOR; // ���_�F
//    uint4 boneIndex : BONEINDICES; // �{�[���̔ԍ�
//    float4 boneWeight : BONEWEIGHTS; // �{�[���̏d��
//};

//struct EmitData
//{
//    matrix matWorld;

//    float4 startColor;

//    float4 endColor;

//    float3 centerPos;
//    float distance;

//    uint computeTime;
//    float deltaTime;
//    float lifeTime;
//    float invisibleTime;

//    float moveTime;
//    float size;
//    uint vertexSize;
//    uint maxParticles;
//};

//static const uint RAND_MAX = 32767;

//static const uint BONE_MAX = 128;

#include<../GPUParticle.hlsli>

struct Mesh
{
    float4 pos : POSITION; // ���W
    float3 normal : NORMAL; // �@��
    float2 uv : TEXCOORD; // uv���W
    float3 tangent : TANGENT; // �ڋ��
    float4 color : COLOR; // ���_�F
    uint4 boneIndex : BONEINDICES; // �{�[���̔ԍ�
    float4 boneWeight : BONEWEIGHTS; // �{�[���̏d��
};

struct EmitData
{
    matrix matWorld;

    float4 startColor;

    float4 endColor;

    float3 centerPos;
    float distance;

    float lifeTime;
    float invisibleTime;
    float moveTime;
    float size;
    
    uint vertexSize;
    uint maxParticles;
    float2 pad;
};

static const uint RAND_MAX = 32767;

static const uint BONE_MAX = 128;

struct ModelParticle
{
    float4 color;

    float3 position;
    float age;

    float3 movePos;
    float alive;

    float3 meshPos;
    float invTime;
    
    float moveTime;
    float totalTime;
    float2 pad;
    
    float2 size;
    float threshold;
    float lifeTime;
};