// ���_�V�F�[�_�[����s�N�Z���V�F�[�_�[�ւ̂����Ɏg�p����\����
struct VSOutput
{
    float4 svpos : SV_POSITION; // �V�X�e���p���_���W
    float2 uv : TEXCOORD; // uv�l
    float4 color : COLOR; // �F(RGBA)
};

struct PSOutput
{
    float4 target0 : SV_TARGET0; // �V�X�e���p���_���W
    float4 target1 : SV_TARGET1; // �V�X�e���p���_���W
};