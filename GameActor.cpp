#include "GameActor.h"

#include <cassert>
#include "GameLevel.h"
#include "Component.h"

GameActor::GameActor(GameLevel* gameLvl)
	: m_GameLvl(gameLvl)
{
	gameLvl->AddEntity(this);
}

GameActor::~GameActor()
{
	for (Component* component : m_Components) {
		delete component;
	}
	m_GameLvl->RemoveEntity(this);
}

void GameActor::Update(float deltaTime)
{
	for (Component* component : m_Components) {
		component->Update(deltaTime);
	}
}

void GameActor::AddComponent(Component* component)
{
	assert(std::find(m_Components.begin(), m_Components.end(), component) == m_Components.end());
	m_Components.push_back(component);
}

void GameActor::RemoveComponent(Component* component)
{
	const auto iter = std::find(m_Components.begin(), m_Components.end(), component);
	assert(iter != m_Components.end());
	m_Components.erase(iter);
}
