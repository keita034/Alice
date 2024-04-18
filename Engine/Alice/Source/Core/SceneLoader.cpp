#include "SceneLoader.h"
#include<MeshShape.h>

#include <fstream>
#include <cassert>

const std::string SceneLoader::sBaseDirectorypath = "Resources/";
AliceInput::IInput* SceneData::input = nullptr;
std::unique_ptr<SceneData> SceneLoader::SLoadFile(AlicePhysics::AlicePhysicsSystem* physicsSystem_,const std::string& filepath_)
{
	std::ifstream lFile;

	lFile.open(filepath_);

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

	lSceneData->sceneDataFilepath = filepath_;

	for (nlohmann::json& jsonObject : lDeserialized["objects"])
	{
		SObjectScan(physicsSystem_,lSceneData.get(), jsonObject);
	}

	return std::move(lSceneData);
}

void SceneLoader::SLoadFile(AlicePhysics::AlicePhysicsSystem* physicsSystem_,SceneData* sceneData_)
{
	std::ifstream lFile;

	lFile.open(sceneData_->sceneDataFilepath);

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

	for (nlohmann::json& jsonObject : lDeserialized["objects"])
	{
		SObjectScan(physicsSystem_,sceneData_, jsonObject);
	}
}

void SceneLoader::SObjectScan(AlicePhysics::AlicePhysicsSystem* physicsSystem_,SceneData* sceneData_, const nlohmann::json& jsonObj_, SceneData::Object* parent_)
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
		lRotation.x = -static_cast<float>(lTransform["rotation"][1]) * AliceMathF::DEG_TO_RAD;
		lRotation.y = -static_cast<float>(lTransform["rotation"][2]) * AliceMathF::DEG_TO_RAD;
		lRotation.z = static_cast<float>(lTransform["rotation"][0]) * AliceMathF::DEG_TO_RAD;

		// スケーリング
		lScaling.x = static_cast<float>(lTransform["scaling"][1]);
		lScaling.y = static_cast<float>(lTransform["scaling"][2]);
		lScaling.z = static_cast<float>(lTransform["scaling"][0]);

		bool meshCollision = static_cast< bool >( jsonObj_[ "isMesh" ] );
		bool valid = static_cast< bool >( jsonObj_[ "isValid" ] );
		lObject->SetIsMeshCollision(meshCollision);
		lObject->SetIsValid(valid);

		//モデル読み込み
		uint32_t lHandle = AliceModel::SCreateModel(sBaseDirectorypath + lFileName);

		//オブジェクト生成
		if (parent_)
		{
			lObject->Initialize(physicsSystem_,lHandle, lTranslation, lRotation, lScaling, parent_->GetTransformPtr());
		}
		else
		{
			lObject->Initialize(physicsSystem_,lHandle, lTranslation, lRotation, lScaling);
		}

		lObject->SetName(lObjectName);

		//子オブジェクト再帰関数
		if (jsonObj_.contains("children"))
		{
			for (const nlohmann::json& json : jsonObj_["children"])
			{
				SObjectScan(physicsSystem_,sceneData_, json, lObject.get());
			}
		}
	}

	if (lType.compare("CAMERA") == 0)
	{
		sceneData_->camera = std::make_unique<GameCamera>();
		sceneData_->camera->Initialize(UPDATE_PROJMATRIXFUNC_PERSPECTIVE);

		const nlohmann::json& lCameradata = jsonObj_["cameradata"];

		// 平行移動
		AliceMathF::Vector3 lEye;
		lEye.x = static_cast<float>(lCameradata["translation"][1]);
		lEye.y = static_cast<float>(lCameradata["translation"][2]);
		lEye.z = -static_cast<float>(lCameradata["translation"][0]);
		sceneData_->camera->SetEye(lEye);

		// 上ベクトル
		AliceMathF::Vector3 lUpVec;
		lUpVec.x = static_cast<float>(lCameradata["up"][1]);
		lUpVec.y = static_cast<float>(lCameradata["up"][2]);
		lUpVec.z = -static_cast<float>(lCameradata["up"][0]);
		sceneData_->camera->SetUp(lUpVec);

		//カメラの方向
		AliceMathF::Vector3 lDirection;
		lDirection.x = static_cast<float>(lCameradata["direction"][1]);
		lDirection.y = static_cast<float>(lCameradata["direction"][2]);
		lDirection.z = -static_cast<float>(lCameradata["direction"][0]);
		sceneData_->camera->SetTarget(sceneData_->camera->GetEye() + lDirection);

		float lNear = static_cast<float>(lCameradata["nearclip"]);
		sceneData_->camera->SetNear(lNear);

		float lFar = static_cast<float>(lCameradata["farclip"]);
		sceneData_->camera->SetFar(lFar);

		float lFovAngleY = static_cast<float>(lCameradata["fovAngleY"]);
		sceneData_->camera->SetFovAngleY(lFovAngleY);

		sceneData_->camera->Update();

	}

	if (lType.compare("LIGHT") == 0)
	{
		// トランスフォームのパラメータ読み込み
		const nlohmann::json& lTransform = jsonObj_["transform"];

		AliceMathF::Vector3 lRotation;

		// 回転角
		lRotation.x = -static_cast<float>(lTransform["rotation"][1])*AliceMathF::DEG_TO_RAD;
		lRotation.y = -static_cast<float>(lTransform["rotation"][2]) * AliceMathF::DEG_TO_RAD;
		lRotation.z = static_cast<float>(lTransform["rotation"][0]) * AliceMathF::DEG_TO_RAD;

		lRotation = lRotation.Normal();

		const nlohmann::json& lCameradata = jsonObj_["cameradata"];

		//ライトカラー
		AliceMathF::Vector4 lColor;
		lColor.x = static_cast<float>(lCameradata["color"][1]);
		lColor.y = static_cast<float>(lCameradata["color"][2]);
		lColor.z = static_cast<float>(lCameradata["color"][0]);
		lColor.w = 1.0f;

		AliceMathF::Vector3 lDirection;
		lDirection.x = static_cast<float>(lCameradata["direction"][1]);
		lDirection.y = static_cast<float>(lCameradata["direction"][2]);
		lDirection.z = -static_cast<float>(lCameradata["direction"][0]);

		sceneData_->light = CreateUniqueLight();
		sceneData_->light->SetLightColor(lColor);
		sceneData_->light->SetLightDir(lDirection);
		sceneData_->light->Update();
		//モデルにライトをセット
		AliceModel::SSetLight(sceneData_->light.get());
	}
}

