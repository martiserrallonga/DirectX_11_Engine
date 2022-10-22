#include "TColor.h"

const TColor TColor::UnloadedTextureColor = TColor(100, 100, 100);
const TColor TColor::UnhandledTextureColor = TColor(250, 0, 0);

const TColor TColor::Red = TColor(255, 0, 0);
const TColor TColor::Green = TColor(0, 255, 0);
const TColor TColor::Blue = TColor(0, 0, 255);
const TColor TColor::White = TColor(255, 255, 255);
const TColor TColor::Black = TColor(0, 0, 0);

TColor::TColor(Byte red, Byte green, Byte blue, Byte alpha)
	: r(red)
	, g(green)
	, b(blue)
	, a(alpha)
{}
