#include "ECS/types.hpp"
#include "Build/solidAPI.hpp"
#include "Boss.hpp"
#include "EngineGenerated/EntityMacros.h"
#include "EngineGenerated/Megalos_kyvos.sld.hpp"
namespace Solid SLDNamespace()
{
	class SOLID_SCRIPT_API SLDClass() Megalos_kyvos : public Boss
	{
	public:
		
	private:
		
	public:
		Megalos_kyvos();
		~Megalos_kyvos();

        void Init() override;
		void Update() override;
		void FixedUpdate() override;
		void LateUpdate() override;

	private:
        virtual void PrimaryAttack(){};
        virtual void SecondaryAttack(){};
        virtual void TertiaryAttack(){};
        virtual void Special1Attack(){};
        virtual void Special2Attack(){};

        virtual void ChooseAttack() {};
		Megalos_kyvos_GENERATED
	};
	
	
	
}

File_GENERATED
