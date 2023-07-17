#include "GameLevel.h"

#include <cassert>
#include "GameActor.h"
#include "PixelSpriteComponent.h"
#include "MoveComponent.h"
#include "PlayerController.h"
#include "PlatformMoveComponent.h"
#include "CollisionComponent.h"
#include "LevelFinishTriggerComponent.h"
#include "CollectableTriggerComponent.h"

#include "GameDescs.h"

#define DEFINE_ADD_REMOVE_ENITIES(ENTITY, CONTAINER)	\
template<>												\
void GameLevel::AddEntity(ENTITY* entity)				\
{														\
	AddToContainer(CONTAINER, entity);					\
}														\
														\
template<>												\
void GameLevel::RemoveEntity(ENTITY* entity)			\
{														\
	RemoveFromContainer(CONTAINER, entity);				\
}														\

DEFINE_ADD_REMOVE_ENITIES(GameActor, m_Actors);
DEFINE_ADD_REMOVE_ENITIES(PixelSpriteComponent, m_DrawComponents);
DEFINE_ADD_REMOVE_ENITIES(CollisionComponent, m_Collisions);

void GameLevel::Init(const GameParams& params)
{
	// Level creation
	using namespace Math;

	constexpr int wallsWidth = 50;
	constexpr int platformsWidth = 50;
	constexpr ColorBGRA wallsColor = ColorBGRA{ 255, 100, 100, 255 };

	const std::vector<QuadDesc> walls = {
		{Vec2DF{ params.worldWidth / 2.0f, wallsWidth / 2.0f }, wallsColor, params.worldWidth, wallsWidth}, // TOP
		//{Vec2DF{ params.worldWidth / 2.0f, params.worldHeight - wallsWidth / 2.0f }, wallsColor, params.worldWidth, wallsWidth}, // BOTTOM
		{Vec2DF{ wallsWidth / 2.0f, params.worldHeight / 2.0f}, wallsColor, wallsWidth, params.worldHeight}, // LEFT
		{Vec2DF{ params.worldWidth - wallsWidth / 2.0f, params.worldHeight / 2.0f}, wallsColor, wallsWidth, params.worldHeight}, // RIGHT

		{Vec2DF{ 100, 200}, wallsColor, 200, platformsWidth}, // START PLATFORM
		{Vec2DF{ (float)(params.worldWidth - 340), 200}, wallsColor, params.worldWidth - 300, wallsWidth}, // RIGHT TOP WALL

		{Vec2DF{ 925, 525}, wallsColor, 100, wallsWidth},
		{Vec2DF{ 75, 495}, wallsColor, wallsWidth, wallsWidth},
	};
	for (const QuadDesc& desc: walls) {
		CreateQuad(desc, this);
	}

	constexpr ColorBGRA deathZonesColor = ColorBGRA{ 100, 100, 255, 255 };

	const std::vector<QuadDesc> deathZones = {
		{Vec2DF{350, 375}, deathZonesColor, platformsWidth, 300},
		{Vec2DF{300, 350}, deathZonesColor, platformsWidth, platformsWidth},
		{Vec2DF{wallsWidth + platformsWidth / 2.0f, (float)(params.worldHeight - wallsWidth - platformsWidth * 2)}, deathZonesColor, platformsWidth, platformsWidth * 4},
		{Vec2DF{700, 375}, deathZonesColor, platformsWidth, 300},
		{Vec2DF{ params.worldWidth / 2.0f, params.worldHeight - wallsWidth / 2.0f }, deathZonesColor, params.worldWidth - wallsWidth * 2, wallsWidth},
		{Vec2DF{ 500, 160 }, deathZonesColor, platformsWidth, 35},
		{Vec2DF{ 700, 160 }, deathZonesColor, platformsWidth, 35},
	};
	for (const QuadDesc& desc : deathZones) {
		CreateDeathZone(desc, this);
	}

	const std::vector<PlatformDesc> platfroms = {
		{QuadDesc{Vec2DF{800, (float)(params.worldHeight - 100)}, wallsColor, 350, platformsWidth}, Vec2DF{800, (float)(params.worldHeight - 100)}, Vec2DF{200, (float)(params.worldHeight - 100)}, 400},
	};
	for (const PlatformDesc& desc : platfroms) {
		CreatePlatform(desc, this);
	}

	PlayerDesc playerDesc;
	const ColorBGRA playerColor{0, 255, 255, 255};
	playerDesc.quad = QuadDesc{ Vec2DF{100, 100}, playerColor, 20, 20 };
	playerDesc.moveSpeed = 300.0f;
	m_Player = CreatePlayer(playerDesc, this);

	{   // FINISH ZONE
		GameActor* finishZone = new GameActor(this);
		PixelSpriteComponent* spriteComp = new PixelSpriteComponent(finishZone, 50, 50);
		spriteComp->SetDrawColor(ColorBGRA{ 0, 255, 0, 255 });
		CollisionComponent* platformColl = new LevelFinishTriggerComponent(finishZone, 50, 50);
		finishZone->SetPosition(Math::Vec2DF{ 800, 300 });
	}

	{	// SIMPLE COLLECTABLE
		GameActor* collectable = CreateQuad(QuadDesc{ Vec2DF{875, 125}, ColorBGRA{234, 67, 201, 255}, 20, 20, false }, this);
		new CollectableTriggerComponent(collectable, 20, 20);
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
		if ((*iter)->IsScheduledDestroy()) {
			const size_t idx = iter - m_Actors.begin();
			pendingActorIdxs.push_back(idx);
		}
	}
	for (size_t idx : pendingActorIdxs) {
		delete m_Actors[idx];
		m_Actors.erase(m_Actors.begin() + idx);
	}

	return !m_IsFinishScheduled && m_IsPlayerAlive;
}

void GameLevel::Draw(uint32_t* buffer, int bufferWidth, int bufferHeight)
{
	for (PixelSpriteComponent* drawComp : m_DrawComponents) {
		drawComp->DrawPixels(buffer, bufferWidth, bufferHeight);
	}
}

template<class T>
inline void GameLevel::AddToContainer(std::vector<T*>& container, T* enitity)
{
	assert(std::find(container.begin(), container.end(), enitity) == container.end());
	container.push_back(enitity);
}

template<class T>
void GameLevel::RemoveFromContainer(std::vector<T*>& container, T* enitity)
{
	const auto iter = std::find(container.begin(), container.end(), enitity);
	assert(iter != container.end());
}

void GameLevel::AddController(std::unique_ptr<Controller> controller)
{
	m_Controllers.push_back(std::move(controller));
}

bool GameLevel::ResolveCollisionsFor(CollisionComponent* collComp, MoveComponent* moveComp, Math::Vec2DF velocity, Math::Vec2DF& newPos) const
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

		// Resolve collision by each axis separatly
		if (collComp->DoCollideHorizontally(other)) {
			collComp->ResolveHorizontally(other, velocity, newPos);
			moveComp->GetOwner()->SetPosition(newPos);
			// before resolve vertical axis, check for horizontal collision again to evade some edgy cases (temporal fix)
			if (collComp->DoCollideHorizontally(other)) {
				collComp->ResolveVertically(other, velocity, newPos);
			}
		}
		
		collided = true;
	}

	return collided;
}

void GameLevel::SchedulePlayerDeath()
{
	m_IsPlayerAlive = false;
}

void GameLevel::ScheduleFinishGame()
{
	m_IsFinishScheduled = true;
}
