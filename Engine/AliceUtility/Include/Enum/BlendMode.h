#pragma once

//ブレンドモード
enum class BlendMode
{
	AX_BLENDMODE_NOBLEND,//ノーブレンド（デフォルト）
	AX_BLENDMODE_ALPHA,//αブレンド
	AX_BLENDMODE_ADD,//加算ブレンド
	AX_BLENDMODE_SUB,//減算ブレンド
	AX_BLENDMODE_MULA,//乗算ブレンド
	AX_BLENDMODE_INVSRC,//反転ブレンド
	AX_BLENDMODE_SCREEN,

	AX_BLENDMODE_MAX,//ブレンド種類数

	AX_BLENDMODE_CUSTOM = -1,
};