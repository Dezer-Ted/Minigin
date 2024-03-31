﻿#include "ColliderComponent.h"

#include "SpriteComponent.h"
#include "../Rendering/Sprite.h"
#include "../SceneObjects/Scene.h"
#include "../Engine/DesignPatterns/Blackboard.h"

dae::ColliderComponent::ColliderComponent(GameObject* pParent) : BaseComponent(pParent)
{
	pParent->GetParentScene()->AddCollider(this);
}

SDL_Rect dae::ColliderComponent::GetBounds() const
{
	return m_Bounds;
}

void dae::ColliderComponent::SetBounds(const SDL_Rect& bounds)
{
	m_Bounds = bounds;
}

void dae::ColliderComponent::AdjustBoundsToSpriteSize()
{
	const auto sprite = GetParent()->GetComponentByClass<SpriteComponent>();
	m_Bounds.h = static_cast<int>(static_cast<float>(sprite->GetCurrentSprite()->m_TargetHeight) * sprite->GetScale());
	m_Bounds.w = static_cast<int>(static_cast<float>(sprite->GetCurrentSprite()->m_TargetWidth) * sprite->GetScale());
}

void dae::ColliderComponent::CollidedWith(ColliderComponent* other)
{
	std::unique_ptr<Blackboard> pBlackboard = std::make_unique<Blackboard>();
	pBlackboard->AddData("OtherCollider", other);
	NotifyObservers(Utils::Collision, this, std::move(pBlackboard));
}

void dae::ColliderComponent::Update()
{
	const auto pos = GetParent()->GetTransform().GetWorldPosition();
	m_Bounds.x = static_cast<int>(pos.x);
	m_Bounds.y = static_cast<int>(pos.y);
}

std::string dae::ColliderComponent::GetParentTag() const
{
	return GetParent()->GetTag();
}
