#include "MoveComponent.h"

#include "GameActor.h"
#include "GameLevel.h"

MoveComponent::MoveComponent(GameActor* actor, CollisionComponent* attachedCollision)
	: Component(actor), 
	  m_AttachedCollision(attachedCollision)
{
}

static float MoveFunctionY(float t, float force = 750.0f, float angle = 0.8f, float gravity = 1000.0f) 
{
	return force * angle * t - gravity * t * t * 0.5f;
}

static float GetTimeAfterTopCollision(float t, float force = 750.0f, float angle = 0.8f, float gravity = 1000.0f) 
{
	// Mirror t value over middle axis of parabola function
	// force * angle - gravity * t == 0 // t = force * angle / gravity
	const float midT = force * angle / gravity;
	if (t >= midT) {
		return t;
	}
	return 2.0f * midT - t;
}

void MoveComponent::Update(float deltaTime)
{
	using namespace Math;

	GameActor* actor = GetOwner();

	const Vec2DF oldPos = actor->GetPosition();
	Vec2DF newPos = oldPos + m_Forces * deltaTime;

	// calculate vertical offset with paraboloid function
	const float lastHeight = -MoveFunctionY(m_SinceLastHit);
	m_SinceLastHit += deltaTime;
	const float newHeight = -MoveFunctionY(m_SinceLastHit);
	const float deltaHeight = newHeight - lastHeight;

	newPos = newPos + Vec2DF{ 0.0f, deltaHeight };

	if (m_AttachedCollision) {
		// resolve collisions
		actor->SetPosition(newPos);
		const Vec2DF velocity = newPos - oldPos;
		const Vec2DF newPosBeforeResolution = newPos;
		bool shouldChangeDir = false;
		if (actor->GetLevel()->ResolveCollisionsFor(m_AttachedCollision, this, velocity, newPos)) {
			if (newPos.y < newPosBeforeResolution.y) {
				m_SinceLastHit = 0.0f;
			} else if (newPos.y > newPosBeforeResolution.y) {
				m_SinceLastHit = GetTimeAfterTopCollision(m_SinceLastHit);
			}
		}
	}
	actor->SetPosition(newPos);
	m_Forces = Vec2DF{0.0f, 0.0f};
}

void MoveComponent::MoveOwner(Math::Vec2DFN direction, float deltaTime)
{
	m_Forces = m_Forces + direction * m_MoveSpeed;
}
