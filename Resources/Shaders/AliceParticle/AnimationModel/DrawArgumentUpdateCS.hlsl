#include<AnimationModelGPUParticle.hlsli>
#include<../../HLSLMath.hlsli>

cbuffer ParticleDatas : register(b0)
{
    EmitData emitData;
}

RWStructuredBuffer<IndirectCommand> DrawArgs : register(u0);

[numthreads(1, 1, 1)]
void main( uint3 DTid : SV_DispatchThreadID )
{
    DrawArgs[0].vertexCountPerInstance = 1;
    DrawArgs[0].instanceCount = emitData.vertexSize;
    DrawArgs[0].startVertexLocation = 0;
    DrawArgs[0].startInstanceLocation = 0;

}