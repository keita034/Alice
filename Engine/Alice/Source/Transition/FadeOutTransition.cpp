#include "FadeOutTransition.h"
#include "FadeOutTransition.h"

#include<Easing.h>

void FadeOutTransition::Initilize(float windowHeight, float windowWidth)
{
	spriteHandle = TextureManager::SLoad("Resources/Default/Color1x1/black.png");
	sprite = CreateUniqueSprite2D(spriteHandle);
	spriteTransform.Initialize();
	spriteTransform.translation = { windowWidth * 0.5f,windowHeight * 0.5f ,0.0f };
	spriteTransform.scale = { windowWidth  ,windowHeight ,1.0f };

	easing = easing = static_cast< float ( * )( float ) >( AliceMathF::Easing::EaseInSine );
}

void FadeOutTransition::Finalize()
{
}

void FadeOutTransition::Update()
{
	if (start && !isEnd)
	{
		frame += increment;

		frame = AliceMathF::Clamp01(frame);

		alpha = 1.0f * easing(frame);

		sprite->SetColor({ 1.0f,1.0f,1.0f, alpha });

		if (frame >= 1.0f)
		{
			isEnd = true;
		}
	}
}


void FadeOutTransition::Draw(bool draw_)
{
	if (start&& !isEnd)
	{
		sprite->Draw(spriteTransform);
	}

	if ( start && draw_ )
	{
		sprite->Draw(spriteTransform);
	}
}

bool FadeOutTransition::Start()
{
	if (start && !isEnd)
	{
		return false;
	}

	start = true;
	frame = 0.0f;

	return true;
}

float FadeOutTransition::GetCoefficient()
{
	return alpha;
}
