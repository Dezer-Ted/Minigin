﻿#include "PlayerComponent.h"

#include "Components/ColliderComponent.h"
#include "Components/SpriteComponent.h"
#include "Engine/DesignPatterns/Blackboard.h"

dae::PlayerComponent::PlayerComponent(GameObject* pParent)
	: BaseComponent(pParent)
{
	m_pSpriteComponent = GetParent()->GetComponentByClass<SpriteComponent>();
	m_pMoveComp = GetParent()->GetComponentByClass<MovementComponent>();
}

void dae::PlayerComponent::Notify(Utils::GameEvent event, BaseComponent* components, std::unique_ptr<Blackboard> pBlackboard)
{
	switch(event)
	{
	case Utils::DirectionChanged:
		{
			ChangeAnimation(m_pMoveComp->GetDirection());
		}
		break;
	case Utils::MovementStopped:
		{
			m_pSpriteComponent->ShouldUpdate(false);
		}
		break;
	case Utils::MovementStarted:
		{
			m_pSpriteComponent->ShouldUpdate(true);
		}
		break;
	case Utils::Collision:
		{
			ColliderComponent* pCollider {nullptr};
			pBlackboard->GetData("OtherCollider",pCollider);
			if(pCollider->GetParentTag() == "Wall")
			{
				m_pMoveComp->UndoMovement();
			}
		}
	}
}

void dae::PlayerComponent::ChangeAnimation(MovementComponent::MovementDirection direction)
{
	switch(direction)
	{
	case MovementComponent::MovementDirection::Up:
		m_pSpriteComponent->SwitchToSprite("WalkUp");
		break;
	case MovementComponent::MovementDirection::Down:
		m_pSpriteComponent->SwitchToSprite("WalkDown");
		break;
	case MovementComponent::MovementDirection::Left:
		m_pSpriteComponent->SwitchToSprite("WalkLeft");
		break;
	case MovementComponent::MovementDirection::Right:
		m_pSpriteComponent->SwitchToSprite("WalkRight");
		break;
	}
}