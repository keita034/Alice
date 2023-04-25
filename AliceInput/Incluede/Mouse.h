#include<AliceMathUtility.h>

enum class MouseButton
{
	LEFT,
	RIGHT,
	CENTER
};

namespace AliceInput
{

	class IMouse
	{
	public:

		/// <summary>
		/// 初期化
		/// </summary>
		virtual void Initialize(void* dinput, void* hwnd) = 0;

		/// <summary>
		/// 更新
		/// </summary>
		virtual void Update(float width, float height) = 0;

		/// <summary>
		/// マウスボタンのトリガー入力
		/// </summary>
		virtual bool TriggerMouseButton(MouseButton button) = 0;

		/// <summary>
		/// マウスボタンの入力
		/// </summary>
		virtual bool InputMouseButton(MouseButton button) = 0;

		/// <summary>
		/// マウスボタンの離した瞬間
		/// </summary>
		virtual bool TriggerReleaseMouseButton(MouseButton button) = 0;

		/// <summary>
		/// マウスの位置
		/// </summary>
		/// <returns>マウスの位置</returns>
		virtual const AliceMathF::Vector2& GetMousePos() = 0;

		/// <summary>
		/// マウスの位置
		/// </summary>
		/// <returns>マウスの位置</returns>
		virtual const AliceMathF::Vector2& GetWorldMousePos() = 0;

		/// <summary>
		/// マウスの位置
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

	//マウスを生成
	IMouse* CreateMouse(void* directInput, void* hwnd);
}