#include "Component.h"

#include "GameActor.h"

Component::Component(GameActor* actor) 
	: m_Owner(actor)
{
	m_Owner->AddComponent(this);
}

Component::~Component()
{
	m_Owner->RemoveComponent(this);
}
