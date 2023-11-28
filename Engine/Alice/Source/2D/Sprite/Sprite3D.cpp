#include<Sprite3D.h>


Sprite3D::Sprite3D()
{
}

Sprite3D::~Sprite3D()
{
}

void Sprite3D::Initialize(uint32_t handle_)
{
	texture = TextureManager::SGetTextureData(handle_);

	PCreatVertexIndexBuffer();

	trimmingRange.z = static_cast<float>(texture->width);
	trimmingRange.w = static_cast<float>(texture->height);
}

void Sprite3D::Draw(Transform& transform_, Camera* camera_, BlendMode blend_, BillboardFlag flag_, const Material* material_)
{
	AliceMathF::Matrix4 lMatTrans, lMatRot, lMatScale;

	//平行移動行列
	lMatTrans.MakeTranslation(transform_.translation);
	//回転行列
	lMatRot.MakeRotation(transform_.rotation);
	//スケール行列
	lMatScale.MakeScaling(transform_.scale);

	switch (flag_)
	{
	case NON_BILLBOARD:
		transform_.matWorld = lMatScale * lMatRot * lMatTrans * camera_->GetViewMatrixInv() * camera_->GetProjectionMatrix();
		break;
	case ALL_BILLBOARD:
	{
		AliceMathF::Matrix4 lMatBillboard = camera_->GetViewMatrix();

		lMatBillboard.m[3][0] = 0;
		lMatBillboard.m[3][1] = 0;
		lMatBillboard.m[3][2] = 0;
		lMatBillboard.m[3][3] = 1;

		transform_.matWorld = lMatScale * lMatRot * lMatBillboard * lMatTrans * camera_->GetViewMatrixInv() * camera_->GetProjectionMatrix();
		break;
	}
	case X_BILLBOARD:
	{
		AliceMathF::Matrix4 lMatBillboard = camera_->GetViewMatrix();

		lMatBillboard.m[1][0] = 0;
		lMatBillboard.m[1][1] = 1;
		lMatBillboard.m[1][2] = 0;
		lMatBillboard.m[1][3] = 0;

		lMatBillboard.m[3][0] = 0;
		lMatBillboard.m[3][1] = 0;
		lMatBillboard.m[3][2] = 0;
		lMatBillboard.m[3][3] = 1;

		transform_.matWorld = lMatScale * lMatRot * lMatBillboard * lMatTrans * camera_->GetViewMatrixInv() * camera_->GetProjectionMatrix();
		break;
	}
	case Y_BILLBOARD:
	{
		AliceMathF::Matrix4 lMatBillboard = camera_->GetViewMatrix();

		lMatBillboard.m[0][0] = 1;
		lMatBillboard.m[0][1] = 0;
		lMatBillboard.m[0][2] = 0;
		lMatBillboard.m[0][3] = 0;

		lMatBillboard.m[3][0] = 0;
		lMatBillboard.m[3][1] = 0;
		lMatBillboard.m[3][2] = 0;
		lMatBillboard.m[3][3] = 1;


		transform_.matWorld = lMatScale * lMatRot * lMatBillboard * lMatTrans * camera_->GetViewMatrixInv() * camera_->GetProjectionMatrix();
		break;
	}
	case XY_BILLBOARD:
	{
		AliceMathF::Matrix4 lMatBillboard = camera_->GetViewMatrix();

		lMatBillboard.m[2][0] = 0;
		lMatBillboard.m[2][1] = 0;
		lMatBillboard.m[2][2] = 1;
		lMatBillboard.m[2][3] = 0;

		lMatBillboard.m[3][0] = 0;
		lMatBillboard.m[3][1] = 0;
		lMatBillboard.m[3][2] = 0;
		lMatBillboard.m[3][3] = 1;

		transform_.matWorld = lMatScale * lMatRot * lMatBillboard * lMatTrans * camera_->GetViewMatrixInv() * camera_->GetProjectionMatrix();
		break;
	}
	default:
		assert(0);
		break;
	}

	switch (blend_)
	{
	case BlendMode::AX_BLENDMODE_NOBLEND:
		spriteMaterial = MaterialManager::SGetMaterial("Sprite3DNoblend");
		break;
	case BlendMode::AX_BLENDMODE_ALPHA:
		spriteMaterial = MaterialManager::SGetMaterial("Sprite3DAlpha");
		break;
	case BlendMode::AX_BLENDMODE_ADD:
		spriteMaterial = MaterialManager::SGetMaterial("Sprite3DAdd");
		break;
	case BlendMode::AX_BLENDMODE_SUB:
		spriteMaterial = MaterialManager::SGetMaterial("Sprite3DSub");
		break;
	case BlendMode::AX_BLENDMODE_MULA:
		spriteMaterial = MaterialManager::SGetMaterial("Sprite3DMula");
		break;
	case BlendMode::AX_BLENDMODE_INVSRC:
		spriteMaterial = MaterialManager::SGetMaterial("Sprite3DInvsrc");
		break;
	case BlendMode::AX_BLENDMODE_MAX:
		spriteMaterial = MaterialManager::SGetMaterial("Sprite3DNoblend");
		//printf("ブレンドの種類数を入れています");
		break;
	case BlendMode::AX_BLENDMODE_CUSTOM:
		if (material_)
		{
			spriteMaterial = material_;
		}
		else
		{
			//printf("マテリアルがnullです");
			spriteMaterial = MaterialManager::SGetMaterial("Sprite3DNoblend");
		}
		break;
	default:
		//printf("ブレンドの範囲外を入れています");
		assert(0);
		break;
	}

	transform_.Update();

	PSpriteDraw(transform_, spriteMaterial);
}

