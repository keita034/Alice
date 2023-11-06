#pragma once

#include<Defined.h>

ALICE_SUPPRESS_WARNINGS_BEGIN

#include<directx/d3dx12.h>

ALICE_SUPPRESS_WARNINGS_END

#include<BlendMode.h>

D3D12_BLEND_DESC CreateBlend(BlendMode mode_);
