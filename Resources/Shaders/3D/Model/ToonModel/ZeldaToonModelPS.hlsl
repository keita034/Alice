#include"ZeldaToonModel.hlsli"

Texture2D<float4> tex : register(t0); //0�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��
SamplerState smp : register(s0); //0�ԃX���b�g�ɐݒ肳�ꂽ�T���v���[

float4 main(VSOutput input) : SV_TARGET
{
    float NdotL = dot(lightv, input.normal);

    float lightIntensity = smoothstep(0, 0.01, NdotL );

    float4 light = lightIntensity * lightcolor;

    float3 halfVector = normalize(lightv + input.eyeDir);
    float NdotH = dot(input.normal, halfVector);
    
    float specularIntensity = pow(NdotH * lightIntensity, 32 * 32);
    float specularIntensitySmooth = smoothstep(0.005, 0.01, specularIntensity);
    float4 specular = specularIntensitySmooth * lightcolor;

    float rimDot = 1 - dot(input.eyeDir, input.normal);

    float rimIntensity = rimDot * pow(NdotL, 0.1);
    rimIntensity = smoothstep(0.716 - 0.01, 0.716 + 0.01, rimIntensity);
    float4 rim = rimIntensity * lightcolor;

    float4 sample = tex.Sample(smp, input.uv);
    sample.a *= m_alpha;
    float4 ambient = float4(m_ambient,1);

    return (light + ambient + specular + rim) * sample;
}