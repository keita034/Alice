#include <AliceFramework.h>

#include<AliceModel.h>
#include<AliceMotionData.h>
#include<MaterialManager.h>
#include<Particle.h>
#include<Sprite.h>
#include<SceneLoader.h>
#include<BaseBuffer.h>
#include<MeshGPUParticleAliceModel.h>

void AliceFramework::SDebugInitialize()
{
	HANDLE stdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	DWORD consoleMode = 0;
	GetConsoleMode(stdOut, &consoleMode);
	consoleMode = consoleMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING;
	SetConsoleMode(stdOut, consoleMode);
}

void AliceFramework::Initialize()
{
	windowsApp = CreateUniqueWindowsApp(L"Blood");//ウィンドウ生成

	Mesh::SSetWindowsApp(windowsApp.get());
	Mesh3D::SSetWindowsApp(windowsApp.get());
	PostEffectManager::SSetWindowsApp(windowsApp.get());
	Camera::SSetWindowsApp(windowsApp.get());
	Sprite::SSetWindowsApp(windowsApp.get());
	BasePostEffect::SSetWindowsApp(windowsApp.get());
	BaseScene::SSetWindowsApp(windowsApp.get());

	//DirectX初期化処理ここから

	directX12Core = std::make_unique<DirectX12Core>();//DirectX12Coreクラス読み込み
	directX12Core->DirectXInitialize(static_cast<float>(windowsApp->GetWindowSize().width), static_cast<float>(windowsApp->GetWindowSize().height), windowsApp->GetHwndPtr(), windowsApp.get());//DirectX12初期化
	directX12Core->SetBackScreenColor(0.1f, 0.25f, 0.5f, 0.0f);	//背景の色変更(R,G,B,A)

	Particle::SSetDirectX12Core(directX12Core.get());
	BasePostEffect::SSetDirectX12Core(directX12Core.get());
	PostEffectManager::SSetDirectX12Core(directX12Core.get());
	Sprite::SSetDirectX12Core(directX12Core.get());
	Mesh::SSetDirectX12Core(directX12Core.get());
	Mesh3D::SSetDirectX12Core(directX12Core.get());
	TextureManager::SSetDirectX12Core(directX12Core.get());
	AliceModel::SCommonInitialize(directX12Core.get());

	PipelineState::SSetDevice(directX12Core->GetDevice());

	BaseBuffer::SSetSwapChain(directX12Core->GetSwapChain());

	MeshGPUParticleAliceModel::SSetMultiAdapters(directX12Core->GetMultiAdapters());
	//DirectX初期化処理ここまで

	//描画初期化処理ここから

	mesh = Mesh::SGetInstance();
	mesh3D = Mesh3D::SGetInstance();

	textureManager = TextureManager::SGetInstance();
	textureManager->Initialize();

	Particle::SSetTextureManager(textureManager);

	postEffectManager = PostEffectManager::SGetInstance();
	postEffectManager->Initialize();

	materialManager = MaterialManager::SGetInstance();

	materialManager->Initialize(directX12Core->GetMultiAdapters());

	//描画初期化処理ここまで

	//その他初期化ここから

	audioManager = CreateUniqueAudioManager();
	BaseScene::SSetAudioManager(audioManager.get());

	fps = CreateUniqueFPS();

	input = AliceInput::CreateUniqueInput(windowsApp->GetHwndPtr(),&windowsApp->GetWndclassex()->hInstance);

	BaseScene::SSetInput(input.get());
	SceneData::SSetInput(input.get());

	AliceMotionData::SCommonInitialize();

	imGuiManager = CreateUniqueImGuiManager(windowsApp.get(), directX12Core.get());

	physicsSystem = std::make_unique<AlicePhysics::AlicePhysicsSystem>();
	physicsSystem->SetDevice(directX12Core->GetDevice());
	physicsSystem->SetCommandList(directX12Core->GetCommandList());
	physicsSystem->Initialize();

	BaseScene::SSetPhysicsSystem(physicsSystem.get());

	gpuParticleEmitter = std::make_unique<GPUParticleEmitter>();
	gpuParticleEmitter->SetMultiAdapters(directX12Core->GetMultiAdapters());
	gpuParticleEmitter->SetSwapChain(directX12Core->GetSwapChain());
	gpuParticleEmitter->Initialize();

	BaseScene::SSetGPUParticleEmitter(gpuParticleEmitter.get());

	sceneManager = SceneManager::SGetInstance();

}

