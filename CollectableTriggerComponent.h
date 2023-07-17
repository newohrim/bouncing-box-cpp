#pragma once

#include "CollisionComponent.h"

class CollectableTriggerComponent : public CollisionComponent
{
public:
	CollectableTriggerComponent(GameActor* actor, int width, int height);

	virtual void OnCollisionHandle(CollisionComponent* from) override;
};

