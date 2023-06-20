#pragma once
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

#include<GameObject.h>
#include<Input.h>

// レベルデータ
class SceneData
{
public:
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
		Object& operator = (const Object&) = delete;
	};

	// オブジェクト配列
	std::vector<std::unique_ptr<Object>> objects;
	//カメラ
	std::unique_ptr<GameCamera>camera;
	//ライト
	std::unique_ptr<Light>light;

	std::string sceneDataFilepath;

	void Update(Camera* camera_ = nullptr);

	void Draw();

	SceneData() = default;
	~SceneData() = default;

	static void SSetInput(AliceInput::IInput* input_);

private:

	static AliceInput::IInput* input;

	//コピーコンストラクタ・代入演算子削除
	SceneData& operator=(const SceneData&) = delete;
	SceneData(const SceneData&) = delete;

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
	static std::unique_ptr<SceneData> SLoadFile(const std::string& filepath_);

	static void SLoadFile(SceneData* sceneData_);

private:

	static void SObjectScan(SceneData* sceneData_,const nlohmann::json& jsonObj_, SceneData::Object* parent_ = nullptr);
};

