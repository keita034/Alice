#include<ShockWaveGPUParticle.hlsli>
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
    if (DTid.x >= maxParticles)
    {
          return;
    }

    DrawCount[0]=0;
    freeList.Append(DTid.x);
}