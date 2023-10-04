#pragma once
#include<stdint.h>
#include<Defined.h>

ALICE_UTILITY_NAMESPACE_BEGIN

class Color
{
public:

	union
	{
		uint32_t color;

		struct
		{
			uint8_t red;
			uint8_t green;
			uint8_t blue;
			uint8_t alpha;
		};
	};

public:

	Color() = default;
	Color(const Color& color_) = default;
	Color(uint32_t color_);
	Color(uint8_t red_,uint8_t green_,uint8_t blue_,uint8_t alpha_ = 255);
	Color(const Color& color_,uint8_t alpha_);

	bool operator == (const Color& color_) const;
	bool operator != (const Color& color_) const;

	uint32_t GetUInt32() const;

	uint8_t operator () (uint32_t index_) const;

	uint8_t& operator () (uint32_t index_);

	const static  Color sBlack;
	const static  Color sDarkRed;
	const static  Color sRed;
	const static  Color sDarkGreen;
	const static  Color sGreen;
	const static  Color sDarkBlue;
	const static  Color sBlue;
	const static  Color sYellow;
	const static  Color sPurple;
	const static  Color sCyan;
	const static  Color sOrange;
	const static  Color sDarkOrange;
	const static  Color sGrey;
	const static  Color sLightGrey;
	const static  Color sWhite;
};

ALICE_UTILITY_NAMESPACE_END