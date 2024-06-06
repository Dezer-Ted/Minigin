﻿#include "PowerUpComponent.h"

#include "PowerUpPickUpComponent.h"
#include "Components/ColliderComponent.h"

dae::PowerUpComponent::PowerUpComponent(GameObject* pOwner) : BaseComponent(pOwner)
{

}

dae::PowerUpComponent::~PowerUpComponent()
{
}

int dae::PowerUpComponent::GetBombUpgrade() const
{
	return m_BombUpgrade;
}

int dae::PowerUpComponent::GetFireUpgrade() const
{
	return m_FireUpgrade;
}

bool dae::PowerUpComponent::GetDetonatorUpgrade() const
{
	return m_HasDetonator;
}

void dae::PowerUpComponent::Notify(Utils::GameEvent event, ObserverEventData* eventData)
{
	if(event == Utils::GameEvent::Collision)
	{
		const auto pCollisionEvent{dynamic_cast<CollisionEventData*>(eventData)};
		if(pCollisionEvent->m_OtherCollider->GetParentTag() != "PowerUp")
			return;
		PowerUps pu = pCollisionEvent->m_OtherCollider->GetParentObject()->GetComponentByClass<PowerUpPickUpComponent>()->GetPowerUp();
		ProcessUpgrade(pu);
	}

}

void dae::PowerUpComponent::ProcessUpgrade(PowerUps powerUp)
{
	switch(powerUp)
	{
	case PowerUps::BombUp:
		++m_BombUpgrade;
		break;
	case PowerUps::FireUp:
		++m_FireUpgrade;
		break;
	case PowerUps::Detonator:
		m_HasDetonator = true;
		break;
	}
}