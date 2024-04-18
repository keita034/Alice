
float3 Float3Lerp(float3 src1, float3 src2, float t)
{
    float3 tmp;

    tmp.x = src1.x + (src2.x - src1.x) * t;
    tmp.y = src1.y + (src2.y - src1.y) * t;
    tmp.z = src1.z + (src2.z - src1.z) * t;

    return tmp;
}