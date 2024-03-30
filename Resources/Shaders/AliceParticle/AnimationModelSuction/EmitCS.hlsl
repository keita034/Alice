//#include<AnimationModelSuction.hlsli>
//#include<../../HLSLMath.hlsli>

//cbuffer ParticleDatas : register(b0)
//{
//    EmitData emitData;
//}

//cbuffer BoneDatas : register(b1)
//{
//    matrix bones[BONE_MAX];
//}

//RWStructuredBuffer<ModelParticle> ParticlePool : register(u0);
//ConsumeStructuredBuffer<uint> freeList : register(u1);

//StructuredBuffer<Mesh> meshs : register(t0);

//struct SkinOutput
//{
//    float4 pos;
//    float3 normal;
//};

////�X�L�j���O�v�Z
//SkinOutput ComputeSkin(Mesh input)
//{
//    //�[���N���A
//    SkinOutput output = (SkinOutput) 0;

//    uint iBone; //�v�Z����{�[���ԍ�
//    float weight; //�{�[���E�F�C�g�i�d�݁j
//    matrix m; //�X�L�j���O�s��

//    //�{�[��0
//    iBone = input.boneIndex.x;
//    weight = input.boneWeight.x;
//    m = bones[iBone];
//    output.pos += weight * mul(m, input.pos);
//    output.normal += weight * mul((float3x3) m, input.normal);

//    //�{�[��1
//    iBone = input.boneIndex.y;
//    weight = input.boneWeight.y;
//    m = bones[iBone];
//    output.pos += weight * mul(m, input.pos);
//    output.normal += weight * mul((float3x3) m, input.normal);

//    //�{�[��2
//    iBone = input.boneIndex.z;
//    weight = input.boneWeight.z;
//    m = bones[iBone];
//    output.pos += weight * mul(m, input.pos);
//    output.normal += weight * mul((float3x3) m, input.normal);

//    //�{�[��3
//    iBone = input.boneIndex.w;
//    weight = input.boneWeight.w;
//    m = bones[iBone];
//    output.pos += weight * mul(m, input.pos);
//    output.normal += weight * mul((float3x3) m, input.normal);

//    return output;
//}

//[numthreads(1024, 1, 1)]
//void main( uint3 DTid : SV_DispatchThreadID )
//{
//    if (DTid.x >= emitData.maxParticles)
//    {
//        return;
//    }
    
//    if (DTid.x >= emitData.vertexSize)
//    {
//        return;
//    }
    
//    uint emitIndex = freeList.Consume();
    
//    SkinOutput skin = (SkinOutput) 0;
//    skin = ComputeSkin(meshs[DTid.x]);

//    float3 pos = mul(emitData.matWorld, skin.pos);
//    float3 vec = pos - emitData.centerPos;
//    vec = normalize(vec);
//    ParticlePool[emitIndex].position = pos;
//    ParticlePool[emitIndex].meshPos = pos;
//    ParticlePool[emitIndex].movePos = ParticlePool[emitIndex].position;
    
//    ParticlePool[emitIndex].invTime = emitData.invisibleTime * RandFloat(DTid.x + emitData.computeTime, 1.0f, 0.0f);
//    ParticlePool[emitIndex].moveTime = emitData.moveTime * RandFloat(DTid.x + emitData.computeTime + emitIndex, 1.0f, 0.0f);
    
//    ParticlePool[emitIndex].color = emitData.startColor;
//    ParticlePool[emitIndex].age = 0;
//    ParticlePool[emitIndex].totalTime = 0;
//    ParticlePool[emitIndex].alive = 3;
//    ParticlePool[emitIndex].size = float2(emitData.size,emitData.size);
//    ParticlePool[emitIndex].lifeTime = emitData.lifeTime;

//}

#include<AnimationModelSuction.hlsli>
#include<../../HLSLMath.hlsli>

