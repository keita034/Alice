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
		virtual void Initialize(uint32_t handle, const AliceMathF::Vector3& pos, const AliceMathF::Vector3& rot = { 0.0f,0.0f,0.0f }, const AliceMathF::Vector3& scl = { 1.0f,1.0f,1.0 },const Transform* parent = nullptr)override;

		/// <summary>
		/// 終了
		/// </summary>
		virtual void Finalize()override;

		/// <summary>
		/// 毎フレーム更新
		/// </summary>
		virtual void Update(Camera* camera)override;

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

	void Update(Camera* camera);

	void Draw();
};

class SceneLoader
{
private:

	static const std::string baseDirectorypath;

public:

	/// <summary>
	/// レベルデータファイルの読み込み
	/// </summary>
	/// <param name="fileName">ファイル名</param>
	static SceneData* LoadFile(const std::string& filepath);

private:

	static void ObjectScan(SceneData* sceneData, nlohmann::json& jsonObj, SceneData::Object* parent = nullptr);
};

