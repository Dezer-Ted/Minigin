﻿#include "CollisionCheck.h"
#include "../Components/ColliderComponent.h"

void dae::CollisionCheck::AddCollider(ColliderComponent* newCollider)
{
	m_Colliders.emplace_back(newCollider);
}

void dae::CollisionCheck::RemoveCollider(ColliderComponent* newCollider)
{
	m_Colliders.erase(std::remove(m_Colliders.begin(), m_Colliders.end(), newCollider));
}

void dae::CollisionCheck::CheckAllColliders()
{
	m_UncheckedColliders = m_Colliders;
	while(!m_UncheckedColliders.empty())
	{
		ColliderComponent* currentCollider = m_UncheckedColliders.back();
		m_UncheckedColliders.pop_back();
		for(auto& otherCollider : m_UncheckedColliders)
		{
			CheckCollision(currentCollider, otherCollider);
		}
	}
}

void dae::CollisionCheck::CheckCollision(ColliderComponent* pFirstCollider, ColliderComponent* pSecondCollider)
{
	const auto firstBounds = pFirstCollider->GetBounds();
	const auto secondBounds = pSecondCollider->GetBounds();

	if(SDL_HasIntersection(&firstBounds, &secondBounds) == SDL_FALSE)
		return;
	pFirstCollider->CollidedWith(pSecondCollider);
	pSecondCollider->CollidedWith(pFirstCollider);
}
