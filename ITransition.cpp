#include "ITransition.h"

void ITransition::SetEasing(const std::function<float(float)>& easing_)
{
	easing = easing_;
}

void ITransition::SetIncrement(float increment_)
{
	increment = increment_;
}

bool ITransition::IsEnd()
{
    return isEnd;
}
