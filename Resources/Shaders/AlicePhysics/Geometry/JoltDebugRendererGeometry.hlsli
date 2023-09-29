struct VSOutput
{
    float4 svpos : SV_POSITION; // �V�X�e���p���_���W
    float4 color : COLOR;
    float3 normal : NORMAL; // �@��
    float2 uv : TEXCOORD; // uv�l
};

//3D�ϊ��s��
cbuffer ConstBufferDataTransform : register(b0)
{
    matrix world; //���[���h�s��
    matrix matWorld; //���[���h���W
    float4 color;
    float isShadow ;
};

cbuffer Light : register(b1)
{
    float3 lightv : packoffset(c0); //���C�g�ւ̕����x�N�g��
    float4 lightcolor : packoffset(c1); //���C�g�̐F(RGB)
};

struct VSInput
{
    // �V�X�e���p���_���W
    float4 pos : POSITION;
    // �@��
    float3 normal : NORMAL;
    //uv�l
    float2 uv : TEXCOORD;
    //�J���[
    float4 col : COLOR;
};
