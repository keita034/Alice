#include<BloodGush.hlsli>
#include<../../HLSLMath.hlsli>

AppendStructuredBuffer<uint> freeList : register(u0);

cbuffer particleGPUData : register(b0)
{
	uint maxParticles;
	uint emitDataIndex;	
};

[numthreads(1024, 1, 1)]
void main( uint3 DTid : SV_DispatchThreadID )
{
    if (DTid.x >= maxParticles)
    {
          return;
    }

    freeList.Append(DTid.x);
}