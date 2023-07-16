#pragma once

#include "Component.h"
#include "Math.h"

class CollisionComponent;

class MoveComponent : public Component
{
public:
	MoveComponent(GameActor* actor, CollisionComponent* attachedCollision = nullptr);

	virtual void Update(float deltaTime) override;

	void MoveOwner(Math::Vec2DFN direction, float deltaTime);

	float GetMoveSpeed() const { return m_MoveSpeed; }
	void SetMoveSpeed(float moveSpeed) { m_MoveSpeed = moveSpeed; }

private:
	CollisionComponent* m_AttachedCollision = nullptr;

	Math::Vec2DF m_Velocity;
	Math::Vec2DF m_Acceleration;
	Math::Vec2DF m_Forces;
	float m_SinceLastHit = 0.0f;
	float m_MoveSpeed = 1.0f;
	bool m_ApplyGravity = true;
};

