#include "ECS/sceneGraphManager.hpp"
#include "ECS/Components/audioSource.hpp"
#include "ECS/Components/boxCollider.hpp"
#include "ECS/Components/scriptList.hpp"
#include "ECS/Components/camera.hpp"
#include "ECS/Components/capsuleCollider.hpp"
#include "ECS/Components/meshRenderer.hpp"
#include "ECS/Components/rigidBody.hpp"
#include "ECS/Components/sphereCollider.hpp"
#include "ECS/Components/transform.hpp"
#include "ECS/Components/audioSource.hpp"
#include "ECS/Components/light.hpp"
#include "Core/engine.hpp"

Solid::GameObject::GameObject(Solid::Entity _entity)
{
	entity = _entity;
}

Solid::GameObject::GameObject(Solid::GameObject *_parent, Solid::Entity _entity)
{
	parent = _parent;
	entity = _entity;
}

Solid::GameObject::~GameObject()
{

	for (GameObject* child : childs)
	{
        Engine::GetInstance()->ecsManager.DestroyEntity(child->entity,FromSceneGraphMgr{child});

        if(child->physicsActor != nullptr)
        {
            if(child->physicsActor->isReleasable())
		        child->physicsActor->release();
            child->physicsActor = nullptr;
        }

		delete child;
	}
}

Solid::GameObject *Solid::GameObject::AddToCurrent(Solid::Entity _entity)
{
	GameObject* obj = new GameObject(this,_entity);
	childs.push_back(obj);
	return obj;
}

void Solid::GameObject::ReParentCurrent(Solid::GameObject *Parent)
{
	GameObject* ParentTemp =this->parent;
	this->parent = Parent;
	for (auto it = ParentTemp->childs.begin(); it != ParentTemp->childs.end(); it++)
	{
		if((*it) == this)
		{
			ParentTemp->childs.erase(it);
			break;
		}
	}
	Parent->childs.push_back(this);
}

void Solid::GameObject::RemoveCurrent()
{
	if(parent != nullptr)
	{
		for (size_t i = 0 ; i < parent->childs.size() ; ++i)
		{
			if (parent->childs.at(i) == this)
			{
				parent->childs.erase(parent->childs.begin() + i);
				if(physicsActor != nullptr)
                {
                    if(physicsActor->isReleasable())
				        physicsActor->release();
				    physicsActor = nullptr;
                }
				delete this;
				break;
			}
		}
	}
}

Solid::Entity Solid::GameObject::GetEntity() const
{
	return entity;
}

Solid::GameObject *Solid::SceneGraphManager::GetNodeFromEntity(Solid::Entity _entity, Solid::GameObject *node)
{
	if(node == nullptr)
		return nullptr;

	GameObject* result = nullptr;

	for (GameObject* node : node->childs)
	{
		if(node->GetEntity() == _entity)
		{
			result = node;
			break;
		}
		else
		{
			result = GetNodeFromEntity(_entity,node);
			if(result != nullptr)
				break;
		}
	}

	return result;
}

Solid::SceneGraphManager::SceneGraphManager() :
		world(-1)
{}

Solid::GameObject *Solid::SceneGraphManager::GetWorld()
{
	return &world;
}

Solid::GameObject *Solid::SceneGraphManager::GetNodeFromEntity(Solid::Entity _entity)
{
	return GetNodeFromEntity(_entity,&world);
}

