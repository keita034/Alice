#include<PostEffectManager.h>
#include<WindowsApp.h>
#include<DefaultMaterial.h>
#include<PostEffectFactory.h>

const float PostEffectManager::clearColor[4] = { 1.0f,1.0f,1.0f,0.0f };
IWindowsApp* PostEffectManager::windowsApp = nullptr;

PostEffectManager* PostEffectManager::GetInstance()
{
	static PostEffectManager instance;
	return &instance;
}

void PostEffectManager::Initialize()
{
	device = DirectX12Core::GetDeviceSta().Get();
	cmdList = DirectX12Core::GetCommandListSta().Get();

	srvHeap = DirectX12Core::GetInstance()->GetSRVDescriptorHeap();
	dsvHeap = DirectX12Core::GetInstance()->GetDSVDescriptorHeap();
	rtvHeap = DirectX12Core::GetInstance()->GetRTVDescriptorHeap();

	width = static_cast<float>(windowsApp->GetWindowSize().width);
	height = static_cast<float>(windowsApp->GetWindowSize().height);

	mainRenderTarget = std::make_unique<RenderTarget>(srvHeap, cmdList);

	mainRenderTarget->Initialize(windowsApp->GetWindowSize().width, windowsApp->GetWindowSize().height, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);

	//頂点バッファの生成
	vertexBuffer = CreateUniqueVertexBuffer(4, sizeof(PosUvColor));

	//インデックスバッファの生成
	indexBuffer = CreateUniqueIndexBuffer(6);

	factory = PostEffectFactory::GetInstance();
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
	mainRenderTarget->Transition(D3D12_RESOURCE_STATE_RENDER_TARGET);

	mainRenderTarget->SetRenderTarget();

	CD3DX12_VIEWPORT viewPort = CD3DX12_VIEWPORT(0.0f, 0.0f, width, height);
	cmdList->RSSetViewports(1, &viewPort);

	CD3DX12_RECT rect = CD3DX12_RECT(0, 0, static_cast<LONG>(width), static_cast<LONG>(height));
	cmdList->RSSetScissorRects(1, &rect);

	mainRenderTarget->ClearRenderTarget();
}

void PostEffectManager::PostDrawScen()
{
	mainRenderTarget->Transition(D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
}

void PostEffectManager::AddPostEffect(const std::string& postEffectName)
{
	postEffects.push_back(factory->CreatePostEffect(postEffectName));

	isAalid = true;
}

void PostEffectManager::Finalize()
{
	postEffects.clear();
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

void PostEffectManager::SubPostEffect(const std::string& postEffectName)
{
	postEffects.remove_if([&postEffectName](BasePostEffect* postEffect)
		{
			return postEffect->GetType() == postEffectName;
		});
}

void PostEffectManager::CreateWindowTex()
{





}

void PostEffectManager::CreateRenderTarget()
{

}

void PostEffectManager::CreateDepthStencilView()
{

}

PostEffectManager::~PostEffectManager()
{
}

void PostEffectManager::Draw()
{
	// 頂点データ
	PosUvColor vertices[] =
	{//		x		y		z		u	v
		{{-1.0f,1.0f,0.0f},{0.0f,0.0f},{1.0f,1.0f,1.0f,1.0f}},//左上インデックス0
		{{-1.0f,-1.0f,0.0f},{0.0f,1.0f},{1.0f,1.0f,1.0f,1.0f}},//左下インデックス1
		{{1.0f,1.0f,0.0f},{1.0f,0.0f},{1.0f,1.0f,1.0f,1.0f}},//右上インデックス2
		{{1.0f,-1.0f,0.0f},{1.0f,1.0f},{1.0f,1.0f,1.0f,1.0f}},//右下インデックス3
	};

	// インデックスデータ
	uint32_t indices[] =
	{
		1, 0, 3, // 三角形1つ目
		2, 3, 0, // 三角形2つ目
	};

	//頂点バッファへのデータ転送
	vertexBuffer->Update(vertices);

	//インデックスバッファへのデータ転送
	indexBuffer->Update(indices);

	postEffectMaterial = MaterialManager::GetMaterial("DefaultPostEffect");

	D3D12_VERTEX_BUFFER_VIEW vbView = vertexBuffer->GetView();
	D3D12_INDEX_BUFFER_VIEW ibView = indexBuffer->GetView();

	// パイプラインステートとルートシグネチャの設定コマンド
	cmdList->SetPipelineState(postEffectMaterial->pipelineState->GetPipelineState());
	cmdList->SetGraphicsRootSignature(postEffectMaterial->rootSignature->GetRootSignature());

	// プリミティブ形状の設定コマンド
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST); // 三角形リスト

	// 頂点バッファビューの設定コマンド
	cmdList->IASetVertexBuffers(0, 1, &vbView);

	//インデックスバッファビューの設定コマンド
	cmdList->IASetIndexBuffer(&ibView);

	// SRVヒープの設定コマンド
	ID3D12DescriptorHeap* descriptorHeaps[] =
	{
		srvHeap->GetHeap(),
	};

	cmdList->SetDescriptorHeaps(_countof(descriptorHeaps), descriptorHeaps);

	//// SRVヒープの先頭にあるSRVをルートパラメータ1番に設定
	cmdList->SetGraphicsRootDescriptorTable(0, mainRenderTarget->GetGpuHandle());

	// 描画コマンド
	cmdList->DrawIndexedInstanced(6, 1, 0, 0, 0);
}

void PostEffectManager::SetWindowsApp(IWindowsApp* windowsApp_)
{
	windowsApp = windowsApp_;
}

