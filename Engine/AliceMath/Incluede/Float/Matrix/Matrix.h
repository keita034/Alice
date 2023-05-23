#pragma once
#include"Matrix4.h"
#include"Matrix4x3.h"
#include"Matrix4x2.h"

#include"Matrix3x4.h"
#include"Matrix3.h"
#include"Matrix3x2.h"

#include"Matrix2x4.h"
#include"Matrix2x3.h"
#include"Matrix2.h"

#include"MatrixT.h"

namespace AliceMathF
{
	Vector3 GetWorldPosition(const AliceMathF::Matrix4& worldMat);
}

