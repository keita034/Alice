#include "SceneLoader.h"

#include <fstream>
#include <cassert>

const std::string SceneLoader::baseDirectorypath = "Resources/";

SceneData* SceneLoader::LoadFile(const std::string& fileName)
{
	std::ifstream file;

	file.open(fileName);

	if (file.fail())
	{
		assert(0);
	}

	nlohmann::json deserialized;

	file >> deserialized;

	assert(deserialized.is_object());
	assert(deserialized.contains("name"));
	assert(deserialized["name"].is_string());

	std::string name = deserialized["name"].get<std::string>();

	assert(name.compare("scene") == 0);

	SceneData* sceneData = new SceneData();

	for (nlohmann::json& jsonObject : deserialized["objects"])
	{
		ObjectScan(sceneData, jsonObject);
	}

	return sceneData;
}

void SceneLoader::ObjectScan(SceneData* sceneData, nlohmann::json& jsonObj, SceneData::Object* parent)
{
	std::string type = jsonObj["type"].get<std::string>();

	if (type.compare("MESH") == 0)
	{
		sceneData->objects.emplace_back(std::make_unique<SceneData::Object>());

		std::unique_ptr<SceneData::Object>& object = sceneData->objects.back();

		std::string fileName;
		std::string objectName;
		AliceMathF::Vector3 translation;
		AliceMathF::Vector3 rotation;
		AliceMathF::Vector3 scaling;

		//オブジェクトの名前
		if (jsonObj.contains("name"))
		{
			objectName = jsonObj["name"];
		}

		//ファイルネーム
		if (jsonObj.contains("file_name"))
		{
			fileName = jsonObj["file_name"];
		}
		else
		{
			fileName = "Default/cube";
		}

		// トランスフォームのパラメータ読み込み
		nlohmann::json& transform = jsonObj["transform"];

		// 平行移動
		translation.x = static_cast<float>(transform["translation"][1]);
		translation.y = static_cast<float>(transform["translation"][2]);
		translation.z = -static_cast<float>(transform["translation"][0]);

		// 回転角
		rotation.x = -static_cast<float>(transform["rotation"][1]);
		rotation.y = -static_cast<float>(transform["rotation"][2]);
		rotation.z = static_cast<float>(transform["rotation"][0]);

		// スケーリング
		scaling.x = static_cast<float>(transform["scaling"][1]);
		scaling.y = static_cast<float>(transform["scaling"][2]);
		scaling.z = static_cast<float>(transform["scaling"][0]);

		//モデル読み込み
		uint32_t handle = AliceModel::CreateModel(baseDirectorypath + fileName);

		//オブジェクト生成
		if (parent)
		{
			object->Initialize(handle, translation, rotation, scaling, parent->GetTransformPtr());
		}
		else
		{
			object->Initialize(handle, translation, rotation, scaling);
		}

		object->SetName(objectName);

		//子オブジェクト再帰関数
		if (jsonObj.contains("children"))
		{
			for (nlohmann::json& json : jsonObj["children"])
			{
				ObjectScan(sceneData, json, object.get());
			}
		}
	}
}

void SceneData::Object::Initialize(uint32_t handle, const AliceMathF::Vector3& pos, const AliceMathF::Vector3& rot, const AliceMathF::Vector3& scl, const Transform* parent)
{
	model = std::make_unique<AliceModel>();
	model->SetModel(handle);
	transform.Initialize();
	transform.translation = pos;
	transform.rotation = rot;
	transform.scale = scl;

	if (parent)
	{
		transform.parent = parent;
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

void SceneData::Update(Camera* camera)
{
	for (std::unique_ptr<Object>& object : objects)
	{
		object->Update(camera);
	}
}

void SceneData::Draw()
{
	for (std::unique_ptr<Object>& object : objects)
	{
		object->Draw();
	}
}
