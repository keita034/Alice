#include<BasicGPUParticle.hlsli>

cbuffer timeData : register(b0)
{
    float deltaTime;
    float TotalTime;
}

cbuffer emitData : register(b1)
{
    uint maxParticles;
    uint particleDataCount;
    uint emitCount;
}

cbuffer ParticleDatas : register(b2)
{
    ParticleData particleData;
}

RWStructuredBuffer<Particle> ParticlePool : register(u0);
AppendStructuredBuffer<uint> freeList : register(u1);
RWStructuredBuffer<uint> DrawList : register(u2);

[numthreads(1024, 1, 1)]
void main(uint3 DTid : SV_DispatchThreadID)
{
    if (DTid.x >= (uint) maxParticles)
    {
        return;
    }

    Particle particle = ParticlePool.Load(DTid.x);

    if (particle.Alive == 0.0f)
    {
        return;
    }
    
    float t = particle.Age / particle.lifeTime;
    
    particle.Age += deltaTime;
	
    particle.Alive = (float) (particle.Age < particle.lifeTime);
	
    particle.Position += particle.Velocity * deltaTime;
	
    particle.Velocity += particle.accel;
    
    particle.Color = lerp(particleData.startColor, particleData.endColor, t);
    
    particle.Size = lerp(particleData.scale.x, particleData.scale.y, t);

    ParticlePool[DTid.x] = particle;

    if (particle.Alive == 0.0f)
    {
        freeList.Append(DTid.x);
    }
    else
    {
        uint drawIndex = DrawList.IncrementCounter();

        uint drawData;
        drawData = DTid.x;

        DrawList[drawIndex] = drawData;
    }
}