#pragma once
#pragma warning(push)
#pragma warning(disable: 4365)
#pragma warning(disable: 4514)
#pragma warning(disable: 4619)
#pragma warning(disable: 4668)
#pragma warning(disable: 4710)
#pragma warning(disable: 5039)

#include<vector>

#pragma warning(pop)

#include<Vector.h>
#include<Matrix.h>
#include<Quaternion.h>
#include<Segment2D.h>
#include<Line2D.h>
#include<Easing.h>

namespace AliceMathF
{
#pragma region 定数
#pragma warning(push)
#pragma warning(disable: 5264)
	//π
	constexpr float AX_PI = 3.1415926535897931f;

	//π/2
	constexpr float AX_PIHalf = 3.1415926535897931f / 2.0f;

	// 2π
	constexpr float AX_2PI = 3.1415926535897931f * 2;

	//ラジアンから度に変換する定数
	constexpr float RAD_TO_DEG = 57.29578f;

	//度からラジアンに変換する定数
	constexpr float DEG_TO_RAD = AX_PI / 180.0f;

	//ε
	constexpr float EPSILON = 2.2204460492503131E-16f;
#pragma warning(pop)

#pragma endregion 定数

#pragma region sin,cos,tan関数

#pragma region sin関数

	//sin関数
	float Sin(float sin_);

	//asin関数
	float Asin(float sin_);

#pragma endregion sin関数

#pragma region cos関数

	//cos関数
	float Cos(float cos_);

	//cos関数
	float Acos(float cos_);

#pragma endregion cos関数

#pragma region tan関数

	//tan関数
	float Tan(float tan_);

	//atan関数
	float Atan(float tan_);

	//atan2関数
	float Atan2(float y_, float x_);

#pragma endregion tan関数

	/// <summary>
	/// sin、cosを両方出す
	/// </summary>
	/// <param name="sin_"> : sinの結果を代入する</param>
	/// <param name="cos_"> : cosの結果を代入する</param>
	/// <param name="angle"> : ラジアン角</param>
	void SinCos(float& sin_, float& cos_, float angle_);

#pragma endregion sin,cos,tan関数

#pragma region 角度変換

	/// <summary>
	/// 度数法からラジアン変換
	/// </summary>
	/// <param name="angle"> : 角度</param>
	float ChangeRadians(const float& angle_);

	/// <summary>
	/// ラジアンから度数法変換
	/// </summary>
	/// <param name="angle"> : 角度</param>
	float ChangeDira(const float& angle_);

#pragma endregion 角度変換

#pragma region sqrt関数

	//sqrt関数
	float Sqrt(float sqrt_);


#pragma endregion sqrt関数

#pragma region pow関数

	//pow関数
	float Pow(float pow_, float powNum_);

#pragma endregion pow関数

#pragma region exp関数

	//exp関数
	float Exp(float exp_);

#pragma endregion exp関数

#pragma region log関数

	//log関数
	float Log(float log_);

	//log10関数
	float Log10(float log_);

#pragma endregion log関数

#pragma region 絶対値の計算

	//絶対値の計算
	float Abs(float abs_);

#pragma endregion 絶対値の計算

#pragma region ceil関数

	//ceil関数
	float Ceil(float ceil_);

#pragma endregion ceil関数

#pragma region floor関数

	//floor関数
	float Floor(float floor_);

#pragma endregion floor関数

#pragma region round関数

	//round関数
	float Round(float round_);

#pragma endregion round関数

	/// <summary>
	/// 値の符号を返す
	/// </summary>
	/// <returns>値が正か0の場合は1を、負の場合は-1を返す</returns>
	float Sign(float sign_);

	/// <summary>
	/// 値を範囲内に納める
	/// </summary>
	/// <param name="Value">値</param>
	/// <param name="low">最低値</param>
	/// <param name="high">最高値</param>
	float Clamp(float Value_, const float low_, const float high_);

	/// <summary>
	/// 0と1の間に値を納め、その値を返す
	/// </summary>
	/// <param name="Value">値</param>
	float Clamp01(float Value_);

	/// <summary>
	/// 線形補間(0～1)
	/// </summary>
	/// <param name="a">開始値</param>
	/// <param name="b">終了値</param>
	/// <param name="t">補間値</param>
	/// <returns>補間された値</returns>
	float Lerp(float a_, float b_, float t_);

	/// <summary>
	/// 線形補間(0～1)
	/// </summary>
	/// <param name="a">開始値</param>
	/// <param name="b">終了値</param>
	/// <param name="t">補間値</param>
	/// <returns>補間された値</returns>
	Vector3 Lerp(const Vector3& a_, const Vector3& b_, float t_);

	/// <summary>
	/// 線形補間(0～1)
	/// </summary>
	/// <param name="a">開始値</param>
	/// <param name="b">終了値</param>
	/// <param name="t">補間値</param>
	/// <returns>補間された値</returns>
	Vector4 Lerp(const Vector4& a_, const Vector4& b_, float t_);

	Vector3 Slerp(const Vector3& v1_, const Vector3& v2_, float t_);

	/// <summary>
	/// 線形補間(0～)
	/// </summary>
	/// <param name="a">開始値</param>
	/// <param name="b">終了値</param>
	/// <param name="t">補間値</param>
	/// <returns>補間された値</returns>
	float LerpUnclamped(float a_, float b_, float t_);

