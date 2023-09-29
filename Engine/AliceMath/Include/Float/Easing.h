#pragma once
#include<Vector3.h>

namespace AliceMathF
{
	namespace Easing
	{
		float EaseInSine(float time_);

		float EaseInSine(float time_, float endTime_, float startPos_, float distance_);

		Vector3 EaseInSine(float time_, float endTime_,const Vector3& startPos_, const Vector3& distance_);

		float EaseOutSine(float time_);

		float EaseInOutSine(float time_);

		float EaseInQuad(float time_);

		float EaseOutQuad(float time_);

		float EaseInOutQuad(float time_);

		float EaseInCubic(float time_);

		float EaseOutCubic(float time_);

		float EaseInOutCubic(float time_);

		float EaseInQuart(float time_);

		float EaseOutQuart(float time_);

		float EaseInOutQuart(float time_);

		float EaseInQuint(float time_);

		float EaseOutQuint(float time_);

		float EaseInOutQuint(float time_);

		float EaseInExpo(float time_);

		float EaseOutExpo(float time_);

		float EaseInOutExpo(float time_);

		float EaseInCirc(float time_);

		float EaseOutCirc(float time_);

		float EaseInOutCirc(float time_);

		float EaseInBack(float time_);

		float EaseOutBack(float time_);

		float EaseInOutBack(float time_);

		float EaseInElastic(float time_);

		float EaseOutElastic(float time_);

		float EaseInOutElastic(float time_);

		float EaseOutBounce(float time_);

		float EaseInBounce(float time_);

		float EaseInOutBounce(float time_);

	}
}