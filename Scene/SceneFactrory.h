#pragma once
#include"AbstractSceneFactory.h"

/// <summary>
/// シーン工場
/// </summary>
class SceneFactrory:public AbstractSceneFactory
{
public:

	static SceneFactrory* GetInstance();

	/// <summary>
	/// シーン生成
	/// </summary>
	/// <param name="sceneName">シーン名</param>
	/// <returns>生成したシーン</returns>
	std::unique_ptr<BaseScene> CreateScene(const std::string& sceneName) override;
};

