#pragma once

#include <vector>
#include <memory>
#include "InputMapping.h"
#include "Controller.h"
#include "Math.h"

class GameActor;
class PixelSpriteComponent;
class CollisionComponent;
class Controller;
class MoveComponent;

struct GameParams {
	int worldWidth;
	int worldHeight;
};

class GameLevel
{
public:
	void Init(const GameParams& params);
	void ProceedInput(InputState inputState, float deltaTime);
	bool Update(float deltaTime);
	void Draw(uint32_t* buffer, int bufferWidth, int bufferHeight);

	template<class T>
	void AddEntity(T* entity);
	template<class T>
	void RemoveEntity(T* entity);

	void AddController(std::unique_ptr<Controller> controller);

	bool ResolveCollisionsFor(CollisionComponent* collComp, MoveComponent* moveComp, Math::Vec2DF velocity, Math::Vec2DF& newPos) const;

	void SchedulePlayerDeath();
	void ScheduleFinishGame();

	GameActor* GetPlayer() const { return m_Player; }

protected:
	template<class T>
	void AddToContainer(std::vector<T*>& container, T* enitity);
	template<class T>
	void RemoveFromContainer(std::vector<T*>& container, T* enitity);

private:
	std::vector<GameActor*> m_Actors;

	std::vector<PixelSpriteComponent*> m_DrawComponents;

	std::vector<CollisionComponent*> m_Collisions;

	std::vector<std::unique_ptr<Controller>> m_Controllers;

	GameActor* m_Player = nullptr;

	bool m_IsPlayerAlive = true;

	bool m_IsFinishScheduled = false;
};
