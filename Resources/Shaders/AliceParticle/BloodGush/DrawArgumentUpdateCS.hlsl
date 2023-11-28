#include<BloodGush.hlsli>
#include<../../HLSLMath.hlsli>

RWStructuredBuffer<uint> DrawList : register(u0);
RWStructuredBuffer<IndirectCommand> DrawArgs : register(u1);

[numthreads(1, 1, 1)]
void main( uint3 DTid : SV_DispatchThreadID )
{
    DrawArgs[0].vertexCountPerInstance = DrawList.IncrementCounter();
    DrawArgs[0].instanceCount = 1;
    DrawArgs[0].startVertexLocation = 0;
    DrawArgs[0].startInstanceLocation = 0;

}