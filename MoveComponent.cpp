#include "MoveComponent.h"

#include "GameActor.h"

MoveComponent::MoveComponent(GameActor* actor) 
	: Component(actor)
{
}

void MoveComponent::MoveOwner(Math::Vec2DFN direction, float deltaTime)
{
	GameActor* actor = GetOwner();
	const Math::Vec2DF actorPos = actor->GetPosition();
	actor->SetPosition(actorPos + direction * m_MoveSpeed * deltaTime);
}
