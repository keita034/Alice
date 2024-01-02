#include<AnimationModelGPUParticle.hlsli>
#include<../../HLSLMath.hlsli>

AppendStructuredBuffer<uint> freeList : register(u0);
ConsumeStructuredBuffer<uint> DrawList : register(u1);

cbuffer ParticleDatas : register(b0)
{
    EmitData emitData;
}

[numthreads(1024, 1, 1)]
void main(uint3 DTid : SV_DispatchThreadID)
{
    if (DTid.x >= emitData.vertexSize)
    {
        return;
    }
    
    int
    
    uint index = DrawList.Consume();
    freeList.Append(index);
}