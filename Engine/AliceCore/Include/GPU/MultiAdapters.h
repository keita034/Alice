#pragma once

#include<Defined.h>

ALICE_SUPPRESS_WARNINGS_BEGIN

#include<memory>

ALICE_SUPPRESS_WARNINGS_END

#include<AdaptersIndex.h>
#include<Adapter.h>
#include<CrossAdapterFence.h>

struct IDXGIFactory7;

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

	virtual ICrossAdapterFence* GetCrossAdapterFence() = 0;

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
