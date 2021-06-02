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
		std::vector<GameObject*> CubeBullets;
	public:
		Megalos_kyvos();
		~Megalos_kyvos();

		SLDField()
		Vec3 Lerp = Vec3::Zero;
        void Init() override;
		void Update() override;
		void FixedUpdate() override;
		void LateUpdate() override;
        void Destroy() override;

    private:
        void PrimaryAttack()override;
        void SecondaryAttack()override;
        void TertiaryAttack()override;
        void Special1Attack()override;
        void Special2Attack()override;

        void ChooseAttack() override;
        void UpdateAttack() override;
		Megalos_kyvos_GENERATED
	};
	
	
	
}

File_GENERATED

