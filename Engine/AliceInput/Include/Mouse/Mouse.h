#pragma once

#pragma warning(push)
#pragma warning(disable: 4365)
#pragma warning(disable: 4668)
#pragma warning(disable: 4710)
#pragma warning(disable: 4711)
#pragma warning(disable: 4820)
#pragma warning(disable: 5039)

#include<memory>
#include<stdint.h>

#pragma warning(pop)

#include<AliceMathUtility.h>

#include"MouseButton.h"

namespace AliceInput
{

	class IMouse
	{
	public:

		/// <summary>
		/// 初期化
		/// </summary>
		/// <param name="directInput_">ダイレクトインプット</param>
		/// <param name="hwnd_">ウィンドウハンドル</param>
		virtual void Initialize(void* directInput_, void* hwnd_) = 0;

		/// <summary>
		/// 更新
		/// </summary>
		/// <param name="width_">ウィンドウ横幅</param>
		/// <param name="height_">ウィンドウ縦幅</param>
		virtual void Update(float width_, float height_) = 0;

		/// <summary>
		/// マウスボタンのトリガー入力
		/// </summary>
		virtual bool TriggerMouseButton(MouseButton button_) = 0;

		/// <summary>
		/// マウスボタンの入力
		/// </summary>
		virtual bool InputMouseButton(MouseButton button_) = 0;

		/// <summary>
		/// マウスボタンの離した瞬間
		/// </summary>
		virtual bool TriggerReleaseMouseButton(MouseButton button_) = 0;

		/// <summary>
		/// カーソルのクライアント座標位置(クランプ)
		/// </summary>
		/// <returns>マウスの位置</returns>
		virtual const AliceMathF::Vector2& GetMousePos() = 0;

		/// <summary>
		/// カーソルのクライアント座標位置
		/// </summary>
		/// <returns>マウスの位置</returns>
		virtual const AliceMathF::Vector2& GetWorldMousePos() = 0;

		/// <summary>
		/// カーソルのワールド座標位置
		/// </summary>
		/// <returns>マウスの位置</returns>
		virtual const AliceMathF::Vector2& GetLocalMousePos() = 0;

		/// <summary>
		/// マウスの位置
		/// </summary>
		/// <returns>マウスの位置</returns>
		virtual const AliceMathF::Vector3& GetMouseMove() = 0;

		IMouse() = default;
		virtual ~IMouse() = default;
	};

	/// <summary>
	/// マウスの生成(ユニーク)
	/// </summary>
	/// <param name="directInput_">ダイレクトインプット</param>
	/// <param name="hwnd_">ウィンドウハンドル</param>
	/// <returns>生成されたポインタ</returns>
	std::unique_ptr<IMouse> CreateUniqueMouse(void* directInput_, void* hwnd_);

	/// <summary>
	/// マウスの生成(シェアード)
	/// </summary>
	/// <param name="directInput_">ダイレクトインプット</param>
	/// <param name="hwnd_">ウィンドウハンドル</param>
	/// <returns>生成されたポインタ</returns>
	std::shared_ptr<IMouse> CreateSharedMouse(void* directInput_, void* hwnd_);
}