#include "CreateBlend.h"

D3D12_BLEND_DESC CreateBlend(BlendMode mode_)
{
	D3D12_BLEND_DESC lBlendDesc = CD3DX12_BLEND_DESC(D3D12_DEFAULT);

	D3D12_RENDER_TARGET_BLEND_DESC lTargetBlendDesc{};
	lTargetBlendDesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL; // RBGA全てのチャンネルを描画
	lTargetBlendDesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
	lTargetBlendDesc.SrcBlendAlpha = D3D12_BLEND_ONE;
	lTargetBlendDesc.DestBlendAlpha = D3D12_BLEND_ZERO;

	switch ( mode_ )
	{
	case BlendMode::AX_BLENDMODE_NOBLEND:

		lTargetBlendDesc.BlendEnable = false;

		break;

	case BlendMode::AX_BLENDMODE_ALPHA:
		lTargetBlendDesc.BlendEnable = true;
		lTargetBlendDesc.BlendOp = D3D12_BLEND_OP_ADD;
		lTargetBlendDesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
		lTargetBlendDesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;

		break;

	case BlendMode::AX_BLENDMODE_ADD:

		lTargetBlendDesc.BlendEnable = true;
		lTargetBlendDesc.BlendOp = D3D12_BLEND_OP_ADD;
		lTargetBlendDesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
		lTargetBlendDesc.DestBlend = D3D12_BLEND_ONE;

		break;

	case BlendMode::AX_BLENDMODE_SUB:

		lTargetBlendDesc.BlendEnable = true;
		lTargetBlendDesc.BlendOp = D3D12_BLEND_OP_REV_SUBTRACT;
		lTargetBlendDesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
		lTargetBlendDesc.DestBlend = D3D12_BLEND_ONE;

		break;

	case BlendMode::AX_BLENDMODE_MULA:

		lTargetBlendDesc.BlendEnable = true;
		lTargetBlendDesc.BlendOp = D3D12_BLEND_OP_ADD;
		lTargetBlendDesc.SrcBlend = D3D12_BLEND_ZERO;
		lTargetBlendDesc.DestBlend = D3D12_BLEND_SRC_COLOR;

		break;

	case BlendMode::AX_BLENDMODE_SCREEN:

		lTargetBlendDesc.BlendEnable = true;
		lTargetBlendDesc.BlendOp = D3D12_BLEND_OP_ADD;
		lTargetBlendDesc.SrcBlend = D3D12_BLEND_INV_DEST_COLOR;
		lTargetBlendDesc.DestBlend = D3D12_BLEND_ONE;

		break;

	case BlendMode::AX_BLENDMODE_INVSRC:

		lBlendDesc.RenderTarget[ 0 ].BlendEnable = true;// ブレンドを有効
		lBlendDesc.RenderTarget[ 0 ].SrcBlend = D3D12_BLEND_INV_DEST_COLOR;// 1.0f-デストカラーの値
		lBlendDesc.RenderTarget[ 0 ].DestBlend = D3D12_BLEND_ZERO;// 使わない

		break;

	case BlendMode::AX_BLENDMODE_MAX:
	case BlendMode::AX_BLENDMODE_CUSTOM:
	default:
		break;
	}

	lBlendDesc.RenderTarget[ 0 ] = lTargetBlendDesc;

	return lBlendDesc;
}
