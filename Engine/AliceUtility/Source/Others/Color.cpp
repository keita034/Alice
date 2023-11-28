#include "Color.h"
#include<AliceAssert.h>

ALICE_UTILITY_NAMESPACE_BEGIN

Color::Color(uint32_t color_) : color(color_)
{
}

Color::Color(uint8_t red_,uint8_t green_,uint8_t blue_,uint8_t alpha_)
{
	colorElement.red = red_;
	colorElement.green = green_;
	colorElement.blue = blue_;
	colorElement.alpha = alpha_;
}

Color::Color(const Color& color_,uint8_t alpha_)
{
	colorElement.red = color_.colorElement.red;
	colorElement.green = color_.colorElement.green;
	colorElement.blue = color_.colorElement.blue;
	colorElement.alpha = alpha_;
}

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

uint8_t Color::operator()(size_t index_) const
{
	AliceAssertArray(index_,4llu,"カラーの配列外アクセス違反");
	return ( &colorElement.red )[ index_ ];
}

uint8_t& Color::operator()(size_t index_)
{
	AliceAssertArray(index_,4llu,"カラーの配列外アクセス違反");
	return ( &colorElement.red )[ index_ ];
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