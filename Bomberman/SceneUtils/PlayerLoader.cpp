﻿#include "PlayerLoader.h"

#include "LevelLoader.h"
#include "../PersistentData.h"
#include "Components/SpriteComponent.h"
#include "../Components/PowerUpComponent.h"
#include "../Components/MovementComponent.h"
#include "Input/InputManager.h"
#include "../Commands/MovementCommand.h"
#include "../Commands/BombCommand.h"
#include "../Components/PlayerComponent.h"
#include "../Commands/DetonateCommand.h"
#include "../Components/VersusPlayerComponent.h"
#include "Components/ColliderComponent.h"
#include "Components/CameraComponent.h"


dae::GameObject* dae::PlayerLoader::LoadPlayer(Scene* pScene, const SDL_Rect& levelBounds, GridComponent* pGrid, const std::string& playerName)
{
	auto go = std::make_shared<dae::GameObject>(pScene);
	go->SetTag("Player");
	go->SetName(playerName);
	auto powerUpComp = LoadUpgrades(go.get());
	auto sprite = go->AddComponent<dae::SpriteComponent>();

	sprite->AddSprite(3, 1, "Character/LeftWalkCycle.png", "WalkLeft");
	sprite->AddSprite(3, 1, "Character/DownWalkCycle.png", "WalkDown");
	sprite->AddSprite(3, 1, "Character/UpWalkCycle.png", "WalkUp");
	sprite->AddSprite(3, 1, "Character/RightWalkCycle.png", "WalkRight");
	sprite->AddSprite(7, 1, "Character/DeathAnimation.png", "DeathAnimation");
	sprite->SwitchToSprite("WalkDown");
	sprite->SetScale(2.5f);
	glm::vec2 startPos = pGrid->GetPositionAtIndex(1, 1);
	go->SetPosition(startPos.x + 5, startPos.y + 5);

	auto collider = go->AddComponent<dae::ColliderComponent>();
	auto moveComp = go->AddComponent<dae::MovementComponent>();
	moveComp->SetSpeed(150.f);
	dae::Singleton<dae::InputManager>::GetInstance().AddControllerCompoundAction<dae::Move>(
		dae::Controller::ButtonInputs::DPadUp,
		dae::Controller::ButtonInputs::DPadDown,
		dae::Controller::ButtonInputs::DPadLeft,
		dae::Controller::ButtonInputs::DPadRight,
		go.get()
	);
	auto playerComp = go->AddComponent<dae::PlayerComponent>();
	playerComp->SetGrid(pGrid);
	dae::Singleton<dae::InputManager>::GetInstance().AddControllerActionMapping<dae::BombCommand>(
		dae::ControllerAction::ActionType::ButtonMap,
		go.get(),
		dae::Controller::ButtonInputs::XButton,
		dae::ControllerAction::InputType::ButtonUp
	);
	dae::Singleton<dae::InputManager>::GetInstance().AddControllerActionMapping<dae::DetonateCommand>(
		dae::ControllerAction::ActionType::ButtonMap,
		go.get(),
		dae::Controller::ButtonInputs::BButton,
		dae::ControllerAction::InputType::ButtonDown
	);
	moveComp->AddObserver(playerComp);
	collider->AddObserver(playerComp);
	collider->AdjustBoundsToSpriteSize();
	collider->AddObserver(moveComp);
	collider->AddObserver(powerUpComp);
	auto cam = go->AddComponent<dae::CameraComponent>();
	cam->SetBounds(levelBounds);
	pScene->Add(go);
	return go.get();
}

