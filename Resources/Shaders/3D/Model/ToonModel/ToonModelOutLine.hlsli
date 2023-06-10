cbuffer cbuff0 : register(b0)
{
    matrix world; //���[���h�s��
    matrix matWorld; //���[���h���W
    float3 cameraPos; //�J�������W(���[���h���W)
};

// ���_�V�F�[�_�[����s�N�Z���V�F�[�_�[�ւ̂����Ɏg�p����\����
struct VSOutput
{
    float4 svpos : SV_POSITION; // �V�X�e���p���_���W
    float4 color : COLOR;
};

struct VSInput
{
    float4 pos : POSITION;
    float3 normal : NORMAL;
    float2 uv : TEXCOORD;
    float3 tangent : TANGENT;
    float4 color : COLOR;
    uint4 index : INDEX;
    float4 weight : WEIGHT;
};