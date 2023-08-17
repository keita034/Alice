#include "FadeInTransition.h"

#include<Easing.h>

void FadeInTransition::Initilize(float windowHeight, float windowWidth)
{
	spriteHandle = TextureManager::SLoad("Resources/Default/Color1x1/black.png");
	sprite = CreateUniqueSprite2D(spriteHandle);
	spriteTransform.Initialize();
	spriteTransform.translation = { windowWidth * 0.5f,windowHeight * 0.5f ,0.0f };
	spriteTransform.scale = { windowWidth  ,windowHeight ,1.0f };

	easing = static_cast< float ( * )( float ) >( AliceMathF::Easing::EaseInSine );
}

void FadeInTransition::Finalize()
{
}

void FadeInTransition::Update()
{
	if (start && !isEnd)
	{
		frame += increment;

		frame = AliceMathF::Clamp01(frame);

		sprite->SetColor({ 1.0f,1.0f,1.0f, 1.0f - easing(frame) });

		if (frame >= 1.0f)
		{
			isEnd = true;
		}
	}
}


void FadeInTransition::Draw(bool draw_)
{
	if ( start && !isEnd )
	{
		sprite->Draw(spriteTransform);
	}

	if ( draw_ )
	{
		sprite->Draw(spriteTransform);
	}
}

bool FadeInTransition::Start()
{
	if (start)
	{
		return false;
	}

	start = true;
	frame = 0.0f;

	return true;
}
