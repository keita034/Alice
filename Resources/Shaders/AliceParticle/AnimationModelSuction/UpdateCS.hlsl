//#include<AnimationModelSuction.hlsli>

//cbuffer ParticleDatas : register(b0)
//{
//    EmitData emitData;
//}

//RWStructuredBuffer<ModelParticle> ParticlePool : register(u0);
//AppendStructuredBuffer<uint> freeList : register(u1);
//AppendStructuredBuffer<uint> DrawList : register(u2);

//[numthreads(1024, 1, 1)]
//void main(uint3 DTid : SV_DispatchThreadID)
//{
//    if (DTid.x >= (uint) emitData.maxParticles)
//    {
//        return;
//    }

//    ModelParticle particle = ParticlePool.Load(DTid.x);
    
//    particle.age += emitData.deltaTime;
//    particle.totalTime += emitData.deltaTime;
//    particle.threshold = particle.totalTime / particle.lifeTime;
//    particle.color = lerp(emitData.startColor, emitData.endColor, particle.threshold);
    
//    if (particle.totalTime > particle.lifeTime)
//    {
//        ParticlePool[DTid.x] = particle;
//        particle.alive = 0;
//        freeList.Append(DTid.x);
//    }

//    if (particle.alive == 0.0f)
//    {
//        return;
//    }
    
//    if (particle.alive == 3.0f)
//    {
//        if (particle.age > particle.invTime)
//        {
//            particle.alive = 2.0f;
//            particle.age = 0.0;
//        }
        
//        particle.threshold = 0.0f;

//    }
//    else if (particle.alive == 2.0f)
//    {
//        float t = particle.age / particle.moveTime;
        
//        particle.position = lerp(particle.movePos, particle.meshPos, t);
        
//        if (particle.age > particle.invTime)
//        {
//            particle.position = particle.meshPos;
//            particle.alive = 1.0f;
//            particle.age = 0.0;
//        }
        
//        DrawList.Append(DTid.x);
//    }
//    else
//    {
//        DrawList.Append(DTid.x);
//    }
   

//    ParticlePool[DTid.x] = particle;


//}

#include<AnimationModelSuction.hlsli>

cbuffer ParticleDatas : register(b0)
{
    EmitData emitData;
}

RWStructuredBuffer<ModelParticle> ParticlePool : register(u0);
AppendStructuredBuffer<uint> freeList : register(u1);
AppendStructuredBuffer<uint> DrawList : register(u2);

[numthreads(1024, 1, 1)]
void main(uint3 DTid : SV_DispatchThreadID)
{
    if (DTid.x >= (uint) emitData.maxParticles)
    {
        return;
    }

    ModelParticle particle = ParticlePool.Load(DTid.x);
    
    float t = particle.age / emitData.lifeTime;
    
    particle.age += emitData.deltaTime;

    particle.alive = (float) (particle.age < particle.lifeTime);

    particle.color = lerp(emitData.startColor, emitData.endColor, t);
    
    particle.threshold = t;

    ParticlePool[DTid.x] = particle;

    if (particle.alive == 0.0f)
    {
        freeList.Append(DTid.x);
    }
    else
    {

        DrawList.Append(DTid.x);
    }
    ParticlePool[DTid.x] = particle;


}