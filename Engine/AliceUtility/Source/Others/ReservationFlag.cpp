#include "ReservationFlag.h"

void ReservationFlag::ForceSetFlag(bool flag_)
{
	flag = flag_;
}

void ReservationFlag::SetFlag(bool flag_)
{
	reservation = flag_;
}

void ReservationFlag::Determine()
{
	if ( reservation )
	{
		reservation = false;

		flag = !flag;
	}
}

ReservationFlag::operator bool() const noexcept
{
	return flag;
}

bool ReservationFlag::operator!() const noexcept
{
	return !static_cast< bool >( *this ) ;
}
