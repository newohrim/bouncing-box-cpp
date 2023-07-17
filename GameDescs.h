#pragma once

#include "GameLevel.h"
#include "GameActor.h"
#include "PlatformMoveComponent.h"
#include "PixelSpriteComponent.h"
#include "CollisionComponent.h"
#include "PlayerController.h"
#include "DeathTriggerComponent.h"

struct QuadDesc {
	Math::Vec2DF location;
	ColorBGRA color;
	int width;
	int height;
	bool addCollision = true;
};

GameActor* CreateQuad(const QuadDesc& desc, GameLevel* gameLvl, CollisionComponent** collOut = nullptr)
{
	GameActor* quad = new GameActor(gameLvl);
	PixelSpriteComponent* spriteComp = new PixelSpriteComponent(quad, desc.width, desc.height);
	spriteComp->SetDrawColor(desc.color);
	if (desc.addCollision) {
		CollisionComponent* collision = new CollisionComponent(quad, (float)desc.width, (float)desc.height);
		if (collOut) {
			*collOut = collision;
		}
	}
	quad->SetPosition(desc.location);
	return quad;
}

struct PlayerDesc {
	QuadDesc quad;
	float moveSpeed;
};

GameActor* CreatePlayer(const PlayerDesc& desc, GameLevel* gameLvl)
{
	CollisionComponent* collision = nullptr;
	GameActor* player = CreateQuad(desc.quad, gameLvl, &collision);
	MoveComponent* moveComp = new MoveComponent(player, collision);
	moveComp->SetMoveSpeed(desc.moveSpeed);
	gameLvl->AddController(std::unique_ptr<Controller>(new PlayerController(moveComp)));
	return player;
}

struct PlatformDesc {
	QuadDesc quad;
	Math::Vec2DF pointA;
	Math::Vec2DF pointB;
	float moveSpeed;
};

GameActor* CreatePlatform(const PlatformDesc& desc, GameLevel* gameLvl) 
{
	GameActor* platform = CreateQuad(desc.quad, gameLvl);
	MoveComponent* moveComp = new PlatformMoveComponent(platform, desc.pointA, desc.pointB);
	moveComp->SetMoveSpeed(desc.moveSpeed);
	return platform;
}

GameActor* CreateDeathZone(const QuadDesc& desc, GameLevel* gameLvl) 
{
	QuadDesc customDesc = desc;
	customDesc.addCollision = false;
	GameActor* deathZone = CreateQuad(customDesc, gameLvl);
	new DeathTriggerComponent(deathZone, desc.width, desc.height);
	return deathZone;
}
