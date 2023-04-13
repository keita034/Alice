#pragma once

#pragma warning(push)
#pragma warning(disable: 4061)
#pragma warning(disable: 4365)
#pragma warning(disable: 4668)
#pragma warning(disable: 4820)
#pragma warning(disable: 5039)

#include<d3dx12.h>

#pragma warning(pop)

#include"BaseBuffer.h"

class DepthStencilBuffer : public BaseBuffer
{
private:

	//幅
	UINT width = 0;
	//高さ
	UINT height = 0;
	//ハンドル
	D3D12_CPU_DESCRIPTOR_HANDLE handle{};

public:

	/// <summary>
	/// 生成
	/// </summary>
	/// <param name="w">横幅</param>
	/// <param name="h">縦幅</param>
	/// <param name="format">フォーマット</param>
	bool Create(UINT w, UINT h, DXGI_FORMAT format);

	bool Resize(UINT w, UINT h);

	/// <summary>
	/// リソースを取得
	/// </summary>
	ID3D12Resource* GetTexture() const;

	/// <summary>
	/// ポインタ番号を取得
	/// </summary>
	const D3D12_CPU_DESCRIPTOR_HANDLE& GetHandle() ;

	~DepthStencilBuffer() = default;
	DepthStencilBuffer() = default;
};