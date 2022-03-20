#pragma once

class CKeyboardEvent
{
public:
	enum class EEventType
	{
		Press,
		Release,
		Invalid,
	};

	CKeyboardEvent();
	CKeyboardEvent(const EEventType aType, const unsigned char aKey);
	bool IsPress() const;
	bool IsRelease() const;
	bool IsValid() const;
	unsigned char GetKeyCode() const;

private:
	EEventType Type;
	unsigned char Key;

};