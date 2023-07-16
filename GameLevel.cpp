#include "GameLevel.h"

#include <cassert>
#include "GameActor.h"
#include "PixelSpriteComponent.h"
#include "MoveComponent.h"
#include "PlayerController.h"
#include "PlatformMoveComponent.h"
#include "CollisionComponent.h"
#include "LevelFinishTriggerComponent.h"

#include "GameDescs.h"

#include <iostream>

void GameLevel::Init(const GameParams& params)
{
	using namespace Math;

	const int wallsWidth = 50;
	const ColorRGBA wallsColor = ColorRGBA{ 255, 100, 100, 255 };

	const std::vector<QuadDesc> walls = {
		{Vec2DF{ 0.0f, wallsWidth / 2.0f }, wallsColor, params.worldWidth, wallsWidth}, // TOP
		{Vec2DF{ 0.0f, params.worldHeight - wallsWidth / 2.0f }, wallsColor, params.worldWidth, wallsWidth}, // BOTTOM
		{Vec2DF{ wallsWidth / 2.0f, params.worldHeight / 2.0f}, wallsColor, wallsWidth, params.worldHeight}, // LEFT
		{Vec2DF{ params.worldWidth - wallsWidth / 2.0f, params.worldHeight / 2.0f}, wallsColor, wallsWidth, params.worldHeight}, // RIGHT
	};
	for (const QuadDesc& desc: walls) {
		CreateQuad(desc, this);
	}

	const std::vector<PlatformDesc> platforms = {
		{PlatformDesc{}}
	};

	{	// PLATFORMS
		GameActor* testPlatform = new GameActor(this);
		PixelSpriteComponent* spriteComp = new PixelSpriteComponent(testPlatform, 300, 100);
		spriteComp->SetDrawColor(ColorRGBA{ 255, 100, 100, 255 });
		CollisionComponent* platformColl = new CollisionComponent(testPlatform, 300, 100);
		//MoveComponent* moveComp = new PlatformMoveComponent(testPlatform, Math::Vec2DF{300.0f, 500.0f}, Math::Vec2DF{ 700.0f, 500.0f });
		//moveComp->SetMoveSpeed(300.0f);
		testPlatform->SetPosition(Math::Vec2DF{ (float)params.worldWidth, (float)params.worldHeight + 400 } *0.5f);
	}

	{
		GameActor* testPlatform = new GameActor(this);
		PixelSpriteComponent* spriteComp = new PixelSpriteComponent(testPlatform, 300, 100);
		spriteComp->SetDrawColor(ColorRGBA{ 255, 100, 100, 255 });
		CollisionComponent* platformColl = new CollisionComponent(testPlatform, 300, 100);
		testPlatform->SetPosition(Math::Vec2DF{ (float)params.worldWidth, (float)params.worldHeight - 200 } *0.5f);
		MoveComponent* moveComp = new PlatformMoveComponent(testPlatform, testPlatform->GetPosition(), testPlatform->GetPosition() + Math::Vec2DF{ 0.0f, -500.0f });
		moveComp->SetMoveSpeed(300.0f);
	}

	{
		GameActor* testPlatform = new GameActor(this);
		PixelSpriteComponent* spriteComp = new PixelSpriteComponent(testPlatform, 300, 100);
		spriteComp->SetDrawColor(ColorRGBA{ 255, 100, 100, 255 });
		CollisionComponent* platformColl = new CollisionComponent(testPlatform, 300, 100);
		//MoveComponent* moveComp = new PlatformMoveComponent(testPlatform, Math::Vec2DF{300.0f, 500.0f}, Math::Vec2DF{ 700.0f, 500.0f });
		//moveComp->SetMoveSpeed(300.0f);
		testPlatform->SetPosition(Math::Vec2DF{ 150, 700 });
	}

	{   // FINISH ZONE
		GameActor* finishZone = new GameActor(this);
		PixelSpriteComponent* spriteComp = new PixelSpriteComponent(finishZone, 100, 100);
		spriteComp->SetDrawColor(ColorRGBA{ 0, 255, 0, 255 });
		CollisionComponent* platformColl = new LevelFinishTriggerComponent(finishZone, 100, 100);
		//MoveComponent* moveComp = new PlatformMoveComponent(testPlatform, Math::Vec2DF{300.0f, 500.0f}, Math::Vec2DF{ 700.0f, 500.0f });
		//moveComp->SetMoveSpeed(300.0f);
		finishZone->SetPosition(Math::Vec2DF{ 150, 600 });
	}

	{	// PLAYER
		m_Player = new GameActor(this);
		PixelSpriteComponent* spriteComp = new PixelSpriteComponent(m_Player, 50, 50);
		spriteComp->SetDrawColor(ColorRGBA{ 0, 255, 255, 255 });
		CollisionComponent* playerCollision = new CollisionComponent(m_Player, 50, 50);
		MoveComponent* moveComp = new MoveComponent(m_Player, playerCollision);
		moveComp->SetMoveSpeed(250.0f);
		AddController(std::unique_ptr<Controller>(new PlayerController(moveComp)));
		m_Player->SetPosition(Math::Vec2DF{ (float)params.worldWidth, (float)params.worldHeight } *0.5f);
	}
}

