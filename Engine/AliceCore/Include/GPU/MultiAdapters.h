#pragma once

#pragma warning(push)
#pragma warning(disable: 4061)
#pragma warning(disable: 4062)
#pragma warning(disable: 4365)
#pragma warning(disable: 4514)
#pragma warning(disable: 4365)
#pragma warning(disable: 4668)
#pragma warning(disable: 4820)
#pragma warning(disable: 5039)

#include<vector>
#include<directx/d3d12.h>
#include<directx/d3dx12.h>
#include<memory>
#include<dxgi1_6.h>
#pragma warning(pop)


#include<AdaptersIndex.h>
#include<Adapter.h>

/// <summary>
/// マルチアダプター
/// </summary>
class IMultiAdapters
{
public:

	virtual void Initialize() = 0;

	virtual IAdapter* GetAdapter(AdaptersIndex index_) = 0;

	virtual IDXGIFactory7* GetFactory() = 0;

	virtual IAdapter* GetMainAdapter() = 0;
	virtual IAdapter* GetSubAdapter() = 0;

	virtual void ExecuteCommand() = 0;

	virtual void BeginCommand(size_t bbIndex_) = 0;

	virtual void WaitPreviousFrame() = 0;

	IMultiAdapters() = default;
	virtual ~IMultiAdapters() = default;
};


/// <summary>
///マルチアダプターの生成(ユニーク)
/// </summary>
/// <returns>生成されたポインタ</returns>
std::unique_ptr<IMultiAdapters> CreateUniqueMultiAdapters();

/// <summary>
/// マルチアダプターの生成(シェアード)
/// </summary>
/// <returns>生成されたポインタ</returns>
std::shared_ptr<IMultiAdapters> CreateSharedMultiAdapters();