void SceneData::Object::Initialize(AlicePhysics::AlicePhysicsSystem* physicsSystem_,uint32_t handle_, const AliceMathF::Vector3& pos_, const AliceMathF::Vector3& rot_, const AliceMathF::Vector3& scl_, const Transform* parent_)
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

	AliceMathF::Matrix4 rotMat;
	rotMat.MakeRotation({0.0f,0.0f,90*AliceMathF::DEG_TO_RAD});

	for (const PosNormUvTangeColSkin& ver :model->GetMeshs()[0]->GetVertices())
	{
		points.push_back({ ver.position.y,ver.position.z,ver.position.x });
	}

	for ( const uint32_t& ind : model->GetMeshs()[ 0 ]->GetIndices() )
	{
		triangles.push_back(ind);
	}

	if ( meshCollision )
	{
		shape.reset(AlicePhysics::CreateMeshShape(points,triangles));

		AlicePhysics::IRigidBodyCreationSettings lSetting;
		lSetting.name = "Field";
		lSetting.type = AlicePhysics::PhysicsRigidBodyType::STATIC;
		lSetting.motionType = AlicePhysics::MotionType::STATIC;
		lSetting.allowSleeping = false;
		lSetting.collisionGroup = CollisionGroup::FIELD;
		lSetting.collisionAttribute = CollisionAttribute::DEFAULT;
		lSetting.rotation = AliceMathF::Quaternion(transform.rotation);
		lSetting.position = transform.translation;
		lSetting.shape = shape.get();
		lSetting.isActive = false;
		physicsSystem_->CreateRigidBody(rigidBody,&lSetting);
		physicsSystem_->AddRigidBody(rigidBody);
	}
}

void SceneData::Object::Finalize(AlicePhysics::AlicePhysicsSystem* physicsSystem_)
{
	if ( rigidBody )
	{
		physicsSystem_->RemoveRigidBody(rigidBody);
	}
}

void SceneData::Object::Update()
{

}

void SceneData::Object::Draw()
{
	if ( isValid )
	{
		model->Draw(transform);
	}
	if ( shape )
	{
		shape->Draw(rigidBody->GetCenterOfMassTransform(),{ 1.0f,1.0f ,1.0f },{ 1.0f ,1.0f ,1.0f ,1.0f },true);

	}
}

void SceneData::Object::Initialize(AlicePhysics::AlicePhysicsSystem* physicsSystem_)
{
	static_cast< void >( physicsSystem_ );
}

void SceneData::Object::OnCollisionEnter(AlicePhysics::RigidBodyUserData* BodyData_,const AliceMathF::Vector3& hitPosdition_)
{
	static_cast< void >( BodyData_ );
	static_cast< void >( hitPosdition_ );

}

void SceneData::Object::OnCollisionStay(AlicePhysics::RigidBodyUserData* BodyData_,const AliceMathF::Vector3& hitPosdition_)
{
	static_cast< void >( BodyData_ );
	static_cast< void >( hitPosdition_ );

}

void SceneData::Object::OnCollisionExit()
{
}

SceneData::Object::~Object()
{

}

void SceneData::Object::SetIsMeshCollision(bool flag)
{
	meshCollision = flag;
}

void SceneData::Object::SetIsValid(bool flag)
{
	isValid = flag;
}

void SceneData::Update(AlicePhysics::AlicePhysicsSystem* physicsSystem_,Camera* camera_)
{
	if (input->TriggerKey(Keys::F5))
	{
		Finalize(physicsSystem_);

		SceneLoader::SLoadFile(physicsSystem_,this);
	}

	if (light)
	{
		light->Update();
	}

	if (camera)
	{
		camera->Update();
	}

	for (std::unique_ptr<Object>& object : objects)
	{
		if (camera_)
		{
			object->Update();
			object->TransUpdate(camera_);
		}
		else
		{
			object->Update();
			object->TransUpdate(camera.get());
		}

	}
}

void SceneData::Draw()
{
	for (std::unique_ptr<Object>& object : objects)
	{
		object->Draw();

	}
}

void SceneData::Finalize(AlicePhysics::AlicePhysicsSystem* physicsSystem_)
{
	if (!objects.empty())
	{
		for ( std::unique_ptr<Object>& object : objects )
		{
			object->Finalize(physicsSystem_);
		}

		objects.clear();
	}

	if (camera)
	{
		camera.reset();
	}

	if (light)
	{
		light.reset();
	}
}

void SceneData::SSetInput(AliceInput::IInput* input_)
{
	input = input_;
}
