#pragma once
#include<VertexBuffer.h>
#include<IndexBuffer.h>
#include<AliceMathUtility.h>
#include<Material.h>
#include<BasePostEffect.h>
#include<RWStructuredBuffer.h>
#include<RenderTarget.h>
#include<WindowsApp.h>

class PostEffectFactory;

class PostEffectManager final
{
private:

	static ICommandList* sCmdList;
	static ISRVDescriptorHeap* sSrvHeap;
	static IWindowsApp* sWindowsApp;
	static std::unique_ptr<PostEffectManager> postEffectManager;

	//頂点バッファ
	std::unique_ptr<IVertexBuffer> vertexBuffer;

	//インデックスバッファ
	std::unique_ptr<IIndexBuffer> indexBuffer;

	Material* postEffectMaterial = nullptr;

	float width = 0.0f;
	float height = 0.0f;

	std::list<BasePostEffect*> postEffects;

	PostEffectFactory* factory = nullptr;

	std::unique_ptr<RenderTarget> mainRenderTarget;

	//有効か
	bool isAalid = false;

	bool isFlip = false;
	int8_t PADING[2]{};

	HRESULT result;

public:
	
	static PostEffectManager* SGetInstance();

	void Initialize();

	void PostInitialize();

	void Update();

	void PreDrawScen();

	void PostDrawScen();

	void AddPostEffect(const std::string& postEffectName_);

	void SubPostEffect(const std::string& postEffectName_);

	void Finalize();

	bool IsAalid();

	void Draw();

	void Destroy();

	static void SSetWindowsApp(IWindowsApp* windowsApp_);
	static void SSetDirectX12Core(ICommandList* cmdList_,ISRVDescriptorHeap* srvHeap_);

	~PostEffectManager();
	PostEffectManager() = default;

private:

	//コピーコンストラクタ・代入演算子削除
	PostEffectManager& operator=(const PostEffectManager&) = delete;
	PostEffectManager(const PostEffectManager&) = delete;
};

