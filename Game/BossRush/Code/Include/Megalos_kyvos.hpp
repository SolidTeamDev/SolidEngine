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
	    struct Data
	    {
            int Index;
	        float Ratio;
	        Vec3  pos = Vec3::Zero;
	    };
		std::vector<GameObject*> CubeBullets;
	    std::vector<Data>       DataBullets;
	public:
	    SLDField()
	    float SpeedPrimaryAttack = 0.5;

		Megalos_kyvos();
		~Megalos_kyvos();

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

        void ChooseAttack() ;
        void UpdateAttack();
		Megalos_kyvos_GENERATED
	};
	
	
	
}

File_GENERATED

