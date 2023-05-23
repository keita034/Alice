#pragma once

#pragma warning(push)
#pragma warning(disable: 4365)
#pragma warning(disable: 4514)
#pragma warning(disable: 4619)
#pragma warning(disable: 4668)
#pragma warning(disable: 4710)
#pragma warning(disable: 5039)

#include<string>

#pragma warning(pop)

#include<BaseScene.h>

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

