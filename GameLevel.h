#pragma once

#include <vector>
#include <memory>
#include "InputMapping.h"
#include "Controller.h"

class GameActor;
class PixelSpriteComponent;
class Controller;

struct GameParams {
	int worldWidth;
	int worldHeight;
};

class GameLevel
{
public:
	void Init(const GameParams& params);
	void ProceedInput(InputState inputState, float deltaTime);
	void Update(float deltaTime);
	void Draw(uint32_t* buffer, int bufferWidth, int bufferHeight);

	void AddGameActor(GameActor* gameActor);
	void RemoveGameActor(GameActor* gameActor);

	void AddDrawComponent(PixelSpriteComponent* drawComp);
	void RemoveDrawComponent(PixelSpriteComponent* drawComp);

	void AddController(std::unique_ptr<Controller> controller);

private:
	std::vector<GameActor*> m_Actors;

	std::vector<PixelSpriteComponent*> m_DrawComponents;

	std::vector<std::unique_ptr<Controller>> m_Controllers;
};

