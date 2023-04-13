#pragma once
#include"ErrorException.h"
#include"BaseScene.h"

/// <summary>
/// シーン工場(概念)
/// </summary>
class AbstractSceneFactory
{
public:

	virtual ~AbstractSceneFactory() = default;

	//シーン生成
	virtual BaseScene* CreateScene(const std::string& sceneName) = 0;
};

