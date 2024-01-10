#include<AnimationModelGPUParticle.hlsli>

RWStructuredBuffer<Particle> ParticlePool : register(u0);

VS_OUTPUT main(uint id : SV_InstanceID)
{
    VS_OUTPUT output;

    uint drawIndex = id;
    Particle particle = ParticlePool.Load(drawIndex);

    output.position = particle.position;
    output.size = particle.size;
    output.color = particle.color;
    output.threshold = particle.threshold;

    return output;
}