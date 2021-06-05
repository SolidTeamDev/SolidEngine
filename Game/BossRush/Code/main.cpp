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
	
	glfwSwapInterval(0);
	ResourcesLoader loader;
	loader.SetManager(&(engine->resourceManager));
	engine->EnableMultiThread(true);
	fs::path p = fs::current_path();
	p.append("Assets");
	loader.LoadResourcesFromFolder(p);
	engine->LoadScene("");
	engine->EnableMultiThread(false);
	Camera sceneCam;
	Time::Update();
	if(engine->activeCamera == nullptr)
		engine->SetActiveCamera(&sceneCam);
	while (!glfwWindowShouldClose(window->GetHandle()))
	{
		glfwPollEvents();
		engine->ForceUpdate();
		engine->Update();
		engine->FixedUpdate();
		engine->LateUpdate();
		renderer->ClearColor({0,0,0,1});
		renderer->Clear(window->GetWindowSize());
		engine->activeCamera->UpdateCamera(window->GetWindowSize());
		engine->rendererSystem->Update(engine->renderer,*engine->activeCamera);
		engine->audioSystem->Update(*engine->activeCamera);
		Time::Update();
		
		window->SwapBuffers();
	}
	return 0;
}
