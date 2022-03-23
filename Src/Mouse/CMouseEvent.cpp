#include "CMouseEvent.h"

CMouseEvent::CMouseEvent()
	: Type(EEventType::Invalid)
{}

CMouseEvent::CMouseEvent(EEventType aType, TMousePoint aPoint)
	: Type(aType)
	, Point(aPoint)
{}

bool CMouseEvent::IsValid() const
{
	return Type != EEventType::Invalid;
}

CMouseEvent::EEventType CMouseEvent::GetType() const
{
	return Type;
}

TMousePoint CMouseEvent::GetPos() const
{
	return Point;
}
