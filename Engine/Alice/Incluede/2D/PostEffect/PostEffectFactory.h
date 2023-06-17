#pragma once
#include<AbstractPostEffectFactory.h>

/// <summary>
/// ポストエフェクト工場
/// </summary>
class PostEffectFactory final : public AbstractPostEffectFactory
{
public:

	static PostEffectFactory* GetInstance();

	/// <summary>
	/// ポストエフェクト生成
	/// </summary>
	/// <param name="sceneName">ポストエフェクト名</param>
	/// <returns>生成したポストエフェクト</returns>
	BasePostEffect* CreatePostEffect(const std::string& postEffectName_) override;

private:

	PostEffectFactory();
	~PostEffectFactory() = default;

	//コピーコンストラクタ・代入演算子削除
	PostEffectFactory& operator=(const PostEffectFactory&) = delete;
	PostEffectFactory(const PostEffectFactory&) = delete;
};

