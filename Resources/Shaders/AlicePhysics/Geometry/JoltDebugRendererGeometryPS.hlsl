#include "JoltDebugRendererGeometry.hlsli"

//Texture2D<float4> tex : register(t0); //0�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��
//SamplerState smp : register(s0); //0�ԃX���b�g�ɐݒ肳�ꂽ�T���v���[

float4 main(VSOutput input) : SV_TARGET
{
    if (isShadow >= 0.0f)
    {
         // �e�N�X�`���}�b�s���O
        //float4 texcolor = tex.Sample(smp, input.uv);

        float3 light_diffuse = saturate(dot(lightv, input.normal));
        float3 shade_color;
        shade_color = float3(0.4f, 0.4f, 0.4f); //�A���r�G���g��
        shade_color += float3(0.8f, 0.8f, 0.8f) * light_diffuse; //�f�B�t���[�Y��
    
        return float4(shade_color, 1.0f) * color;
    }
    else
    {
        return color;
    }

    
}