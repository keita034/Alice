#include<AnimationModelGPUParticle.hlsli>

RWStructuredBuffer<ModelParticle> ParticlePool : register(u0);

ConsumeStructuredBuffer<uint> DrawList : register(u1);
RWStructuredBuffer<uint> DrawListdecrement : register(u2);

VS_OUTPUT main(uint id : SV_InstanceID)
{
    VS_OUTPUT output;

    uint drawIndex = DrawList.Consume();
    ModelParticle particle = ParticlePool.Load(drawIndex);
    output.position = particle.position;

    output.color = float4(particle.color, 1);
    output.threshold = 0.0f;

    if (particle.alive == 1)
    {
        output.size = float2(particle.size, particle.size);
    }
    else
    {
        output.size = float2(0, 0);
    }
    
    return output;
}