	/// <summary>
	/// Lerpの角度版(0～1)
	/// </summary>
	/// <param name="a">開始値</param>
	/// <param name="b">終了値</param>
	/// <param name="t">補間値</param>
	/// <returns>補間された角度</returns>
	float LerpAngle(float a_, float b_, float t_);

	/// <summary>
	/// tはlength_より大きくはならず0より小さくならず、その間をループ
	/// </summary>
	/// <param name="t">値</param>
	/// <param name="length_">指定値</param>
	float Repeat(float t_, float length_);

	/// <summary>
	/// 現在地から目的地まで一定速度で移動
	/// </summary>
	/// <param name="current">現在地</param>
	/// <param name="target">目的地</param>
	/// <param name="maxDelta">速度</param>
	float MoveTowards(float current_, float target_, float maxDelta_);

	/// <summary>
	/// 現在の角度から目的の角度まで一定速度で移動
	/// </summary>
	/// <param name="current">現在の角度</param>
	/// <param name="target">目的の角度</param>
	/// <param name="maxDelta">速度</param>
	float MoveTowardsAngle(float current_, float target_, float maxDelta_);

	/// <summary>
	/// 角度の差を返す(度数法)
	/// </summary>
	/// <param name="current">現在の角度</param>
	/// <param name="target">目標となる角度</param>
	float DeltaAngle(float current_, float target_);

	/// <summary>
	/// スムージング補間
	/// </summary>
	/// <param name="from">開始値</param>
	/// <param name="to">終了値</param>
	/// <param name="t">補間値</param>
	/// <returns>補間された角度</returns>
	float SmoothStep(float from_, float to_, float t_);

	/// <summary>
	/// ガンマ補正
	/// </summary>
	float Gamma(float value_, float absmax_, float gamma_);

	/// <summary>
	/// 何を何処に何秒で指定してオブジェクトを移動させる
	/// </summary>
	/// <param name="current">現在地</param>
	/// <param name="target">目的地</param>
	/// <param name="currentVelocity">事前に初期化した速度</param>
	/// <param name="smoothTime">目的までの到達時間</param>
	/// <param name="maxSpeed">最高速度</param>
	/// <param name="deltaTime">関数が前回実行されてからの経過時間</param>
	float SmoothDamp(float current_, float target_, float& currentVelocity_, float smoothTime_, float maxSpeed_, float deltaTime_);

	/// <summary>
	/// tが0とlength_の間で往復する(0～length_)
	/// </summary>
	float PingPong(float t_, float length_);

	/// <summary>
	/// lerpの補間値を出す
	/// </summary>
	/// <param name="a">開始値</param>
	/// <param name="b">終了値</param>
	/// <param name="value">開始と終了の間の値</param>
	/// <returns>補間値</returns>
	float InverseLerp(float a_, float b_, float value_);

	/// <summary>
	/// 2本の線の交点を計算
	/// </summary>
	/// <param name="p1">始点1</param>
	/// <param name="p2">終点1</param>
	/// <param name="p3">始点2</param>
	/// <param name="p4">終点2</param>
	/// <param name="result">結果(出力用)</param>
	/// <returns>交差しているか</returns>
	bool LineIntersection(const Vector2& p1_, const Vector2& p2_, const Vector2& p3_, const Vector2& p4_, Vector2& result_);

	bool LineSegmentIntersection(const Vector2& p1_, const Vector2& p2_, const Vector2& p3_, const Vector2& p4_, Vector2& result_);

	/// <summary>
	/// 二つの値がほぼ等しいか
	/// </summary>
	bool Approximately(float a_, float b_);

	/// <summary>
	/// ランダムな値を取得
	/// </summary>
	/// <param name="min">最小値</param>
	/// <param name="max">最大値</param>
	float GetRand(float min_, float max_);

		/// <summary>
	/// エルミート曲線
	/// </summary>
	/// <param name="p0">制御点</param>
	/// <param name="p1">制御点</param>
	/// <param name="v0">制御点</param>
	/// <param name="v1">制御点</param>
	/// <param name="t">時間(0.0～1.0)</param>
	/// <returns></returns>
	Vector3 HermiteGetPoint(const Vector3& p0_, const Vector3& p1_, const Vector3& v0_, const Vector3& v1_, float t_);

	/// <summary>
	/// キャットムルーロムスプライン 
	/// </summary>
	/// <param name="points">制御点</param>
	/// <param name="t">時間(0.0～1.0)</param>
	Vector3 CatmullRomSpline(std::vector<Vector3>& points_, float t_);

	// 分離軸に投影された軸成分から投影線分長を算出
	float LenSegOnSeparateAxis(const Vector3* sep_, const Vector3* e1_, const Vector3* e2_, const Vector3* e3_ = 0);

	/// <summary>
	/// 対角線を出す
	/// </summary>
	/// <param name="width">横幅</param>
	/// <param name="height">縦幅</param>
	/// <returns>対角線</returns>
	float GetDiagonal(float width_, float height_);

	/// <summary>
	/// 中心点を出す
	/// </summary>
	/// <param name="p1">1つ目の座標</param>
	/// <param name="p2">2つ目の座標</param>
	float GetCenter(float p1_,float p2_);

	AliceMathF::Vector4 GetColer(uint8_t r,uint8_t g,uint8_t b,uint8_t a = 255);
};

