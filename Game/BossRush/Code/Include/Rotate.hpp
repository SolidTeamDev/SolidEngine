#include "ECS/types.hpp"
#include "Build/solidAPI.hpp"
#include "ECS/Components/script.hpp"
#include "Core/Maths/solidMaths.hpp"
#include "EngineGenerated/EntityMacros.h"
#include "EngineGenerated/Rotate.sld.hpp"
namespace Solid SLDNamespace()
{
	class SOLID_SCRIPT_API SLDClass() Rotate : public Script
	{
	public:
		
	private:
		
	public:
		Rotate();
		~Rotate();
		SLDField()
		bool RandomAngle;
		SLDField()
		float SpeedRotation = 0;
		SLDField()
		Vec3 Angle = Vec3::Zero;

		void Update() override;
		void FixedUpdate() override;
		void LateUpdate() override;
		Rotate_GENERATED
	};
	
	
	
}

File_GENERATED
