#include "CollectableTriggerComponent.h"

#include "GameActor.h"
#include "GameLevel.h"
#include "PixelSpriteComponent.h"

CollectableTriggerComponent::CollectableTriggerComponent(GameActor* actor, int width, int height) 
	: CollisionComponent(actor, width, height, false)
{
}

void CollectableTriggerComponent::OnCollisionHandle(CollisionComponent* from)
{
	GameLevel* gameLvl = GetOwner()->GetLevel();
	if (gameLvl->GetPlayer() != from->GetOwner()) {
		return;
	}

	gameLvl->GetPlayer()->GetComponent<PixelSpriteComponent>()->
		SetDrawColor(ColorBGRA{234, 67, 201, 255});
	GetOwner()->ScheduleDestroy();
}
