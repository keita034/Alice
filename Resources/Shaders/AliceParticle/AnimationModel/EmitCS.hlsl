#include<AnimationModelGPUParticle.hlsli>
#include<../../HLSLMath.hlsli>

cbuffer ParticleDatas : register(b0)
{
    EmitData emitData;
}

cbuffer BoneDatas : register(b1)
{
    matrix bones[BONE_MAX];
}

StructuredBuffer<Mesh> meshs : register(t0);

RWStructuredBuffer<ModelParticle> ParticlePool : register(u0);
ConsumeStructuredBuffer<uint> freeList : register(u1);

//スキニング計算
SkinOutput ComputeSkin(Mesh input)
{
    //ゼロクリア
    SkinOutput output = (SkinOutput) 0;

    uint iBone; //計算するボーン番号
    float weight; //ボーンウェイト（重み）
    matrix m; //スキニング行列

    //ボーン0
    iBone = input.boneIndex.x;
    weight = input.boneWeight.x;
    m = bones[iBone];
    output.pos += weight * mul(m, input.pos);

    //ボーン1
    iBone = input.boneIndex.y;
    weight = input.boneWeight.y;
    m = bones[iBone];
    output.pos += weight * mul(m, input.pos);

    //ボーン2
    iBone = input.boneIndex.z;
    weight = input.boneWeight.z;
    m = bones[iBone];
    output.pos += weight * mul(m, input.pos);

    //ボーン3
    iBone = input.boneIndex.w;
    weight = input.boneWeight.w;
    m = bones[iBone];
    output.pos += weight * mul(m, input.pos);

    return output;
}

[numthreads(1024, 1, 1)]
void main( uint3 DTid : SV_DispatchThreadID )
{
    if (DTid.x >= emitData.vertexSize)
    {
        return;
    }
    
    uint emitIndex = freeList.Consume();
    
    SkinOutput skin = (SkinOutput) 0;
    skin = ComputeSkin(meshs[DTid.x]);

    ParticlePool[emitIndex].position = mul(emitData.matWorld, skin.pos).xyz;
    ParticlePool[emitIndex].color = emitData.startColor.xyz;
    ParticlePool[emitIndex].size = emitData.size;
    ParticlePool[emitIndex].alive = 1.0f;
    ParticlePool[emitIndex].boneIndex = meshs[DTid.x].boneIndex;
    ParticlePool[emitIndex].boneWeight = meshs[DTid.x].boneWeight;
    ParticlePool[emitIndex].pos = meshs[DTid.x].pos.xyz;
    ParticlePool[emitIndex].index = emitData.meshIndex;
    
}