dae::GameObject* dae::PlayerLoader::LoadCoopPlayer(Scene* pScene, const SDL_Rect& levelBounds, GridComponent* pGrid, const std::string& playerName)
{
	auto go = std::make_shared<dae::GameObject>(pScene);
	go->SetTag("Player");
	go->SetName(playerName);
	auto powerUpComp = LoadUpgrades(go.get());
	auto sprite = go->AddComponent<dae::SpriteComponent>();

	sprite->AddSprite(3, 1, "Character/LeftWalkCycle.png", "WalkLeft");
	sprite->AddSprite(3, 1, "Character/DownWalkCycle.png", "WalkDown");
	sprite->AddSprite(3, 1, "Character/UpWalkCycle.png", "WalkUp");
	sprite->AddSprite(3, 1, "Character/RightWalkCycle.png", "WalkRight");
	sprite->AddSprite(7, 1, "Character/DeathAnimation.png", "DeathAnimation");
	sprite->SwitchToSprite("WalkDown");
	sprite->SetScale(2.5f);
	glm::vec2 startPos = pGrid->GetPositionAtIndex(1, 1);
	go->SetPosition(startPos.x + 5, startPos.y + 5);

	auto collider = go->AddComponent<dae::ColliderComponent>();
	auto moveComp = go->AddComponent<dae::MovementComponent>();
	moveComp->SetSpeed(150.f);

	dae::Singleton<dae::InputManager>::GetInstance().AddCompoundKeyboardAction<dae::Move>(SDL_SCANCODE_UP, SDL_SCANCODE_DOWN, SDL_SCANCODE_LEFT,
	                                                                                      SDL_SCANCODE_RIGHT, go.get());

	auto playerComp = go->AddComponent<dae::PlayerComponent>();
	playerComp->SetGrid(pGrid);

	dae::Singleton<dae::InputManager>::GetInstance().AddKeyBoardActionMapping<dae::BombCommand>(
		KeyboardAction::ActionType::ButtonMap, KeyboardAction::InputType::OnButtonDown, go.get(), SDL_SCANCODE_X);
	dae::Singleton<dae::InputManager>::GetInstance().AddKeyBoardActionMapping<dae::DetonateCommand>(
		KeyboardAction::ActionType::ButtonMap, KeyboardAction::InputType::OnButtonDown, go.get(), SDL_SCANCODE_C);

	moveComp->AddObserver(playerComp);
	collider->AddObserver(playerComp);
	collider->AdjustBoundsToSpriteSize();
	collider->AddObserver(moveComp);
	collider->AddObserver(powerUpComp);

	pScene->Add(go);
	return go.get();
}

dae::GameObject* dae::PlayerLoader::LoadVersusPlayer(Scene* pScene, const SDL_Rect& levelBounds, GridComponent* pGrid)
{
	auto go = std::make_shared<dae::GameObject>(pScene);
	go->SetTag("Enemy");
	glm::vec2 pos{pGrid->GetRandomOpenCellPosition(3, 8)};
	go->SetPosition(pos.x, pos.y);
	auto spriteComp{go->AddComponent<dae::SpriteComponent>()};
	spriteComp->AddSprite(4, 1, "Enemies/DeathAnims/OrangeDeathAnim.png", "DeathAnim");
	spriteComp->AddSprite(4, 1, "Enemies/Balloom/Walk.png", "Walk");
	spriteComp->SetScale(2.5f);
	auto collider = go->AddComponent<dae::ColliderComponent>();
	collider->AdjustBoundsToSpriteSize();
	auto moveComp = go->AddComponent<dae::MovementComponent>();
	moveComp->SetSpeed(80.f);
	auto versusPlayer = go->AddComponent<VersusPlayerComponent>();
	collider->AddObserver(versusPlayer);

	dae::Singleton<dae::InputManager>::GetInstance().AddCompoundKeyboardAction<dae::Move>(SDL_SCANCODE_UP, SDL_SCANCODE_DOWN, SDL_SCANCODE_LEFT,
	                                                                                      SDL_SCANCODE_RIGHT, go.get());
	collider->AddObserver(moveComp);
	pScene->Add(go);
	return go.get();
}

dae::PowerUpComponent* dae::PlayerLoader::LoadUpgrades(GameObject* pPlayer)
{
	auto powerUpComp = pPlayer->AddComponent<PowerUpComponent>();
	if(!PersistentData::GetInstance().CheckIfKeyExists(pPlayer->GetName()))
		return powerUpComp;
	UpgradeDataHolder upgradeHolder{PersistentData::GetInstance().GetUpgradeData(pPlayer->GetName())};
	powerUpComp->LoadExistingUpgrades(upgradeHolder);
	return powerUpComp;
}
