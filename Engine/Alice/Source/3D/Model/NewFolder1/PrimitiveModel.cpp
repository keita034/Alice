#include "PrimitiveModel.h"
#include"DefaultMaterial.h"

void PrimitiveModel::Create(ModelShape shape, ModelData* data)
{

	switch (shape)
	{
	case ModelShape::Cube:
		data->maxIndex = cubeMaxIndex;
		data->maxVert = cubeMaxVert;

		//頂点バッファ・インデックス生成
		data->vertexBuffer = std::make_unique<ComputeVertexBuffer>();
		data->vertexBuffer->Create(data->maxVert, sizeof(PosNormalUv));

		data->indexBuffer = std::make_unique<IndexBuffer>();
		data->indexBuffer->Create(data->maxIndex);
		CubeCreate(data);
		break;

	case ModelShape::Sphere:
		data->maxIndex = sphereMaxIndex;
		data->maxVert = sphereMaxVert;

		//頂点バッファ・インデックス生成
		data->vertexBuffer = std::make_unique<ComputeVertexBuffer>();
		data->vertexBuffer->Create(data->maxVert, sizeof(PosNormalUv));

		data->indexBuffer = std::make_unique<IndexBuffer>();
		data->indexBuffer->Create(data->maxIndex);

		SphereCreate(data);
		break;

	case ModelShape::Capsule:
		break;

	case ModelShape::Cylinder:
		data->maxIndex = cylinderMaxIndex;
		data->maxVert = cylinderMaxVert;

		//頂点バッファ・インデックス生成
		data->vertexBuffer = std::make_unique<ComputeVertexBuffer>();
		data->vertexBuffer->Create(data->maxVert, sizeof(PosNormalUv));

		data->indexBuffer = std::make_unique<IndexBuffer>();
		data->indexBuffer->Create(data->maxIndex);

		CylinderCreate(data);
		break;

	case ModelShape::Cone:
		data->maxIndex = coneMaxIndex;
		data->maxVert = coneMaxVert;

		//頂点バッファ・インデックス生成
		data->vertexBuffer = std::make_unique<ComputeVertexBuffer>();
		data->vertexBuffer->Create(data->maxVert, sizeof(PosNormalUv));

		data->indexBuffer = std::make_unique<IndexBuffer>();
		data->indexBuffer->Create(data->maxIndex);

		ConeCreate(data);
		break;

	default:
		break;
	}

	data->constBuffVelocity = std::make_unique<ConstantBuffer>();
	data->constBuffVelocity->Create(sizeof(float));
	float zero = 0.0f;
	data->constBuffVelocity->Update(&zero);

	data->textureData = DefaultMaterial::GetDefaultMaterial()->DEFAULT_TEXTURE;
}

