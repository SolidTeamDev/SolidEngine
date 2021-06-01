#pragma once

#include "Core/engine.hpp"

#include "ECS/Components/meshRenderer.hpp"
#include "ECS/Components/audioSource.hpp"
#include "ECS/Components/rigidBody.hpp"
#include "ECS/Components/boxCollider.hpp"
#include "ECS/Components/capsuleCollider.hpp"
#include "ECS/Components/sphereCollider.hpp"
namespace Solid
{

	struct FieldData
	{
		std::string fName;
		std::vector<unsigned char> fData;
	};

	struct CompData
	{
		std::string compName;
		std::vector<FieldData> fields;
		GameObject* go = nullptr;
		std::size_t entityCompIndex;
		std::size_t CompListIndex;
	};

	struct CompDataSave
	{
		void* compID;
		std::vector<FieldData> fields;
		std::size_t entityCompIndex;
		std::size_t CompListIndex;
	};
	template<class T>
	struct PrimaryCompSave
	{

		T comp;
		void* TID = nullptr;

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

	    static bool play;
	    static bool paused;
        Engine* engine = nullptr;

	    static std::vector<CompDataSave> compsSave;
	    static std::vector<PrimaryCompSave<Transform>> transSave;
	    static std::vector<PrimaryCompSave<Camera>> camSave;
	    static std::vector<PrimaryCompSave<AudioSource>> audioSave;
	    static std::vector<PrimaryCompSave<MeshRenderer>> meshSave;
	    static std::vector<PrimaryCompSave<RigidBody>> rbSave;
	    static std::vector<PrimaryCompSave<BoxCollider>> boxSave;
	    static std::vector<PrimaryCompSave<SphereCollider>> sphereSave;
	    static std::vector<PrimaryCompSave<CapsuleCollider>> capsuleSave;
	    static SceneResource* tempScene ;
    public:
        static InputManager* editorInputManager;

        static float camSpeed;
        //static Framebuffer sceneFramebuffer;
        //static Framebuffer playFramebuffer;
        static Camera editorCamera;

        json CurrentProjectJson;

        Editor();
        ~Editor();
        void InitFromProject(fs::path _projectPath);
	    void LoadResources(fs::path& p);
        void Run();
        static void Play();
	    static void Pause();
	    static void Stop();
        friend class EngineCleanerInterface;
    };


} //!namespace


