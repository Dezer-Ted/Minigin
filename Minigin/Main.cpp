#pragma once
#include <SDL.h>

#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include "GameObject.h"
#include "Minigin.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "Scene.h"
#include "GameObject.h"
#include "InputManager.h"
#include "Utils.h"
#include "Components/EncircleComponent.h"
#include "Components/FPSComponent.h"
#include "Components/MiscFunctionComponent.h"
#include "Components/TextComponent.h"
#include "Components/TextureComponent.h"
#include "Components/TrashTheCacheComponent.h"
#include "Command.h"


#pragma comment(lib, "XInput.lib")
void load()
{
	auto& scene = dae::SceneManager::GetInstance().CreateScene("Demo");

	auto go = std::make_shared<dae::GameObject>();
	go->AddComponent<dae::TextureComponent>();
	go->GetComponentByClass<dae::TextureComponent>()->SetTexture("background.tga");
	scene.Add(go);

	go = std::make_shared<dae::GameObject>();
	go->AddComponent<dae::TextureComponent>();
	go->GetComponentByClass<dae::TextureComponent>()->SetTexture("logo.tga");

	go->SetPosition(216, 180);
	scene.Add(go);

	auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);

	go = std::make_shared<dae::GameObject>();
	go->AddComponent<dae::TextComponent>();
	go->SetPosition(80, 20);
	go->GetComponentByClass<dae::TextComponent>()->SetText("Programming 4 Assignment", font, SDL_Color{255, 255, 255, 255});
	scene.Add(go);

	go = std::make_shared<dae::GameObject>();
	go->AddComponent<dae::FPSComponent>();
	go->SetPosition(0.f, 0.f);
	scene.Add(go);

	go = std::make_shared<dae::GameObject>();
	go->AddComponent<dae::TextureComponent>()->SetTexture("pacman.tga");
	go->SetPosition(320, 240);
	go->AddComponent<dae::MovementComponent>();
	dae::Singleton<dae::InputManager>::GetInstance().AddControllerActionMapping<dae::Move>(dae::ControllerAction::ActionType::LeftAnalogStick, go.get());
	scene.Add(go);
	
	go = std::make_shared<dae::GameObject>();
	go->AddComponent<dae::TextureComponent>()->SetTexture("pacman.tga");
	go->SetPosition(200, 200);
	go->AddComponent<dae::MovementComponent>();
	dae::InputManager::GetInstance().AddCompoundKeyboardAction(SDL_SCANCODE_W,SDL_SCANCODE_S,SDL_SCANCODE_A,SDL_SCANCODE_D,go.get());
	scene.Add(go);
}

int main(int, char*[])
{
	dae::Minigin engine("../Data/");
	engine.Run(load);
	return 0;
}
