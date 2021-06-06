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
#include "ECS/Components/light.hpp"
#include "ECS/Components/particleEffect.hpp"

#include <TextEditor.h>

#include <string>

namespace Solid
{
    class Engine;
    class InspectorInterface
    {
    private:
        struct CodeEditor
        {
            enum class ECodeType
            {
                VERTEX,
                FRAGMENT,
                COMPUTE
            };
            TextEditor imCodeEditor;
            ECodeType codeType;
            bool isCodeEditorOpen = false;
        };

        CodeEditor codeEditor;

        Engine* engine = nullptr;

    public:
        InspectorInterface();
        ~InspectorInterface() = default;

        void Draw();
        void DrawComponents();

        void AddComponents();
        void CreateScriptWindow();

        void EditTransform(Transform& _trs);
        void EditMeshRenderer(MeshRenderer& _meshRenderer);
        void EditAudioSource(AudioSource& _audioSource);
        void EditAnimation(Animation& _anim);

        void EditLight(Light& _light);
        void EditCamera(Camera& _camera);
		void EditParticleEffect(ParticleEffect&);
        void EditRigidBody(RigidBody& _rigidBody);
        void EditBoxCollider(BoxCollider& _boxCollider);
        void EditSphereCollider(SphereCollider& _sphereCollider);
        void EditCapsuleCollider(CapsuleCollider& _capsuleCollider);

        void EditComp(Components* _comp);

        bool EditBool(bool &_num, const std::string &_label);
        bool EditInt(int &_num, const std::string &_label, float _step);
        bool EditFloat(float& _num, const std::string& _label, float _step);
        bool EditVec3(Vec3 &_vec, const std::string &_label, float _step);
        bool EditVec4(Vec4 &_vec, const std::string &_label, float _step);
        bool EditVec2(Vec2 &_vec, const std::string &_label, float _step);
        void EditText(std::string& _str, const std::string& _label);
        void EditTexture(MatText &_texture, const std::string& _label);

        bool p_open = true;
        bool openCreateScript = false;
    };


}