void PrimitiveModel::CubeCreate(ModelData* data)
{
	//定数バッファ生成(マテリアル)
	data->constBuffMaterial = std::make_unique<ConstantBuffer>();
	data->constBuffMaterial->Create(sizeof(ConstBuffDataMaterial));

	AliceMathF::Vector3 one = { 0.8f,0.8f,0.8f };
	data->modelMaterial.ambient = one;
	data->modelMaterial.diffuse = one;
	data->modelMaterial.specular = one;
	data->modelMaterial.alpha = data->modelMaterial.alpha;
	data->constBuffMaterial->Update(&data->modelMaterial);

	// 頂点データ
	//正面
	PosNormalUv vertexInf = { { -1.0f, -1.0f, -1.0f }, {}, { 0.0f, 1.0f } };//左上インデックス0
	data->vertices.push_back(vertexInf);
	vertexInf = { {-1.0f, 1.0f,-1.0f},{},{ 0.0f, 0.0f} };//左下インデックス1
	data->vertices.push_back(vertexInf);
	vertexInf = { { 1.0f,-1.0f,-1.0f},{},{ 1.0f, 1.0f} };//右上インデックス2
	data->vertices.push_back(vertexInf);
	vertexInf = { { 1.0f, 1.0f,-1.0f},{},{ 1.0f, 0.0f} };//右下インデックス3
	data->vertices.push_back(vertexInf);

	//右
	vertexInf = { { 1.0f,-1.0f,-1.0f},{},{ 0.0f, 1.0f} };//左上インデックス4
	data->vertices.push_back(vertexInf);
	vertexInf = { { 1.0f, 1.0f,-1.0f},{},{ 0.0f, 0.0f} };//左下インデックス5
	data->vertices.push_back(vertexInf);
	vertexInf = { { 1.0f,-1.0f, 1.0f},{},{ 1.0f, 1.0f} };//右上インデックス6
	data->vertices.push_back(vertexInf);
	vertexInf = { { 1.0f, 1.0f, 1.0f},{},{ 1.0f, 0.0f} };//右下インデックス7
	data->vertices.push_back(vertexInf);

	//左
	vertexInf = { {-1.0f,-1.0f, 1.0f},{},{ 0.0f, 1.0f} };//左上インデックス8
	data->vertices.push_back(vertexInf);
	vertexInf = { {-1.0f, 1.0f, 1.0f},{},{ 0.0f, 0.0f} };//左下インデックス9
	data->vertices.push_back(vertexInf);
	vertexInf = { {-1.0f,-1.0f,-1.0f},{},{ 1.0f, 1.0f} };//右上インデックス10
	data->vertices.push_back(vertexInf);
	vertexInf = { {-1.0f, 1.0f,-1.0f},{},{ 1.0f, 0.0f} };//右下インデックス11
	data->vertices.push_back(vertexInf);

	//裏
	vertexInf = { { 1.0f,-1.0f, 1.0f},{},{ 0.0f, 1.0f} };//左上インデックス12
	data->vertices.push_back(vertexInf);
	vertexInf = { { 1.0f, 1.0f, 1.0f},{},{ 0.0f, 0.0f} };//左下インデックス13
	data->vertices.push_back(vertexInf);
	vertexInf = { {-1.0f,-1.0f, 1.0f},{},{ 1.0f, 1.0f} };//右上インデックス14
	data->vertices.push_back(vertexInf);
	vertexInf = { {-1.0f, 1.0f, 1.0f},{},{ 1.0f, 0.0f} };//右下インデックス15
	data->vertices.push_back(vertexInf);

	//上
	vertexInf = { {-1.0f, 1.0f,-1.0f},{},{ 0.0f, 1.0f} };//左上インデックス16
	data->vertices.push_back(vertexInf);
	vertexInf = { {-1.0f, 1.0f, 1.0f},{},{ 0.0f, 0.0f} };//左下インデックス17
	data->vertices.push_back(vertexInf);
	vertexInf = { { 1.0f, 1.0f,-1.0f},{},{ 1.0f, 1.0f} };//右上インデックス18
	data->vertices.push_back(vertexInf);
	vertexInf = { { 1.0f, 1.0f, 1.0f},{},{ 1.0f, 0.0f} };//右下インデックス19
	data->vertices.push_back(vertexInf);

	//底
	vertexInf = { {-1.0f,-1.0f, 1.0f},{},{ 0.0f, 1.0f} };//左上インデックス20
	data->vertices.push_back(vertexInf);
	vertexInf = { {-1.0f,-1.0f,-1.0f},{},{ 0.0f, 0.0f} };//左下インデックス21
	data->vertices.push_back(vertexInf);
	vertexInf = { { 1.0f,-1.0f, 1.0f},{},{ 1.0f, 1.0f} };//右上インデックス22
	data->vertices.push_back(vertexInf);
	vertexInf = { { 1.0f,-1.0f,-1.0f},{},{ 1.0f, 0.0f} };//右下インデックス23
	data->vertices.push_back(vertexInf);

// インデックスデータ

	//正面
	data->indices.push_back(0), data->indices.push_back(1), data->indices.push_back(3);//0
	data->indices.push_back(3), data->indices.push_back(2), data->indices.push_back(0);//1

	//右
	data->indices.push_back(4), data->indices.push_back(5), data->indices.push_back(7);//2
	data->indices.push_back(7), data->indices.push_back(6), data->indices.push_back(4);//3

	//左
	data->indices.push_back(8), data->indices.push_back(9), data->indices.push_back(11);//4
	data->indices.push_back(11), data->indices.push_back(10), data->indices.push_back(8);//5

	//裏
	data->indices.push_back(12), data->indices.push_back(13), data->indices.push_back(15);//6
	data->indices.push_back(15), data->indices.push_back(14), data->indices.push_back(12);//7

	//上
	data->indices.push_back(16), data->indices.push_back(17), data->indices.push_back(19);//8
	data->indices.push_back(19), data->indices.push_back(18), data->indices.push_back(16);//9

	//底
	data->indices.push_back(20), data->indices.push_back(21), data->indices.push_back(23);//10
	data->indices.push_back(23), data->indices.push_back(22), data->indices.push_back(20);//11

	for (size_t i = 0; i < cubeMaxIndex / 3; i++)
	{
		//三角形1つごとに計算していく
		//三角形のインデックスを取り出して、一時的な変数に入れる
		uint32_t Index0 = data->indices[i * 3 + 0];
		uint32_t Index1 = data->indices[i * 3 + 1];
		uint32_t Index2 = data->indices[i * 3 + 2];
		//三角形を構成する頂点座標をベクトルに代入
		AliceMathF::Vector3 p0 = data->vertices[Index0].pos;
		AliceMathF::Vector3 p1 = data->vertices[Index1].pos;
		AliceMathF::Vector3 p2 = data->vertices[Index2].pos;
		//p0->p1ベクトル、p0->p2ベクトルを計算(ベクトルの減算)
		AliceMathF::Vector3 v1 = p1 - p0;
		AliceMathF::Vector3 v2 = p2 - p0;
		//外積は両方から垂直なベクトル
		AliceMathF::Vector3 normal = v1.Cross(v2);
		//正規化する
		normal.Normal();
		data->vertices[Index0].normal = normal;
		data->vertices[Index1].normal = normal;
		data->vertices[Index2].normal = normal;

	}
	//頂点バッファへのデータ転送
	data->vertexBuffer->Update(data->vertices.data());

	//インデックスバッファへのデータ転送
	data->indexBuffer->Update(data->indices.data());

	data->matWorld = AliceMathF::MakeIdentity();
}

