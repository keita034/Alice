#include<Sprite2D.h>
#include<PosUvColor.h>

Sprite2D::Sprite2D()
{
}

Sprite2D::~Sprite2D()
{
}

void Sprite2D::Initialize(uint32_t handle_)
{

	float lWidth = static_cast<float>(sWindowsApp->GetWindowSize().width);
	float lHeight = static_cast<float>(sWindowsApp->GetWindowSize().height);

	AliceMathF::MakeOrthogonalLOffCenter(0.0f, lWidth, lHeight, 0.0f, 0.0f, 1.0f, matProjection);

	texture = TextureManager::SGetTextureData(handle_);

	trimmingRange.z = static_cast<float>(texture->width);
	trimmingRange.w = static_cast<float>(texture->height);

	spriteSize.x = static_cast<float>(texture->width);
	spriteSize.y = static_cast<float>(texture->height);

	PCreatVertexIndexBuffer();
}

//描画
void Sprite2D::Draw(Transform& transform_, BlendMode blend_, const Material* material_)
{
	float lIsFlipX, lIsFlipY;
	lIsFlipX = flipX ? -1.0f : 1.0f;
	lIsFlipY = flipY ? -1.0f : 1.0f;

	float lLeft = ((0.0f - anchorPoint.x) * static_cast<float>(spriteSize.x)) * lIsFlipX;
	lLeft *= leftScale;

	float lRight = ((1.0f - anchorPoint.x) * static_cast<float>(spriteSize.x)) * lIsFlipX;
	lRight *= rightScale;
	
	float lTop = ((0.0f - anchorPoint.y) * static_cast<float>(spriteSize.y)) * lIsFlipY;
	lTop *= topScale;
	
	float lBottom = ((1.0f - anchorPoint.y) * static_cast<float>(spriteSize.y)) * lIsFlipY;
	lBottom *= bottomScale;

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
		1, 0, 3, // 三角形1つ目
		2, 3, 0, // 三角形2つ目
	};

	//頂点バッファへのデータ転送
	vertexBuffer->Update(lVertices);

	//インデックスバッファへのデータ転送
	indexBuffer->Update(lIndices);

	AliceMathF::Matrix4 lMatTrans, lMatRot, lMatScale, lMatWorld;
	//スケーリング倍率
	lMatScale.MakeScaling(transform_.scale);
	//回転行列
	lMatRot.MakeRotation(transform_.rotation);
	// matWorld_に移動量を掛け算
	lMatTrans.MakeTranslation(transform_.translation);
	//ワールド行列
	lMatWorld = lMatScale * lMatRot * lMatTrans;

	//親行列の指定がある場合は、掛け算する
	if (transform_.parent)
	{
		lMatWorld *= transform_.parent->matWorld;
	}

	transform_.matWorld = lMatWorld;

	transform_.GetWorldViewpojCamera()->matWorld = transform_.matWorld * matProjection;

	transform_.Update();

	switch (blend_)
	{
	case BlendMode::AX_BLENDMODE_NOBLEND:
		spriteMaterial = MaterialManager::SGetMaterial("Sprite2DNoblend");
		break;
	case BlendMode::AX_BLENDMODE_ALPHA:
		spriteMaterial = MaterialManager::SGetMaterial("Sprite2DAlpha");
		break;
	case BlendMode::AX_BLENDMODE_ADD:
		spriteMaterial = MaterialManager::SGetMaterial("Sprite2DAdd");
		break;
	case BlendMode::AX_BLENDMODE_SUB:
		spriteMaterial = MaterialManager::SGetMaterial("Sprite2DSub");
		break;
	case BlendMode::AX_BLENDMODE_MULA:
		spriteMaterial = MaterialManager::SGetMaterial("Sprite2DMula");
		break;
	case BlendMode::AX_BLENDMODE_INVSRC:
		spriteMaterial = MaterialManager::SGetMaterial("Sprite2DInvsrc");
		break;
	case BlendMode::AX_BLENDMODE_MAX:
		spriteMaterial = MaterialManager::SGetMaterial("Sprite2DNoblend");
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
			spriteMaterial = MaterialManager::SGetMaterial("Sprite2DNoblend");
		}
		break;
	default:
		//printf("ブレンドの範囲外を入れています");
		assert(0);
		break;
	}

	PSpriteDraw(transform_, spriteMaterial);
}

