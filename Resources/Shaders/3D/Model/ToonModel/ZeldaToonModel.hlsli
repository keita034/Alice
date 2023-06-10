cbuffer cbuff0 : register(b0)
{
    matrix world; //���[���h�s��
    matrix matWorld; //���[���h���W
    float3 cameraPos; //�J�������W(���[���h���W)
};

cbuffer material : register(b1)
{
    float3 m_ambient : packoffset(c0); //�A���r�G���g�W��
    float3 m_diffuse : packoffset(c1); //�f�B�t���[�Y�W��
    float3 m_specular : packoffset(c2); //�X�y�L�����[�W��
    float3 emission : packoffset(c3); //�G�~�b�V�����W��
    float shininess : packoffset(c4.x); //����
    float m_alpha : packoffset(c4.y); //�A���t�@
};

cbuffer cbuff2 : register(b2)
{
    float3 lightv : packoffset(c0); //���C�g�ւ̕����x�N�g��
    float4 lightcolor : packoffset(c1); //���C�g�̐F(RGB)
};



// ���_�V�F�[�_�[����s�N�Z���V�F�[�_�[�ւ̂����Ɏg�p����\����
struct VSOutput
{
    float4 svpos : SV_POSITION; // �V�X�e���p���_���W
    float3 eyeDir : EYEDIR;
    float3 normal : NORMAL; // �@��
    float2 uv : TEXCOORD; // uv�l
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