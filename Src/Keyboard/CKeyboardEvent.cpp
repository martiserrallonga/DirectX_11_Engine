#include "CKeyboardEvent.h"

CKeyboardEvent::CKeyboardEvent()
	: Type(EEventType::Invalid)
	, Key(0U)
{}

CKeyboardEvent::CKeyboardEvent(const EEventType aType, const unsigned char aKey)
	: Type(aType)
	, Key(aKey)
{}

bool CKeyboardEvent::IsPress() const
{
	return Type == EEventType::Press;
}

bool CKeyboardEvent::IsRelease() const
{
	return Type == EEventType::Release;
}

bool CKeyboardEvent::IsValid() const
{
	return Type != EEventType::Invalid;
}

unsigned char CKeyboardEvent::GetKeyCode() const
{
	return Key;
}


