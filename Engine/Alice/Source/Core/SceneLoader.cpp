#include "SceneLoader.h"

#include <fstream>
#include <cassert>

const std::string SceneLoader::sBaseDirectorypath = "Resources/";

std::unique_ptr<SceneData> SceneLoader::SLoadFile(const std::string& fileName_)
{
	std::ifstream lFile;

	lFile.open(fileName_);

	if (lFile.fail())
	{
		assert(0);
	}

	nlohmann::json lDeserialized;

	lFile >> lDeserialized;

	assert(lDeserialized.is_object());
	assert(lDeserialized.contains("name"));
	assert(lDeserialized["name"].is_string());

	std::string lName = lDeserialized["name"].get<std::string>();

	assert(lName.compare("scene") == 0);

	std::unique_ptr<SceneData> lSceneData = std::make_unique<SceneData>();

	for (nlohmann::json& jsonObject : lDeserialized["objects"])
	{
		SObjectScan(lSceneData.get(), jsonObject);
	}

	return std::move(lSceneData);
}

void SceneLoader::SObjectScan(SceneData* sceneData_, const nlohmann::json& jsonObj_, SceneData::Object* parent_)
{
	std::string lType = jsonObj_["type"].get<std::string>();

	if (lType.compare("MESH") == 0)
	{
		sceneData_->objects.emplace_back(std::make_unique<SceneData::Object>());

		std::unique_ptr<SceneData::Object>& lObject = sceneData_->objects.back();

		std::string lFileName;
		std::string lObjectName;

		//オブジェクトの名前
		if (jsonObj_.contains("name"))
		{
			lObjectName = jsonObj_["name"];
		}

		//ファイルネーム
		if (jsonObj_.contains("file_name"))
		{
			lFileName = jsonObj_["file_name"];
		}
		else
		{
			lFileName = "Default/cube";
		}

		// トランスフォームのパラメータ読み込み
		const nlohmann::json& lTransform = jsonObj_["transform"];

		AliceMathF::Vector3 lTranslation;
		AliceMathF::Vector3 lRotation;
		AliceMathF::Vector3 lScaling;

		// 平行移動
		lTranslation.x = static_cast<float>(lTransform["translation"][1]);
		lTranslation.y = static_cast<float>(lTransform["translation"][2]);
		lTranslation.z = -static_cast<float>(lTransform["translation"][0]);

		// 回転角
		lRotation.x = -static_cast<float>(lTransform["rotation"][1]);
		lRotation.y = -static_cast<float>(lTransform["rotation"][2]);
		lRotation.z = static_cast<float>(lTransform["rotation"][0]);

		// スケーリング
		lScaling.x = static_cast<float>(lTransform["scaling"][1]);
		lScaling.y = static_cast<float>(lTransform["scaling"][2]);
		lScaling.z = static_cast<float>(lTransform["scaling"][0]);

		//モデル読み込み
		uint32_t lHandle = AliceModel::SCreateModel(sBaseDirectorypath + lFileName);

		//オブジェクト生成
		if (parent_)
		{
			lObject->Initialize(lHandle, lTranslation, lRotation, lScaling, parent_->GetTransformPtr());
		}
		else
		{
			lObject->Initialize(lHandle, lTranslation, lRotation, lScaling);
		}

		lObject->SetName(lObjectName);

		//子オブジェクト再帰関数
		if (jsonObj_.contains("children"))
		{
			for (const nlohmann::json& json : jsonObj_["children"])
			{
				SObjectScan(sceneData_, json, lObject.get());
			}
		}
	}
}

void SceneData::Object::Initialize(uint32_t handle_, const AliceMathF::Vector3& pos_, const AliceMathF::Vector3& rot_, const AliceMathF::Vector3& scl_, const Transform* parent_)
{
	model = std::make_unique<AliceModel>();
	model->SetModel(handle_);
	transform.Initialize();
	transform.translation = pos_;
	transform.rotation = rot_;
	transform.scale = scl_;

	if (parent_)
	{
		transform.parent = parent_;
	}
}

void SceneData::Object::Finalize()
{
}

void SceneData::Object::Update(Camera* camera)
{
	transform.TransUpdate(camera);
}

void SceneData::Object::Draw()
{
	model->Draw(transform);
}

void SceneData::Object::Initialize()
{
}

void SceneData::Update(Camera* camera_)
{
	for (std::unique_ptr<Object>& object : objects)
	{
		object->Update(camera_);
	}
}

void SceneData::Draw()
{
	for (std::unique_ptr<Object>& object : objects)
	{
		object->Draw();
	}
}
