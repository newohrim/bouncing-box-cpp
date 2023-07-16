#include "LevelFinishTriggerComponent.h"

#include "GameActor.h"
#include "GameLevel.h"

LevelFinishTriggerComponent::LevelFinishTriggerComponent(GameActor* actor, int zoneWidth, int zoneHeight) 
	: CollisionComponent(actor, zoneWidth, zoneHeight, false)
{
}

void LevelFinishTriggerComponent::OnCollisionHandle(CollisionComponent* from)
{
	if (from->GetOwner() != GetOwner()->GetLevel()->GetPlayer()) {
		return;
	}

	GetOwner()->GetLevel()->ScheduleFinishGame();
}
