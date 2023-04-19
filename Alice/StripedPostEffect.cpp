#include "StripedPostEffect.h"

void StripedPostEffect::Initialize()
{
	if (needsInit)
	{
		needsInit = false;

		type = "STRIPEDPOSTEFFECT";



	}
}


void StripedPostEffect::PostUpdate(RenderTarget* mainRenderTarget)
{
}

const std::string& StripedPostEffect::GetType()
{
	return type;
}

void StripedPostEffect::MainRenderTargetDraw(RenderTarget* mainRenderTarget)
{
}
