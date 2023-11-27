#include<LaserGPUParticle.hlsli>

Texture2D<float4> mainTex : register(t0); // 0�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��
SamplerState mainSmp : register(s0); // 0�ԃX���b�g�ɐݒ肳�ꂽ�T���v���[

Texture2D<float4> subTex : register(t0); // 0�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��
SamplerState subSmp : register(s0); // 0�ԃX���b�g�ɐݒ肳�ꂽ�T���v���[

float4 main(GS_OUTPUT input) : SV_TARGET
{
    float4 texCol = mainTex.Sample(mainSmp, input.uv);
   
    float3 col = input.color.rgb * texCol.rgb;
    float a = input.color.a * texCol.a;
  
    return float4(col,a);
}