void PrimitiveModel::ConeCreate(ModelData* data)
{
	//定数バッファ生成(マテリアル)
	data->constBuffMaterial = std::make_unique<ConstantBuffer>();
	data->constBuffMaterial->Create(sizeof(ConstBuffDataMaterial));
	data->constBuffMaterial->Update(&data->modelMaterial);

	// 頂点データ
	constexpr UINT NumDiv = 32; // 分割数
	float RadianPerDivision = AliceMathF::AX_2PI / float(NumDiv); // 分割1単位の角度

	//分割点分割数より1つ多くしておいて1周回るようにする
	std::array<float, NumDiv + 1> pointsZ{};
	std::array<float, NumDiv + 1> pointsX{};
	std::array<float, NumDiv + 1> pointsU{};
	std::array<float, NumDiv + 1> pointsV{};

	//上側の計算
	//頂点座標の計算
	for (size_t i = 0; i < NumDiv; ++i)
	{
		float baseZ = cos(RadianPerDivision * static_cast<float>(i)) * 1.0f;
		float baseX = sin(RadianPerDivision * static_cast<float>(i)) * 1.0f;
		pointsZ[i] = baseZ * -1.0f;
		pointsX[i] = baseX;
	}

	//UV座標の計算
	for (size_t i = 0; i < NumDiv; ++i)
	{
		float baseU = cos(RadianPerDivision * static_cast<float>(i)) * 0.25f;
		float baseV = sin(RadianPerDivision * static_cast<float>(i)) * 0.25f;
		pointsU[i] = baseU + 0.25f;
		pointsV[i] = baseV + 0.5f;
	}

	//最後と最初は一緒
	(*pointsZ.rbegin()) = (*pointsZ.begin());
	(*pointsX.rbegin()) = (*pointsX.begin());
	(*pointsU.rbegin()) = (*pointsU.begin());
	(*pointsV.rbegin()) = (*pointsV.begin());

	PosNormalUv vertexInf{};
	//計算した値を代入
	for (size_t i = 0; i < static_cast<size_t>(NumDiv + 1.0f); i++)
	{
		vertexInf = { { pointsX[i],-1.0f,pointsZ[i] },{},{ pointsU[i],pointsV[i]} };
		data->vertices.push_back(vertexInf);
	}

	vertexInf = { { 0.0f,1.0f,0.0f },{},{ 0.25f,0.5f } };
	data->vertices.push_back(vertexInf);

	//下側の計算
	//UV座標の計算
	for (size_t i = 0; i < NumDiv; ++i)
	{
		float baseU = cos(RadianPerDivision * static_cast<float>(i)) * 0.25f;
		float baseV = sin(RadianPerDivision * static_cast<float>(i)) * 0.25f;
		pointsU[i] = baseU + 0.75f;
		pointsV[i] = baseV + 0.5f;
	}

	(*pointsU.rbegin()) = (*pointsU.begin());
	(*pointsV.rbegin()) = (*pointsV.begin());

	//計算した値を代入
	for (size_t i = 1; i < static_cast<size_t>(NumDiv + 1.0f); i++)
	{
		vertexInf = { { pointsX[i],-1.0f,pointsZ[i] },{},{ pointsU[i],pointsV[i]} };
		data->vertices.push_back(vertexInf);
	}

	vertexInf = { { 0.0f,-1.0f,0.0f },{},{ 0.75f,0.5 } };
	data->vertices.push_back(vertexInf);

	// インデックスデータ
	uint8_t count = 0;
	uint32_t indexData = 0;

	//インデックスデータの計算上側
	for (uint32_t i = 0; i < 96; i++)
	{
		if (count == 3)
		{
			count = 0;
		}

		switch (count)
		{
		case 0:
			data->indices.push_back(indexData);
			break;
		case 1:
			data->indices.push_back(33);
			break;
		case 2:
			indexData++;
			data->indices.push_back(indexData);
			break;
		}

		count++;
	}

	count = 0;
	indexData = 35;
	//インデックスデータの計側
	for (size_t i = 96; i < data->maxIndex; i++)
	{
		if (count == 3)
		{
			count = 0;
		}

		switch (count)
		{
		case 0:
			data->indices.push_back(indexData);
			break;
		case 1:
			data->indices.push_back(65);
			break;
		case 2:
			data->indices.push_back(indexData - static_cast<unsigned>(1));
			indexData++;
			break;
		}

		count++;
	}

	for (UINT i = 0; i < data->indices.size() / 3; i++)
	{
		//三角形1つごとに計算していく
		//三角形のインデックスを取り出して、一時的な変数に入れる
		uint32_t Index0 = data->indices[static_cast<size_t>(i * 3 + 0)];
		uint32_t Index1 = data->indices[static_cast<size_t>(i * 3 + 1)];
		uint32_t Index2 = data->indices[static_cast<size_t>(i * 3 + 2)];
		//三角形を構成する頂点座標をベクトルに代入
		AliceMathF::Vector3 p0 = data->vertices[Index0].pos;
		AliceMathF::Vector3 p1 = data->vertices[Index1].pos;
		AliceMathF::Vector3 p2 = data->vertices[Index2].pos;
		//p0->p1ベクトル、p0->p2ベクトルを計算(ベクトルの減算)
		AliceMathF::Vector3 v1 = p1 - p0;
		AliceMathF::Vector3 v2 = p2 - p0;
		//外積は両方から垂直なベクトル
		AliceMathF::Vector3 normal = v1.Cross(v2);
		//正規化する
		normal.Normal();
		data->vertices[Index0].normal = normal;
		data->vertices[Index1].normal = normal;
		data->vertices[Index2].normal = normal;
	}

	//頂点バッファへのデータ転送
	data->vertexBuffer->Update(data->vertices.data());

	//インデックスバッファへのデータ転送
	data->indexBuffer->Update(data->indices.data());

	//ワールド座標
	data->matWorld = AliceMathF::MakeIdentity();
}