void Sprite2D::AnimationDraw(Transform& transform_, uint16_t radiusX, uint16_t radiusY, float& frame, float frameDiv, BlendMode blend_, const Material* material_)
{
	size_t lAnimeFrame = static_cast<size_t>(frame / frameDiv);

	size_t lWidth = static_cast<size_t>(radiusX) * 2;
	size_t lHeight = static_cast<size_t>(radiusY) * 2;

	float lTexTop = trimmingRange.y / static_cast<float>(texture->height);
	float lTexRight = trimmingRange.z / static_cast<float>(texture->width);

	float lWidthU = static_cast<float>(lWidth) / (lTexRight * static_cast<float>(texture->width));

	//画像の半分のサイズ

	if (texture->width / lWidth < lAnimeFrame + 1)
	{
		frame = 0;
	}

	float lIsFlipX, lIsFlipY;
	lIsFlipX = flipX ? -1.0f : 1.0f;
	lIsFlipY = flipY ? -1.0f : 1.0f;

	float lLeft = ((0.0f - anchorPoint.x) * static_cast<float>(lWidth)) * lIsFlipX;
	lLeft *= leftScale;
	float lRight = ((1.0f - anchorPoint.x) * static_cast<float>(lWidth)) * lIsFlipX;
	lRight *= rightScale;
	float lTop = ((0.0f - anchorPoint.y) * static_cast<float>(lHeight)) * lIsFlipY;
	lTop *= topScale;
	float lBottom = ((1.0f - anchorPoint.y) * static_cast<float>(lHeight)) * lIsFlipY;
	lBottom *= bottomScale;

	// 頂点データ
	PosUvColor lVertices[] =
	{//		x		y		z		u	v
		{{lLeft, lTop, 0.0f},{lWidthU * static_cast<float>(lAnimeFrame),lTexTop},{1.0f,1.0f,1.0f,1.0f}},//左上インデックス0
		{{lLeft, lBottom, 0.0f},{lWidthU * static_cast<float>(lAnimeFrame),lTexRight},{1.0f,1.0f,1.0f,1.0f}},//左下インデックス1
		{{lRight, lTop, 0.0f},{lWidthU * static_cast<float>((lAnimeFrame + 1)),lTexTop},{1.0f,1.0f,1.0f,1.0f}},//右上インデックス2
		{{lRight, lBottom, 0.0f},{lWidthU * static_cast<float>((lAnimeFrame + 1)),lTexRight},{1.0f,1.0f,1.0f,1.0f}},//右下インデックス3
	};

	// インデックスデータ
	uint32_t lIndices[] =
	{
		1, 0, 3, // 三角形1つ目
		2, 3, 0, // 三角形2つ目
	};

	//頂点バッファへのデータ転送
	vertexBuffer->Update(lVertices);

	//インデックスバッファへのデータ転送
	indexBuffer->Update(lIndices);

	AliceMathF::Matrix4 lMatTrans, lMatRot, lMatScale, lMatWorld;
	//スケーリング倍率
	lMatScale.MakeScaling(transform_.scale);
	//回転行列
	lMatRot.MakeRotation(transform_.rotation);
	// matWorld_に移動量を掛け算
	lMatTrans.MakeTranslation(transform_.translation);
	//ワールド行列
	lMatWorld = lMatScale * lMatRot * lMatTrans;

	transform_.matWorld = lMatWorld * matProjection;

	transform_.GetWorldViewpojCamera()->matWorld = transform_.matWorld;
	transform_.Update();

	switch (blend_)
	{
	case BlendMode::AX_BLENDMODE_NOBLEND:
		spriteMaterial = MaterialManager::SGetMaterial("Sprite2DNoblend");
		break;
	case BlendMode::AX_BLENDMODE_ALPHA:
		spriteMaterial = MaterialManager::SGetMaterial("Sprite2DAlpha");
		break;
	case BlendMode::AX_BLENDMODE_ADD:
		spriteMaterial = MaterialManager::SGetMaterial("Sprite2DAdd");
		break;
	case BlendMode::AX_BLENDMODE_SUB:
		spriteMaterial = MaterialManager::SGetMaterial("Sprite2DSub");
		break;
	case BlendMode::AX_BLENDMODE_MULA:
		spriteMaterial = MaterialManager::SGetMaterial("Sprite2DMula");
		break;
	case BlendMode::AX_BLENDMODE_INVSRC:
		spriteMaterial = MaterialManager::SGetMaterial("Sprite2DInvsrc");
		break;
	case BlendMode::AX_BLENDMODE_MAX:
		spriteMaterial = MaterialManager::SGetMaterial("Sprite2DNoblend");
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
			spriteMaterial = MaterialManager::SGetMaterial("Sprite2DNoblend");
		}
		break;
	default:
		//printf("ブレンドの範囲外を入れています");
		assert(0);
		break;
	}

	PSpriteDraw(transform_, spriteMaterial);
}

float Sprite2D::GetLeftSize()
{
	float lSize = AliceMathF::Abs((0.0f - anchorPoint.x) * static_cast<float>(texture->width) * leftScale);
  	return lSize;
}

float Sprite2D::GetRightSize()
{
	float lSize = AliceMathF::Abs((1.0f - anchorPoint.x) * static_cast<float>(texture->width) * rightScale);
	return lSize;
}

float Sprite2D::GetTopSize()
{
	float lSize = AliceMathF::Abs((0.0f - anchorPoint.y) * static_cast<float>(texture->height) * topScale);
	return lSize;
}

float Sprite2D::GetBottomSize()
{
	float lSize = AliceMathF::Abs((1.0f - anchorPoint.y) * static_cast<float>(texture->height) * bottomScale);
	return lSize;
}

void Sprite2D::SetLeftScale(float scale_)
{
	leftScale = scale_;
}

void Sprite2D::SetRightScale(float scale_)
{
	rightScale = scale_;
}

void Sprite2D::SetTopScale(float scale_)
{
	topScale = scale_;
}

void Sprite2D::SetBottomScale(float scale_)
{
	bottomScale = scale_;

}

float Sprite2D::GetHeight()
{
	return GetTopSize() + GetBottomSize();
}

float Sprite2D::GetWidth()
{
	return GetLeftSize() + GetRightSize();
}

std::unique_ptr<Sprite2D> CreateUniqueSprite2D(uint32_t handle_)
{
	std::unique_ptr<Sprite2D>lSprite2D = std::make_unique<Sprite2D>();
	lSprite2D->Initialize(handle_);
	return std::move(lSprite2D);
}

std::shared_ptr<Sprite2D> CreateSharedSprite2D(uint32_t handle_)
{
	std::shared_ptr<Sprite2D> lSprite2D = std::make_unique<Sprite2D>();
	lSprite2D->Initialize(handle_);
	return lSprite2D;
}