template<class T>
T * Solid::SceneGraphManager::AddComp(const std::string &className, std::vector<char> &buffer, std::uint64_t &readPos,
                                       GameObject *go, Components *cmp, std::size_t FieldNum, std::size_t cmpNameSize)
{
	T *t = Engine::GetInstance()->ecsManager.AddComponent(go, *(T *) cmp);
	for (int i = 0; i < FieldNum; ++i)
	{
		short isNull = -1;

		//Get Comp FieldName
		ResourcesLoader::ReadFromBuffer(buffer.data(), &cmpNameSize, sizeof(std::size_t),
		                                readPos, buffer.size());
		std::string Name;
		Name.resize(cmpNameSize / sizeof(std::string::value_type));
		ResourcesLoader::ReadFromBuffer(buffer.data(), Name.data(), cmpNameSize, readPos, buffer.size());

		//Get is Null
		ResourcesLoader::ReadFromBuffer(buffer.data(), &isNull, sizeof(short), readPos, buffer.size());
		std::size_t memSize = 0;
		//Get Field Data
		const rfk::Field *f = t->getArchetype().getField(Name);
		std::size_t fieldSize = 0;
		ResourcesLoader::ReadFromBuffer(buffer.data(), &fieldSize, sizeof(std::size_t),
		                                readPos, buffer.size());
		if(f == nullptr)
		{
			readPos+=fieldSize;
			continue;
		}
		if (f->type.archetype->name == "String")
		{
			String *str = (String *) f->getDataAddress(t);
			str->resize(fieldSize / sizeof(std::string::value_type));

			ResourcesLoader::ReadFromBuffer(buffer.data(), str->data(), fieldSize, readPos, buffer.size());
		}
		else if (f->type.archetype->name == "vectorStr")
		{
			std::size_t vecSize = 0;
			vectorStr *vstr = (vectorStr *) f->getDataAddress(t);
			ResourcesLoader::ReadFromBuffer(buffer.data(), &vecSize, sizeof(std::size_t),
			                                readPos, buffer.size());
			for (int k = 0; k < vecSize; ++k)
			{
				String str;
				ResourcesLoader::ReadFromBuffer(buffer.data(), &memSize, sizeof(std::size_t),
				                                readPos, buffer.size());
				str.resize(memSize);
				ResourcesLoader::ReadFromBuffer(buffer.data(), str.data(), memSize, readPos, buffer.size());

				vstr->push_back(std::move(str));
			}
		}
		else
		{
			if (isNull == 256)
			{}
			else
			{}
			char *buf = new char[fieldSize]();
			ResourcesLoader::ReadFromBuffer(buffer.data(), buf, fieldSize, readPos, buffer.size());

			f->setData(t, ((void *) buf), fieldSize);
			delete[] buf;

		}


		if (className == "Transform" && Name == "rotation")
		{

			((Transform*)t)->SetRotation(((Transform *) t)->GetLocalRotation());
		}
	}
	if(className.find("Collider") != std::string::npos)
		t->Release();
	t->Init();
	return t;

}



