#include"ChromaticAberration.hlsli"

Texture2D<float4> tex : register(t0); // 0�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��
SamplerState smp : register(s0); // 0�ԃX���b�g�ɐݒ肳�ꂽ�T���v���[

float4 main(VSOutput input) : SV_TARGET
{
    float4 col = tex.Sample(smp, input.uv);

    float2 uv = input.uv - 0.5;

    half2 uvR = uv * (1.0 - size * 2.0) + 0.5;
    col.r = tex.Sample(smp, uvR).r;
    
    half2 uvG = uv * (1.0h - size) + 0.5h;
    col.g = tex.Sample(smp, uvG).g;
 
    return col;
}