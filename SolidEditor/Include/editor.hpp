#pragma once

#include "Core/engine.hpp"
#include "GameCompiler/gameCompiler.hpp"

namespace Solid
{
    struct Vec2d;
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


        Engine* engine = nullptr;
        GameCompiler* Compiler;

    public:
        static InputManager<int>* editorInputManager;

        Editor();
        ~Editor();
	    void LoadResources(bool _solid);
        void Run();
        friend class EngineCleanerInterface;
    };


} //!namespace


