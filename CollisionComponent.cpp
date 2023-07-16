#include "CollisionComponent.h"

#include "GameLevel.h"
#include "GameActor.h"
#include "Math.h"

#include <cmath>

using namespace Math;

CollisionComponent::CollisionComponent(GameActor* actor, int width, int height) 
	: Component(actor), 
	  m_Width(width), 
	  m_Height(height)
{
	actor->GetLevel()->AddCollisionComponent(this);
}

CollisionComponent::~CollisionComponent()
{
	GetOwner()->GetLevel()->RemoveCollisionComponent(this);
}

void CollisionComponent::ResolveHorizontally(CollisionComponent* other, Vec2DF velocity, Vec2DF& newPos) const
{
	/*const Vec2DF posUpperLeftA = GetOwner()->GetPosition() -
		Vec2DF{ (float)this->m_Width, (float)this->m_Height } * this->GetOwner()->GetScale() * 0.5f;
	const Vec2DF posUpperLeftB = other->GetOwner()->GetPosition() -
		Vec2DF{ (float)other->m_Width, (float)other->m_Height } * other->GetOwner()->GetScale() * 0.5f;*/

	/*float hDelta;
	if (posUpperLeftA.x < posUpperLeftB.x) {
		hDelta = posUpperLeftB.x - posUpperLeftA.x + this->m_Width;
	} else {
		hDelta = posUpperLeftA.x - posUpperLeftB.x + other->m_Width;
	}*/

	const float hDelta = Dot(velocity, Vec2DF{1.0f, 0.0f});
	newPos.x -= hDelta;

	//newPos.x -= hDelta;
}

void CollisionComponent::ResolveVertically(CollisionComponent* other, Vec2DF velocity, Math::Vec2DF& newPos) const
{
	/*const Vec2DF posUpperLeftA = this->GetOwner()->GetPosition() -
		Vec2DF{ (float)this->m_Width, (float)this->m_Height } * this->GetOwner()->GetScale() * 0.5f;
	const Vec2DF posUpperLeftB = other->GetOwner()->GetPosition() -
		Vec2DF{ (float)other->m_Width, (float)other->m_Height } * other->GetOwner()->GetScale() * 0.5f;*/

	/*float vDelta;
	if (posUpperLeftA.y < posUpperLeftB.y) {
		vDelta = posUpperLeftB.y - posUpperLeftA.y + this->m_Height;
	}
	else {
		vDelta = posUpperLeftA.y - posUpperLeftB.y + other->m_Height;
	}*/

	const float vDelta = Dot(velocity, Vec2DF{ 0.0f, 1.0f });
	newPos.y -= vDelta;

	//newPos.y -= vDelta;
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
