#pragma once
#include<GameObject.h>

#pragma warning(push)
#pragma warning(disable: 4365)
#pragma warning(disable: 4514)
#pragma warning(disable: 4623)
#pragma warning(disable: 4625)
#pragma warning(disable: 4626)
#pragma warning(disable: 4820)
#pragma warning(disable: 5027)

#include<string>
#include <nlohmann/json.hpp>

#pragma warning(pop)


// レベルデータ
struct SceneData
{

	class Object : public GameObject
	{

	protected:


	public:

		/// <summary>
		/// 初期化
		/// </summary>
		virtual void Initialize(
			uint32_t handle_, 
			const AliceMathF::Vector3& pos_, 
			const AliceMathF::Vector3& rot_ = { 0.0f,0.0f,0.0f }, 
			const AliceMathF::Vector3& scl_ = { 1.0f,1.0f,1.0 },
			const Transform* parent_ = nullptr)override;

		/// <summary>
		/// 終了
		/// </summary>
		virtual void Finalize()override;

		/// <summary>
		/// 毎フレーム更新
		/// </summary>
		virtual void Update(Camera* camera_)override;

		/// <summary>
		/// 描画
		/// </summary>
		virtual void Draw()override;

		Object() = default;
		virtual ~Object() = default;

	private:

		virtual void Initialize()override;

		Object(const Object&) = delete;
		void operator = (const Object&) = delete;
	};

	// オブジェクト配列
	std::vector<std::unique_ptr<Object>> objects;

	void Update(Camera* camera_);

	void Draw();
};

class SceneLoader
{
private:

	static const std::string sBaseDirectorypath;

public:

	/// <summary>
	/// レベルデータファイルの読み込み
	/// </summary>
	/// <param name="fileName">ファイル名</param>
	static SceneData* SLoadFile(const std::string& filepath_);

private:

	static void SObjectScan(SceneData* sceneData_,const nlohmann::json& jsonObj_, SceneData::Object* parent_ = nullptr);
};

