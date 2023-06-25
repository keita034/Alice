#include"BloomDraw.hlsli"

Texture2D<float4> tex0 : register(t0); // 0�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��
SamplerState smp0 : register(s0); // 0�ԃX���b�g�ɐݒ肳�ꂽ�T���v���[

Texture2D<float4> tex1 : register(t1); // 1�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��
SamplerState smp1 : register(s1); // 1�ԃX���b�g�ɐݒ肳�ꂽ�T���v���[

float4 main(VSOutput input) : SV_TARGET
{
    float4 colortex0 = tex0.Sample(smp0, input.uv);
    float4 colortex1 = tex1.Sample(smp1, input.uv);
    
    float4 color = colortex0 + colortex1;
 
    return float4(color.rgb, 1);
}