void AliceFramework::Finalize()
{
	BaseBuffer::Finalize();
	MeshGPUParticleAliceModel::Finalize();
	AliceModel::Finalize();
	BasePipelineState::Finalize();
	sceneManager->Finalize();
	BaseScene::BaseSceneFinalize();
	gpuParticleEmitter->Finalize();
	physicsSystem->Finalize();
	imGuiManager->Finalize();
	imGuiManager.release();
	textureManager->Finalize();
	mesh->Destroy();
	mesh3D->Destroy();
	postEffectManager->Destroy();
	materialManager->Finalize();
	audioManager->Destroy();
	windowsApp->Break();
}

AliceFramework::~AliceFramework()
{

}

void AliceFramework::Update()
{

	fps->FpsControlBegin();
	//準備処理
	mesh->DrawReset();
	mesh3D->DrawReset();

	input->Update(static_cast<float>(windowsApp->GetWindowSize().width), static_cast<float>(windowsApp->GetWindowSize().height));
	audioManager->Update();

	imGuiManager->Bigin();

	sceneManager->Update();

	gpuParticleEmitter->Update(fps->GetDeltaTime());

	physicsSystem->SetViewProjection(sceneManager->GetSceneCamera()->GetViewMatrixInv(),sceneManager->GetSceneCamera()->GetProjectionMatrix());
	physicsSystem->SetLight(Light::SGetLightDirPtr(),Light::SGetLightColorPtr());

	physicsSystem->Update(fps->GetDeltaTime(),fps->GetFrameRate());

	postEffectManager->PostInitialize();

	imGuiManager->End();


}

void AliceFramework::PostUpdate()
{
	fps->FpsControlEnd();

}

void AliceFramework::Draw()
{
	if (postEffectManager->IsAalid())
	{
		directX12Core->BeginCommand();
		//描画処理
		postEffectManager->PreDrawScen();

		sceneManager->Draw();

		gpuParticleEmitter->Draw(sceneManager->GetSceneCamera()->GetViewProjectionMatrix(),sceneManager->GetSceneCamera()->GetBillboardMatrix());

		physicsSystem->Draw();

		postEffectManager->PostDrawScen();

		postEffectManager->Update();

		directX12Core->ExecuteCommand(false);

		directX12Core->BeginDraw();//描画準備
		postEffectManager->Draw();

		imGuiManager->Draw();
		//DirectX毎フレーム処理　ここまで
		directX12Core->EndDraw();//描画後処理
	}
	else
	{
		directX12Core->BeginDraw();//描画準備

		sceneManager->Draw();

		{
			AliceMathF::Matrix4 mat = sceneManager->GetSceneCamera()->GetViewMatrixInv() * sceneManager->GetSceneCamera()->GetProjectionMatrix();

			gpuParticleEmitter->Draw(mat,sceneManager->GetSceneCamera()->GetBillboardMatrix());
		}

		physicsSystem->Draw();

		imGuiManager->Draw();

		directX12Core->EndDraw();//描画後処理
	}
}

bool AliceFramework::IsEndRequst()
{
	return !windowsApp->MessageWindow()|| input->TriggerKey(Keys::ESC);
}

void AliceFramework::Run()
{
	//初期化処理
	Initialize();

	// ゲームループ
	while (true)
	{
		if (IsEndRequst())
		{
			break;
		}

		//更新処理
		Update();

			//描画処理
			Draw();

		PostUpdate();
	}

	//ゲームの終了
	Finalize();
}