void PrimitiveModel::CylinderCreate(ModelData* data)
{
	//定数バッファ生成(マテリアル)
	data->constBuffMaterial = std::make_unique<ConstantBuffer>();
	data->constBuffMaterial->Create(sizeof(ConstBuffDataMaterial));
	data->constBuffMaterial->Update(&data->modelMaterial);

	// 頂点データ
	constexpr UINT NumDiv = 32; // 分割数
	float RadianPerDivision = AliceMathF::AX_2PI / float(NumDiv); // 分割1単位の角度

	//分割点分割数より1つ多くしておいて1周回るようにする
	std::array<float, NumDiv + 1> pointsZ{};
	std::array<float, NumDiv + 1> pointsX{};
	std::array<float, NumDiv + 1> pointsU{};
	std::array<float, NumDiv + 1> pointsV{};

	//上側の計算
	//頂点座標の計算
	for (size_t i = 0; i < NumDiv; ++i)
	{
		float baseZ = cos(RadianPerDivision * static_cast<float>(i)) * 1.0f;
		float baseX = sin(RadianPerDivision * static_cast<float>(i)) * 1.0f;
		pointsZ[i] = baseZ * -1.0f;
		pointsX[i] = baseX;
		//UV座標の計算
		float baseU = cos(RadianPerDivision * static_cast<float>(i)) * 0.25f;
		float baseV = sin(RadianPerDivision * static_cast<float>(i)) * 0.25f;
		pointsU[i] = baseU + 0.75f;
		pointsV[i] = baseV + 0.75f;
	}

	//最後と最初は一緒
	(*pointsZ.rbegin()) = (*pointsZ.begin());
	(*pointsX.rbegin()) = (*pointsX.begin());
	(*pointsU.rbegin()) = (*pointsU.begin());
	(*pointsV.rbegin()) = (*pointsV.begin());

	PosNormalUv vertexInf;

	//計算した値を代入
	for (size_t i = 0; i < NumDiv; i++)
	{
		vertexInf = { { pointsX[i],1.0f,pointsZ[i] },{},{ pointsV[i],pointsU[i] } };
		data->vertices.push_back(vertexInf);
	}

	vertexInf = { { 0.0f,1.0f,0.0f },{},{ 0.75f,0.75f } };
	data->vertices.push_back(vertexInf);

	//下側の計算
	//UV座標の計算
	for (size_t i = 0; i < NumDiv; ++i)
	{
		float baseU = cos(RadianPerDivision * static_cast<float>(i)) * 0.25f;
		float baseV = sin(RadianPerDivision * static_cast<float>(i)) * 0.25f;
		pointsU[i] = baseU + 0.25f;
		pointsV[i] = baseV + 0.75f;
	}

	(*pointsU.rbegin()) = (*pointsU.begin());
	(*pointsV.rbegin()) = (*pointsV.begin());

	//計算した値を代入
	for (size_t i = 0; i < static_cast<size_t>(NumDiv); i++)
	{
		vertexInf = { { pointsX[i],-1.0f,pointsZ[i] },{},{ pointsU[i],pointsV[i] }, };
		data->vertices.push_back(vertexInf);
	}

	vertexInf = { { 0.0f,-1.0f,0.0f },{},{ 0.25f,0.75f } };
	data->vertices.push_back(vertexInf);

	//UV座標の計算
	for (size_t i = 0; i < static_cast<size_t>(NumDiv + 1); ++i)
	{
		pointsU[i] = static_cast<float>(i) * (1.0f / 31.0f);
		pointsV[i] = 0.5f;
	}
	(*pointsU.rbegin()) = 1.0f;

	//計算した値を代入
	for (size_t i = 0; i < static_cast<size_t>(NumDiv + 1); i++)
	{
		vertexInf = { { pointsX[i],-1.0f,pointsZ[i] },{},{ pointsU[i],pointsV[i] } };
		data->vertices.push_back(vertexInf);
	}

	//UV座標の計算
	for (size_t i = 0; i < static_cast<size_t>(NumDiv + 1); ++i)
	{
		pointsU[i] = static_cast<float>(i) * (1.0f / 31.0f);
		pointsV[i] = 0.0f;
	}

	(*pointsU.rbegin()) = 1.0f;

	//計算した値を代入
	for (size_t i = 0; i < static_cast<size_t>(NumDiv + 1); i++)
	{
		vertexInf = { { pointsX[i],1.0f,pointsZ[i] },{},{ pointsU[i],pointsV[i] } };
		data->vertices.push_back(vertexInf);
	}

	// インデックスデータ
	uint8_t count = 0;
	uint32_t indexData = 0;

	//インデックスデータの計算上側
	for (uint32_t i = 0; i < 93; i++)
	{
		if (count == 3)
		{
			count = 0;
		}

		if (i == 92)
		{
			data->indices.push_back(0);
			count++;
		}

		switch (count)
		{
		case 0:
			data->indices.push_back(indexData);
			break;
		case 1:
			data->indices.push_back(32);
			break;
		case 2:
			indexData++;
			data->indices.push_back(indexData);

			break;
		}

		count++;
	}

	count = 0;
	indexData = 34;
	//インデックスデータの計側
	for (size_t i = 0; i < 95; i++)
	{
		if (count == 3)
		{
			count = 0;
		}

		if (i == 93)
		{
			data->indices.push_back(33);
			count++;
		}

		switch (count)
		{
		case 0:
			data->indices.push_back(indexData);
			break;
		case 1:
			data->indices.push_back(65);
			break;
		case 2:
			data->indices.push_back(indexData - static_cast<unsigned>(1));
			indexData++;
			break;
		}
		count++;
	}

	count = 0;
	indexData = 66;
	//インデックスデータの計側
	for (size_t i = 0; i < 96; i++)
	{
		if (count == 3)
		{
			count = 0;
		}

		if (i == 95)
		{
			data->indices.push_back(98);
			count++;
		}

		switch (count)
		{
		case 0:
			data->indices.push_back(indexData);
			break;
		case 1:
			data->indices.push_back(indexData + static_cast<unsigned>(33));
			break;
		case 2:
			data->indices.push_back(indexData + static_cast<unsigned>(1));
			indexData += 1;
			break;
		}
		count++;

	}

	count = 0;
	indexData = 98;
	//インデックスデータの計側
	for (size_t i = 0; i < 99; i++)
	{
		if (count == 3)
		{
			count = 0;
		}

		if (i == 97)
		{
			data->indices.push_back(98);
			count++;
		}

		switch (count)
		{
		case 0:
			data->indices.push_back(indexData + static_cast<unsigned>(1));
			break;
		case 1:
			data->indices.push_back(indexData - static_cast<unsigned>(32));
			break;
		case 2:
			data->indices.push_back(indexData);
			indexData += 1;
			break;
		}
		count++;
	}

	for (UINT i = 0; i < data->indices.size() / 3; i++)
	{
		//三角形1つごとに計算していく
		//三角形のインデックスを取り出して、一時的な変数に入れる
		uint32_t Index0 = data->indices[static_cast<std::vector<uint32_t, std::allocator<uint32_t>>::size_type>(i) * 3 + 0];
		uint32_t Index1 = data->indices[static_cast<std::vector<uint32_t, std::allocator<uint32_t>>::size_type>(i) * 3 + 1];
		uint32_t Index2 = data->indices[static_cast<std::vector<uint32_t, std::allocator<uint32_t>>::size_type>(i) * 3 + 2];
		//三角形を構成する頂点座標をベクトルに代入
		AliceMathF::Vector3 p0 = data->vertices[Index0].pos;
		AliceMathF::Vector3 p1 = data->vertices[Index1].pos;
		AliceMathF::Vector3 p2 = data->vertices[Index2].pos;
		//p0->p1ベクトル、p0->p2ベクトルを計算(ベクトルの減算)
		AliceMathF::Vector3 v1 = p1 - p0;
		AliceMathF::Vector3 v2 = p2 - p0;
		//外積は両方から垂直なベクトル
		AliceMathF::Vector3 normal = v1.Cross(v2);
		//正規化する
		normal.Normal();
		data->vertices[Index0].normal = normal;
		data->vertices[Index1].normal = normal;
		data->vertices[Index2].normal = normal;

	}

	//頂点バッファへのデータ転送
	data->vertexBuffer->Update(data->vertices.data());

	//インデックスバッファへのデータ転送
	data->indexBuffer->Update(data->indices.data());

	data->matWorld = AliceMathF::MakeIdentity();
}

