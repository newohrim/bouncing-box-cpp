#pragma once

#include <vector>
#include <memory>
#include <typeinfo>
#include "Math.h"

class GameLevel;
class Component;

// Main game entity. Contains different components.
class GameActor
{
public:
	GameActor(GameLevel* gameLvl);
	virtual ~GameActor();

	void Update(float deltaTime);

	void AddComponent(Component* component);
	void RemoveComponent(Component* component);

	template<class T>
	T* GetComponent() const;

	Math::Vec2DF GetPosition() const { return m_Pos; }
	void SetPosition(Math::Vec2DF position) { m_Pos = position; }

	float GetScale() const { return m_Scale; }
	void SetScale(float scale) { m_Scale = scale; }

	bool IsScheduledDestroy() const { return !m_IsAlive; }
	void ScheduleDestroy() { m_IsAlive = false; }

	GameLevel* GetLevel() const { return m_GameLvl; }

private:
	std::vector<Component*> m_Components;

	GameLevel* m_GameLvl;

	Math::Vec2DF m_Pos;

	float m_Scale = 1.0f;

	bool m_IsAlive = true;
};

template<class T>
inline T* GameActor::GetComponent() const
{
	for (Component* comp : m_Components) {
		T* castedComp = dynamic_cast<T*>(comp);
		if (castedComp != nullptr) {
			return castedComp;
		}
	}
}
