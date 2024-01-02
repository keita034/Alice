#include<AnimationModelGPUParticle.hlsli>

RWStructuredBuffer<Particle> ParticlePool : register(u0);
RWStructuredBuffer<uint> DrawList : register(u1);

VS_OUTPUT main(uint id : SV_InstanceID)
{
    VS_OUTPUT output;

    uint drawIndex = DrawList[id];
    Particle particle = ParticlePool.Load(drawIndex);

    output.position = particle.position;
    output.size = particle.size;
    output.color = particle.color;
    output.threshold = particle.threshold;

    return output;
}