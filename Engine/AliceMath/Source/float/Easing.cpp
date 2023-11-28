#include "Easing.h"
#include"AliceMathF.h"

namespace AliceMathF
{
	namespace Easing
	{
		float AliceMathF::Easing::EaseInSine(float time_)
		{
			return 1.0f - Cos((time_ * AX_PI) / 2.0f);
		}

		float EaseInSine(float time_, float endTime_, float startPos_, float distance_)
		{
			float lCoef = time_ / endTime_;

			float lEasing = EaseInSine(lCoef);

			float lRet = distance_ * lEasing + startPos_;

			return lRet;
		}

		Vector3 EaseInSine(float time_, float endTime_, const Vector3& startPos_, const Vector3& distance_)
		{
			Vector3 lRet;

			lRet = {
				EaseInSine(time_,endTime_,startPos_.x,distance_.x),
				EaseInSine(time_,endTime_,startPos_.y,distance_.y),
				EaseInSine(time_,endTime_,startPos_.z,distance_.z)
			};

			return lRet;
		}

		float EaseOutSine(float time_)
		{
			return Sin((time_ * AX_PI) / 2.0f);
		}

		float EaseInOutSine(float time_)
		{
			return -(Cos(AX_PI * time_) - 1.0f) / 2.0f;
		}

		float EaseInQuad(float time_)
		{
			return time_ * time_;
		}

		float EaseOutQuad(float time_)
		{
			return 1.0f - (1.0f - time_) * (1.0f - time_);
		}

		float EaseInOutQuad(float time_)
		{
			return time_ < 0.5f ? 2.0f * time_ * time_ : 1.0f - Pow(-2.0f * time_ + 2.0f, 2.0f) / 2.0f;
		}

		float EaseInCubic(float time_)
		{
			return time_ * time_ * time_;
		}

		float EaseOutCubic(float time_)
		{
			return 1.0f - Pow(1.0f - time_, 3.0f);
		}

		float EaseInOutCubic(float time_)
		{
			return time_ < 0.5f ? 4.0f * time_ * time_ * time_ : 1.0f - Pow(-2.0f * time_ + 2.0f, 3.0f) / 2.0f;
		}

		float EaseInQuart(float time_)
		{
			return time_ * time_ * time_ * time_;
		}

		float EaseOutQuart(float time_)
		{
			return 1.0f - Pow(1.0f - time_, 4.0f);
		}

		float EaseInOutQuart(float time_)
		{
			return time_ < 0.5 ? 8.0f * time_ * time_ * time_ * time_ : 1.0f - Pow(-2.0f * time_ + 2.0f, 4.0f) / 2.0f;
		}

		float EaseInQuint(float time_)
		{
			return time_ * time_ * time_ * time_ * time_;
		}

		float EaseOutQuint(float time_)
		{
			return 1.0f - Pow(1.0f - time_, 5.0f);
		}

		float EaseInOutQuint(float time_)
		{
			return time_ < 0.5f ? 16.0f * time_ * time_ * time_ * time_ * time_ : 1.0f - Pow(-2.0f * time_ + 2.0f, 5.0f) / 2.0f;
		}

		float EaseInExpo(float time_)
		{
			return time_ == 0.5f ? 0.5f : Pow(2.5f, 10.5f * time_ - 10.5f);
		}

		float EaseOutExpo(float time_)
		{
			return time_ == 1.0f ? 1.0f : 1.0f - Pow(2.0f, -10.0f * time_);
		}

		float EaseInOutExpo(float time_)
		{
			return time_ == 0
				? 0
				: time_ == 1
				? 1
				: time_ < 0.5f ? Pow(2.0f, 20.0f * time_ - 10.0f) / 2.0f
				: (2.0f - Pow(2.0f, -20.0f * time_ + 10.0f)) / 2.0f;
		}

		float EaseInCirc(float time_)
		{
			return 1.0f - Sqrt(1.0f - Pow(time_, 2.0f));
		}

