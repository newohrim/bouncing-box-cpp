#pragma once

#include "CollisionComponent.h"

class LevelFinishTriggerComponent : public CollisionComponent
{
public:
	LevelFinishTriggerComponent(GameActor* actor, int zoneWidth, int zoneHeight);

	virtual void OnCollisionHandle(CollisionComponent* from) override;
};

