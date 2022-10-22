#pragma once
using Byte = unsigned char;

struct TColor
{
	static const TColor UnloadedTextureColor;
	static const TColor UnhandledTextureColor;
	
	static const TColor Red;
	static const TColor Green;
	static const TColor Blue;
	static const TColor White;
	static const TColor Black;

	TColor(Byte red, Byte green, Byte blue, Byte alpha = 255);

	Byte r;
	Byte g;
	Byte b;
	Byte a;

};

