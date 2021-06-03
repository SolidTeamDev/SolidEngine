#include "ECS/types.hpp"
#include "Build/solidAPI.hpp"
#include "ECS/Components/script.hpp"
#include "Core/Maths/solidVector.hpp"
#include "Boss.hpp"
#include "EngineGenerated/EntityMacros.h"
#include "EngineGenerated/Stereos.sld.hpp"
namespace Solid SLDNamespace()
{
	class SOLID_SCRIPT_API SLDClass() Stereos : public Boss
	{
	private:
		GameObject* player;
		Vec3 target;
		std::vector<GameObject*> SolidMinions;

		bool charging = false;
		bool shooting = false;
		float cdShoot = 0.f;

	public:
		Stereos();
		~Stereos();

		void Init() override;
		void Update() override;
		void FixedUpdate() override;
		void LateUpdate() override;
		void Destroy() override;

		void PrimaryAttack()override;
		void SecondaryAttack()override;
		void TertiaryAttack()override;
		void Special1Attack()override;
		void Special2Attack()override;

		void ChooseAttack() override;
		void UpdateAttack() override;

		Stereos_GENERATED
	};
	
	
	
}

File_GENERATED