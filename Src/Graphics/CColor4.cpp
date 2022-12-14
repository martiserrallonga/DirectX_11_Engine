#include "CColor4.h"

CColor4::CColor4(const aiColor4D& color) {
	r = static_cast<uint8_t>(color.r * 255.f);
	g = static_cast<uint8_t>(color.g * 255.f);
	b = static_cast<uint8_t>(color.b * 255.f);
	a = static_cast<uint8_t>(color.a * 255.f);
}

CColor4::operator aiColor4D() const {
	aiColor4D color;
	color.r = static_cast<float>(r) * 255.f;
	color.g = static_cast<float>(g) * 255.f;
	color.b = static_cast<float>(b) * 255.f;
	color.a = static_cast<float>(a) * 255.f;
	return color;
}
