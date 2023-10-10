#include<BasicGPUParticle.hlsli>

RWStructuredBuffer<Particle> ParticlePool : register(u0);
RWStructuredBuffer<uint> DrawList : register(u1);

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