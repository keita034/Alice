#include<BasicGPUParticle.hlsli>

StructuredBuffer<Particle> ParticlePool : register(t0);
StructuredBuffer<uint> DrawList : register(t1);

VS_OUTPUT main(uint id : SV_VertexID)
{
    VS_OUTPUT output;

    uint drawIndex = DrawList.Load(id);
    Particle particle = ParticlePool.Load(drawIndex);

    output.position = particle.Position;
    output.size = particle.Size;
    output.color = particle.Color;

    return output;
}