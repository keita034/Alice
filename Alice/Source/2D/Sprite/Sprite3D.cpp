#include<Sprite3D.h>


Sprite3D::Sprite3D()
{
}

Sprite3D::~Sprite3D()
{
}

void Sprite3D::Initialize(const uint32_t handle)
{
	texture = TextureManager::GetTextureData(handle);

	SpriteInitialize();

	CreatVertexIndexBuffer();

	trimmingRange.z = static_cast<float>(texture->width);
	trimmingRange.w = static_cast<float>(texture->height);
}

void Sprite3D::Draw(Transform& transform, Camera* camera, BlendMode blend, BillboardFlag flag, Material* material)
{
	AliceMathF::Matrix4 mTrans, mRot, mScale;

	//平行移動行列
	mTrans.MakeTranslation(transform.translation);
	//回転行列
	mRot.MakeRotation(transform.rotation);
	//スケール行列
	mScale.MakeScaling(transform.scale);

	switch (flag)
	{
	case NonBillboard:
		transform.matWorld = mScale * mRot * mTrans * camera->GetViewMatrixInv() * camera->GetProjectionMatrix();
		break;
	case AllBillboard:
	{
		AliceMathF::Matrix4 mat = camera->GetViewMatrix();

		mat.m[3][0] = 0;
		mat.m[3][1] = 0;
		mat.m[3][2] = 0;
		mat.m[3][3] = 1;

		transform.matWorld = mScale * mRot * mat * mTrans * camera->GetViewMatrixInv() * camera->GetProjectionMatrix();
		break;
	}
	case XBillboard:
	{
		AliceMathF::Matrix4 mat = camera->GetViewMatrix();

		mat.m[1][0] = 0;
		mat.m[1][1] = 1;
		mat.m[1][2] = 0;
		mat.m[1][3] = 0;

		mat.m[3][0] = 0;
		mat.m[3][1] = 0;
		mat.m[3][2] = 0;
		mat.m[3][3] = 1;

		transform.matWorld = mScale * mRot * mat * mTrans * camera->GetViewMatrixInv() * camera->GetProjectionMatrix();
		break;
	}
	case YBillboard:
	{
		AliceMathF::Matrix4 mat = camera->GetViewMatrix();

		mat.m[0][0] = 1;
		mat.m[0][1] = 0;
		mat.m[0][2] = 0;
		mat.m[0][3] = 0;

		mat.m[3][0] = 0;
		mat.m[3][1] = 0;
		mat.m[3][2] = 0;
		mat.m[3][3] = 1;


		transform.matWorld = mScale * mRot * mat * mTrans * camera->GetViewMatrixInv() * camera->GetProjectionMatrix();
		break;
	}
	case XYBillboard:
	{
		AliceMathF::Matrix4 mat = camera->GetViewMatrix();

		mat.m[2][0] = 0;
		mat.m[2][1] = 0;
		mat.m[2][2] = 1;
		mat.m[2][3] = 0;

		mat.m[3][0] = 0;
		mat.m[3][1] = 0;
		mat.m[3][2] = 0;
		mat.m[3][3] = 1;

		transform.matWorld = mScale * mRot * mat * mTrans * camera->GetViewMatrixInv() * camera->GetProjectionMatrix();
		break;
	}
	default:
		assert(0);
		break;
	}

	switch (blend)
	{
	case BlendMode::AX_BLENDMODE_NOBLEND:
		spriteMaterial = MaterialManager::GetMaterial("Sprite3DNoblend");
		break;
	case BlendMode::AX_BLENDMODE_ALPHA:
		spriteMaterial = MaterialManager::GetMaterial("Sprite3DAlpha");
		break;
	case BlendMode::AX_BLENDMODE_ADD:
		spriteMaterial = MaterialManager::GetMaterial("Sprite3DAdd");
		break;
	case BlendMode::AX_BLENDMODE_SUB:
		spriteMaterial = MaterialManager::GetMaterial("Sprite3DSub");
		break;
	case BlendMode::AX_BLENDMODE_MULA:
		spriteMaterial = MaterialManager::GetMaterial("Sprite3DMula");
		break;
	case BlendMode::AX_BLENDMODE_INVSRC:
		spriteMaterial = MaterialManager::GetMaterial("Sprite3DInvsrc");
		break;
	case BlendMode::AX_BLENDMODE_MAX:
		spriteMaterial = MaterialManager::GetMaterial("Sprite3DNoblend");
		//printf("ブレンドの種類数を入れています");
		break;
	case BlendMode::AX_BLENDMODE_CUSTOM:
		if (material)
		{
			spriteMaterial = material;
		}
		else
		{
			//printf("マテリアルがnullです");
			spriteMaterial = MaterialManager::GetMaterial("Sprite3DNoblend");
		}
		break;
	default:
		//printf("ブレンドの範囲外を入れています");
		assert(0);
		break;
	}

	transform.Update();

	SpriteDraw(transform, spriteMaterial);
}

