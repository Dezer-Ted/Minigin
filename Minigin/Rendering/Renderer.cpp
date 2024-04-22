#include <stdexcept>
#include "Renderer.h"

#include <iostream>

#include "../imgui-1.89.5/imgui.h"
#include "../imgui-1.89.5/imgui_internal.h"
#include "../SceneObjects/SceneManager.h"
#include "Texture2D.h"
#include "../Components/CameraComponent.h"
#include "../imgui-1.89.5/backends/imgui_impl_opengl3.h"
#include "../imgui-1.89.5/backends/imgui_impl_sdl2.h"

int GetOpenGLDriverIndex()
{
	auto       openglIndex = -1;
	const auto driverCount = SDL_GetNumRenderDrivers();
	for(auto i = 0; i < driverCount; i++)
	{
		SDL_RendererInfo info;
		if(!SDL_GetRenderDriverInfo(i, &info)) if(!strcmp(info.name, "opengl")) openglIndex = i;
	}
	return openglIndex;
}

void dae::Renderer::Init(SDL_Window* window)
{
	m_window = window;
	m_renderer = SDL_CreateRenderer(window, GetOpenGLDriverIndex(), SDL_RENDERER_ACCELERATED);
	if(m_renderer == nullptr)
	{
		throw std::runtime_error(std::string("SDL_CreateRenderer Error: ") + SDL_GetError());
	}
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui_ImplSDL2_InitForOpenGL(window, SDL_GL_GetCurrentContext());
	ImGui_ImplOpenGL3_Init();
}


void dae::Renderer::Render() const
{
	const auto& color = GetBackgroundColor();
	SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, color.a);
	SDL_RenderClear(m_renderer);


	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(m_window);
	ImGui::NewFrame();
	SceneManager::GetInstance().Render();
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	SDL_RenderPresent(m_renderer);
}

void dae::Renderer::Destroy()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
	if(m_renderer != nullptr)
	{
		SDL_DestroyRenderer(m_renderer);
		m_renderer = nullptr;
	}

}

void dae::Renderer::RenderTexture(const Texture2D& texture, float x, float y) const
{
	SDL_Rect        dst{};
	const glm::vec2 offSet{m_pCurrentCamera->GetOffSet()};
	dst.x = static_cast<int>(x - offSet.x);
	dst.y = static_cast<int>(y - offSet.y);

	SDL_QueryTexture(texture.GetSDLTexture(), nullptr, nullptr, &dst.w, &dst.h);
	SDL_RenderCopy(GetSDLRenderer(), texture.GetSDLTexture(), nullptr, &dst);
}

void dae::Renderer::RenderTexture(const Texture2D& texture, const float x, const float y, const SDL_Rect* sourceRect) const
{
	SDL_Rect        dst{};
	const glm::vec2 offSet{m_pCurrentCamera->GetOffSet()};
	dst.x = static_cast<int>(x - offSet.x);
	dst.y = static_cast<int>(y - offSet.y);
	dst.h = sourceRect->h;
	dst.w = sourceRect->w;

	SDL_RenderCopy(GetSDLRenderer(), texture.GetSDLTexture(), sourceRect, &dst);
}

void dae::Renderer::RenderTexture(const Texture2D& texture, float x, float y, float width, float height) const
{
	RenderTexture(texture, x, y, width, height, nullptr);
}

void dae::Renderer::RenderTexture(const Texture2D& texture, const float x, const float y, const float width, const float height,
                                  const SDL_Rect*  sourceRect) const
{
	SDL_Rect        dst{};
	const glm::vec2 offSet{m_pCurrentCamera->GetOffSet()};
	dst.x = static_cast<int>(x - offSet.x);
	dst.y = static_cast<int>(y - offSet.y);
	dst.w = static_cast<int>(width);
	dst.h = static_cast<int>(height);
	SDL_RenderCopy(GetSDLRenderer(), texture.GetSDLTexture(), sourceRect, &dst);
}

void dae::Renderer::SetCamera(CameraComponent* cam)
{
	if(cam != nullptr) m_pCurrentCamera = cam;
}


SDL_Renderer* dae::Renderer::GetSDLRenderer() const { return m_renderer; }