void PrimitiveModel::SphereCreate(ModelData* data)
{
	//定数バッファ生成(マテリアル)
	data->constBuffMaterial = std::make_unique<ConstantBuffer>();
	data->constBuffMaterial->Create(sizeof(ConstBuffDataMaterial));


	AliceMathF::Vector3 one = { 0.8f,0.8f,0.8f };
	data->modelMaterial.ambient = one;
	data->modelMaterial.diffuse = one;
	data->modelMaterial.specular = one;
	data->modelMaterial.alpha = data->modelMaterial.alpha;

	data->constBuffMaterial->Update(&data->modelMaterial);

	// 頂点データ
	float u = 0.0f;
	float v = 0.0f;

	PosNormalUv tmp = { {0, 0, 0},{1,1,1},{0,0} };

	data->vertices.push_back(tmp);

	size_t dividedInVertical = 16;
	size_t dividedInHorizontal = 32;
	float radius = 1;
	float Deg2Rad = (AliceMathF::AX_PI * 2.0f) / 360.0f;


	for (size_t p = 1; p < dividedInVertical; p++)
	{
		tmp.pos.y = std::cos(Deg2Rad * static_cast<float>(p) * 180.0f / static_cast<float>(dividedInVertical)) * radius;
		float t = std::sin(Deg2Rad * static_cast<float>(p) * 180.0f / static_cast<float>(dividedInVertical)) * radius;

		v += 1.0f / 16.0f;
		u = -1.0f / 32.0f;

		for (size_t q = 0; q < dividedInHorizontal + 1; q++)
		{
			u += 1.0f / 32.0f;
			tmp.uv = { u,v };
			tmp.pos.x = std::cos(Deg2Rad * static_cast<float>(q) * 360.0f / static_cast<float>(dividedInHorizontal)) * t;
			tmp.pos.z = std::sin(Deg2Rad * static_cast<float>(q) * 360.0f / static_cast<float>(dividedInHorizontal)) * t;
			data->vertices.push_back(tmp);
		}
	}

	u = 0.0f;
	v = 0.0f;
	for (size_t i = 0; i < dividedInHorizontal; i++)
	{
		u += 1.0f / 31.0f;
		tmp.uv = { u,v };
		tmp.pos = AliceMathF::Vector3(0, radius, 0);
		data->vertices.push_back(tmp);
	}

	u = 0.0f;
	v = 1.0f;
	for (size_t i = 0; i < dividedInHorizontal; i++)
	{
		u += 1.0f / 31.0f;
		tmp.uv = { u,v };
		tmp.pos = AliceMathF::Vector3(0, -radius, 0);
		data->vertices.push_back(tmp);
	}

#pragma endregion

#pragma region 頂点順序を格納

	uint32_t apexIndex = 496;

	for (size_t i = 0; i < dividedInHorizontal; i++)
	{
		if (i == dividedInHorizontal - 1)
		{
			data->indices.push_back(static_cast<uint32_t>(apexIndex));
			data->indices.push_back(static_cast<uint32_t>(i + 2));
			data->indices.push_back(static_cast<uint32_t>(i + 1));
			break;
		}

		data->indices.push_back(static_cast<uint32_t>(apexIndex++));
		data->indices.push_back(static_cast<uint32_t>(i + 2));
		data->indices.push_back(static_cast<uint32_t>(i + 1));
	}

	for (size_t p = 0; p < dividedInVertical - 1; p++)
	{
		auto firstIndexInLayer = p * dividedInHorizontal + 1;

		for (size_t q = 0; q < dividedInHorizontal; q++)
		{
			if (q == dividedInHorizontal - 1)
			{
				data->indices.push_back(static_cast<uint32_t>(firstIndexInLayer + q));
				data->indices.push_back(static_cast<uint32_t>(firstIndexInLayer));
				data->indices.push_back(static_cast<uint32_t>(firstIndexInLayer + dividedInHorizontal));

				data->indices.push_back(static_cast<uint32_t>(firstIndexInLayer + q));
				data->indices.push_back(static_cast<uint32_t>(firstIndexInLayer + dividedInHorizontal));
				data->indices.push_back(static_cast<uint32_t>(firstIndexInLayer + q + dividedInHorizontal));

				break;
			}

			data->indices.push_back(static_cast<uint32_t>(firstIndexInLayer + q));
			data->indices.push_back(static_cast<uint32_t>(firstIndexInLayer + q + 1));
			data->indices.push_back(static_cast<uint32_t>(firstIndexInLayer + q + 1 + dividedInHorizontal));

			data->indices.push_back(static_cast<uint32_t>(firstIndexInLayer + q));
			data->indices.push_back(static_cast<uint32_t>(firstIndexInLayer + q + dividedInHorizontal + 1));
			data->indices.push_back(static_cast<uint32_t>(firstIndexInLayer + q + dividedInHorizontal));
		}
	}

	apexIndex = 528;
	for (size_t i = 0; i < dividedInHorizontal; i++)
	{
		if (i == dividedInHorizontal - 1)
		{
			data->indices.push_back(static_cast<uint32_t>(apexIndex));
			data->indices.push_back(static_cast<uint32_t>(static_cast<size_t>(496) - 1 - dividedInHorizontal + i));
			data->indices.push_back(static_cast<uint32_t>(496 - dividedInHorizontal + i));
			break;
		}
		data->indices.push_back(static_cast<uint32_t>(apexIndex++));
		data->indices.push_back(static_cast<uint32_t>(static_cast<size_t>(496) - 1 - dividedInHorizontal + i));
		data->indices.push_back(static_cast<uint32_t>(496 - dividedInHorizontal + i));
	}

#pragma endregion



	for (UINT i = 0; i < data->indices.size() / 3; i++)
	{
		//三角形1つごとに計算していく
		//三角形のインデックスを取り出して、一時的な変数に入れる
		uint32_t Index0 = data->indices[static_cast<std::vector<uint32_t, std::allocator<uint32_t>>::size_type>(i) * 3 + 0];
		uint32_t Index1 = data->indices[static_cast<std::vector<uint32_t, std::allocator<uint32_t>>::size_type>(i) * 3 + 1];
		uint32_t Index2 = data->indices[static_cast<std::vector<uint32_t, std::allocator<uint32_t>>::size_type>(i) * 3 + 2];
		//三角形を構成する頂点座標をベクトルに代入
		AliceMathF::Vector3 p0 = data->vertices[Index0].pos;
		AliceMathF::Vector3 p1 = data->vertices[Index1].pos;
		AliceMathF::Vector3 p2 = data->vertices[Index2].pos;
		//p0->p1ベクトル、p0->p2ベクトルを計算(ベクトルの減算)
		AliceMathF::Vector3 v1 = p1 - p0;
		AliceMathF::Vector3 v2 = p2 - p0;
		//外積は両方から垂直なベクトル
		AliceMathF::Vector3 normal = v1.Cross(v2);
		//正規化する
		normal.Normal();
		data->vertices[Index0].normal = normal;
		data->vertices[Index1].normal = normal;
		data->vertices[Index2].normal = normal;

	}

	//頂点バッファへのデータ転送
	data->vertexBuffer->Update(data->vertices.data());

	//インデックスバッファへのデータ転送
	data->indexBuffer->Update(data->indices.data());

	data->matWorld = AliceMathF::MakeIdentity();
}
