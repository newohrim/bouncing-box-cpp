#pragma once

#include "Component.h"
#include "Math.h"

class CollisionComponent : public Component
{
public:
	CollisionComponent(GameActor* actor, int width, int height);
	~CollisionComponent();

	bool DoCollide(CollisionComponent* other) { return CollideAABBvsAABB(this, other); }
	bool DoCollideHorizontally(CollisionComponent* other) { return CollideAABBvsAABB_Horizontally(this, other); }
	bool DoCollideVertically(CollisionComponent* other) { return CollideAABBvsAABB_Vertically(this, other); }

	void ResolveHorizontally(CollisionComponent* other, Math::Vec2DF velocity, Math::Vec2DF& newPos) const;
	void ResolveVertically(CollisionComponent* other, Math::Vec2DF velocity, Math::Vec2DF& newPos) const;

protected:
	static bool CollideAABBvsAABB(CollisionComponent* compA, CollisionComponent* compB);
	static bool CollideAABBvsAABB_Horizontally(CollisionComponent* compA, CollisionComponent* compB);
	static bool CollideAABBvsAABB_Vertically(CollisionComponent* compA, CollisionComponent* compB);

private:
	int m_Width;
	int m_Height;
};

