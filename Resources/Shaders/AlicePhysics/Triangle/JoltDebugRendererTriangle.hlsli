struct VSOutput
{
    // �V�X�e���p���_���W
    float4 svpos : SV_POSITION;
    // uv�l
    float4 col : COLOR;
};

struct VSInput
{
    float4 pos : POSITION;
    float4 col : COLOR;
};