#include "JoltDebugRendererTriangle.hlsli"

VSOutput main(VSInput input)
{
    VSOutput vsOutput;
    vsOutput.svpos = input.pos;
    vsOutput.col = input.col;
    return vsOutput;
}