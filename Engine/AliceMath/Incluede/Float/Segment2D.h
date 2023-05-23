#pragma once
#include <Vector2.h>

/// <summary>
/// 線分
/// </summary>
class Segment2D
{
private:

	//始点
	AliceMathF::Vector2 start;
	//終点
	AliceMathF::Vector2 end;


public:

	Segment2D() = default;
	~Segment2D() = default;

	/// <summary>
	/// 始点をセット
	/// </summary>
	void SetStart(const AliceMathF::Vector2& pos);

	/// <summary>
	/// 始点を取得
	/// </summary>
	const AliceMathF::Vector2& GetStart();

	/// <summary>
	/// 終点をセット
	/// </summary>
	void SetEnd(const AliceMathF::Vector2& pos);

	/// <summary>
	/// 始点を取得
	/// </summary>
	const AliceMathF::Vector2& GetEnd();

	/// <summary>
	/// 線分を生成
	/// </summary>
	/// <param name="center">中心座標</param>
	/// <param name="axis">軸の方向</param>
	/// <param name="height">縦幅</param>
	void CreateSegment2D(const AliceMathF::Vector2& center, const AliceMathF::Vector2& axis, float height);

	AliceMathF::Vector2 GetVector();
};

