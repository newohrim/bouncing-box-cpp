#pragma once

#include <vector>
#include <memory>
#include "Math.h"

class GameLevel;
class Component;

class GameActor
{
	friend GameLevel;

public:
	GameActor(GameLevel* gameLvl);
	//GameActor(std::weak_ptr<GameLevel> gameLvl, std::initializer_list<ComponentPtr> components);
	virtual ~GameActor();

	void Update(float deltaTime);

	void AddComponent(Component* component);
	void RemoveComponent(Component* component);

	Math::Vec2DF GetPosition() const { return m_Pos; }
	void SetPosition(Math::Vec2DF position) { m_Pos = position; }

	float GetScale() const { return m_Scale; }
	void SetScale(float scale) { m_Scale = scale; }

	GameLevel* GetLevel() const { return m_GameLvl; }

private:
	std::vector<Component*> m_Components;

	GameLevel* m_GameLvl;

	Math::Vec2DF m_Pos;
	float m_Scale = 1.0f;

	bool m_IsAlive = true;
};

