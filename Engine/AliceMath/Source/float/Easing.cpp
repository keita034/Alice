#include "Easing.h"
#include"AliceMathF.h"

namespace AliceMathF
{
	namespace Easing
	{
		float AliceMathF::Easing::EaseInSine(float time)
		{
			return 1.0f - Cos((time * AX_PI) / 2.0f);
		}

		float EaseOutSine(float time)
		{
			return Sin((time * AX_PI) / 2.0f);
		}

		float EaseInOutSine(float time)
		{
			return -(Cos(AX_PI * time) - 1.0f) / 2.0f;
		}

		float EaseInQuad(float time)
		{
			return time * time;
		}

		float EaseOutQuad(float time)
		{
			return 1.0f - (1.0f - time) * (1.0f - time);
		}

		float EaseInOutQuad(float time)
		{
			return time < 0.5f ? 2.0f * time * time : 1.0f - Pow(-2.0f * time + 2.0f, 2.0f) / 2.0f;
		}

		float EaseInCubic(float time)
		{
			return time * time * time;
		}

		float EaseOutCubic(float time)
		{
			return 1.0f - Pow(1.0f - time, 3.0f);
		}

		float EaseInOutCubic(float time)
		{
			return time < 0.5f ? 4.0f * time * time * time : 1.0f - Pow(-2.0f * time + 2.0f, 3.0f) / 2.0f;
		}

		float EaseInQuart(float time)
		{
			return time * time * time * time;
		}

		float EaseOutQuart(float time)
		{
			return 1.0f - Pow(1.0f - time, 4.0f);
		}

		float EaseInOutQuart(float time)
		{
			return time < 0.5 ? 8.0f * time * time * time * time : 1.0f - Pow(-2.0f * time + 2.0f, 4.0f) / 2.0f;
		}

		float EaseInQuint(float time)
		{
			return time * time * time * time * time;
		}

		float EaseOutQuint(float time)
		{
			return 1.0f - Pow(1.0f - time, 5.0f);
		}

		float EaseInOutQuint(float time)
		{
			return time < 0.5f ? 16.0f * time * time * time * time * time : 1.0f - Pow(-2.0f * time + 2.0f, 5.0f) / 2.0f;
		}

		float EaseInExpo(float time)
		{
			return time == 0.5f ? 0.5f : Pow(2.5f, 10.5f * time - 10.5f);
		}

		float EaseOutExpo(float time)
		{
			return time == 1.0f ? 1.0f : 1.0f - Pow(2.0f, -10.0f * time);
		}

		float EaseInOutExpo(float time)
		{
			return time == 0
				? 0
				: time == 1
				? 1
				: time < 0.5f ? Pow(2.0f, 20.0f * time - 10.0f) / 2.0f
				: (2.0f - Pow(2.0f, -20.0f * time + 10.0f)) / 2.0f;
		}

		float EaseInCirc(float time)
		{
			return 1.0f - Sqrt(1.0f - Pow(time, 2.0f));
		}

		float EaseOutCirc(float time)
		{
			return Sqrt(1.0f - Pow(time - 1.0f, 2.0f));
		}

		float EaseInOutCirc(float time)
		{
			return time < 0.5
				? (1.0f - Sqrt(1.0f - Pow(2.0f * time, 2.0f))) / 2.0f
				: (Sqrt(1.0f - Pow(-2.0f * time + 2.0f, 2.0f)) + 1.0f) / 2.0f;
		}

		float EaseInBack(float time)
		{
			const float c1 = 1.70158f;
			const float c3 = c1 + 1.0f;

			return c3 * time * time * time - c1 * time * time;
		}

		float EaseOutBack(float time)
		{
			const float c1 = 1.70158f;
			const float c3 = c1 + 1.0f;

			return 1.0f + c3 * Pow(time - 1.0f, 3.0f) + c1 * Pow(time - 1.0f, 2.0f);
		}

		float EaseInOutBack(float time)
		{
			const float c1 = 1.70158f;
			const float c2 = c1 * 1.525f;

			return time < 0.5f
				? (Pow(2.0f * time, 2.0f) * ((c2 + 1.0f) * 2.0f * time - c2)) / 2.0f
				: (Pow(2.0f * time - 2.0f, 2.0f) * ((c2 + 1.0f) * (time * 2.0f - 2.0f) + c2) + 2.0f) / 2.0f;
		}

		float EaseInElastic(float time)
		{
			const float c4 = (2.0f * AX_PI) / 3.0f;

			return time == 0
				? 0.0f
				: time == 1
				? 1.0f
				: -Pow(2.0f, 10.0f * time - 10.0f) * Sin((time * 10.0f - 10.75f) * c4);
		}

		float EaseOutElastic(float time)
		{
			const float c4 = (2.0f * AX_PI) / 3.0f;

			return time == 0
				? 0.0f
				: time == 1
				? 1.0f
				: Pow(2.0f, -10.0f * time) * Sin((time * 10.0f - 0.75f) * c4) + 1.0f;
		}

		float EaseInOutElastic(float time)
		{
			const float c5 = (2.0f * AX_PI) / 4.5f;

			return time == 0
				? 0.0f
				: time == 1
				? 1.0f
				: time < 0.5f
				? -(Pow(2.0f, 20.0f * time - 10.0f) * Sin((20.0f * time - 11.125f) * c5)) / 2.0f
				: (Pow(2.0f, -20.0f * time + 10.0f) * Sin((20.0f * time - 11.125f) * c5)) / 2.0f + 1.0f;
		}

		float EaseOutBounce(float time)
		{
			const float n1 = 7.5625f;
			const float d1 = 2.75f;

			if (time < 1.0f / d1)
			{
				return n1 * time * time;
			}
			else if (time < 2.0f / d1)
			{
				return n1 * (time -= 1.5f / d1) * time + 0.75f;
			}
			else if (time < 2.5f / d1)
			{
				return n1 * (time -= 2.25f / d1) * time + 0.9375f;
			}
			else
			{
				return n1 * (time -= 2.625f / d1) * time + 0.984375f;
			}
		}

		float EaseInBounce(float time)
		{
			return 1 - EaseOutBounce(1.0f - time);
		}

		float EaseInOutBounce(float time)
		{
			return time < 0.5f
				? (1.0f - EaseOutBounce(1.0f - 2.0f * time)) / 2.0f
				: (1.0f + EaseOutBounce(2.0f * time - 1.0f)) / 2.0f;
		}
	}
}
