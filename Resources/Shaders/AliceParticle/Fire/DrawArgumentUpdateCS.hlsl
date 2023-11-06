#include<FireGPUParticle.hlsli>
#include<../../HLSLMath.hlsli>

RWStructuredBuffer<uint> DrawList : register(u0);
RWStructuredBuffer<IndirectCommand> DrawArgs : register(u1);

[numthreads(1, 1, 1)]
void main( uint3 DTid : SV_DispatchThreadID )
{
    DrawArgs[DTid.x].vertexCountPerInstance = DrawList.IncrementCounter();
    DrawArgs[DTid.x].instanceCount = 1;
    DrawArgs[DTid.x].startVertexLocation = 0;
    DrawArgs[DTid.x].startInstanceLocation = 0;

}