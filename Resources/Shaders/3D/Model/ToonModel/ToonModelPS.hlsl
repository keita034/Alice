#include"ToonModel.hlsli"

Texture2D<float4> tex : register(t0); //0�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��
SamplerState smp : register(s0); //0�ԃX���b�g�ɐݒ肳�ꂽ�T���v���[
Texture2D<float4> ramp : register(t1); //1�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��
SamplerState rampSmp : register(s1); //1�ԃX���b�g�ɐݒ肳�ꂽ�T���v���[

float4 main(VSOutput input) : SV_TARGET
{
    float3 light_diffuse = saturate(dot(lightv, input.normal));
    float3 shade_color;
    shade_color = m_ambient; //�A���r�G���g��
    shade_color += m_diffuse * light_diffuse; //�f�B�t���[�Y��
    //Ramp�e�N�X�`������T���v�����O
    float3 rampCol = ramp.Sample(rampSmp, shade_color.rg).rgb;
    float4 texcolor = tex.Sample(smp, input.uv);
    return float4(texcolor.rgb * rampCol, texcolor.a * m_alpha);
    
    //return float4(texcolor.rgb * shade_color, texcolor.a * m_alpha);
    
    //return tex.Sample(smp, input.uv);
}