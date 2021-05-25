#include "Physics/physicsEventCallback.hpp"

#include <PxRigidActor.h>
#include "ECS/sceneGraphManager.hpp"
#include "ECS/Components/scriptList.hpp"
#include "Core/engine.hpp"
#include "Core/Debug/log.hpp"

namespace Solid
{
    void PhysicsEventCallback::onContact(const physx::PxContactPairHeader &pairHeader, const physx::PxContactPair *pairs, physx::PxU32 nbPairs)
    {
        Engine* engine = Engine::GetInstance();
        for(size_t i = 0 ; i < nbPairs; i++)
        {
            Log::Send("On contact : " + std::to_string(nbPairs), Log::ELogSeverity::DEBUG);
            GameObject* go1 = (GameObject*)pairHeader.actors[0]->userData;
            GameObject* go2 = (GameObject*)pairHeader.actors[1]->userData;

            physx::PxPairFlags flagStatus = pairs[i].events;

            if (engine->ecsManager.GotComponent<ScriptList>(go1->GetEntity()))
            {
                std::vector<Script *> scriptListGo1 = engine->ecsManager.GetComponent<ScriptList>(
                        go1->GetEntity()).GetAllScripts();

                for (auto &script : scriptListGo1)
                {
                    if (flagStatus == physx::PxPairFlag::eNOTIFY_TOUCH_FOUND)
                        script->OnContactEnter(go2);
                    else if (flagStatus == physx::PxPairFlag::eNOTIFY_TOUCH_LOST)
                        script->OnContactExit(go2);
                }
            }
            if (engine->ecsManager.GotComponent<ScriptList>(go2->GetEntity()))
            {
                std::vector<Script *> scriptListGo2 = engine->ecsManager.GetComponent<ScriptList>(
                        go2->GetEntity()).GetAllScripts();

                for(auto& script : scriptListGo2)
                {
                    if(flagStatus == physx::PxPairFlag::eNOTIFY_TOUCH_FOUND)
                        script->OnContactEnter(go1);
                    else if(flagStatus == physx::PxPairFlag::eNOTIFY_TOUCH_LOST)
                        script->OnContactExit(go1);
                }
            }
        }
    }

    void PhysicsEventCallback::onTrigger(physx::PxTriggerPair *pairs, physx::PxU32 count)
    {
        Engine* engine = Engine::GetInstance();

        for(size_t i = 0 ; i < count; i++)
        {
            Log::Send("On trigger : " + std::to_string(count), Log::ELogSeverity::DEBUG);
            GameObject *go1 = (GameObject *) pairs[i].triggerActor->userData;
            GameObject *go2 = (GameObject *) pairs[i].otherActor->userData;

            physx::PxPairFlag::Enum flagStatus = pairs[i].status;

            if (engine->ecsManager.GotComponent<ScriptList>(go1->GetEntity()))
            {
                std::vector<Script *> scriptListGo1 = engine->ecsManager.GetComponent<ScriptList>(
                        go1->GetEntity()).GetAllScripts();

                for (auto &script : scriptListGo1)
                {
                    if (flagStatus == physx::PxPairFlag::eNOTIFY_TOUCH_FOUND)
                        script->OnTriggerEnter(go2);
                    else if (flagStatus == physx::PxPairFlag::eNOTIFY_TOUCH_LOST)
                        script->OnTriggerExit(go2);
                }
            }
            if (engine->ecsManager.GotComponent<ScriptList>(go2->GetEntity()))
            {
                std::vector<Script *> scriptListGo2 = engine->ecsManager.GetComponent<ScriptList>(
                        go2->GetEntity()).GetAllScripts();

                for(auto& script : scriptListGo2)
                {
                    if(flagStatus == physx::PxPairFlag::eNOTIFY_TOUCH_FOUND)
                        script->OnTriggerEnter(go1);
                    else if(flagStatus == physx::PxPairFlag::eNOTIFY_TOUCH_LOST)
                        script->OnTriggerExit(go1);
                }
            }
        }
    }
} //!namespace


