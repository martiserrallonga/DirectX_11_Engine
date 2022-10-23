#pragma once

#include "assimp/color4.h"

using Byte = unsigned char;

class CColor4 : public aiColor4t<Byte>
{
	using aiColor4t::aiColor4t;

public:
	explicit CColor4(const aiColor4D& color);
	explicit operator aiColor4D() const;

	bool IsBlack() const {
		return r == 0 && g == 0 && b == 0;
	}
};

namespace color {
	const CColor4 UnloadedTextureColor(100, 100, 100, 255);
	const CColor4 UnhandledTextureColor( 250, 0, 0, 255 );
	const CColor4 Red(255, 0, 0, 255);
	const CColor4 Green(0, 255, 0, 255);
	const CColor4 Blue(0, 0, 255, 255);
	const CColor4 White(255, 255, 255, 255);
	const CColor4 Black(0, 0, 0, 255);
}
