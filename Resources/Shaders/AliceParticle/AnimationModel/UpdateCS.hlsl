#include<AnimationModelGPUParticle.hlsli>

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


RWStructuredBuffer<Particle> ParticlePool : register(u0);

StructuredBuffer<Mesh> meshs : register(t0);

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
    if (DTid.x >= (uint) emitData.vertexSize)
    {
        return;
    }

    uint particleIndex = DTid.x;
    
    Particle particle = ParticlePool.Load(particleIndex);
 
    SkinOutput skin = (SkinOutput) 0;
    skin = ComputeSkin(meshs[particle.index]);

    particle.position = mul(emitData.matWorld, skin.pos);
    
    particle.color = emitData.startColor;

    ParticlePool[particleIndex] = particle;
}