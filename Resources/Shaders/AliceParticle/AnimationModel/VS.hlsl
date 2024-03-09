#include<AnimationModelGPUParticle.hlsli>

RWStructuredBuffer<ModelParticle> ParticlePool : register(u0);

VS_OUTPUT main(uint id : SV_InstanceID)
{
    VS_OUTPUT output;

    uint drawIndex = id;
    ModelParticle particle = ParticlePool.Load(drawIndex);

    output.position = particle.position;
    output.size = float2(particle.size, particle.size);
    output.color = float4(particle.color,1);
    output.threshold = 0.0f;

    return output;
}