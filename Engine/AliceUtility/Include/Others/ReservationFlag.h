#pragma once
#include<PadingType.h>

/// <summary>
/// フラグを好きなタイミングで変えれる
/// </summary>
class ReservationFlag
{
private:
	bool flag;
	bool reservation;

	Byte6 PADING;
public:

	/// <summary>
	/// 強制的にフラグを変更
	/// </summary>
	void ForceSetFlag(bool flag_);

	/// <summary>
	/// フラグ変更を予約
	/// </summary>
	void SetFlag(bool flag_);

	/// <summary>
	/// 判定
	/// </summary>
	void Determine();

	explicit operator bool() const noexcept;

	bool operator!() const noexcept;
};