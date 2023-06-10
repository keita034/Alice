#include"ToonModel.hlsli"

Texture2D<float4> tex : register(t0); //0�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��
SamplerState smp : register(s0); //0�ԃX���b�g�ɐݒ肳�ꂽ�T���v���[
Texture2D<float4> ramp : register(t1); //1�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��
SamplerState rampSmp : register(s1); //1�ԃX���b�g�ɐݒ肳�ꂽ�T���v���[

float4 main(VSOutput input) : SV_TARGET
{
    //�n�[�t�����o�[�g
    float light_diffuse = (dot(input.normal, lightv));
    light_diffuse = light_diffuse * 0.5f + 0.5f;
    light_diffuse = light_diffuse * light_diffuse;
    
    float4 texcolor = tex.Sample(smp, input.uv);
 
    float4 rampCol = ramp.Sample(rampSmp, float2(light_diffuse, 0.5));
 
    return float4(texcolor.rgb * rampCol.rgb, texcolor.a * m_alpha);
}