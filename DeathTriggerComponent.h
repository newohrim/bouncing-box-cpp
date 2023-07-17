#pragma once

#include "CollisionComponent.h"

class DeathTriggerComponent : public CollisionComponent
{
public:
	DeathTriggerComponent(GameActor* actor, int width, int height);

	virtual void OnCollisionHandle(CollisionComponent* from) override;
};