void GameLevel::ProceedInput(InputState inputState, float deltaTime)
{
	for (const std::unique_ptr<Controller>& controller : m_Controllers) {
		// TODO: add controller deletion after target termination
		controller->ProceedInput(inputState, deltaTime);
	}
}

bool GameLevel::Update(float deltaTime)
{
	for (GameActor* gameActor : m_Actors) {
		gameActor->Update(deltaTime);
	}
	std::vector<size_t> pendingActorIdxs;
	for (auto iter = m_Actors.begin(); iter != m_Actors.end(); ++iter) {
		if (!(*iter)->m_IsAlive) {
			const size_t idx = iter - m_Actors.begin();
			pendingActorIdxs.push_back(idx);
		}
	}
	for (size_t idx : pendingActorIdxs) {
		m_Actors.erase(m_Actors.begin() + idx);
	}

	return !m_IsFinishScheduled;
}

void GameLevel::Draw(uint32_t* buffer, int bufferWidth, int bufferHeight)
{
	for (PixelSpriteComponent* drawComp : m_DrawComponents) {
		drawComp->DrawPixels(buffer, bufferWidth, bufferHeight);
	}
}

void GameLevel::AddGameActor(GameActor* gameActor)
{
	assert(std::find(m_Actors.begin(), m_Actors.end(), gameActor) == m_Actors.end());
	m_Actors.push_back(gameActor);
}

void GameLevel::RemoveGameActor(GameActor* gameActor)
{
	const auto iter = std::find(m_Actors.begin(), m_Actors.end(), gameActor);
	assert(iter != m_Actors.end());
	(*iter)->m_IsAlive = false;
}

void GameLevel::AddDrawComponent(PixelSpriteComponent* drawComp)
{
	assert(std::find(m_DrawComponents.begin(), m_DrawComponents.end(), drawComp) == m_DrawComponents.end());
	m_DrawComponents.push_back(drawComp);
}

void GameLevel::RemoveDrawComponent(PixelSpriteComponent* drawComp)
{
	const auto iter = std::find(m_DrawComponents.begin(), m_DrawComponents.end(), drawComp);
	assert(iter != m_DrawComponents.end());
	m_DrawComponents.erase(iter);
}

void GameLevel::AddCollisionComponent(CollisionComponent* collComp)
{
	assert(std::find(m_Collisions.begin(), m_Collisions.end(), collComp) == m_Collisions.end());
	m_Collisions.push_back(collComp);
}

void GameLevel::RemoveCollisionComponent(CollisionComponent* collComp)
{
	const auto iter = std::find(m_Collisions.begin(), m_Collisions.end(), collComp);
	assert(iter != m_Collisions.end());
	m_Collisions.erase(iter);
}

void GameLevel::AddController(std::unique_ptr<Controller> controller)
{
	m_Controllers.push_back(std::move(controller));
}

bool GameLevel::ResolveCollisionsFor(CollisionComponent* collComp, MoveComponent* moveComp, Math::Vec2DF& velocity, Math::Vec2DF& newPos) const
{
	bool collided = false;
	for (CollisionComponent* other : m_Collisions) {
		if (other == collComp || !collComp->DoCollide(other)) {
			continue;
		}
		
		other->OnCollisionHandle(collComp);

		if (!other->IsSolid()) {
			continue;
		}

		if (collComp->DoCollideHorizontally(other)) {
			collComp->ResolveHorizontally(other, velocity, newPos);
			moveComp->GetOwner()->SetPosition(newPos);
			if (collComp->DoCollideHorizontally(other)) {
				collComp->ResolveVertically(other, velocity, newPos);
			}
		}
		
		collided = true;
	}

	return collided;
}

void GameLevel::ScheduleFinishGame()
{
	m_IsFinishScheduled = true;
}
