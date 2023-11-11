#include<FireGPUParticle.hlsli>

RWStructuredBuffer<Particle> ParticlePool : register(u0);
ConsumeStructuredBuffer<uint> DrawList : register(u1);
RWStructuredBuffer<uint> DrawCount : register(u2);

VS_OUTPUT main(uint id : SV_VertexID)
{
    VS_OUTPUT output;

    uint drawIndex = DrawList.Consume();
    Particle particle = ParticlePool.Load(drawIndex);

    output.position = particle.position;
    output.size = particle.size;
    output.color = particle.color;
    output.threshold = particle.threshold;

    DrawCount[0] = 0;
    return output;
}