void Sprite3D::Draw(Transform& transform_, BlendMode blend_, const Material* material_)
{
	float lIsFlipX, lIsFlipY;
	lIsFlipX = flipX ? -1.0f : 1.0f;
	lIsFlipY = flipY ? -1.0f : 1.0f;

	float lLeft = ((0.0f - anchorPoint.x) * (static_cast<float>(texture->width) * 0.0625f)) * lIsFlipX;
	float lRight = ((1.0f - anchorPoint.x) * (static_cast<float>(texture->width) * 0.0625f)) * lIsFlipX;
	float lTop = ((0.0f - anchorPoint.y) * (static_cast<float>(texture->height) * 0.0625f)) * lIsFlipY;
	float lBottom = ((1.0f - anchorPoint.y) * (static_cast<float>(texture->height) * 0.0625f)) * lIsFlipY;

	float lTexLeft = trimmingRange.x / static_cast<float>(texture->width);
	float lTexTop = trimmingRange.y / static_cast<float>(texture->height);
	float lTexRight = trimmingRange.z / static_cast<float>(texture->width);
	float lTexBottom = trimmingRange.w / static_cast<float>(texture->height);

	// 頂点データ
	PosUvColor lVertices[] =
	{//		x		y		z		u	v
		{{lLeft,lTop,0.0f},{lTexLeft,lTexTop},{color.x,color.y,color.z,color.w}},//左上インデックス0
		{{lLeft,lBottom,0.0f},{lTexLeft,lTexBottom},{color.x,color.y,color.z,color.w}},//左下インデックス1
		{{lRight,lTop,0.0f},{lTexRight,lTexTop},{color.x,color.y,color.z,color.w}},//右上インデックス2
		{{lRight,lBottom,0.0f},{lTexRight,lTexBottom},{color.x,color.y,color.z,color.w}},//右下インデックス3
	};

	// インデックスデータ
	uint32_t lIndices[] =
	{
		3, 0, 1, // 三角形1つ目
		0, 3, 2, // 三角形2つ目
	};

	//頂点バッファへのデータ転送
	vertexBuffer->Update(lVertices);

	//インデックスバッファへのデータ転送
	indexBuffer->Update(lIndices);

	switch (blend_)
	{
	case BlendMode::AX_BLENDMODE_NOBLEND:
		spriteMaterial = MaterialManager::SGetMaterial("Sprite3DNoblend");
		break;
	case BlendMode::AX_BLENDMODE_ALPHA:
		spriteMaterial = MaterialManager::SGetMaterial("Sprite3DAlpha");
		break;
	case BlendMode::AX_BLENDMODE_ADD:
		spriteMaterial = MaterialManager::SGetMaterial("Sprite3DAdd");
		break;
	case BlendMode::AX_BLENDMODE_SUB:
		spriteMaterial = MaterialManager::SGetMaterial("Sprite3DSub");
		break;
	case BlendMode::AX_BLENDMODE_MULA:
		spriteMaterial = MaterialManager::SGetMaterial("Sprite3DMula");
		break;
	case BlendMode::AX_BLENDMODE_INVSRC:
		spriteMaterial = MaterialManager::SGetMaterial("Sprite3DInvsrc");
		break;
	case BlendMode::AX_BLENDMODE_MAX:
		spriteMaterial = MaterialManager::SGetMaterial("Sprite3DNoblend");
		//printf("ブレンドの種類数を入れています");
		break;
	case BlendMode::AX_BLENDMODE_CUSTOM:
		if (material_)
		{
			spriteMaterial = material_;
		}
		else
		{
			//printf("マテリアルがnullです");
			spriteMaterial = MaterialManager::SGetMaterial("Sprite3DNoblend");
		}
		break;
	default:
		//printf("ブレンドの範囲外を入れています");
		assert(0);
		break;
	}

	PSpriteDraw(transform_, spriteMaterial);
}

