#include "Core/engine.hpp"
#include "Resources/ressources.hpp"
#include <filesystem>

namespace fs = std::filesystem;
using namespace Solid;

int main()
{
	Engine* engine = Engine::GetInstance();
	WindowParams windowParams
	{
	    .title = "Solid BossRush",
	    .windowSize = {1280,720}
	};
	RendererParams rendererParams
	{
	    .rendererType = ERendererType::OpenGl45
	};
	engine->InitEngineContext(windowParams, rendererParams);
	if(!engine->IsEngineContextInitialized())
		throw ThrowError("Engine not correctly initialized !",ESolidErrorCode::S_INIT_ERROR);
	Window* window = engine->window;
	Renderer* renderer = engine->renderer;
	InputManager<int> * GameInputManager = new InputManager<int>(window->GetHandle());
	glfwSwapInterval(0);
	ResourcesLoader loader;
	loader.SetManager(&(engine->resourceManager));
	engine->EnableMultiThread(true);
	fs::path p = fs::current_path();
	p.append("Assets");
	loader.LoadResourcesFromFolder(p);
	engine->LoadScene(fs::current_path());
	engine->EnableMultiThread(false);
	Camera sceneCam;
	while (!glfwWindowShouldClose(window->GetHandle()))
	{
		glfwPollEvents();
		GameInputManager->Update();
		engine->Update();
		engine->FixedUpdate();
		engine->LateUpdate();
		renderer->ClearColor({0,0,0,1});
		renderer->Clear(window->GetWindowSize());
		sceneCam.UpdateCamera(window->GetWindowSize());
		engine->rendererSystem->Update(engine->renderer,sceneCam);
		engine->audioSystem->Update(sceneCam);
		Time::Update();
		
		window->SwapBuffers();
	}
	return 0;
}
