#include "CKeyboard.h"

using EEvent = CKeyboardEvent::EEventType;

CKeyboardEvent CKeyboard::ReadKey()
{
	if (mKeyBuffer.empty()) return CKeyboardEvent();

	CKeyboardEvent Event = mKeyBuffer.front();
	mKeyBuffer.pop();
	return Event;
}

unsigned char CKeyboard::ReadChar()
{
	if (mCharBuffer.empty()) return 0U;

	unsigned char Event = mCharBuffer.front();
	mCharBuffer.pop();
	return Event;
}

bool CKeyboard::IsKeyPressed(unsigned char keycode) const
{
	return mKeyStates[keycode];
}

bool CKeyboard::IsKeyBufferEmpty() const
{
	return mKeyBuffer.empty();
}

bool CKeyboard::IsCharBufferEmpty() const
{
	return mCharBuffer.empty();
}

void CKeyboard::OnKeyPressed(unsigned char key)
{
	mKeyStates[key] = true;
	mKeyBuffer.push(CKeyboardEvent(EEvent::Press, key));
}

void CKeyboard::OnKeyReleased(unsigned char key)
{
	mKeyStates[key] = false;
	mKeyBuffer.push(CKeyboardEvent(EEvent::Release, key));
}

void CKeyboard::OnChar(unsigned char key)
{
	mCharBuffer.push(key);
}

