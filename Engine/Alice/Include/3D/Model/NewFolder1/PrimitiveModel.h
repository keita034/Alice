#pragma once
#include"ErrorException.h"
#include"Model.h"

class PrimitiveModel
{
private:
	//キューブのインデックス数と頂点数
	static 	const UINT cubeMaxIndex = 36;
	static const UINT cubeMaxVert = 24;

	//円錐のインデックス数と頂点数
	static const UINT coneMaxIndex = 191;
	static const UINT coneMaxVert = 66;

	//シリンダーのインデックス数と頂点数
	static const UINT cylinderMaxIndex = 384;
	static const UINT cylinderMaxVert = 132;

	//球のインデックス数と頂点数
	static const UINT sphereMaxIndex = 3072;
	static const UINT sphereMaxVert = 560;

public:

	//初期化
	static void Create(ModelShape shape,ModelData* data);

private:

	static void CubeCreate(ModelData* data);

	static void ConeCreate(ModelData* data);

	static void CylinderCreate(ModelData* data);

	static void SphereCreate(ModelData* data);

	PrimitiveModel& operator=(const PrimitiveModel&) = delete;
	PrimitiveModel(const PrimitiveModel&) = delete;
};
