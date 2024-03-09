#include<AnimationModelGPUParticle.hlsli>

RWStructuredBuffer<ModelParticle> ParticlePool : register(u0);

VS_OUTPUT main(uint id : SV_InstanceID)
{
    VS_OUTPUT output;

    uint drawIndex = id;
    ModelParticle particle = ParticlePool.Load(drawIndex);

    output.position = particle.position;
    output.size = particle.size;
    output.color = particle.color;
    output.threshold = 0.0f;

    return output;
}