#include "GameLevel.h"

#include <cassert>
#include "GameActor.h"
#include "PixelSpriteComponent.h"
#include "MoveComponent.h"
#include "PlayerController.h"
#include "PlatformMoveComponent.h"

void GameLevel::Init(const GameParams& params)
{
	{	// PLATFORMS
		GameActor* testPlatform = new GameActor(this);
		PixelSpriteComponent* spriteComp = new PixelSpriteComponent(testPlatform, 300, 100);
		spriteComp->SetDrawColor(ColorRGBA{0, 255, 0, 255});
		//MoveComponent* moveComp = new PlatformMoveComponent(testPlatform, Math::Vec2DF{300.0f, 500.0f}, Math::Vec2DF{ 700.0f, 500.0f });
		//moveComp->SetMoveSpeed(300.0f);
		testPlatform->SetPosition(Math::Vec2DF{ (float)params.worldWidth, (float)params.worldHeight + 400 } *0.5f);
	}

	{	// PLAYER
		GameActor* testActor = new GameActor(this);
		PixelSpriteComponent* spriteComp = new PixelSpriteComponent(testActor, 50, 50);
		spriteComp->SetDrawColor(ColorRGBA{ 0, 255, 255, 255 });
		MoveComponent* moveComp = new MoveComponent(testActor);
		moveComp->SetMoveSpeed(250.0f);
		AddController(std::unique_ptr<Controller>(new PlayerController(moveComp)));
		testActor->SetPosition(Math::Vec2DF{ (float)params.worldWidth, (float)params.worldHeight } *0.5f);
	}
}

void GameLevel::ProceedInput(InputState inputState, float deltaTime)
{
	for (const std::unique_ptr<Controller>& controller : m_Controllers) {
		// TODO: add controller deletion after target termination
		controller->ProceedInput(inputState, deltaTime);
	}
}

void GameLevel::Update(float deltaTime)
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

void GameLevel::AddController(std::unique_ptr<Controller> controller)
{
	m_Controllers.push_back(std::move(controller));
}
