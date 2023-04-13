#include "Button.h"

Button::Button()
{
	input = Input::GetInstance();
	mesh = Mesh::GetInstance();

}

Button::Button(uint32_t handle)
{
	input = Input::GetInstance();
	mesh = Mesh::GetInstance();

	sprite.reset(Sprite::Create2DSprite(handle));
}

Button::~Button()
{
}

void Button::DrawButton(float x, float y, float radiusX, float radiusY, AliceMathF::Vector4 color, float posX, float posY, bool fillFlag, int& sceneFlag, int sceneNextNum)
{
	if (x - radiusX <= posX && x + radiusX >= posX && y - radiusY <= posY && y + radiusY >= posY)
	{
		if (input->TriggerPush(DIK_SPACE) || input->MouseButtonTrigger(LEFT) || input->ButtonTrigger(A))
		{
			sceneFlag = sceneNextNum;
		}

		mesh->DrawBox(x, y, radiusX, radiusY, 0.0f, { color.x - 50,color.y - 50,color.z - 50,color.w }, fillFlag);
	}
	else
	{
		mesh->DrawBox(x, y, radiusX, radiusY, 0.0f, color, fillFlag);
	}
}

void Button::DrawGraphButton(Transform& transform, float posX, float posY, int& sceneFlag, int sceneNextNum)
{
	if (transform.translation.x - static_cast<float>(textureData.width) / 2.0f <= posX &&
		transform.translation.x + static_cast<float>(textureData.width) / 2.0f >= posX &&
		transform.translation.y - static_cast<float>(textureData.height) / 2 <= posY &&
		transform.translation.y + static_cast<float>(textureData.height) / 2 >= posY)
	{
		if (input->TriggerPush(DIK_SPACE) || input->MouseButtonTrigger(LEFT) || input->ButtonTrigger(A))
		{
			sceneFlag = sceneNextNum;
		}
	}
	sprite->Draw(transform);
}