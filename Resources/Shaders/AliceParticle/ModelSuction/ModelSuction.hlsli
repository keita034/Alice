#include<../GPUParticle.hlsli>

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

    float4 startColor;
    float4 endColor;
    
    float3 pos;
    float pad;
    
    float deltaTime;
    float lifeTime;
    float size;
    float speed;

    uint vertexSize;
    uint computeTime;
    uint maxParticles;
    uint emitSize;
};

static const uint RAND_MAX = 32767;

static const uint BONE_MAX = 128;

