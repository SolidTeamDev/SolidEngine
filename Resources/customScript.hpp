#pragma once
#include <string>
#include "../SolidEngine/Include/Core/Maths/Vector/vector3.hpp"
#include "ECS/Components/script.hpp"
#include "../Generated/customScript.sld.hpp"

namespace Solid SLDNamespace()
{
    class SLDClass() CustomScript : public Script
    {
    public:
        CustomScript() = default;
        ~CustomScript() = default;

        SLDField()
        std::string name = "Custom";
        SLDField()
        std::string SubName = "Custom";

        SLDField()
        float x = 0;
        SLDField()
        float y = 0;
        SLDField()
        float z = 0;

        SLDField()
        Vec3 Test = 0;




        void Update() override
        {
            std::cout << "Update from :" << name << std::endl;
        }


        void FixedUpdate()override
        {

        }


        void LateUpdate()override
        {

        }

        SLDMethod()
        std::string GetName()
        {
            return std::string(name + " Returned");
        }
        SLDMethod()
        void GetName2(std::string& outStr)
        {
            outStr= std::string(name + "ParaMed");
        }

        SLDMethod()
        Vec3 GetVec()
        {
            return Vec3{10,10,10};
        }

        CustomScript_GENERATED

    };

} //!namespace

File_GENERATED
