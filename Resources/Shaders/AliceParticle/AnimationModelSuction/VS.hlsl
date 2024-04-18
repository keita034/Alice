#include<AnimationModelSuction.hlsli>

RWStructuredBuffer<ModelParticle> ParticlePool : register(u0);
ConsumeStructuredBuffer<uint> DrawList : register(u1);

VS_OUTPUT main(uint id : SV_VertexID)
{
    VS_OUTPUT output;

    uint drawIndex = DrawList.Consume();
    ModelParticle particle = ParticlePool.Load(drawIndex);

    output.position = particle.position;
    output.size = particle.size;
    output.color = particle.color;
    output.threshold = particle.threshold;

    return output;
}