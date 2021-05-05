#pragma once

#include "Core/Maths/solidMaths.hpp"
#include "ECS/Components/transform.hpp"
#include "ECS/Components/meshRenderer.hpp"
#include "ECS/Components/audioSource.hpp"
#include "ECS/Components/rigidBody.hpp"
#include "ECS/Components/boxCollider.hpp"
#include "ECS/Components/sphereCollider.hpp"
#include "ECS/Components/capsuleCollider.hpp"
#include "ECS/Components/animation.hpp"

#include <string>

namespace Solid
{
    class Engine;
    class InspectorInterface
    {
    public:
        InspectorInterface()  = default;
        ~InspectorInterface() = default;

        void Draw();
        void DrawComponents();
        void AddComponents();
        void CreateScriptWindow();

        void EditTransform(Transform& _trs);
        void EditMeshRenderer(MeshRenderer& _meshRenderer);
        void EditAudioSource(AudioSource& _audioSource);
        void EditAnimation(Animation& _anim);


        void EditComp(Components* _comp);

        void EditBool(bool &_num, const std::string &_label);
        void EditInt(int &_num, const std::string &_label, float _step);
        bool EditVec3(Vec3 &_vec, const std::string &_label, float _step);
        bool EditVec4(Vec4 &_vec, const std::string &_label, float _step);
        void EditVec2(Vec2 &_vec, const std::string &_label, float _step);
        void EditText(std::string& _str, const std::string& _label);
        void EditFloat(float&      _num, const std::string& _label, float _step);

        bool p_open = true;
        bool openCreateScript = false;
	    bool fvAdd = false;
	    bool textureAdd = false;
	    bool namePopup = false;
	    std::string nameStr;
	    MaterialResource* matToModify = nullptr;
	    const char* beginField = "NONE";
	    int chosenType =(int)MaterialResource::EFieldType::NONE;
	    void DrawUniformNamePopup();

    };


}
