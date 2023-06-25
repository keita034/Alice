#include"CRT.hlsli"

Texture2D<float4> tex : register(t0); // 0�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��
SamplerState smp : register(s0); // 0�ԃX���b�g�ɐݒ肳�ꂽ�T���v���[

float4 main(VSOutput input) : SV_TARGET
{
    float3 textureColor = tex.Sample(smp, input.uv).rgb;

    textureColor -= abs(sin(input.uv.y * 100.0 + time * 5.0)) * 0.08;
    textureColor -= abs(sin(input.uv.y * 300.0 - time * 10.0)) * 0.05;
    return float4(textureColor, 1.0);
    
}