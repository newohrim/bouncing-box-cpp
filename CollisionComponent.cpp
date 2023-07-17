#include "CollisionComponent.h"

#include "GameLevel.h"
#include "GameActor.h"
#include "Math.h"

#include <cmath>

using namespace Math;

CollisionComponent::CollisionComponent(GameActor* actor, int width, int height, bool isSolid) 
	: Component(actor), 
	  m_Width(width), 
	  m_Height(height),
	  m_IsSolid(isSolid)
{
	actor->GetLevel()->AddEntity(this);
}

CollisionComponent::~CollisionComponent()
{
	GetOwner()->GetLevel()->RemoveEntity(this);
}

void CollisionComponent::ResolveHorizontally(CollisionComponent* other, Vec2DF velocity, Vec2DF& newPos) const
{
	const float hDelta = Dot(velocity, Vec2DF{1.0f, 0.0f});
	newPos.x -= hDelta;
}

void CollisionComponent::ResolveVertically(CollisionComponent* other, Vec2DF velocity, Math::Vec2DF& newPos) const
{
	const Vec2DF toOther = other->GetOwner()->GetPosition() - GetOwner()->GetPosition();
	const float vDelta = Dot(velocity, Vec2DF{ 0.0f, 1.0f });
	newPos.y -= vDelta;
}

bool CollisionComponent::CollideAABBvsAABB(CollisionComponent* compA, CollisionComponent* compB)
{
	const Vec2DF posUpperLeftA = compA->GetOwner()->GetPosition() - 
		Vec2DF{(float)compA->m_Width, (float)compA->m_Height} * compA->GetOwner()->GetScale() * 0.5f;
	const Vec2DF posUpperLeftB = compB->GetOwner()->GetPosition() -
		Vec2DF{(float)compB->m_Width, (float)compB->m_Height} * compB->GetOwner()->GetScale() * 0.5f;
	const bool collX = posUpperLeftA.x + compA->m_Width >= posUpperLeftB.x && 
		posUpperLeftB.x + compB->m_Width >= posUpperLeftA.x;
	const bool collY = posUpperLeftA.y + compA->m_Height >= posUpperLeftB.y && 
		posUpperLeftB.y + compB->m_Height >= posUpperLeftA.y;

	return collX && collY;
}

bool CollisionComponent::CollideAABBvsAABB_Horizontally(CollisionComponent* compA, CollisionComponent* compB)
{
	const Vec2DF posUpperLeftA = compA->GetOwner()->GetPosition() -
		Vec2DF{ (float)compA->m_Width, (float)compA->m_Height } * compA->GetOwner()->GetScale() * 0.5f;
	const Vec2DF posUpperLeftB = compB->GetOwner()->GetPosition() -
		Vec2DF{ (float)compB->m_Width, (float)compB->m_Height } * compB->GetOwner()->GetScale() * 0.5f;

	return posUpperLeftA.x + compA->m_Width >= posUpperLeftB.x &&
		posUpperLeftB.x + compB->m_Width >= posUpperLeftA.x;
}

bool CollisionComponent::CollideAABBvsAABB_Vertically(CollisionComponent* compA, CollisionComponent* compB)
{
	const Vec2DF posUpperLeftA = compA->GetOwner()->GetPosition() -
		Vec2DF{ (float)compA->m_Width, (float)compA->m_Height } *compA->GetOwner()->GetScale() * 0.5f;
	const Vec2DF posUpperLeftB = compB->GetOwner()->GetPosition() -
		Vec2DF{ (float)compB->m_Width, (float)compB->m_Height } *compB->GetOwner()->GetScale() * 0.5f;

	return posUpperLeftA.y + compA->m_Height >= posUpperLeftB.y &&
		posUpperLeftB.y + compB->m_Height >= posUpperLeftA.y;
}
