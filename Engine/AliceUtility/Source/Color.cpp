#include "Color.h"
#include"AliceAssert.h"

ALICE_UTILITY_NAMESPACE_BEGIN

Color::Color(uint32_t color_) : color(color_)
{
}

Color::Color(uint8_t red_,uint8_t green_,uint8_t blue_,uint8_t alpha_) : red(red_),green(green_),blue(blue_),alpha(alpha_) {}

Color::Color(const Color& color_,uint8_t alpha_) : red(color_.red),green(color_.green),blue(color_.blue),alpha(color_.alpha) {}

bool Color::operator==(const Color& color_) const
{
	return color == color_.color;
}

bool Color::operator!=(const Color& color_) const
{
	return color != color_.color;
}

uint32_t Color::GetUInt32() const
{
	return color;
}

uint8_t Color::operator()(uint32_t index_) const
{
	AliceAssertArray(inIdx, 4);
	return ( &red )[ index_ ];
}

uint8_t& Color::operator()(uint32_t index_)
{
	AliceAssertArray(inIdx,4);
	return ( &red )[ index_ ];
}

const Color Color::sBlack(0,0,0);
const Color Color::sRed(255,0,0);
const Color Color::sGreen(0,255,0);
const Color Color::sBlue(0,0,255);
const Color Color::sYellow(255,255,0);
const Color Color::sPurple(255,0,255);
const Color Color::sCyan(0,255,255);
const Color Color::sOrange(255,128,0);
const Color Color::sGrey(128,128,128);
const Color Color::sWhite(255,255,255);

ALICE_UTILITY_NAMESPACE_END