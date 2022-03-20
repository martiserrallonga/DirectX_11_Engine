#pragma once
#include "CKeyboardEvent.h"
#include <array>
#include <queue>

class CKeyboard
{
public:
	CKeyboardEvent ReadKey();
	unsigned char ReadChar();

	bool IsKeyPressed(unsigned char keycode) const;
	bool IsKeyBufferEmpty() const;
	bool IsCharBufferEmpty() const;

	void OnKeyPressed(unsigned char key);
	void OnKeyReleased(unsigned char key);
	void OnChar(unsigned char key);

	bool IsAutoRepeatKeys() const { return mAutoRepeatKeys; }
	bool IsAutoRepeatChars() const { return mAutoRepeatChars; }
	void SetAutoRepeatKeys(bool aRepeat) { mAutoRepeatKeys = aRepeat; }
	void SetAutoRepeatChars(bool aRepeat) { mAutoRepeatChars = aRepeat; }

private:
	std::array<bool, 256> mKeyStates;
	std::queue<CKeyboardEvent> mKeyBuffer;
	std::queue<unsigned char> mCharBuffer;

	bool mAutoRepeatKeys = false;
	bool mAutoRepeatChars = false;
};