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
	    enum class EStepAttack
	    {
	        One,
	        Two,
	        Three,
	        None
	    };
	    struct Data
	    {
            int Index;
	        float Ratio;
	        Vec3  SaveVec3 = Vec3::Zero;
	        EStepAttack Step = EStepAttack::None;
	    };
		std::vector<GameObject*> CubeBullets;
	    std::vector<Data>       DataBullets;
	public:
	    SLDField()
	    float SpeedPrimaryAttack = 0.5;
        SLDField()
        float SpeedSecondaryAttack = 1;
        SLDField()
        float SizeSecondaryAttack = 10;
        SLDField()
        float RotationAttackSpeed = 1;
        SLDField()
        float TimeSpecialAttack = 0.5;
        float CurrTimeSpe = 0;
        SLDField()
        int NumberSpawnCubeS2 = 5;
        int CurrNumber = 0;

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

        void ChooseAttack() override;
        void UpdateAttack() override;

        void CreateBulletsCube(std::string namePrefab, Vec3 pos, Vec3 scale);
		Megalos_kyvos_GENERATED
	};
	
	
	
}

File_GENERATED

