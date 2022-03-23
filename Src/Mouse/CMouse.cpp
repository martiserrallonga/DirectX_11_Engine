#include "CMouse.h"

using EEvent = CMouseEvent::EEventType;

CMouseEvent CMouse::ReadEvent()
{
	if (mEventBuffer.empty()) return CMouseEvent();
		
	CMouseEvent Event = mEventBuffer.front();
	mEventBuffer.pop();
	return Event;
}

TMousePoint CMouse::GetPos() const
{
	return mPoint;
}

bool CMouse::IsLeftPressed()const
{
	return mLeftPressed;
}

bool CMouse::IsRightPressed() const
{
	return mRightPressed;
}

bool CMouse::IsMiddlePressed() const
{
	return mMiddlePressed;
}

bool CMouse::IsEventBufferEmpty() const
{
	return mEventBuffer.empty();
}

void CMouse::OnMouseMove(TMousePoint aPoint)
{
	mPoint = aPoint;
	mEventBuffer.emplace(EEvent::Move, aPoint);
}

void CMouse::OnLeftPressed(TMousePoint aPoint)
{
	mLeftPressed = true;
	mEventBuffer.emplace(EEvent::LPress, aPoint);
}

void CMouse::OnLeftReleased(TMousePoint aPoint)
{
	mLeftPressed = false;
	mEventBuffer.emplace(EEvent::LRelease, aPoint);
}

void CMouse::OnRightPressed(TMousePoint aPoint)
{
	mRightPressed = true;
	mEventBuffer.emplace(EEvent::RPress, aPoint);
}

void CMouse::OnRightReleased(TMousePoint aPoint)
{
	mRightPressed = false;
	mEventBuffer.emplace(EEvent::RRelease, aPoint);
}

void CMouse::OnMiddlePressed(TMousePoint aPoint)
{
	mMiddlePressed = true;
	mEventBuffer.emplace(EEvent::MPress, aPoint);
}

void CMouse::OnMiddleReleased(TMousePoint aPoint)
{
	mMiddlePressed = false;
	mEventBuffer.emplace(EEvent::MRelease, aPoint);
}

void CMouse::OnWheelUp(TMousePoint aPoint)
{
	mEventBuffer.emplace(EEvent::WheelUp, aPoint);
}

void CMouse::OnWheelDown(TMousePoint aPoint)
{
	mEventBuffer.emplace(EEvent::WheelDown, aPoint);
}
