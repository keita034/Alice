#include<BasicGPUParticle.hlsli>

cbuffer objectData : register(b0)
{
    matrix view;
    matrix projection;
};

cbuffer timeData : register(b1)
{
    float deltaTime;
    float TotalTime;
}

cbuffer emitData : register(b2)
{
    uint maxParticles;
    uint particleDataCount;
}

RWStructuredBuffer<ParticleData> particleDatas : register(u0);
RWStructuredBuffer<Particle> ParticlePool : register(u1);
AppendStructuredBuffer<uint> freeList : register(u2);
RWStructuredBuffer<uint> DrawList : register(u3);

[numthreads(32, 1, 1)]
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
    
    ParticleData particleData = particleDatas[particle.index];
    
    float t = particle.Age / particleData.lifeTime;
    
    particle.Age += deltaTime;
	
    particle.Alive = (float) (particle.Age < particleDatas[particle.index].lifeTime);
	
    particle.Position += particle.Velocity * deltaTime;
	
    particle.Velocity += particle.accel;
    
    particle.Color = lerp(particleData.startColor, particleData.emitCount, t);
    
    particle.Size = lerp(particleData.scale.x, particleData.scale.y, t);

    ParticlePool[DTid.x] = particle;

	// 新しく死んだ？
    if (particle.Alive == 0.0f)
    {
		// デッドリストに追加
        freeList.Append(DTid.x);
    }
    else
    {
		// ドローリストのカウンターをインクリメントし、返された（インクリメント前の）インデックスに新しいドローデータを置く。
        uint drawIndex = DrawList.IncrementCounter();

		// ドローデータの設定
        uint drawData;
        drawData = DTid.x; // この粒子の実際のインデックス

        DrawList[drawIndex] = drawData;
    }
}