void Solid::SceneGraphManager::AddAllComps(GameObject *elt, std::vector<char> &buffer, uint64_t &readPos)
{
	rfk::Namespace const* n = rfk::Database::getNamespace("Solid");
	Engine* engine = Engine::GetInstance();
	ECSManager& ecsManager = engine->ecsManager;
	GameCompiler* Compiler = Engine::GetInstance()->Compiler;
	{
		GameObject* go = elt;
		//get num of comps
		std::size_t cmpNum = 0;
		std::size_t cmpNameSize = 0;
		ResourcesLoader::ReadFromBuffer(buffer.data(), &cmpNum, sizeof(std::size_t), readPos, buffer.size());


		for (int j = 0; j < cmpNum; ++j)
		{
			//get Comp Class / str
			ResourcesLoader::ReadFromBuffer(buffer.data(), &cmpNameSize, sizeof(std::size_t), readPos, buffer.size());
			std::string className;
			className.resize(cmpNameSize / sizeof(std::string::value_type));
			ResourcesLoader::ReadFromBuffer(buffer.data(), className.data(), cmpNameSize, readPos, buffer.size());

			//get Field Num
			std::size_t FieldNum = 0;
			ResourcesLoader::ReadFromBuffer(buffer.data(), &FieldNum, sizeof(std::size_t), readPos, buffer.size());


			//get SkipSize
			std::size_t SkipSize = 0;
			ResourcesLoader::ReadFromBuffer(buffer.data(), &SkipSize, sizeof(std::size_t), readPos, buffer.size());

			rfk::Class const *myClass = n->getClass(className);
			rfk::Namespace const* ns = Compiler->GetNamespace("Solid");
			if(myClass == nullptr && ns != nullptr)
				myClass = ns->getClass(className);
			//COMPCUSTO
			if(myClass != nullptr)
			{
				Components *cmp = myClass->makeInstance<Components>();

				if (className == "Transform")
				{

					AddComp<Transform>(className, buffer, readPos, go, cmp,FieldNum, cmpNameSize);
					delete cmp;
				}
				else if (className == "AudioSource")
				{
					AddComp<AudioSource>(className, buffer, readPos, go, cmp,FieldNum, cmpNameSize);
					delete cmp;
				}
				else if (className == "MeshRenderer")
				{
					AddComp<MeshRenderer>(className, buffer, readPos, go, cmp,FieldNum, cmpNameSize);
					delete cmp;
				}
				else if (className == "Animation")
				{
					AddComp<Animation>(className, buffer, readPos, go, cmp,FieldNum, cmpNameSize);
					delete cmp;
				}
				else if (className == "RigidBody")
				{
					AddComp<RigidBody>(className, buffer, readPos, go, cmp,FieldNum, cmpNameSize);
					delete cmp;
				}
				else if (className == "BoxCollider")
				{
					AddComp<BoxCollider>(className, buffer, readPos, go, cmp,FieldNum, cmpNameSize);
					delete cmp;
				}
				else if (className == "SphereCollider")
				{
					AddComp<SphereCollider>(className, buffer, readPos, go, cmp,FieldNum, cmpNameSize);
					delete cmp;
				}
				else if (className == "CapsuleCollider")
				{
					AddComp<CapsuleCollider>(className, buffer, readPos, go, cmp,FieldNum, cmpNameSize);
					delete cmp;
				}
				else if (className == "Camera")
				{
					Camera* cam = AddComp<Camera>(className, buffer, readPos, go, cmp,FieldNum, cmpNameSize);
					if(cam->IsActive())
						cam->SetActiveCamera();
					delete cmp;
				}
				else if (className == "Light")
				{
					AddComp<Light>(className, buffer, readPos, go, cmp,FieldNum, cmpNameSize);
					delete cmp;
				}
				else if (className == "ParticleEffect")
				{



					ParticleEffect *t = Engine::GetInstance()->ecsManager.AddComponent(go, *(ParticleEffect *) cmp);
					std::size_t sizep = 0;
					ResourcesLoader::ReadFromBuffer(buffer.data(), &sizep, sizeof(std::size_t),readPos, buffer.size());
					t->particlesSize = sizep;

					std::size_t isNull = 0;
					ResourcesLoader::ReadFromBuffer(buffer.data(), &isNull, sizeof(std::size_t), readPos, buffer.size());
					if(isNull == 128)
					{
						std::size_t numP = 0;
						ResourcesLoader::ReadFromBuffer(buffer.data(), &numP, sizeof(std::size_t), readPos, buffer.size());
						t->SetCount(numP);
					}
					isNull = 0;
					ResourcesLoader::ReadFromBuffer(buffer.data(), &isNull, sizeof(std::size_t), readPos, buffer.size());
					if(isNull == 128)
					{
						float em = 0;
						ResourcesLoader::ReadFromBuffer(buffer.data(), &em, sizeof(float), readPos, buffer.size());
						t->SetEmitRate(em);
					}
					isNull = 0;
					ResourcesLoader::ReadFromBuffer(buffer.data(), &isNull, sizeof(std::size_t), readPos, buffer.size());
					if(isNull == 128)
					{

						std::size_t nsize = 0;
						ResourcesLoader::ReadFromBuffer(buffer.data(), &nsize, sizeof(std::size_t), readPos, buffer.size());
						std::string TexName;
						TexName.resize(nsize / sizeof(std::string::value_type));
						ResourcesLoader::ReadFromBuffer(buffer.data(), TexName.data(), nsize, readPos, buffer.size());
						t->SetTex(Engine::GetInstance()->graphicsResourceMgr.GetTexture(TexName.c_str()));
					}
					isNull = 0;
					ResourcesLoader::ReadFromBuffer(buffer.data(), &isNull, sizeof(std::size_t), readPos, buffer.size());
					if(isNull == 128)
					{

					}
					isNull = 0;
					ResourcesLoader::ReadFromBuffer(buffer.data(), &isNull, sizeof(std::size_t), readPos, buffer.size());
					if(isNull == 128)
					{
						if(t->velFromPosGen == nullptr)
							t->velFromPosGen = std::make_shared<VelFromPosGen>();
						ResourcesLoader::ReadFromBuffer(buffer.data(), &t->velFromPosGen->offset, sizeof(Vec4), readPos, buffer.size());
						ResourcesLoader::ReadFromBuffer(buffer.data(), &t->velFromPosGen->minScale, sizeof(float), readPos, buffer.size());
						ResourcesLoader::ReadFromBuffer(buffer.data(), &t->velFromPosGen->maxScale, sizeof(float), readPos, buffer.size());

					}
					else
					{
						t->velFromPosGen = nullptr;
					}
					isNull = 0;
					ResourcesLoader::ReadFromBuffer(buffer.data(), &isNull, sizeof(std::size_t), readPos, buffer.size());
					if(isNull == 128)
					{
						if(t->colGen == nullptr)
							t->colGen = std::make_shared<BasicColorGen>();

						{
							auto ptr = t->colGen;
							ResourcesLoader::ReadFromBuffer(buffer.data(), &ptr->minStartCol, sizeof(Vec4), readPos, buffer.size());
							ResourcesLoader::ReadFromBuffer(buffer.data(), &ptr->maxStartCol, sizeof(Vec4), readPos, buffer.size());
							ResourcesLoader::ReadFromBuffer(buffer.data(), &ptr->minEndCol, sizeof(Vec4), readPos, buffer.size());
							ResourcesLoader::ReadFromBuffer(buffer.data(), &ptr->maxEndCol, sizeof(Vec4), readPos, buffer.size());
						}
					}
					else
					{
						t->colGen = nullptr;
					}
					isNull = 0;
					ResourcesLoader::ReadFromBuffer(buffer.data(), &isNull, sizeof(std::size_t), readPos, buffer.size());
					if(isNull == 128)
					{
						if(t->sphereVelGen == nullptr)
							t->sphereVelGen = std::make_shared<SphereVelGen>();

						{
							auto ptr = t->sphereVelGen;
							ResourcesLoader::ReadFromBuffer(buffer.data(), &ptr->minVel, sizeof(float), readPos, buffer.size());
							ResourcesLoader::ReadFromBuffer(buffer.data(), &ptr->maxVel, sizeof(float), readPos, buffer.size());

						}
					}
					else
					{
						t->sphereVelGen = nullptr;
					}
					isNull = 0;
					ResourcesLoader::ReadFromBuffer(buffer.data(), &isNull, sizeof(std::size_t), readPos, buffer.size());
					if(isNull == 128)
					{
						if(t->spherePosGen == nullptr)
							t->spherePosGen = std::make_shared<SpherePosGen>();

						{
							auto ptr = t->spherePosGen;
							ResourcesLoader::ReadFromBuffer(buffer.data(), &ptr->center, sizeof(Vec4), readPos, buffer.size());
							ResourcesLoader::ReadFromBuffer(buffer.data(), &ptr->radX, sizeof(float), readPos, buffer.size());
							ResourcesLoader::ReadFromBuffer(buffer.data(), &ptr->radY, sizeof(float), readPos, buffer.size());
							ResourcesLoader::ReadFromBuffer(buffer.data(), &ptr->radZ, sizeof(float), readPos, buffer.size());

						}
					}
					else
					{
						t->spherePosGen = nullptr;
					}
					isNull = 0;
					ResourcesLoader::ReadFromBuffer(buffer.data(), &isNull, sizeof(std::size_t), readPos, buffer.size());
					if(isNull == 128)
					{
						if(t->timeGen == nullptr)
							t->timeGen = std::make_shared<BasicTimeGen>();

						{
							auto ptr = t->timeGen;
							ResourcesLoader::ReadFromBuffer(buffer.data(), &ptr->minTime, sizeof(float), readPos, buffer.size());
							ResourcesLoader::ReadFromBuffer(buffer.data(), &ptr->maxTime, sizeof(float), readPos, buffer.size());


						}
					}
					else
					{
						t->timeGen = nullptr;
					}
					isNull = 0;
					ResourcesLoader::ReadFromBuffer(buffer.data(), &isNull, sizeof(std::size_t), readPos, buffer.size());
					if(isNull == 128)
					{
						if(t->velGen == nullptr)
							t->velGen = std::make_shared<BasicVelGen>();

						{
							auto ptr = t->velGen;
							ResourcesLoader::ReadFromBuffer(buffer.data(), &ptr->minStartVel, sizeof(Vec4), readPos, buffer.size());
							ResourcesLoader::ReadFromBuffer(buffer.data(), &ptr->maxStartVel, sizeof(Vec4), readPos, buffer.size());

						}
					}
					else
					{
						t->velGen = nullptr;
					}
					isNull = 0;
					ResourcesLoader::ReadFromBuffer(buffer.data(), &isNull, sizeof(std::size_t), readPos, buffer.size());
					if(isNull == 128)
					{
						if(t->boxPosGen == nullptr)
							t->boxPosGen = std::make_shared<BoxPosGen>();

						{
							auto ptr = t->boxPosGen;
							ResourcesLoader::ReadFromBuffer(buffer.data(), &ptr->pos, sizeof(Vec4), readPos, buffer.size());
							ResourcesLoader::ReadFromBuffer(buffer.data(), &ptr->maxStartPosOffset, sizeof(Vec4), readPos, buffer.size());

						}
					}
					else
					{
						t->boxPosGen = nullptr;
					}
					isNull = 0;
					ResourcesLoader::ReadFromBuffer(buffer.data(), &isNull, sizeof(std::size_t), readPos, buffer.size());
					if(isNull == 128)
					{
						if(t->killerZoneUpdater == nullptr)
							t->killerZoneUpdater = std::make_shared<KillerZoneUpdater>();

						{
							auto ptr = t->killerZoneUpdater;
							ResourcesLoader::ReadFromBuffer(buffer.data(), &ptr->pos, sizeof(Vec3), readPos, buffer.size());
							ResourcesLoader::ReadFromBuffer(buffer.data(), &ptr->offset, sizeof(Vec3), readPos, buffer.size());

						}
					}
					else
					{
						t->killerZoneUpdater = nullptr;
					}
					isNull = 0;
					ResourcesLoader::ReadFromBuffer(buffer.data(), &isNull, sizeof(std::size_t), readPos, buffer.size());
					if(isNull == 128)
					{
						if(t->attractorUpdater == nullptr)
							t->attractorUpdater = std::make_shared<AttractorUpdater>();

						{
							auto ptr = t->attractorUpdater;
							std::size_t attsize = 0;
							ResourcesLoader::ReadFromBuffer(buffer.data(), &attsize, sizeof(std::size_t), readPos, buffer.size());
							t->attractorUpdater->attractors.resize(attsize);
							ResourcesLoader::ReadFromBuffer(buffer.data(), ptr->attractors.data(), sizeof(Vec4) * attsize, readPos, buffer.size());

						}
					}
					else
					{
						t->attractorUpdater = nullptr;
					}
					isNull = 0;
					ResourcesLoader::ReadFromBuffer(buffer.data(), &isNull, sizeof(std::size_t), readPos, buffer.size());
					if(isNull == 128)
					{
						if(t->velColUpdater == nullptr)
							t->velColUpdater = std::make_shared<VelColorUpdater>();

						{
							auto ptr = t->velColUpdater;
							ResourcesLoader::ReadFromBuffer(buffer.data(), &ptr->minVel, sizeof(Vec4), readPos, buffer.size());
							ResourcesLoader::ReadFromBuffer(buffer.data(), &ptr->maxVel, sizeof(Vec4), readPos, buffer.size());

						}
					}
					else
					{
						t->velColUpdater = nullptr;
					}
					isNull = 0;
					ResourcesLoader::ReadFromBuffer(buffer.data(), &isNull, sizeof(std::size_t), readPos, buffer.size());
					if(isNull == 128)
					{
						if(t->posColUpdater == nullptr)
							t->posColUpdater = std::make_shared<PosColorUpdater>();


						{
							auto ptr = t->posColUpdater;
							ResourcesLoader::ReadFromBuffer(buffer.data(), &ptr->minPos, sizeof(Vec4), readPos, buffer.size());
							ResourcesLoader::ReadFromBuffer(buffer.data(), &ptr->maxPos, sizeof(Vec4), readPos, buffer.size());

						}
					}
					else
					{
						t->posColUpdater = nullptr;
					}
					isNull = 0;
					ResourcesLoader::ReadFromBuffer(buffer.data(), &isNull, sizeof(std::size_t), readPos, buffer.size());
					if(isNull == 128)
					{
						if(t->colorUpdater == nullptr)
							t->colorUpdater = std::make_shared<BasicColorUpdater>();

						{
							auto ptr = t->colorUpdater;

						}
					}
					else
					{
						t->colorUpdater = nullptr;
					}
					isNull = 0;
					ResourcesLoader::ReadFromBuffer(buffer.data(), &isNull, sizeof(std::size_t), readPos, buffer.size());
					if(isNull == 128)
					{
						if(t->eulerUpdater == nullptr)
							t->eulerUpdater = std::make_shared<EulerUpdater>();

						{
							auto ptr = t->eulerUpdater;
							ResourcesLoader::ReadFromBuffer(buffer.data(), &ptr->globalAcceleration, sizeof(Vec4), readPos, buffer.size());

						}
					}
					else
					{
						t->eulerUpdater = nullptr;
					}
					isNull = 0;
					ResourcesLoader::ReadFromBuffer(buffer.data(), &isNull, sizeof(std::size_t), readPos, buffer.size());
					if(isNull == 128)
					{
						if(t->floorUpdater == nullptr)
							t->floorUpdater = std::make_shared<FloorUpdater>();

						{
							auto ptr = t->floorUpdater;
							ResourcesLoader::ReadFromBuffer(buffer.data(), &ptr->bounceFactor, sizeof(float), readPos, buffer.size());
							ResourcesLoader::ReadFromBuffer(buffer.data(), &ptr->floorY, sizeof(float), readPos, buffer.size());


						}
					}
					else
					{
						t->floorUpdater = nullptr;
					}
					isNull = 0;
					ResourcesLoader::ReadFromBuffer(buffer.data(), &isNull, sizeof(std::size_t), readPos, buffer.size());
					if(isNull == 128)
					{
						if(t->timeUpdater == nullptr)
							t->timeUpdater = std::make_shared<BasicTimeUpdater>();

					}
					else
					{
						t->timeUpdater = nullptr;
					}
					t->UpdateSystem();
					t->UpdateEmitter();
					delete cmp;
				}
				else if (ns != nullptr && myClass->isSubclassOf(*ns->getClass("Script")))
				{

					if(!ecsManager.GotComponent<ScriptList>(go->GetEntity()))
					{
						ecsManager.AddComponent(go, ScriptList());
					}
					Script* s = ecsManager.GetComponent<ScriptList>(go->GetEntity()).AddScript((Script*)cmp);
					for (int i = 0; i < FieldNum; ++i)
					{
						short isNull = -1;

						//Get Comp FieldName
						ResourcesLoader::ReadFromBuffer(buffer.data(), &cmpNameSize, sizeof(std::size_t),
						                                readPos, buffer.size());
						std::string Name;
						Name.resize(cmpNameSize / sizeof(std::string::value_type));
						ResourcesLoader::ReadFromBuffer(buffer.data(), Name.data(), cmpNameSize, readPos, buffer.size());

						//Get is Null
						ResourcesLoader::ReadFromBuffer(buffer.data(), &isNull, sizeof(short), readPos, buffer.size());
						std::size_t memSize = 0;
						//Get Field Data

						const rfk::Field *f = s->getArchetype().getField(Name, rfk::EFieldFlags::Default,
						                                                 true);

						std::size_t fieldSize = 0;
						ResourcesLoader::ReadFromBuffer(buffer.data(), &fieldSize, sizeof(std::size_t),
						                                readPos, buffer.size());

						if(f == nullptr)
						{
							readPos += fieldSize;
							continue;
						}

						if (f->type.archetype->name == "String")
						{
							String *str = (String *) f->getDataAddress(s);
							str->resize(fieldSize / sizeof(std::string::value_type));

							ResourcesLoader::ReadFromBuffer(buffer.data(), str->data(), fieldSize, readPos, buffer.size());
						}
						else if (f->type.archetype->name == "vectorStr")
						{
							std::size_t vecSize = 0;
							vectorStr *vstr = (vectorStr *) f->getDataAddress(s);
							ResourcesLoader::ReadFromBuffer(buffer.data(), &vecSize, sizeof(std::size_t),
							                                readPos, buffer.size());
							for (int k = 0; k < vecSize; ++k)
							{
								String str;
								ResourcesLoader::ReadFromBuffer(buffer.data(), &memSize, sizeof(std::size_t),
								                                readPos, buffer.size());
								str.resize(memSize);
								ResourcesLoader::ReadFromBuffer(buffer.data(), str.data(), memSize, readPos, buffer.size());

								vstr->push_back(std::move(str));
							}
						}
						else
						{
							if (isNull == 256)
							{}
							else
							{}
							char *buf = new char[fieldSize]();
							ResourcesLoader::ReadFromBuffer(buffer.data(), buf, fieldSize, readPos, buffer.size());

							f->setData(s, ((void *) buf), fieldSize);
							delete[] buf;

						}





					}
				}
			}
			else
			{
				readPos+= SkipSize;
			}
		}
	}
	//get num of Childs
	std::size_t childNum = 0;
	ResourcesLoader::ReadFromBuffer(buffer.data(), &childNum, sizeof(std::size_t), readPos, buffer.size());

	for (int games = 0; games < childNum; ++games)
	{
		GameObject* go = elt->childs.at(games);


		AddAllComps(go, buffer, readPos);
	}
}


