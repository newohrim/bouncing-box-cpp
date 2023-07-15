#pragma once

#include <memory>

class GameActor;

class Component
{
public:
	Component(GameActor* actor);
	virtual ~Component();

	virtual void Update(float deltaTime) {};

	GameActor* GetOwner() const { return m_Owner; }
	
private:
	GameActor* m_Owner;
};

