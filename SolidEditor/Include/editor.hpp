#pragma once

#include "Core/engine.hpp"
#include "GameCompiler/gameCompiler.hpp"

#include "ECS/Components/camera.hpp"

namespace Solid
{
	class SOLID_API EngineCleanerInterface
	{
	protected:
		EngineCleanerInterface()
		{

		}

		~EngineCleanerInterface()
		{
			Engine::DeleteInstance();
		}

	};

    class Editor
    {
    private:
	    class SOLID_API Engine_Cleaner : public EngineCleanerInterface
	    {
	    public:
		    Engine_Cleaner()
		    {

		    }

		    ~Engine_Cleaner()
		    {

		    }
	    }Clean;

        InputManager<int>* editorInputManager = nullptr;
        Engine* engine = nullptr;
        GameCompiler* Compiler;
        Camera editorCamera;

        void UpdateEditorCamera();
        void LoadResources();
    public:

        static Framebuffer sceneFramebuffer;

        Editor();
        ~Editor();
        
        void Run();
        friend class EngineCleanerInterface;
    };


} //!namespace