void Sprite3D::Draw(Transform& transform, BlendMode blend, Material* material)
{

	float isFlipX, isFlipY;
	isFlipX = flipX ? -1.0f : 1.0f;
	isFlipY = flipY ? -1.0f : 1.0f;

	float left = ((0.0f - anchorPoint.x) * (static_cast<float>(texture->width) * 0.0625f)) * isFlipX;
	float right = ((1.0f - anchorPoint.x) * (static_cast<float>(texture->width) * 0.0625f)) * isFlipX;
	float top = ((0.0f - anchorPoint.y) * (static_cast<float>(texture->height) * 0.0625f)) * isFlipY;
	float bottom = ((1.0f - anchorPoint.y) * (static_cast<float>(texture->height) * 0.0625f)) * isFlipY;

	float texLeft = trimmingRange.x / static_cast<float>(texture->width);
	float texTop = trimmingRange.y / static_cast<float>(texture->height);
	float texRight = trimmingRange.z / static_cast<float>(texture->width);
	float texBottom = trimmingRange.w / static_cast<float>(texture->height);

	// 頂点データ
	PosUvColor vertices[] =
	{//		x		y		z		u	v
		{{left,top,0.0f},{texLeft,texTop},{color.x,color.y,color.z,color.w}},//左上インデックス0
		{{left,bottom,0.0f},{texLeft,texBottom},{color.x,color.y,color.z,color.w}},//左下インデックス1
		{{right,top,0.0f},{texRight,texTop},{color.x,color.y,color.z,color.w}},//右上インデックス2
		{{right,bottom,0.0f},{texRight,texBottom},{color.x,color.y,color.z,color.w}},//右下インデックス3
	};

	// インデックスデータ
	uint32_t indices[] =
	{
		3, 0, 1, // 三角形1つ目
		0, 3, 2, // 三角形2つ目
	};

	//頂点バッファへのデータ転送
	vertexBuffer->Update(vertices);

	//インデックスバッファへのデータ転送
	indexBuffer->Update(indices);

	switch (blend)
	{
	case BlendMode::AX_BLENDMODE_NOBLEND:
		spriteMaterial = MaterialManager::GetMaterial("Sprite3DNoblend");
		break;
	case BlendMode::AX_BLENDMODE_ALPHA:
		spriteMaterial = MaterialManager::GetMaterial("Sprite3DAlpha");
		break;
	case BlendMode::AX_BLENDMODE_ADD:
		spriteMaterial = MaterialManager::GetMaterial("Sprite3DAdd");
		break;
	case BlendMode::AX_BLENDMODE_SUB:
		spriteMaterial = MaterialManager::GetMaterial("Sprite3DSub");
		break;
	case BlendMode::AX_BLENDMODE_MULA:
		spriteMaterial = MaterialManager::GetMaterial("Sprite3DMula");
		break;
	case BlendMode::AX_BLENDMODE_INVSRC:
		spriteMaterial = MaterialManager::GetMaterial("Sprite3DInvsrc");
		break;
	case BlendMode::AX_BLENDMODE_MAX:
		spriteMaterial = MaterialManager::GetMaterial("Sprite3DNoblend");
		//printf("ブレンドの種類数を入れています");
		break;
	case BlendMode::AX_BLENDMODE_CUSTOM:
		if (material)
		{
			spriteMaterial = material;
		}
		else
		{
			//printf("マテリアルがnullです");
			spriteMaterial = MaterialManager::GetMaterial("Sprite3DNoblend");
		}
		break;
	default:
		//printf("ブレンドの範囲外を入れています");
		assert(0);
		break;
	}

	SpriteDraw(transform, spriteMaterial);
}

