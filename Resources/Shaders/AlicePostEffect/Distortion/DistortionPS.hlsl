#include"Distortion.hlsli"

Texture2D<float4> tex : register(t0); // 0�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��
SamplerState smp : register(s0); // 0�ԃX���b�g�ɐݒ肳�ꂽ�T���v���[

float4 main(VSOutput input) : SV_TARGET
{
    float2 pos = input.uv;
    pos -= float2(center.x, center.y);

    float3 v3 = float3(pos.x, pos.y, 0);
    pos *= float2(pow(float2(length(v3), length(v3)), power));
    pos += float2(center.x, center.y);
    
    float3 textureColor = tex.Sample(smp, pos).rgb;

    return float4(textureColor, 1.0);
    
}