void Sprite3D::AnimationDraw(Transform& transform_, uint16_t radiusX, uint16_t radiusY, float& frame, float frameDiv, BlendMode blend, const Material* material)
{
	size_t lAnimeFrame = static_cast<size_t>(frame / frameDiv);

	size_t lWidth = static_cast<size_t>(radiusX) * 2;
	size_t lHeight = static_cast<size_t>(radiusY) * 2;
	float lTexTop = trimmingRange.y / static_cast<float>(texture->height);
	float lTexRight = trimmingRange.z / static_cast<float>(texture->width);

	float widthU = static_cast<float>(lWidth) / (lTexRight * static_cast<float>(texture->width));

	//画像の半分のサイズ

	if (texture->width / lWidth < lAnimeFrame + 1)
	{
		frame = 0;
	}

	float lIsFlipX, lIsFlipY;
	lIsFlipX = flipX ? -1.0f : 1.0f;
	lIsFlipY = flipY ? -1.0f : 1.0f;

	float lLeft = ((0.0f - anchorPoint.x) * (static_cast<float>(lWidth / 10))) * lIsFlipX;
	float lRight = ((1.0f - anchorPoint.x) * (static_cast<float>(lWidth / 10))) * lIsFlipX;
	float lTop = ((0.0f - anchorPoint.y) * static_cast<float>(lHeight / 10)) * lIsFlipY;
	float lBottom = ((1.0f - anchorPoint.y) * static_cast<float>(lHeight / 10)) * lIsFlipY;

	// 頂点データ
	PosUvColor lVertices[] =
	{//		x		y		z		u	v
		{{lLeft, lTop, 0.0f},{widthU * static_cast<float>(lAnimeFrame),lTexTop},{1.0f,1.0f,1.0f,1.0f}},//左上インデックス0
		{{lLeft, lBottom, 0.0f},{widthU * static_cast<float>(lAnimeFrame),lTexRight},{1.0f,1.0f,1.0f,1.0f}},//左下インデックス1
		{{lRight, lTop, 0.0f},{widthU * static_cast<float>((lAnimeFrame + 1)),lTexTop},{1.0f,1.0f,1.0f,1.0f}},//右上インデックス2
		{{lRight, lBottom, 0.0f},{widthU * static_cast<float>((lAnimeFrame + 1)),lTexRight},{1.0f,1.0f,1.0f,1.0f}},//右下インデックス3
	};

	// インデックスデータ
	uint32_t lIndices[] =
	{
		3, 0, 1, // 三角形1つ目
		0, 3, 2, // 三角形2つ目
	};

	//頂点バッファへのデータ転送
	vertexBuffer->Update(lVertices);

	//インデックスバッファへのデータ転送
	indexBuffer->Update(lIndices);

	switch (blend)
	{
	case BlendMode::AX_BLENDMODE_NOBLEND:
		spriteMaterial = MaterialManager::SGetMaterial("Sprite3DNoblend");
		break;
	case BlendMode::AX_BLENDMODE_ALPHA:
		spriteMaterial = MaterialManager::SGetMaterial("Sprite3DAlpha");
		break;
	case BlendMode::AX_BLENDMODE_ADD:
		spriteMaterial = MaterialManager::SGetMaterial("Sprite3DAdd");
		break;
	case BlendMode::AX_BLENDMODE_SUB:
		spriteMaterial = MaterialManager::SGetMaterial("Sprite3DSub");
		break;
	case BlendMode::AX_BLENDMODE_MULA:
		spriteMaterial = MaterialManager::SGetMaterial("Sprite3DMula");
		break;
	case BlendMode::AX_BLENDMODE_INVSRC:
		spriteMaterial = MaterialManager::SGetMaterial("Sprite3DInvsrc");
		break;
	case BlendMode::AX_BLENDMODE_MAX:
		spriteMaterial = MaterialManager::SGetMaterial("Sprite3DNoblend");
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
			spriteMaterial = MaterialManager::SGetMaterial("Sprite3DNoblend");
		}
		break;
	default:
		//printf("ブレンドの範囲外を入れています");
		assert(0);
		break;
	}

	transform_.Update();


	PSpriteDraw(transform_, spriteMaterial);
}