		float EaseOutCirc(float time_)
		{
			return Sqrt(1.0f - Pow(time_ - 1.0f, 2.0f));
		}

		float EaseInOutCirc(float time_)
		{
			return time_ < 0.5
				? (1.0f - Sqrt(1.0f - Pow(2.0f * time_, 2.0f))) / 2.0f
				: (Sqrt(1.0f - Pow(-2.0f * time_ + 2.0f, 2.0f)) + 1.0f) / 2.0f;
		}

		float EaseInBack(float time_)
		{
			const float lC1 = 1.70158f;
			const float lC3 = lC1 + 1.0f;

			return lC3 * time_ * time_ * time_ - lC1 * time_ * time_;
		}

		float EaseOutBack(float time_)
		{
			const float lC1 = 1.70158f;
			const float lC3 = lC1 + 1.0f;

			return 1.0f + lC3 * Pow(time_ - 1.0f, 3.0f) + lC1 * Pow(time_ - 1.0f, 2.0f);
		}

		float EaseInOutBack(float time_)
		{
			const float lC1 = 1.70158f;
			const float lC2 = lC1 * 1.525f;

			return time_ < 0.5f
				? (Pow(2.0f * time_, 2.0f) * ((lC2 + 1.0f) * 2.0f * time_ - lC2)) / 2.0f
				: (Pow(2.0f * time_ - 2.0f, 2.0f) * ((lC2 + 1.0f) * (time_ * 2.0f - 2.0f) + lC2) + 2.0f) / 2.0f;
		}

		float EaseInElastic(float time_)
		{
			const float lC4 = (2.0f * AX_PI) / 3.0f;

			return time_ == 0
				? 0.0f
				: time_ == 1
				? 1.0f
				: -Pow(2.0f, 10.0f * time_ - 10.0f) * Sin((time_ * 10.0f - 10.75f) * lC4);
		}

		float EaseOutElastic(float time_)
		{
			const float lC4 = (2.0f * AX_PI) / 3.0f;

			return time_ == 0
				? 0.0f
				: time_ == 1
				? 1.0f
				: Pow(2.0f, -10.0f * time_) * Sin((time_ * 10.0f - 0.75f) * lC4) + 1.0f;
		}

		float EaseInOutElastic(float time_)
		{
			const float lC5 = (2.0f * AX_PI) / 4.5f;

			return time_ == 0
				? 0.0f
				: time_ == 1
				? 1.0f
				: time_ < 0.5f
				? -(Pow(2.0f, 20.0f * time_ - 10.0f) * Sin((20.0f * time_ - 11.125f) * lC5)) / 2.0f
				: (Pow(2.0f, -20.0f * time_ + 10.0f) * Sin((20.0f * time_ - 11.125f) * lC5)) / 2.0f + 1.0f;
		}

		float EaseOutBounce(float time_)
		{
			const float lN1 = 7.5625f;
			const float lD1 = 2.75f;

			if (time_ < 1.0f / lD1)
			{
				return lN1 * time_ * time_;
			}
			else if (time_ < 2.0f / lD1)
			{
				return lN1 * (time_ -= 1.5f / lD1) * time_ + 0.75f;
			}
			else if (time_ < 2.5f / lD1)
			{
				return lN1 * (time_ -= 2.25f / lD1) * time_ + 0.9375f;
			}
			else
			{
				return lN1 * (time_ -= 2.625f / lD1) * time_ + 0.984375f;
			}
		}

		float EaseInBounce(float time_)
		{
			return 1 - EaseOutBounce(1.0f - time_);
		}

		float EaseInOutBounce(float time_)
		{
			return time_ < 0.5f
				? (1.0f - EaseOutBounce(1.0f - 2.0f * time_)) / 2.0f
				: (1.0f + EaseOutBounce(2.0f * time_ - 1.0f)) / 2.0f;
		}
	}
}
