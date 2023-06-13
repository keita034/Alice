#include <AliceFramework.h>

#include<AliceModel.h>
#include<AliceMotionData.h>
#include<DefaultMaterial.h>
#include<Collision2DManager.h>

void AliceFramework::DebugInitialize()
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

	directX12Core = DirectX12Core::GetInstance();//DirectX12Coreクラス読み込み
	directX12Core->DirectXInitialize(static_cast<float>(windowsApp->GetWindowSize().height), static_cast<float>(windowsApp->GetWindowSize().width), windowsApp->GetHwnd(), windowsApp.get());//DirectX12初期化
	directX12Core->SetBackScreenColor(0.1f, 0.25f, 0.5f, 0.0f);	//背景の色変更(R,G,B,A)

	//DirectX初期化処理ここまで

	//描画初期化処理ここから
	Mesh::SetWindowsApp(windowsApp.get());
	Mesh3D::SetWindowsApp(windowsApp.get());
	PostEffectManager::SetWindowsApp(windowsApp.get());
	Camera::SetWindowsApp(windowsApp.get());

	mesh = Mesh::GetInstance();
	mesh3D = Mesh3D::GetInstance();

	textureManager = TextureManager::GetInstance();
	textureManager->Initialize();

	MaterialManager::GetInstance()->Initialize();
	//描画初期化処理ここまで

	//その他初期化ここから

	audioManager = AudioManager::GetInstance();
	audioManager->Initialize();

	input = AliceInput::CreateUniqueInput(windowsApp->GetHwnd(), &windowsApp->GetWndclassex()->hInstance);

	BaseScene::SetInput(input.get());
	fps = new FPS;

	AliceModel::CommonInitialize();
	AliceMotionData::CommonInitialize();

	imGuiManager = std::make_unique<ImGuiManager>();
	imGuiManager->Initialize(windowsApp.get(), directX12Core);
	
	sceneManager = SceneManager::GetInstance();

	BasePostEffect::SetSrvHeap(directX12Core->GetSRVDescriptorHeap());

	postEffectManager = PostEffectManager::GetInstance();

	postEffectManager->Initialize();
	//その他初期化ここまで
}

void AliceFramework::Finalize()
{
	imGuiManager->Finalize();
	imGuiManager.release();

	windowsApp->Break();
	directX12Core->Destroy();
	mesh->Destroy();
	mesh3D->Destroy();
	textureManager->Destroy();
	audioManager->Destroy();
	delete fps;
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

	Collision2DManager::GetInstance()->CheckAllCollisions();

	sceneManager->Update();


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