void Sprite3D::AnimationDraw(Transform& transform, uint16_t radiusX, uint16_t radiusY, float& frame, float frameDiv, BlendMode blend, Material* material)
{
	size_t animeFrame = static_cast<size_t>(frame / frameDiv);

	size_t width = static_cast<size_t>(radiusX) * 2;
	size_t height = static_cast<size_t>(radiusY) * 2;
	float texTop = trimmingRange.y / static_cast<float>(texture->height);
	float texRight = trimmingRange.z / static_cast<float>(texture->width);

	float widthU = static_cast<float>(width) / (texRight * static_cast<float>(texture->width));

	//画像の半分のサイズ

	if (texture->width / width < animeFrame + 1)
	{
		frame = 0;
	}

	float isFlipX, isFlipY;
	isFlipX = flipX ? -1.0f : 1.0f;
	isFlipY = flipY ? -1.0f : 1.0f;

	float left = ((0.0f - anchorPoint.x) * (static_cast<float>(width / 10))) * isFlipX;
	float right = ((1.0f - anchorPoint.x) * (static_cast<float>(width / 10))) * isFlipX;
	float top = ((0.0f - anchorPoint.y) * static_cast<float>(height / 10)) * isFlipY;
	float bottom = ((1.0f - anchorPoint.y) * static_cast<float>(height / 10)) * isFlipY;

	// 頂点データ
	PosUvColor vertices[] =
	{//		x		y		z		u	v
		{{left, top, 0.0f},{widthU * static_cast<float>(animeFrame),texTop},{1.0f,1.0f,1.0f,1.0f}},//左上インデックス0
		{{left, bottom, 0.0f},{widthU * static_cast<float>(animeFrame),texRight},{1.0f,1.0f,1.0f,1.0f}},//左下インデックス1
		{{right, top, 0.0f},{widthU * static_cast<float>((animeFrame + 1)),texTop},{1.0f,1.0f,1.0f,1.0f}},//右上インデックス2
		{{right, bottom, 0.0f},{widthU * static_cast<float>((animeFrame + 1)),texRight},{1.0f,1.0f,1.0f,1.0f}},//右下インデックス3
	};

	// インデックスデータ
	uint32_t indices[] =
	{
		3, 0, 1, // 三角形1つ目
		0, 3, 2, // 三角形2つ目
	};

	//頂点バッファへのデータ転送
	vertexBuffer->Update(vertices);

	//インデックスバッファへのデータ転送
	indexBuffer->Update(indices);

	switch (blend)
	{
	case BlendMode::AX_BLENDMODE_NOBLEND:
		spriteMaterial = MaterialManager::GetMaterial("Sprite3DNoblend");
		break;
	case BlendMode::AX_BLENDMODE_ALPHA:
		spriteMaterial = MaterialManager::GetMaterial("Sprite3DAlpha");
		break;
	case BlendMode::AX_BLENDMODE_ADD:
		spriteMaterial = MaterialManager::GetMaterial("Sprite3DAdd");
		break;
	case BlendMode::AX_BLENDMODE_SUB:
		spriteMaterial = MaterialManager::GetMaterial("Sprite3DSub");
		break;
	case BlendMode::AX_BLENDMODE_MULA:
		spriteMaterial = MaterialManager::GetMaterial("Sprite3DMula");
		break;
	case BlendMode::AX_BLENDMODE_INVSRC:
		spriteMaterial = MaterialManager::GetMaterial("Sprite3DInvsrc");
		break;
	case BlendMode::AX_BLENDMODE_MAX:
		spriteMaterial = MaterialManager::GetMaterial("Sprite3DNoblend");
		//printf("ブレンドの種類数を入れています");
		break;
	case BlendMode::AX_BLENDMODE_CUSTOM:
		if (material)
		{
			spriteMaterial = material;
		}
		else
		{
			//printf("マテリアルがnullです");
			spriteMaterial = MaterialManager::GetMaterial("Sprite3DNoblend");
		}
		break;
	default:
		//printf("ブレンドの範囲外を入れています");
		assert(0);
		break;
	}

	transform.Update();


	SpriteDraw(transform, spriteMaterial);
}

