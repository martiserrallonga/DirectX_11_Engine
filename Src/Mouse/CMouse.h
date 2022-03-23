#pragma once
#include "CMouseEvent.h"
#include <queue>

class CMouse
{
public:
	CMouseEvent ReadEvent();
	TMousePoint GetPos() const;
	
	bool IsLeftPressed() const;
	bool IsRightPressed() const;
	bool IsMiddlePressed() const;
	bool IsEventBufferEmpty() const;
	
	void OnMouseMove(TMousePoint aPoint);
	void OnLeftPressed(TMousePoint aPoint);
	void OnLeftReleased(TMousePoint aPoint);
	void OnRightPressed(TMousePoint aPoint);
	void OnRightReleased(TMousePoint aPoint);
	void OnMiddlePressed(TMousePoint aPoint);
	void OnMiddleReleased(TMousePoint aPoint);
	void OnWheelUp(TMousePoint aPoint);
	void OnWheelDown(TMousePoint aPoint);

private:
	std::queue<CMouseEvent> mEventBuffer;
	TMousePoint mPoint;

	bool mLeftPressed = false;
	bool mRightPressed = false;
	bool mMiddlePressed = false;

};
