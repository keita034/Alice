#pragma once
#include<BasePostEffect.h>

class AbstractPostEffectFactory
{
public:

	virtual ~AbstractPostEffectFactory() = default;

	//シーン生成
	virtual BasePostEffect* CreatePostEffect(const std::string& postEffectName) = 0;
};

