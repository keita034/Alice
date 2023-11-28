#include<ShockWaveGPUParticle.hlsli>

Texture2D<float4> tex : register(t0); // 0�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��
SamplerState smp : register(s0); // 0�ԃX���b�g�ɐݒ肳�ꂽ�T���v���[

float4 main(GS_OUTPUT input) : SV_TARGET
{
    float4 texCol = tex.Sample(smp, input.uv);
   
    float4 col = input.color * step(input.threshold + 0.0001f, texCol.r);
  
    return col;
}