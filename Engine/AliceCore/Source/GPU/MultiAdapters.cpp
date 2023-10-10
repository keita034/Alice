#include "MultiAdapters.h"

class MultiAdapters : public IMultiAdapters
{
private:

	struct AdapterSetting
	{
		uint32_t maxDSV;
		uint32_t maxRTV;
		uint32_t maxSRV;
		uint32_t maxCBV;
		uint32_t maxUAV;
	};

	AdapterSetting mainAdapterSetting = { 1024,1024,300000,300000,400000 };
	AdapterSetting subAdapterSetting = { 1024,1024,300000,300000,400000 };

	Microsoft::WRL::ComPtr<IDXGIFactory7> dxgiFactory;

	std::vector<std::unique_ptr< IAdapter>>adapters;

public:

	void Initialize()override;

	IAdapter* GetAdapter(AdaptersIndex index_)override;

	IDXGIFactory7* GetFactory()override;

	IAdapter* GetMainAdapter()override;
	IAdapter* GetSubAdapter()override;

	void ExecuteCommand()override;

	void BeginCommand(size_t bbIndex_)override;

	void WaitPreviousFrame()override;

	MultiAdapters() = default;
	~MultiAdapters() = default;
};

void MultiAdapters::Initialize()
{
	//DXGIファクトリー
	HRESULT lResult = CreateDXGIFactory(IID_PPV_ARGS(dxgiFactory.ReleaseAndGetAddressOf()));
	if ( FAILED(lResult) )
	{

	}

	//アダプター列挙用
	std::vector<Microsoft::WRL::ComPtr<IDXGIAdapter4>> lAdapters;
	//ここに特定の名前を持つアダプターオブジェクトが入る
	Microsoft::WRL::ComPtr<IDXGIAdapter4> lTmpAdapter;

	//パフォーマンスが高いのもから全て列挙
	for ( uint32_t i = 0; dxgiFactory->EnumAdapterByGpuPreference(i,DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE,IID_PPV_ARGS(lTmpAdapter.ReleaseAndGetAddressOf())) != DXGI_ERROR_NOT_FOUND; i++ )
	{
		lAdapters.push_back(lTmpAdapter);
	}

	// 妥当なアダプタを選別する
	for ( size_t i = 0; i < lAdapters.size(); i++ )
	{
		DXGI_ADAPTER_DESC3 lAdapterDesc;
		// アダプターの情報を取得する
		lAdapters[ i ]->GetDesc3(&lAdapterDesc);

		// ソフトウェアデバイスを回避
		if ( !( lAdapterDesc.Flags & DXGI_ADAPTER_FLAG3_SOFTWARE ) )
		{
			std::unique_ptr<IAdapter>lAdapter;

			if ( adapters.empty() )
			{
				lAdapter = CreateUniqueAdapter(lAdapters[ i ].Get(),mainAdapterSetting.maxDSV,mainAdapterSetting.maxRTV,mainAdapterSetting.maxSRV,mainAdapterSetting.maxCBV,mainAdapterSetting.maxUAV);

			}
			else
			{
				lAdapter = CreateUniqueAdapter(lAdapters[ i ].Get(),subAdapterSetting.maxDSV,subAdapterSetting.maxRTV,subAdapterSetting.maxSRV,subAdapterSetting.maxCBV,subAdapterSetting.maxUAV);

			}

			adapters.push_back(std::move(lAdapter));

		}
	}
}

IAdapter* MultiAdapters::GetAdapter(AdaptersIndex index_)
{
	return adapters[ static_cast< size_t >( index_ ) ].get();
}

IDXGIFactory7* MultiAdapters::GetFactory()
{
	return dxgiFactory.Get();
}

IAdapter* MultiAdapters::GetMainAdapter()
{
	return GetAdapter(AdaptersIndex::MAIN);
}

IAdapter* MultiAdapters::GetSubAdapter()
{
	return GetAdapter(AdaptersIndex::SUB);
}

void MultiAdapters::ExecuteCommand()
{
	for ( std::unique_ptr<IAdapter>& lAdapter : adapters )
	{
		lAdapter->CommandListExecute();
	}
}

void MultiAdapters::BeginCommand(size_t bbIndex_)
{
	for ( std::unique_ptr<IAdapter>& lAdapter : adapters )
	{
		lAdapter->BeginCommand(bbIndex_);

	}
}

void MultiAdapters::WaitPreviousFrame()
{
	for ( std::unique_ptr<IAdapter>& lAdapter : adapters )
	{
		lAdapter->WaitPreviousFrame();
	}
}

std::unique_ptr<IMultiAdapters> CreateUniqueMultiAdapters()
{
	std::unique_ptr<IMultiAdapters> lMultiAdapters = std::make_unique<MultiAdapters>();
	lMultiAdapters->Initialize();
	return std::move(lMultiAdapters);
}

std::shared_ptr<IMultiAdapters> CreateSharedMultiAdapters()
{
	std::shared_ptr<IMultiAdapters> lMultiAdapters = std::make_shared<MultiAdapters>();
	lMultiAdapters->Initialize();
	return lMultiAdapters;
}
