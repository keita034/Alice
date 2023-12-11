static const float PI = 3.14f;
static const float PI2 = 6.28f;

//弧度法から度数法に
float RadianToAngle(float RADIAN)
{
    return RADIAN * (180.0f / PI);
}

//度数法から弧度法に
float AngleToRadian(float ANGLE)
{
    float radian = ANGLE * (PI / 180.0f);
    return radian;
}

//平行移動行列
matrix Translate(float3 VECTOR)
{
    matrix matTrans;
    matTrans[0] = float4(1.0f, 0.0f, 0.0f, VECTOR.x);
    matTrans[1] = float4(0.0f, 1.0f, 0.0f, VECTOR.y);
    matTrans[2] = float4(0.0f, 0.0f, 1.0f, VECTOR.z);
    matTrans[3] = float4(0.0f, 0.0f, 0.0f, 1.0f);
    return matTrans;
}

//スケール行列
matrix Scale(float3 VECTOR)
{
    matrix matScale;
    matScale[0] = float4(VECTOR.x, 0.0f, 0.0f, 0.0f);
    matScale[1] = float4(0.0f, VECTOR.y, 0.0f, 0.0f);
    matScale[2] = float4(0.0f, 0.0f, VECTOR.z, 0.0f);
    matScale[3] = float4(0.0f, 0.0f, 0.0f, 1.0f);
    return matScale;
}

//X軸回転行列
matrix RotateX(float ANGLE)
{
    float lsin = sin(AngleToRadian(ANGLE));
    float lcos = cos(AngleToRadian(ANGLE));

    matrix matRotaX;
    matRotaX[0] = float4(1.0f, 0.0f, 0.0f, 0.0f);
    matRotaX[1] = float4(0.0f, lcos, lsin, 0.0f);
    matRotaX[2] = float4(0.0f, -lsin, lcos, 0.0f);
    matRotaX[3] = float4(0.0f, 0.0f, 0.0f, 1.0f);
    return matRotaX;
}

//Y軸回転行列
matrix RotateY(float ANGLE)
{
    float lsin = sin(AngleToRadian(ANGLE));
    float lcos = cos(AngleToRadian(ANGLE));
    
    matrix matRotaY;
    matRotaY[0] = float4(lcos, 0.0f, -lsin, 0.0f);
    matRotaY[1] = float4(0.0f, 1.0f, 0.0f, 0.0f);
    matRotaY[2] = float4(lsin, 0.0f, lcos, 0.0f);
    matRotaY[3] = float4(0.0f, 0.0f, 0.0f, 1.0f);
    return matRotaY;
}

//Z軸回転行列
matrix RotateZ(float ANGLE)
{
    float lsin = sin(AngleToRadian(ANGLE));
    float lcos = cos(AngleToRadian(ANGLE));
    
    matrix matRotaZ;
    matRotaZ[0] = float4(lcos, lsin, 0.0f, 0.0f);
    matRotaZ[1] = float4(-lsin, lcos, 0.0f, 0.0f);
    matRotaZ[2] = float4(0.0f, 0.0f, 1.0f, 0.0f);
    matRotaZ[3] = float4(0.0f, 0.0f, 0.0f, 1.0f);
    return matRotaZ;
}

//単位行列
matrix MatrixIdentity()
{
    matrix matIdentity;
    matIdentity[0] = float4(1.0f, 0.0f, 0.0f, 0.0f);
    matIdentity[1] = float4(0.0f, 1.0f, 0.0f, 0.0f);
    matIdentity[2] = float4(0.0f, 0.0f, 1.0f, 0.0f);
    matIdentity[3] = float4(0.0f, 0.0f, 0.0f, 1.0f);
    return matIdentity;
}

//回転行列
matrix Rotate(float3 ANGLE)
{
    matrix matRot = MatrixIdentity();
    matRot = mul(RotateZ(ANGLE.z), matRot);
    matRot = mul(RotateX(ANGLE.x), matRot);
    matRot = mul(RotateY(ANGLE.y), matRot);
    return matRot;
}

//ランダム
float Rand(int SEED, int SEED2, int MAX, int MIN)
{
    float rand = frac(sin(dot(float2(SEED, SEED), float2(12.9898, 78.233))) * 43758.5453);
    return clamp(rand, MIN, MAX);
}

uint WangHashRand(uint seed)
{
    seed = (seed ^ 61) ^ (seed >> 16);
    seed *= 9;
    seed = seed ^ (seed >> 4);
    seed *= 0x27d4eb2d;
    seed = seed ^ (seed >> 15);
    return seed;
}

float3 RandVec3(uint SEED, float MAX, float MIN)
{
    uint rand = WangHashRand(SEED * 1847483629);
    float3 result;
    result.x = (rand % 1024) / 1024.0f;
    rand /= 1024;
    result.y = (rand % 1024) / 1024.0f;
    rand /= 1024;
    result.z = (rand % 1024) / 1024.0f;

    result.x = (MAX + abs(MIN)) * result.x - abs(MIN);
    result.y = (MAX + abs(MIN)) * result.y - abs(MIN);
    result.z = (MAX + abs(MIN)) * result.z - abs(MIN);

    if (result.x <= MIN)
    {
        result.x = MIN;
    }
    if (result.y <= MIN)
    {
        result.y = MIN;
    }
    if (result.z <= MIN)
    {
        result.z = MIN;
    }
    return result;
}

float RandFloat(uint SEED, float MAX, float MIN)
{
    uint rand = WangHashRand(SEED * 1847483629);
    float result;
    result = (rand % 1024) / 1024.0f;
    rand /= 1024;

    result = (MAX + abs(MIN)) * result - abs(MIN);

    if (result.x <= MIN)
    {
        result.x = MIN;
    }

    return result;
}

float3 RandomPointInUnitSphere(uint SEED)
{
    float3 rand = RandVec3(SEED, 1.0f, 0.0f);

    float theta = rand.x * 2 * 3.14159;
    float phi = rand.y * 3.14159;
    float r = pow(rand.z, 1.0 / 3.0);

    float3 randomPoint;
    randomPoint.x = r * sin(phi) * cos(theta);
    randomPoint.y = r * cos(phi);
    randomPoint.z = r * sin(phi) * sin(theta);

    return randomPoint;
}

float3 RandomPointInUnitSphere(uint SEED,float RADIUS)
{
    float3 rand = RandVec3(SEED, 1.0f, 0.0f);

    float theta = rand.x * 2 * 3.14159;
    float phi = rand.y * 3.14159;
    float r = pow(rand.z, 1.0 / 3.0) * RADIUS;

    float3 randomPoint;
    randomPoint.x = r * sin(phi) * cos(theta);
    randomPoint.y = r * cos(phi);
    randomPoint.z = r * sin(phi) * sin(theta);

    return randomPoint;
}

float3 RandomPointInUnitSphere(uint SEED, float3 RADIUS)
{
    float3 rand = RandVec3(SEED, 1.0f, 0.0f);

    float theta = rand.x * 2 * 3.14159;
    float phi = rand.y * 3.14159;
    float rX = pow(rand.z, 1.0 / 3.0) * RADIUS.x;
    float rY = pow(rand.z, 1.0 / 3.0) * RADIUS.y;
    float rZ = pow(rand.z, 1.0 / 3.0) * RADIUS.z;

    float3 randomPoint;
    randomPoint.x = rX * sin(phi) * cos(theta);
    randomPoint.y = rY * cos(phi);
    randomPoint.z = rZ * sin(phi) * sin(theta);

    return randomPoint;
}