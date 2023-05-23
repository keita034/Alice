#include "RainParticle.hlsli"

float4 main(GSOutput input) : SV_TARGET
{
    return input.color;
}