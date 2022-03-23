#pragma once
struct TMousePoint
{
	int x = 0;
	int y = 0;
};

class CMouseEvent
{
public:
	enum class EEventType
	{
		Move,
		RawMove,
		LPress,
		LRelease,
		RPress,
		RRelease,
		MPress,
		MRelease,
		WheelUp,
		WheelDown,
		Invalid
	};

	CMouseEvent();
	CMouseEvent(EEventType aType, TMousePoint aPoint);
	bool IsValid() const;
	EEventType GetType() const;
	TMousePoint GetPos() const;

private:
	EEventType Type;
	TMousePoint Point;

};