#include<Sprite2D.h>
#include<WindowsApp.h>

Sprite2D::Sprite2D()
{
}

Sprite2D::~Sprite2D()
{
}

void Sprite2D::Initialize(uint32_t handle)
{
	SpriteInitialize();

	float width = static_cast<float>(WindowsApp::GetInstance()->GetWindowSize().width);
	float height = static_cast<float>(WindowsApp::GetInstance()->GetWindowSize().height);

	AliceMathF::MakeOrthogonalLOffCenter(0.0f, width, height, 0.0f, 0.0f, 1.0f, matProjection);

	texture = TextureManager::GetTextureData(handle);

	trimmingRange.z = static_cast<float>(texture->width);
	trimmingRange.w = static_cast<float>(texture->height);

	spriteSize.x = static_cast<float>(texture->width);
	spriteSize.y = static_cast<float>(texture->height);

	CreatVertexIndexBuffer();
}

//描画
void Sprite2D::Draw(Transform& transform, BlendMode blend, Material* material)
{
	float isFlipX, isFlipY;
	isFlipX = flipX ? -1.0f : 1.0f;
	isFlipY = flipY ? -1.0f : 1.0f;

	float left = ((0.0f - anchorPoint.x) * static_cast<float>(spriteSize.x)) * isFlipX;
	left *= leftScale;

	float right = ((1.0f - anchorPoint.x) * static_cast<float>(spriteSize.x)) * isFlipX;
	right *= rightScale;
	
	float top = ((0.0f - anchorPoint.y) * static_cast<float>(spriteSize.y)) * isFlipY;
	top *= topScale;
	
	float bottom = ((1.0f - anchorPoint.y) * static_cast<float>(spriteSize.y)) * isFlipY;
	bottom *= bottomScale;

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
		1, 0, 3, // 三角形1つ目
		2, 3, 0, // 三角形2つ目
	};

	//頂点バッファへのデータ転送
	vertexBuffer->Update(vertices);

	//インデックスバッファへのデータ転送
	indexBuffer->Update(indices);

	AliceMathF::Matrix4 mTrans, mRot, mScale, matWorld;
	//スケーリング倍率
	mScale.MakeScaling(transform.scale);
	//回転行列
	mRot.MakeRotation(transform.rotation);
	// matWorld_に移動量を掛け算
	mTrans.MakeTranslation(transform.translation);
	//ワールド行列
	matWorld = mScale * mRot * mTrans;

	//親行列の指定がある場合は、掛け算する
	if (transform.parent)
	{
		matWorld *= transform.parent->matWorld;
	}

	transform.matWorld = matWorld ;

	transform.GetWorldViewpojCamera()->matWorld = transform.matWorld * matProjection;

	transform.Update();

	switch (blend)
	{
	case BlendMode::AX_BLENDMODE_NOBLEND:
		spriteMaterial = MaterialManager::GetMaterial("Sprite2DNoblend");
		break;
	case BlendMode::AX_BLENDMODE_ALPHA:
		spriteMaterial = MaterialManager::GetMaterial("Sprite2DAlpha");
		break;
	case BlendMode::AX_BLENDMODE_ADD:
		spriteMaterial = MaterialManager::GetMaterial("Sprite2DAdd");
		break;
	case BlendMode::AX_BLENDMODE_SUB:
		spriteMaterial = MaterialManager::GetMaterial("Sprite2DSub");
		break;
	case BlendMode::AX_BLENDMODE_MULA:
		spriteMaterial = MaterialManager::GetMaterial("Sprite2DMula");
		break;
	case BlendMode::AX_BLENDMODE_INVSRC:
		spriteMaterial = MaterialManager::GetMaterial("Sprite2DInvsrc");
		break;
	case BlendMode::AX_BLENDMODE_MAX:
		spriteMaterial = MaterialManager::GetMaterial("Sprite2DNoblend");
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
			spriteMaterial = MaterialManager::GetMaterial("Sprite2DNoblend");
		}
		break;
	default:
		//printf("ブレンドの範囲外を入れています");
		assert(0);
		break;
	}

	SpriteDraw(transform, spriteMaterial);
}