void Sprite3D::AnimationDraw(Transform& transform_, uint16_t radiusX, uint16_t radiusY, float& frame, float frameDiv, Camera* camera_, BlendMode blend, BillboardFlag flag, const Material* material)
{
	AliceMathF::Matrix4 lMatTrans, lMatRot, lMatScale;

	//平行移動行列
	lMatTrans.MakeTranslation(transform_.translation);
	//回転行列
	lMatRot.MakeRotation(transform_.rotation);
	//スケール行列
	lMatScale.MakeScaling(transform_.scale);

	switch (flag)
	{
	case NON_BILLBOARD:
		transform_.matWorld = lMatScale * lMatRot * lMatTrans * camera_->GetViewMatrixInv() * camera_->GetProjectionMatrix();
		break;
	case ALL_BILLBOARD:
	{
		AliceMathF::Matrix4 lMatBillboard = camera_->GetViewMatrix();

		lMatBillboard.m[3][0] = 0;
		lMatBillboard.m[3][1] = 0;
		lMatBillboard.m[3][2] = 0;
		lMatBillboard.m[3][3] = 1;

		transform_.matWorld = lMatScale * lMatRot * lMatBillboard * lMatTrans * camera_->GetViewMatrixInv() * camera_->GetProjectionMatrix();
		break;
	}
	case X_BILLBOARD:
	{
		AliceMathF::Matrix4 lMatBillboard = camera_->GetViewMatrix();

		lMatBillboard.m[1][0] = 0;
		lMatBillboard.m[1][1] = 1;
		lMatBillboard.m[1][2] = 0;
		lMatBillboard.m[1][3] = 0;

		lMatBillboard.m[3][0] = 0;
		lMatBillboard.m[3][1] = 0;
		lMatBillboard.m[3][2] = 0;
		lMatBillboard.m[3][3] = 1;

		transform_.matWorld = lMatScale * lMatRot * lMatBillboard * lMatTrans * camera_->GetViewMatrixInv() * camera_->GetProjectionMatrix();
		break;
	}
	case Y_BILLBOARD:
	{
		AliceMathF::Matrix4 lMatBillboard = camera_->GetViewMatrix();

		lMatBillboard.m[0][0] = 1;
		lMatBillboard.m[0][1] = 0;
		lMatBillboard.m[0][2] = 0;
		lMatBillboard.m[0][3] = 0;

		lMatBillboard.m[3][0] = 0;
		lMatBillboard.m[3][1] = 0;
		lMatBillboard.m[3][2] = 0;
		lMatBillboard.m[3][3] = 1;


		transform_.matWorld = lMatScale * lMatRot * lMatBillboard * lMatTrans * camera_->GetViewMatrixInv() * camera_->GetProjectionMatrix();
		break;
	}
	case XY_BILLBOARD:
	{
		AliceMathF::Matrix4 lMatBillboard = camera_->GetViewMatrix();

		lMatBillboard.m[2][0] = 0;
		lMatBillboard.m[2][1] = 0;
		lMatBillboard.m[2][2] = 1;
		lMatBillboard.m[2][3] = 0;

		lMatBillboard.m[3][0] = 0;
		lMatBillboard.m[3][1] = 0;
		lMatBillboard.m[3][2] = 0;
		lMatBillboard.m[3][3] = 1;

		transform_.matWorld = lMatScale * lMatRot * lMatBillboard * lMatTrans * camera_->GetViewMatrixInv() * camera_->GetProjectionMatrix();
		break;
	}
	default:
		assert(0);
		break;
	}

	transform_.Update();

	AnimationDraw(transform_, radiusX, radiusY, frame, frameDiv, blend, material);
}

std::unique_ptr<Sprite3D> CreateUniqueSprite3D(uint32_t handle_)
{
	std::unique_ptr<Sprite3D>lSprite3D = std::make_unique<Sprite3D>();
	lSprite3D->Initialize(handle_);
	return std::move(lSprite3D);
}

std::shared_ptr<Sprite3D> CreateSharedSprite3D(uint32_t handle_)
{
	std::shared_ptr<Sprite3D> lSprite3D = std::make_unique<Sprite3D>();
	lSprite3D->Initialize(handle_);
	return lSprite3D;
}
