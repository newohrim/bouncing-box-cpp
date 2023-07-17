#include "DeathTriggerComponent.h"

#include "GameActor.h"
#include "GameLevel.h"

DeathTriggerComponent::DeathTriggerComponent(GameActor* actor, int width, int height) 
	: CollisionComponent(actor, width, height, true)
{
}

void DeathTriggerComponent::OnCollisionHandle(CollisionComponent* from)
{
	GameLevel* gameLvl = GetOwner()->GetLevel();
	if (gameLvl->GetPlayer() != from->GetOwner()) {
		return;
	}

	gameLvl->SchedulePlayerDeath();
}
