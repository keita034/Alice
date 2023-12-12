#include<MeshGPUParticle.hlsli>
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
    EmitData emitDatas[EMIT_DATA_MAX];
}

cbuffer BoneDatas : register(b3)
{
    matrix bones[BONE_MAX];
}

Texture2D<float4> tex : register(t0);
StructuredBuffer<Mesh> meshs : register(t1);
StructuredBuffer<uint> indices : register(t1);

RWStructuredBuffer<Particle> ParticlePool : register(u0);
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
void main( uint3 DTid : SV_DispatchThreadID )
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
    
    EmitData emitData = emitDatas[emitDataIndex];

    uint emitIndex = freeList.Consume();
    
    SkinOutput skin = (SkinOutput) 0;
    skin = ComputeSkin(meshs[DTid.x]);

    ParticlePool[emitIndex].position = mul(emitData.matWorld, skin.pos);
    ParticlePool[emitIndex].velocity = emitData.velocity * emitData.speed;
    ParticlePool[emitIndex].color = emitData.startColor;
    ParticlePool[emitIndex].age = 0;
    ParticlePool[emitIndex].size = float2(emitData.size, emitData.size);
    ParticlePool[emitIndex].alive = 1.0f;
    ParticlePool[emitIndex].index = emitDataIndex;
    ParticlePool[emitIndex].lifeTime = emitData.lifeTime;
    
}