cbuffer timeData : register(b0)
{
    float deltaTime;
    float TotalTime;
    uint computeTime;
}

cbuffer ParticleData : register(b1)
{
    uint maxParticles;
    uint emitDataIndex;
}

cbuffer ParticleDatas : register(b2)
{
    EmitData emitData;
}

cbuffer BoneDatas : register(b3)
{
    matrix bones[BONE_MAX];
}

Texture2D<float4> tex : register(t0);
StructuredBuffer<Mesh> meshs : register(t1);

RWStructuredBuffer<ModelParticle> ParticlePool : register(u0);
ConsumeStructuredBuffer<uint> freeList : register(u1);
RWStructuredBuffer<uint> DrawList : register(u2);


struct SkinOutput
{
    float4 pos;
    float3 normal;
};

//�X�L�j���O�v�Z
SkinOutput ComputeSkin(Mesh input)
{
    //�[���N���A
    SkinOutput output = (SkinOutput) 0;

    uint iBone; //�v�Z����{�[���ԍ�
    float weight; //�{�[���E�F�C�g�i�d�݁j
    matrix m; //�X�L�j���O�s��

    //�{�[��0
    iBone = input.boneIndex.x;
    weight = input.boneWeight.x;
    m = bones[iBone];
    output.pos += weight * mul(m, input.pos);
    output.normal += weight * mul((float3x3) m, input.normal);

    //�{�[��1
    iBone = input.boneIndex.y;
    weight = input.boneWeight.y;
    m = bones[iBone];
    output.pos += weight * mul(m, input.pos);
    output.normal += weight * mul((float3x3) m, input.normal);

    //�{�[��2
    iBone = input.boneIndex.z;
    weight = input.boneWeight.z;
    m = bones[iBone];
    output.pos += weight * mul(m, input.pos);
    output.normal += weight * mul((float3x3) m, input.normal);

    //�{�[��3
    iBone = input.boneIndex.w;
    weight = input.boneWeight.w;
    m = bones[iBone];
    output.pos += weight * mul(m, input.pos);
    output.normal += weight * mul((float3x3) m, input.normal);

    return output;
}

[numthreads(1024, 1, 1)]
void main(uint3 DTid : SV_DispatchThreadID)
{
    if (DTid.x >= maxParticles)
    {
        return;
    }
    
    int2 uv;
    float2 texSize;
    tex.GetDimensions(texSize.x, texSize.y);
    
    uv = int2(texSize * meshs[DTid.x].uv);
    
    float4 texcolor = tex[uv];
    
    if (texcolor.a == 0.0f)
    {
        return;
    }

    if (DTid.x >= emitData.vertexSize)
    {
        return;
    }
    
    uint emitIndex = freeList.Consume();
    
    SkinOutput skin = (SkinOutput) 0;
    skin = ComputeSkin(meshs[DTid.x]);
    
    float3 pos = mul(emitData.matWorld, skin.pos);
    float3 vec = pos - emitData.centerPos;
    vec = normalize(vec);

    ParticlePool[emitIndex].position = pos + (vec * emitData.distance);
    ParticlePool[emitIndex].meshPos = pos;
    ParticlePool[emitIndex].movePos = pos + (vec * emitData.distance);;
    
    ParticlePool[emitIndex].invTime = emitData.invisibleTime * RandFloat(DTid.x + computeTime, 1.0f, 0.0f);
    ParticlePool[emitIndex].moveTime = emitData.moveTime * RandFloat(DTid.x + computeTime + emitIndex, 1.0f, 0.0f);
    
    ParticlePool[emitIndex].color = emitData.startColor;
    ParticlePool[emitIndex].age = 0;
    ParticlePool[emitIndex].totalTime = 0;
    ParticlePool[emitIndex].alive = 3;
    ParticlePool[emitIndex].size = float2(emitData.size, emitData.size);
    ParticlePool[emitIndex].lifeTime = emitData.lifeTime;
    
}