#pragma once


#include "Core/Maths/Vector/vector2.hpp"
#include "Core/Maths/Matrix/matrix4.hpp"
#include "Core/Maths/Quaternion/quaternion.hpp"
#include "transform.hpp"

#include "ECS/types.hpp"
#include "Build/solidAPI.hpp"
#include "EngineGenerated/camera.sld.hpp"

namespace Solid SLDNamespace()
{
    class SOLID_API SLDClass() Camera : public Components
    {
    private:
        Mat4<float> projection;
        Mat4<float> view;

    public:
	    SLDField()
	    Transform   transform;
	    SLDField()
	    float fov;
	    SLDField()
	    float near;
	    SLDField()
	    float far;

        Camera();

        Mat4<float> GetView();
        Mat4<float> GetProjection() const;

        void UpdateCamera(const Vec2i _spaceScreen);
        Camera_GENERATED
    };
} //!namespace

File_GENERATED