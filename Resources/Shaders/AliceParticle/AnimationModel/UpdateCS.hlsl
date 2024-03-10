#include<AnimationModelGPUParticle.hlsli>

cbuffer ParticleDatas : register(b0)
{
    EmitData emitData;
}

cbuffer BoneDatas : register(b1)
{
    matrix bones[BONE_MAX];
}

RWStructuredBuffer<ModelParticle> ParticlePool : register(u0);
AppendStructuredBuffer<uint> DrawList : register(u1);
RWStructuredBuffer<uint> drawListIncrement : register(u2);

//�X�L�j���O�v�Z
SkinOutput ComputeSkin(ModelParticle input)
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
    output.pos += weight * mul(m, float4(input.pos,1));

    //�{�[��1
    iBone = input.boneIndex.y;
    weight = input.boneWeight.y;
    m = bones[iBone];
    output.pos += weight * mul(m, float4(input.pos, 1));

    //�{�[��2
    iBone = input.boneIndex.z;
    weight = input.boneWeight.z;
    m = bones[iBone];
    output.pos += weight * mul(m, float4(input.pos, 1));

    //�{�[��3
    iBone = input.boneIndex.w;
    weight = input.boneWeight.w;
    m = bones[iBone];
    output.pos += weight * mul(m, float4(input.pos, 1));

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
    
    ModelParticle particle = ParticlePool.Load(particleIndex);
 
    SkinOutput skin = (SkinOutput) 0;
    skin = ComputeSkin(particle);

    particle.position = mul(emitData.matWorld, skin.pos);
    
    //particle.color = emitData.startColor;

    ParticlePool[particleIndex] = particle;
    
    if (particle.alive == 1)
    {
        DrawList.Append(particleIndex);
    }
}