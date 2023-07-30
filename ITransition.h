#pragma once
#include<functional>

class ITransition
{
protected:

	bool isEnd = false;
	bool start = false;

	float frame = 0.0f;

	float increment = 0.01f;

	std::function<float(float)> easing;

public:

	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Initilize(float windowHeight, float windowWidth) = 0;
	 
	/// <summary>
	/// 終了
	/// </summary>
	virtual void Finalize() = 0;

	/// <summary>
	/// 毎フレーム更新
	/// </summary>
	virtual void Update() = 0;

	/// <summary>
	/// 描画
	/// </summary>
	virtual void Draw() = 0;

	void SetEasing(const std::function<float(float)>& easing_);

	void SetIncrement(float increment_ = 0.01f);

	/// <summary>
	/// 終了したか
	/// </summary>
	bool IsEnd();

	virtual bool Start() = 0;

	ITransition() = default;
	virtual ~ITransition() = default;
};

