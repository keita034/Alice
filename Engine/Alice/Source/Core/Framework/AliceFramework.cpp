#include <AliceFramework.h>

#include<AliceModel.h>
#include<AliceMotionData.h>
#include<DefaultMaterial.h>
#include<Collision2DManager.h>
#include<Particle.h>
#include<Sprite.h>
#include<SceneLoader.h>

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
	windowsApp = CreateUniqueWindowsApp();//ウィンドウ生成

	//DirectX初期化処理ここから

	directX12Core = std::make_unique<DirectX12Core>();//DirectX12Coreクラス読み込み
	directX12Core->DirectXInitialize(static_cast<float>(windowsApp->GetWindowSize().width), static_cast<float>(windowsApp->GetWindowSize().height), windowsApp->GetHwndPtr(), windowsApp.get());//DirectX12初期化
	directX12Core->SetBackScreenColor(0.1f, 0.25f, 0.5f, 0.0f);	//背景の色変更(R,G,B,A)

	//DirectX初期化処理ここまで

	//描画初期化処理ここから

	Mesh::SSetWindowsApp(windowsApp.get());
	Mesh3D::SSetWindowsApp(windowsApp.get());
	PostEffectManager::SSetWindowsApp(windowsApp.get());
	Camera::SSetWindowsApp(windowsApp.get());
	Sprite::SSetWindowsApp(windowsApp.get());
	BasePostEffect::SSetWindowsApp(windowsApp.get());

	Particle::SSetDirectX12Core(directX12Core.get());
	BasePostEffect::SSetDirectX12Core(directX12Core.get());
	PostEffectManager::SSetDirectX12Core(directX12Core.get());
	Sprite::SSetDirectX12Core(directX12Core.get());
	Mesh::SSetDirectX12Core(directX12Core.get());
	Mesh3D::SSetDirectX12Core(directX12Core.get());
	PipelineState::SSetDirectX12Core(directX12Core.get());
	TextureManager::SSetDirectX12Core(directX12Core.get());

	mesh = Mesh::SGetInstance();
	mesh3D = Mesh3D::SGetInstance();

	textureManager = TextureManager::SGetInstance();
	textureManager->Initialize();

	Particle::SSetTextureManager(textureManager);

	MaterialManager::SGetInstance()->Initialize(directX12Core->GetDevice());
	//描画初期化処理ここまで

	//その他初期化ここから

	audioManager = CreateUniqueAudioManager();
	BaseScene::SSetAudioManager(audioManager.get());

	input = AliceInput::CreateUniqueInput(windowsApp->GetHwndPtr(), &windowsApp->GetWndclassex()->hInstance);

	BaseScene::SSetInput(input.get());
	SceneData::SSetInput(input.get());


	fps = CreateUniqueFPS();

	AliceModel::SCommonInitialize(directX12Core.get());
	AliceMotionData::SCommonInitialize();

	imGuiManager = CreateUniqueImGuiManager(windowsApp.get(), directX12Core.get());
	
	sceneManager = SceneManager::SGetInstance();

	postEffectManager = PostEffectManager::SGetInstance();

	postEffectManager->Initialize();
	//その他初期化ここまで
}

void AliceFramework::Finalize()
{
	imGuiManager->Finalize();
	imGuiManager.release();


	mesh->Destroy();
	audioManager->Destroy();
	windowsApp->Break();
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

	Collision2DManager::SGetInstance()->CheckAllCollisions();

	sceneManager->Update();

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

		//if (windowsApp->RenderShould())
		{
			//描画処理
			Draw();
		}

		PostUpdate();
	}

	//ゲームの終了
	Finalize();
}
