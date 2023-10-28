#include "CreateBlend.h"

D3D12_BLEND_DESC CreateBlend(BlendMode mode_)
{
	D3D12_BLEND_DESC lBlendDesc = CD3DX12_BLEND_DESC(D3D12_DEFAULT);

	switch ( mode_ )
	{
	case BlendMode::AX_BLENDMODE_NOBLEND:

		break;
	case BlendMode::AX_BLENDMODE_ALPHA:
		lBlendDesc.RenderTarget[ 0 ].BlendEnable = true;// ブレンドを有効
		lBlendDesc.RenderTarget[ 0 ].BlendOpAlpha = D3D12_BLEND_OP_ADD;//加算
		lBlendDesc.RenderTarget[ 0 ].SrcBlendAlpha = D3D12_BLEND_ONE;//ソースの値を100%使う
		lBlendDesc.RenderTarget[ 0 ].DestBlendAlpha = D3D12_BLEND_ZERO;//テストの値を0%使う
		// 半透明合成
		lBlendDesc.RenderTarget[ 0 ].BlendOp = D3D12_BLEND_OP_ADD;// 加算
		lBlendDesc.RenderTarget[ 0 ].SrcBlend = D3D12_BLEND_SRC_ALPHA;// ソースのアルファ値
		lBlendDesc.RenderTarget[ 0 ].DestBlend = D3D12_BLEND_INV_SRC_ALPHA;;// 1.0f-ソースのアルファ値

		break;
	case BlendMode::AX_BLENDMODE_ADD:
		lBlendDesc.RenderTarget[ 0 ].BlendEnable = true;// ブレンドを有効
		lBlendDesc.RenderTarget[ 0 ].SrcBlend = D3D12_BLEND_SRC_ALPHA;// ソースの値を100% 使う
		lBlendDesc.RenderTarget[ 0 ].DestBlend = D3D12_BLEND_ONE;// デストの値を100% 使う
		break;
	case BlendMode::AX_BLENDMODE_SUB:
		lBlendDesc.RenderTarget[ 0 ].BlendEnable = true;// ブレンドを有効
		lBlendDesc.RenderTarget[ 0 ].BlendOp = D3D12_BLEND_OP_REV_SUBTRACT;// デストからソースを減算
		lBlendDesc.RenderTarget[ 0 ].SrcBlend = D3D12_BLEND_SRC_ALPHA;// ソースの値を100% 使う
		lBlendDesc.RenderTarget[ 0 ].DestBlend = D3D12_BLEND_ONE;// デストの値を100% 使う
		break;
	case BlendMode::AX_BLENDMODE_MULA:
		lBlendDesc.RenderTarget[ 0 ].BlendEnable = true;// ブレンドを有効
		lBlendDesc.RenderTarget[ 0 ].SrcBlend = D3D12_BLEND_ZERO;
		lBlendDesc.RenderTarget[ 0 ].DestBlend = D3D12_BLEND_SRC_COLOR;
		break;
	case BlendMode::AX_BLENDMODE_INVSRC:
		lBlendDesc.RenderTarget[ 0 ].BlendEnable = true;// ブレンドを有効
		lBlendDesc.RenderTarget[ 0 ].SrcBlend = D3D12_BLEND_INV_DEST_COLOR;// 1.0f-デストカラーの値
		lBlendDesc.RenderTarget[ 0 ].DestBlend = D3D12_BLEND_ZERO;// 使わない
		break;
	case BlendMode::AX_BLENDMODE_MAX:
		break;
	case BlendMode::AX_BLENDMODE_CUSTOM:
		break;
	default:
		break;
	}

	return lBlendDesc;
}
