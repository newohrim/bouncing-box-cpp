#include "MoveComponent.h"

#include "GameActor.h"
#include "GameLevel.h"

MoveComponent::MoveComponent(GameActor* actor, CollisionComponent* attachedCollision)
	: Component(actor), 
	  m_AttachedCollision(attachedCollision)
{
}

static float MoveFunctionY(float t, float force = 1000.0f, float angle = 0.8f, float gravity = 1000.0f) 
{
	return force * angle * t - gravity * t * t * 0.5f;
}

void MoveComponent::Update(float deltaTime)
{
	using namespace Math;

	GameActor* actor = GetOwner();

	/*constexpr Vec2DF gravityVector{ 0.0f, 9.8f };
	m_Acceleration =
		m_ApplyGravity ? gravityVector : Vec2DF{ 0.0f, 0.0f } + m_Forces;
	const Vec2DF actorPos = actor->GetPosition();
	Vec2DF newPos = actorPos + m_Velocity * deltaTime;*/

	/*if (actor->GetPosition().y > 500.0f) {
		actor->SetPosition(Vec2DF{actor->GetPosition().x, 500.0f});
		m_SinceLastHit = 0.0f;
	}*/

	const Vec2DF oldPos = actor->GetPosition();
	Vec2DF newPos = oldPos + m_Forces * deltaTime;

	const float lastHeight = -MoveFunctionY(m_SinceLastHit);
	m_SinceLastHit += deltaTime;
	const float newHeight = -MoveFunctionY(m_SinceLastHit);
	const float deltaHeight = newHeight - lastHeight;

	newPos = Vec2DF{ newPos.x, newPos.y + deltaHeight };

	if (m_AttachedCollision) {
		// resolve collisions
		actor->SetPosition(newPos);
		m_Velocity = newPos - oldPos;
		const Vec2DF newPosBeforeResolution = newPos;
		if (actor->GetLevel()->ResolveCollisionsFor(m_AttachedCollision, this, m_Velocity, newPos) && newPos.y != newPosBeforeResolution.y) {
			m_SinceLastHit = 0.0f;
		}
	}
	actor->SetPosition(newPos);
	//m_Velocity = m_Velocity + m_Acceleration;
	m_Forces = Vec2DF{0.0f, 0.0f};
}

void MoveComponent::MoveOwner(Math::Vec2DFN direction, float deltaTime)
{
	m_Forces = m_Forces + direction * m_MoveSpeed;
}
