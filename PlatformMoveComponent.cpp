#include "PlatformMoveComponent.h"

#include "GameActor.h"

PlatformMoveComponent::PlatformMoveComponent(GameActor* actor, Math::Vec2DF pointA, Math::Vec2DF pointB)
	: MoveComponent(actor)
{
	m_Points[0] = pointA;
	m_Points[1] = pointB;
}

void PlatformMoveComponent::Update(float deltaTime)
{
	MovePlatform(deltaTime);
}

void PlatformMoveComponent::MovePlatform(float deltaTime)
{
	const Math::Vec2DF actorPos = GetOwner()->GetPosition();
	const Math::Vec2DF dirOverA = m_Points[0] - actorPos;
	const Math::Vec2DF dirOverB = m_Points[1] - actorPos;
	Math::Vec2DF moveDir = m_Points[1] - m_Points[0];
	if (Math::LengthSq(moveDir) < Math::EPSILON) {
		return;
	}
	if (Math::Dot(moveDir, dirOverB) < 0.0f) {
		m_IsBackward = true;
	}
	else if (Math::Dot(moveDir, dirOverA) > 0.0f) {
		m_IsBackward = false;
	}
	if (m_IsBackward) {
		moveDir = moveDir * -1.0f;
	}
	MoveOwner(Math::Normalize(moveDir), deltaTime);
}