Solid::GameObject *
Solid::SceneGraphManager::Instantiate(std::string _prefabName, Solid::GameObject *parent, std::string _name)
{
	PrefabResource* prefab =Engine::GetInstance()->resourceManager.GetPrefabByName(_prefabName.c_str());
	if(prefab == nullptr)
		return nullptr;
	std::uint64_t readPos = 0;



	std::vector<char>  buffer = prefab->PrefabBinary;
	json j;

	std::size_t jsonSize = 0;
	ResourcesLoader::ReadFromBuffer(buffer.data(), &jsonSize, sizeof(std::size_t), readPos, buffer.size());
	std::string jsonStr;
	jsonStr.resize(jsonSize / sizeof(std::string::value_type));
	ResourcesLoader::ReadFromBuffer(buffer.data(), jsonStr.data(), jsonSize, readPos, buffer.size());
	j = j.parse(jsonStr) ;

	Engine* engine = Engine::GetInstance();



	std::function<void(json&,Entity)> Lambda = [&, engine](json& j,Entity e){
		for(auto it = j.begin(); it != j.end(); ++it)
		{
			std::string key = (it).key();
			if(key.find("GameObject") != std::string::npos)
			{
				GameObject* enti = engine->ecsManager.CreateEntity( it.value()["Name"], e);
				if(it.value()["TAG"].is_string())
					enti->tag = it.value()["TAG"];
				Lambda(std::ref(it.value()), enti->GetEntity());
			}
		}
	};
	GameObject* ent = nullptr;

	for(auto it = j["Scene"].begin(); it != j["Scene"].end(); ++it)
	{
		std::string key = (it).key();
		if(key.find("GameObject") != std::string::npos)
		{
			std::string name = it.value()["Name"];
			if(parent == nullptr)
			{
				ent = engine->ecsManager.CreateEntity(name);
			}
			else
			{
				ent = engine->ecsManager.CreateEntity(name, parent->GetEntity());
			}
			if(it.value()["TAG"].is_string())
				ent->tag = it.value()["TAG"];
			Lambda(std::ref(it.value()), ent->GetEntity());
		}
	}

	AddAllComps(ent, buffer, readPos);
	if(engine->IsPlaying())
	{
		engine->InitScript();
	}
	return ent;
}