void Sprite2D::AnimationDraw(Transform& transform, uint16_t radiusX, uint16_t radiusY, float& frame, float frameDiv, BlendMode blend, Material* material)
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

	float left = ((0.0f - anchorPoint.x) * static_cast<float>(width)) * isFlipX;
	left *= leftScale;
	float right = ((1.0f - anchorPoint.x) * static_cast<float>(width)) * isFlipX;
	right *= rightScale;
	float top = ((0.0f - anchorPoint.y) * static_cast<float>(height)) * isFlipY;
	top *= topScale;
	float bottom = ((1.0f - anchorPoint.y) * static_cast<float>(height)) * isFlipY;
	bottom *= bottomScale;

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
		1, 0, 3, // 三角形1つ目
		2, 3, 0, // 三角形2つ目
	};

	//頂点バッファへのデータ転送
	vertexBuffer->Update(vertices);

	//インデックスバッファへのデータ転送
	indexBuffer->Update(indices);

	AliceMathF::Matrix4 mTrans, mRot, mScale, matWorld;
	//スケーリング倍率
	mScale.MakeScaling(transform.scale);
	//回転行列
	mRot.MakeRotation(transform.rotation);
	// matWorld_に移動量を掛け算
	mTrans.MakeTranslation(transform.translation);
	//ワールド行列
	matWorld = mScale * mRot * mTrans;

	transform.matWorld = matWorld * matProjection;

	transform.GetWorldViewpojCamera()->matWorld = transform.matWorld;
	transform.Update();

	switch (blend)
	{
	case BlendMode::AX_BLENDMODE_NOBLEND:
		spriteMaterial = MaterialManager::GetMaterial("Sprite2DNoblend");
		break;
	case BlendMode::AX_BLENDMODE_ALPHA:
		spriteMaterial = MaterialManager::GetMaterial("Sprite2DAlpha");
		break;
	case BlendMode::AX_BLENDMODE_ADD:
		spriteMaterial = MaterialManager::GetMaterial("Sprite2DAdd");
		break;
	case BlendMode::AX_BLENDMODE_SUB:
		spriteMaterial = MaterialManager::GetMaterial("Sprite2DSub");
		break;
	case BlendMode::AX_BLENDMODE_MULA:
		spriteMaterial = MaterialManager::GetMaterial("Sprite2DMula");
		break;
	case BlendMode::AX_BLENDMODE_INVSRC:
		spriteMaterial = MaterialManager::GetMaterial("Sprite2DInvsrc");
		break;
	case BlendMode::AX_BLENDMODE_MAX:
		spriteMaterial = MaterialManager::GetMaterial("Sprite2DNoblend");
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
			spriteMaterial = MaterialManager::GetMaterial("Sprite2DNoblend");
		}
		break;
	default:
		//printf("ブレンドの範囲外を入れています");
		assert(0);
		break;
	}

	SpriteDraw(transform, spriteMaterial);
}

float Sprite2D::GetLeftSize()
{
	float size = AliceMathF::Abs((0.0f - anchorPoint.x) * static_cast<float>(texture->width) * leftScale);
  	return size;
}

float Sprite2D::GetRightSize()
{
	float size = AliceMathF::Abs((1.0f - anchorPoint.x) * static_cast<float>(texture->width) * rightScale);
	return size;
}

float Sprite2D::GetTopSize()
{
	float size = AliceMathF::Abs((0.0f - anchorPoint.y) * static_cast<float>(texture->height) * topScale);
	return size;
}

float Sprite2D::GetBottomSize()
{
	float size = AliceMathF::Abs((1.0f - anchorPoint.y) * static_cast<float>(texture->height) * bottomScale);
	return size;
}

void Sprite2D::SetLeftScale(float scl)
{
	leftScale = scl;
}

void Sprite2D::SetRightScale(float scl)
{
	rightScale = scl;
}

void Sprite2D::SetTopScale(float scl)
{
	topScale = scl;
}

void Sprite2D::SetBottomScale(float scl)
{
	bottomScale = scl;

}

float Sprite2D::GetHeight()
{
	return GetTopSize() + GetBottomSize();
}

float Sprite2D::GetWidth()
{
	return GetLeftSize() + GetRightSize();
}
