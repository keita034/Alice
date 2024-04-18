#include"Mosaic.hlsli"

Texture2D<float4> tex : register(t0); // 0�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��
SamplerState smp : register(s0); // 0�ԃX���b�g�ɐݒ肳�ꂽ�T���v���[

float4 main(VSOutput input) : SV_TARGET
{
    float2 pos;
    
    pos.x = floor(input.uv.x * roughness.x) / roughness.x;
    pos.y = floor(input.uv.y * roughness.y) / roughness.y;
    
    float3 textureColor = tex.Sample(smp, pos).rgb;

    return float4(textureColor, 1.0);
}