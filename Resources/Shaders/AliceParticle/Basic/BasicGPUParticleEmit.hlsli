
struct Particle
{
    float4 Color;
    float3 Position;
    float Age;
    float3 Velocity;
    float Size;
    float Alive;
    float3 Padding;
};

struct ParticleData
{
    float3 startPosition;
    float4 startColor;
    float4 endColor;
    float3 velocity;
    float lifeTime;
    float3 acceleration;
    int emitCount;
    int maxParticles;
};