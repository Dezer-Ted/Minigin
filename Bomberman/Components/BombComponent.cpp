﻿#include "BombComponent.h"

#include "ExplosionComponent.h"
#include "../GridComponent.h"
#include "Components/ColliderComponent.h"
#include "Components/SpriteComponent.h"
#include "Engine/DeltaTime.h"
#include "Engine/DesignPatterns/Blackboard.h"
#include "Engine/DesignPatterns/Blackboard.h"
#include "Engine/DesignPatterns/Blackboard.h"
#include "Engine/DesignPatterns/Blackboard.h"
#include "Engine/DesignPatterns/Blackboard.h"
#include "Engine/DesignPatterns/Blackboard.h"
#include "Engine/DesignPatterns/Blackboard.h"
#include "Engine/DesignPatterns/Blackboard.h"
#include "Engine/DesignPatterns/Blackboard.h"
#include "Engine/DesignPatterns/Blackboard.h"
#include "Engine/DesignPatterns/Blackboard.h"
#include "Engine/DesignPatterns/Blackboard.h"
#include "Engine/DesignPatterns/ServiceLocator.h"
#include "Engine/DesignPatterns/Singleton.h"
#include "Rendering/Sprite.h"
#include "SceneObjects/GameObject.h"
#include "SceneObjects/Scene.h"

dae::BombComponent::BombComponent(dae::GameObject* pOwner)
	: BaseComponent(pOwner)
{

}

dae::BombComponent::~BombComponent()
{

}

void dae::BombComponent::Update()
{
	m_CurrentTimer += dae::Singleton<DeltaTime>::GetInstance().GetDeltaTime();
	if(m_CurrentTimer >= m_FuseTime)
	{
		Explode();
	}
}

void dae::BombComponent::SetExplosionRange(int range)
{
	m_ExplosionRange = range;
}

void dae::BombComponent::Init(GridComponent* pGrid, const glm::vec2& gridPos)
{
	m_pGrid = pGrid;
	m_GridPos = gridPos;
}

void dae::BombComponent::Explode() const
{
	ServiceLocator::GetSoundSystem()->Play("BombExplodes");
	GetParent()->DestroyObject();
	CreateExplosion();
}


void dae::BombComponent::CreateExplosion() const
{
	float scale{GetParent()->GetComponentByClass<SpriteComponent>()->GetScale()};
	auto  scene{GetParent()->GetParentScene()};
	auto  pos{GetParent()->GetTransform().GetWorldPosition()};

	GameObject* parent{CreateExplosionCenter(scene, pos, scale)};
	CreateExplosionArms(scene, pos, scale, parent);
}

::dae::GameObject* dae::BombComponent::CreateExplosionCenter(Scene* scene, const glm::vec3& pos, float scale) const
{
	auto centerPiece{std::make_shared<GameObject>(scene)};
	auto spriteComp{centerPiece->AddComponent<SpriteComponent>()};
	spriteComp->AddSprite(4, 1, "Explosion/ExplosionCenter.png", "ExplodeCenter");
	centerPiece->SetPosition(pos.x, pos.y);
	spriteComp->SetScale(scale);
	auto collider{centerPiece->AddComponent<ColliderComponent>()};
	collider->AdjustBoundsToSpriteSize();
	centerPiece->SetTag("Explosion");
	auto explodeComp{centerPiece->AddComponent<ExplosionComponent>()};
	spriteComp->AddObserver(explodeComp);
	scene->Add(centerPiece);
	return centerPiece.get();

}

void dae::BombComponent::CreateMidPiece(dae::Scene* scene, float scale, dae::GameObject* pParent, std::vector<int> directions, size_t i) const
{
	for(int j = 0; j < m_ExplosionRange + 1; ++j)
	{
		glm::vec2 targetPos{m_GridPos};
		int       posMultiplierX{};
		int       posMultiplierY{};
		switch(directions[i])
		{
		default:
		case 0:
			targetPos.y -= static_cast<float>(j) + 1.f;
			posMultiplierY = -(j + 1);
			break;
		case 90:
			targetPos.x += static_cast<float>(j) + 1.f;
			posMultiplierX = j + 1;
			break;
		case 180:
			targetPos.y += static_cast<float>(j) + 1.f;
			posMultiplierY = j + 1;
			break;
		case 270:
			posMultiplierX = -(j + 1);
			targetPos.x -= static_cast<float>(j) + 1.f;
			break;
		}

		if(m_pGrid->GetWallstateAtPos(targetPos) != WallState::open)
			break;

		auto midPiece{std::make_shared<GameObject>(scene)};
		midPiece->SetParent(pParent);
		midPiece->SetTag("Explosion");
		auto spriteComp = midPiece->AddComponent<SpriteComponent>();

		if(j >= m_ExplosionRange)
			spriteComp->AddSprite(4, 1, "Explosion/ExplosionEndPiece.png", "ExplodeEnd");
		else
			spriteComp->AddSprite(4, 1, "Explosion/ExplosionLinePiece.png", "ExplodeLine");

		spriteComp->SetScale(scale);
		midPiece->GetTransform().SetLocalPosition(
			static_cast<float>(m_pGrid->GetCellWidth()) * static_cast<float>(posMultiplierX),
			static_cast<float>(m_pGrid->GetCellHeight()) * static_cast<float>(posMultiplierY),
			0);
		midPiece->GetTransform().SetAngleDeg(static_cast<float>(directions[i]));
		auto collider{midPiece->AddComponent<ColliderComponent>()};
		collider->AdjustBoundsToSpriteSize();
		scene->Add(midPiece);


	}
}

void dae::BombComponent::CreateExplosionArms(Scene* scene, const glm::vec3& pos, float scale, dae::GameObject* pParent) const
{
	std::vector<int> directions{0, 90, 180, 270};
	for(size_t i = 0; i < directions.size(); ++i)
	{
		CreateMidPiece(scene, scale, pParent, directions, i);
	}
}