#pragma once

#include "Core/engine.hpp"
#include "GameCompiler/gameCompiler.hpp"

namespace Solid
{


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


        Engine* engine = nullptr;
        GameCompiler* Compiler;

    public:
        static InputManager<int>* editorInputManager;

        static float camSpeed;
        static Framebuffer sceneFramebuffer;
        static Camera editorCamera;
        json CurrentProjectJson;

        Editor();
        ~Editor();
        void InitFromProject();
	    void LoadResources(fs::path& p);
        void Run();
        friend class EngineCleanerInterface;
    };


} //!namespace


