#include<FireGPUParticle.hlsli>
#include<../../HLSLMath.hlsli>

AppendStructuredBuffer<uint> freeList : register(u0);
RWStructuredBuffer<uint> DrawCount : register(u1);

cbuffer particleGPUData : register(b0)
{
	uint maxParticles;
	uint emitDataIndex;	
};

[numthreads(32, 1, 1)]
void main( uint3 DTid : SV_DispatchThreadID )
{
    DrawCount[0] = 0;

    if (DTid.x >= maxParticles)
    {
          return;
    }

    freeList.Append(DTid.x);
}