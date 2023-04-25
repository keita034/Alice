#include "Striped.hlsli"

Texture2D<float4> tex : register(t0); // 0�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��
SamplerState smp : register(s0); // 0�ԃX���b�g�ɐݒ肳�ꂽ�T���v���[

PSOutput main(VSOutput input) : SV_TARGET
{
    PSOutput output;
    
     //�e�N�X�`���̐F�𓾂�
    float4 textureColor = tex.Sample(smp, input.uv);
    
    output.target0 = textureColor;
    
    output.target1 = float4(1 - (textureColor).rgb, 1);

    return output;
}