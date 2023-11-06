#include<PostEffectManager.h>
#include<WindowsApp.h>
#include<MaterialManager.h>
#include<PostEffectFactory.h>

ICommandList* PostEffectManager::sCmdList = nullptr;
ISRVDescriptorHeap* PostEffectManager::sSrvHeap = nullptr;
IWindowsApp* PostEffectManager::sWindowsApp = nullptr;
std::unique_ptr<PostEffectManager> PostEffectManager::postEffectManager;

PostEffectManager* PostEffectManager::SGetInstance()
{
	if ( !postEffectManager )
	{
		PostEffectManager* lInstance = new PostEffectManager();
		postEffectManager.reset(lInstance);
	}

	return postEffectManager.get();
}

void PostEffectManager::Initialize()
{
	width = static_cast<float>(sWindowsApp->GetWindowSize().width);
	height = static_cast<float>(sWindowsApp->GetWindowSize().height);

	ID3D12GraphicsCommandList* lCmdList = sCmdList->GetGraphicCommandList();

	mainRenderTarget = std::make_unique<RenderTarget>(sSrvHeap,lCmdList);

	mainRenderTarget->Initialize(sWindowsApp->GetWindowSize().width, sWindowsApp->GetWindowSize().height, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);

	//頂点バッファの生成
	vertexBuffer = CreateUniqueVertexBuffer(4, sizeof(PosUvColor));

	//インデックスバッファの生成
	indexBuffer = CreateUniqueIndexBuffer(6);

	factory = PostEffectFactory::SGetInstance();
}

void PostEffectManager::PostInitialize()
{
	for (auto itr = postEffects.begin(); itr != postEffects.end(); itr++)
	{
		itr._Ptr->_Myval->Initialize();
	}
}

void PostEffectManager::Update()
{
	for (auto itr = postEffects.begin(); itr != postEffects.end(); itr++)
	{
		itr._Ptr->_Myval->PostUpdate(mainRenderTarget.get());
	}

}

void PostEffectManager::PreDrawScen()
{
	ID3D12GraphicsCommandList* lCmdList = sCmdList->GetGraphicCommandList();

	mainRenderTarget->Transition(D3D12_RESOURCE_STATE_RENDER_TARGET);

	mainRenderTarget->SetRenderTarget();

	CD3DX12_VIEWPORT lViewPort = CD3DX12_VIEWPORT(0.0f, 0.0f, width, height);
	lCmdList->RSSetViewports(1, &lViewPort);

	CD3DX12_RECT lRect = CD3DX12_RECT(0, 0, static_cast<LONG>(width), static_cast<LONG>(height));
	lCmdList->RSSetScissorRects(1, &lRect);

	mainRenderTarget->ClearRenderTarget();
}

void PostEffectManager::PostDrawScen()
{
	mainRenderTarget->Transition(D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
}

void PostEffectManager::AddPostEffect(const std::string& postEffectName_)
{
	postEffects.push_back(factory->CreatePostEffect(postEffectName_));

	isAalid = true;
}

void PostEffectManager::Finalize()
{
	postEffects.clear();
}

void PostEffectManager::Destroy()
{
	postEffects.clear();
	postEffectManager.reset();
}

bool PostEffectManager::IsAalid()
{
	if (postEffects.size() != 0)
	{
		isAalid = true;
	}
	else
	{
		isAalid = false;
	}

	return isAalid;
}

void PostEffectManager::SubPostEffect(const std::string& postEffectName_)
{
	postEffects.remove_if([&postEffectName_](BasePostEffect* postEffect_)
		{
			return postEffect_->GetType() == postEffectName_;
		});
}

PostEffectManager::~PostEffectManager()
{
}

void PostEffectManager::Draw()
{
	// 頂点データ
	PosUvColor lVertices[] =
	{//		x		y		z		u	v
		{{-1.0f,1.0f,0.0f},{0.0f,0.0f},{1.0f,1.0f,1.0f,1.0f}},//左上インデックス0
		{{-1.0f,-1.0f,0.0f},{0.0f,1.0f},{1.0f,1.0f,1.0f,1.0f}},//左下インデックス1
		{{1.0f,1.0f,0.0f},{1.0f,0.0f},{1.0f,1.0f,1.0f,1.0f}},//右上インデックス2
		{{1.0f,-1.0f,0.0f},{1.0f,1.0f},{1.0f,1.0f,1.0f,1.0f}},//右下インデックス3
	};

	// インデックスデータ
	uint32_t lIndices[] =
	{
		1, 0, 3, // 三角形1つ目
		2, 3, 0, // 三角形2つ目
	};

	//頂点バッファへのデータ転送
	vertexBuffer->Update(lVertices);

	//インデックスバッファへのデータ転送
	indexBuffer->Update(lIndices);

	postEffectMaterial = MaterialManager::SGetMaterial("DefaultPostEffect");

	D3D12_VERTEX_BUFFER_VIEW lVbView = vertexBuffer->GetView();
	D3D12_INDEX_BUFFER_VIEW lIView = indexBuffer->GetView();
	ID3D12GraphicsCommandList* lCmdList = sCmdList->GetGraphicCommandList();

	// パイプラインステートとルートシグネチャの設定コマンド
	lCmdList->SetPipelineState(postEffectMaterial->pipelineState->GetPipelineState());
	lCmdList->SetGraphicsRootSignature(postEffectMaterial->rootSignature->GetRootSignature());

	// プリミティブ形状の設定コマンド
	lCmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST); // 三角形リスト

	// 頂点バッファビューの設定コマンド
	lCmdList->IASetVertexBuffers(0, 1, &lVbView);

	//インデックスバッファビューの設定コマンド
	lCmdList->IASetIndexBuffer(&lIView);

	// SRVヒープの設定コマンド
	ID3D12DescriptorHeap* lDescriptorHeaps[] =
	{
		sSrvHeap->GetHeap(),
	};

	lCmdList->SetDescriptorHeaps(_countof(lDescriptorHeaps), lDescriptorHeaps);

	//// SRVヒープの先頭にあるSRVをルートパラメータ1番に設定
	lCmdList->SetGraphicsRootDescriptorTable(0, mainRenderTarget->GetGpuHandle());

	// 描画コマンド
	lCmdList->DrawIndexedInstanced(6, 1, 0, 0, 0);
}


void PostEffectManager::SSetDirectX12Core(DirectX12Core* directX12Core_)
{
	sCmdList = directX12Core_->GetCommandList();
	sSrvHeap = directX12Core_->GetSRVDescriptorHeap();
}

void PostEffectManager::SSetWindowsApp(IWindowsApp* windowsApp_)
{
	sWindowsApp = windowsApp_;
}

