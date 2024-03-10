#include<AnimationModelGPUParticle.hlsli>
#include<../../HLSLMath.hlsli>


RWStructuredBuffer<uint> drawList : register(u0);
RWStructuredBuffer<IndirectCommand> DrawArgs : register(u1);

[numthreads(1, 1, 1)]
void main( uint3 DTid : SV_DispatchThreadID )
{
    DrawArgs[0].vertexCountPerInstance = drawList.IncrementCounter();
    DrawArgs[0].instanceCount = 1;
    DrawArgs[0].startVertexLocation = 0;
    DrawArgs[0].startInstanceLocation = 0;
}