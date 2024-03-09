#include<AnimationModelGPUParticle.hlsli>
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

StructuredBuffer<Mesh> meshs : register(t0);

RWStructuredBuffer<ModelParticle> ParticlePool : register(u0);

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
    if (DTid.x >= maxParticles)
    {
       return;
    }

    if (DTid.x >= emitData.vertexSize)
    {
        return;
    }
    
    uint emitIndex = DTid.x;
    
    SkinOutput skin = (SkinOutput) 0;
    skin = ComputeSkin(meshs[DTid.x]);

    ParticlePool[emitIndex].position = mul(emitData.matWorld, skin.pos);
    ParticlePool[emitIndex].color = emitData.startColor;
    ParticlePool[emitIndex].size = float2(emitData.size, emitData.size);
    ParticlePool[emitIndex].alive = 1.0f;
    ParticlePool[emitIndex].meshIndex = DTid.x;
    ParticlePool[emitIndex].boneIndex = meshs[DTid.x].boneIndex;
    ParticlePool[emitIndex].boneWeight = meshs[DTid.x].boneWeight;
    ParticlePool[emitIndex].pos = meshs[DTid.x].pos.xyz;
    
}