void Sprite3D::AnimationDraw(Transform& transform, uint16_t radiusX, uint16_t radiusY, float& frame, float frameDiv, Camera* camera, BlendMode blend, BillboardFlag flag, Material* material)
{
	AliceMathF::Matrix4 mTrans, mRot, mScale;

	//平行移動行列
	mTrans.MakeTranslation(transform.translation);
	//回転行列
	mRot.MakeRotation(transform.rotation);
	//スケール行列
	mScale.MakeScaling(transform.scale);

	switch (flag)
	{
	case NonBillboard:
		transform.matWorld = mScale * mRot * mTrans * camera->GetViewMatrixInv() * camera->GetProjectionMatrix();
		break;
	case AllBillboard:
	{
		AliceMathF::Matrix4 mat = camera->GetViewMatrix();

		mat.m[3][0] = 0;
		mat.m[3][1] = 0;
		mat.m[3][2] = 0;
		mat.m[3][3] = 1;

		transform.matWorld = mScale * mRot * mat * mTrans * camera->GetViewMatrixInv() * camera->GetProjectionMatrix();
		break;
	}
	case XBillboard:
	{
		AliceMathF::Matrix4 mat = camera->GetViewMatrix();

		mat.m[1][0] = 0;
		mat.m[1][1] = 1;
		mat.m[1][2] = 0;
		mat.m[1][3] = 0;

		mat.m[3][0] = 0;
		mat.m[3][1] = 0;
		mat.m[3][2] = 0;
		mat.m[3][3] = 1;

		transform.matWorld = mScale * mRot * mat * mTrans * camera->GetViewMatrixInv() * camera->GetProjectionMatrix();
		break;
	}
	case YBillboard:
	{
		AliceMathF::Matrix4 mat = camera->GetViewMatrix();

		mat.m[0][0] = 1;
		mat.m[0][1] = 0;
		mat.m[0][2] = 0;
		mat.m[0][3] = 0;

		mat.m[3][0] = 0;
		mat.m[3][1] = 0;
		mat.m[3][2] = 0;
		mat.m[3][3] = 1;


		transform.matWorld = mScale * mRot * mat * mTrans * camera->GetViewMatrixInv() * camera->GetProjectionMatrix();
		break;
	}
	case XYBillboard:
	{
		AliceMathF::Matrix4 mat = camera->GetViewMatrix();

		mat.m[2][0] = 0;
		mat.m[2][1] = 0;
		mat.m[2][2] = 1;
		mat.m[2][3] = 0;

		mat.m[3][0] = 0;
		mat.m[3][1] = 0;
		mat.m[3][2] = 0;
		mat.m[3][3] = 1;

		transform.matWorld = mScale * mRot * mat * mTrans * camera->GetViewMatrixInv() * camera->GetProjectionMatrix();
		break;
	}
	default:
		assert(0);
		break;
	}

	transform.Update();

	AnimationDraw(transform, radiusX, radiusY, frame, frameDiv, blend, material);
}
