
struct Col
{
    float4 col;
};

RWStructuredBuffer<Col> col : register(u0);

[numthreads(1, 1, 1)]
void main( uint3 DTid : SV_DispatchThreadID )
{
    col[0].col = float4(1,0,1,1);

}