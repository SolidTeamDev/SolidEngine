#include "ECS/types.hpp"
#include "Build/solidAPI.hpp"
#include "Core/Maths/solidMaths.hpp"
#include "ECS/Components/script.hpp"
#include "EngineGenerated/EntityMacros.h"
#include "EngineGenerated/Boss.sld.hpp"

namespace Solid SLDNamespace()
{
	class SOLID_SCRIPT_API SLDClass() Boss : public Script
	{
	public:
		enum class EAttack
        {
		    PRIMARY,
            SECONDARY,
            TERTIARY,
            SPECIAL1,
            SPECIAL2,
            NONE
        };
		EAttack StateAttack = EAttack::NONE;

	    SLDField()
		float BossHealth = 100;
		SLDField()
		Vec3 Target = Vec3::Zero;
    protected:
	    class Engine* engine;
		GameObject* Player = nullptr;
	public:
		Boss();
		~Boss();

        virtual void Init() override;
		virtual void Update() override;
        virtual void FixedUpdate() override;
        virtual void LateUpdate() override;

	private:
	    virtual void PrimaryAttack() = 0;
        virtual void SecondaryAttack() = 0;
        virtual void TertiaryAttack() = 0;
        virtual void Special1Attack() = 0;
        virtual void Special2Attack() = 0;

        virtual void ChooseAttack() = 0;

		Boss_GENERATED
	};
	
	
	
}

File_GENERATED
