#pragma once

#include "MoveComponent.h"

class PlatformMoveComponent : public MoveComponent
{
public:
	PlatformMoveComponent(GameActor* actor, Math::Vec2DF pointA, Math::Vec2DF pointB);

	virtual void Update(float deltaTime) override;

	virtual void MoveOwner(Math::Vec2DFN dir, float deltaTime) override;

protected:
	void MovePlatform(float deltaTime);

private:
	Math::Vec2DF m_Points[2];

	bool m_IsBackward = false;
};

