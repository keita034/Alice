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

////スキニング計算
//SkinOutput ComputeSkin(Mesh input)
//{
//    //ゼロクリア
//    SkinOutput output = (SkinOutput) 0;

//    uint iBone; //計算するボーン番号
//    float weight; //ボーンウェイト（重み）
//    matrix m; //スキニング行列

//    //ボーン0
//    iBone = input.boneIndex.x;
//    weight = input.boneWeight.x;
//    m = bones[iBone];
//    output.pos += weight * mul(m, input.pos);
//    output.normal += weight * mul((float3x3) m, input.normal);

//    //ボーン1
//    iBone = input.boneIndex.y;
//    weight = input.boneWeight.y;
//    m = bones[iBone];
//    output.pos += weight * mul(m, input.pos);
//    output.normal += weight * mul((float3x3) m, input.normal);

//    //ボーン2
//    iBone = input.boneIndex.z;
//    weight = input.boneWeight.z;
//    m = bones[iBone];
//    output.pos += weight * mul(m, input.pos);
//    output.normal += weight * mul((float3x3) m, input.normal);

//    //ボーン3
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

cbuffer ParticleDatas : register(b0)
{
    EmitData emitData;
}

cbuffer BoneDatas : register(b1)
{
    matrix bones[BONE_MAX];
}

RWStructuredBuffer<ModelParticle> ParticlePool : register(u0);
ConsumeStructuredBuffer<uint> freeList : register(u1);

StructuredBuffer<Mesh> meshs : register(t0);

struct SkinOutput
{
    float4 pos;
    float3 normal;
};

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
    output.normal += weight * mul((float3x3) m, input.normal);

    //ボーン1
    iBone = input.boneIndex.y;
    weight = input.boneWeight.y;
    m = bones[iBone];
    output.pos += weight * mul(m, input.pos);
    output.normal += weight * mul((float3x3) m, input.normal);

    //ボーン2
    iBone = input.boneIndex.z;
    weight = input.boneWeight.z;
    m = bones[iBone];
    output.pos += weight * mul(m, input.pos);
    output.normal += weight * mul((float3x3) m, input.normal);

    //ボーン3
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
    if (DTid.x >= emitData.maxParticles)
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
    
    ModelParticle particle = (ModelParticle) 0;

    ParticlePool[emitIndex].age = 0;
    ParticlePool[emitIndex].alive = 1.0f;
    ParticlePool[emitIndex].color = emitData.startColor;
    ParticlePool[emitIndex].invTime = 0;
    ParticlePool[emitIndex].lifeTime = emitData.lifeTime;
    ParticlePool[emitIndex].meshPos = float3(0, 0, 0);
    ParticlePool[emitIndex].movePos = float3(0, 0, 0);
    ParticlePool[emitIndex].moveTime = 0;
    ParticlePool[emitIndex].pad = float2(0, 0);
    ParticlePool[emitIndex].position = mul(emitData.matWorld, skin.pos);
    ParticlePool[emitIndex].size = float2(emitData.size, emitData.size);
    ParticlePool[emitIndex].threshold = 0;
    ParticlePool[emitIndex].totalTime = 0;

}