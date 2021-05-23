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
				Engine::GetInstance()->ecsManager.DestroyEntity(entity,FromSceneGraphMgr{});
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

Solid::GameObject *
Solid::SceneGraphManager::Instantiate(std::string _prefabName, Solid::GameObject *parent, std::string _name)
{
	PrefabResource* prefab =Engine::GetInstance()->resourceManager.GetPrefabByName(_prefabName.c_str());
	if(prefab == nullptr)
		return nullptr;
	rfk::Namespace const* n = rfk::Database::getNamespace("Solid");
	std::uint64_t readPos = 0;
	std::size_t cmpNameSize = 0;


	std::vector<char>  buffer = prefab->PrefabBinary;
	json j;

	std::size_t jsonSize = 0;
	ResourcesLoader::ReadFromBuffer(buffer.data(), &jsonSize, sizeof(std::size_t),readPos);
	std::string jsonStr;
	jsonStr.resize(jsonSize / sizeof(std::string::value_type));
	ResourcesLoader::ReadFromBuffer(buffer.data(), jsonStr.data(), jsonSize,readPos);
	j = j.parse(jsonStr) ;

	Engine* engine = Engine::GetInstance();
	ECSManager& ecsManager = engine->ecsManager;


	std::function<void(json&,Entity)> Lambda = [&, engine](json& j,Entity e){
		for(auto it = j.begin(); it != j.end(); ++it)
		{
			std::string key = (it).key();
			if(key.find("GameObject") != std::string::npos)
			{
				GameObject* enti = engine->ecsManager.CreateEntity( it.value()["Name"], e);
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
				ent = engine->ecsManager.CreateEntity(name);
			else
				ent = engine->ecsManager.CreateEntity(name, parent->GetEntity());
			Lambda(std::ref(it.value()), ent->GetEntity());
		}
	}
	GameCompiler* Compiler = Engine::GetInstance()->Compiler;

	std::function<void(GameObject*)> AddAllComps = [&](GameObject* elt)
	{
		//add comps to elt
		{
			GameObject* go = elt;
			//get num of comps
			std::size_t cmpNum = 0;
			ResourcesLoader::ReadFromBuffer(buffer.data(), &cmpNum, sizeof(std::size_t), readPos);


			for (int j = 0; j < cmpNum; ++j)
			{
				//get Comp Class / str
				ResourcesLoader::ReadFromBuffer(buffer.data(), &cmpNameSize, sizeof(std::size_t), readPos);
				std::string className;
				className.resize(cmpNameSize / sizeof(std::string::value_type));
				ResourcesLoader::ReadFromBuffer(buffer.data(), className.data(), cmpNameSize, readPos);

				//get Field Num
				std::size_t FieldNum = 0;
				ResourcesLoader::ReadFromBuffer(buffer.data(), &FieldNum, sizeof(std::size_t), readPos);


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
						Transform *t = Engine::GetInstance()->ecsManager.AddComponent(go, *(Transform *) cmp);
						for (int i = 0; i < FieldNum; ++i)
						{
							short isNull = -1;

							//Get Comp FieldName
							ResourcesLoader::ReadFromBuffer(buffer.data(), &cmpNameSize, sizeof(std::size_t),
							                                readPos);
							std::string Name;
							Name.resize(cmpNameSize / sizeof(std::string::value_type));
							ResourcesLoader::ReadFromBuffer(buffer.data(), Name.data(), cmpNameSize, readPos);

							//Get is Null
							ResourcesLoader::ReadFromBuffer(buffer.data(), &isNull, sizeof(short), readPos);
							std::size_t memSize = 0;
							//Get Field Data
							ResourcesLoader::ReadFromBuffer(buffer.data(), &memSize, sizeof(std::size_t), readPos);
							if (isNull == 256)
							{}
							else
							{}
							char *buf = new char[memSize]();
							ResourcesLoader::ReadFromBuffer(buffer.data(), buf, memSize, readPos);
							const rfk::Field *f = t->getArchetype().getField(Name);
							f->setData(t, ((void *) buf), memSize);
							delete[] buf;

							if (Name == "rotation")
							{
								t->SetRotation(t->GetRotation());
							}
						}

					}
					else if (className == "AudioSource")
					{
						AudioSource *t = Engine::GetInstance()->ecsManager.AddComponent(go, *(AudioSource *) cmp);

						//Get Comp FieldName
						for (int i = 0; i < FieldNum; ++i)
						{
							short isNull = -1;

							//Get Comp FieldName
							ResourcesLoader::ReadFromBuffer(buffer.data(), &cmpNameSize, sizeof(std::size_t),
							                                readPos);
							std::string Name;
							Name.resize(cmpNameSize / sizeof(std::string::value_type));
							ResourcesLoader::ReadFromBuffer(buffer.data(), Name.data(), cmpNameSize, readPos);

							//Get IsNull
							ResourcesLoader::ReadFromBuffer(buffer.data(), &isNull, sizeof(short), readPos);
							std::size_t memSize = 0;
							//get Field Data
							const rfk::Field *f = t->getArchetype().getField(Name);
							if (f->type.archetype->name == "String")
							{
								ResourcesLoader::ReadFromBuffer(buffer.data(), &memSize, sizeof(std::size_t),
								                                readPos);
								String *str = (String *) f->getDataAddress(t);
								str->resize(memSize / sizeof(std::string::value_type));

								ResourcesLoader::ReadFromBuffer(buffer.data(), str->data(), memSize, readPos);
							}
							else
							{

								ResourcesLoader::ReadFromBuffer(buffer.data(), &memSize, sizeof(std::size_t),
								                                readPos);
								char *buf = new char[memSize]();
								ResourcesLoader::ReadFromBuffer(buffer.data(), buf, memSize, readPos);

								if (isNull == 256)
								{
									std::string s = std::string(buf, memSize);
									f->setData(t, s);
								}
								else
								{

									f->setData(t, ((void *) buf), memSize);
								}


								delete[] buf;
							}

						}
						t->Init();
					}
					else if (className == "MeshRenderer")
					{
						MeshRenderer *t = Engine::GetInstance()->ecsManager.AddComponent(go, *(MeshRenderer *) cmp);

						//Get Comp FieldName
						for (int i = 0; i < FieldNum; ++i)
						{
							short isNull = -1;

							//Get Comp FieldName
							ResourcesLoader::ReadFromBuffer(buffer.data(), &cmpNameSize, sizeof(std::size_t),
							                                readPos);
							std::string Name;
							Name.resize(cmpNameSize / sizeof(std::string::value_type));
							ResourcesLoader::ReadFromBuffer(buffer.data(), Name.data(), cmpNameSize, readPos);

							//Get IsNull
							ResourcesLoader::ReadFromBuffer(buffer.data(), &isNull, sizeof(short), readPos);
							std::size_t memSize = 0;
							//get Field Data
							const rfk::Field *f = t->getArchetype().getField(Name);
							if (f->type.archetype->name == "String")
							{
								ResourcesLoader::ReadFromBuffer(buffer.data(), &memSize, sizeof(std::size_t),
								                                readPos);
								String *str = (String *) f->getDataAddress(t);
								str->resize(memSize / sizeof(std::string::value_type));

								ResourcesLoader::ReadFromBuffer(buffer.data(), str->data(), memSize, readPos);
							}
							else if (f->type.archetype->name == "vectorStr")
							{
								std::size_t vecSize = 0;
								ResourcesLoader::ReadFromBuffer(buffer.data(), &vecSize, sizeof(std::size_t),
								                                readPos);
								vectorStr *vstr = (vectorStr *) f->getDataAddress(t);

								for (int k = 0; k < vecSize; ++k)
								{
									String str;
									ResourcesLoader::ReadFromBuffer(buffer.data(), &memSize, sizeof(std::size_t),
									                                readPos);
									str.resize(memSize);
									ResourcesLoader::ReadFromBuffer(buffer.data(), str.data(), memSize, readPos);

									vstr->push_back(std::move(str));
								}
							}
							else
							{

								ResourcesLoader::ReadFromBuffer(buffer.data(), &memSize, sizeof(std::size_t),
								                                readPos);
								char *buf = new char[memSize]();
								ResourcesLoader::ReadFromBuffer(buffer.data(), buf, memSize, readPos);

								if (isNull == 256)
								{
									std::string s = std::string(buf, memSize);
									f->setData(t, s);
								}
								else
								{

									f->setData(t, ((void *) buf), memSize);
								}


								delete[] buf;
							}

						}
						t->Init();
					}
					else if (className == "Light")
					{
						Light *t = Engine::GetInstance()->ecsManager.AddComponent(go, *(Light *) cmp);

						for (int i = 0; i < FieldNum; ++i)
						{
							short isNull = -1;

							//Get Comp FieldName
							ResourcesLoader::ReadFromBuffer(buffer.data(), &cmpNameSize, sizeof(std::size_t),
							                                readPos);
							std::string Name;
							Name.resize(cmpNameSize / sizeof(std::string::value_type));
							ResourcesLoader::ReadFromBuffer(buffer.data(), Name.data(), cmpNameSize, readPos);

							//Get is Null
							ResourcesLoader::ReadFromBuffer(buffer.data(), &isNull, sizeof(short), readPos);
							std::size_t memSize = 0;
							//Get Field Data
							ResourcesLoader::ReadFromBuffer(buffer.data(), &memSize, sizeof(std::size_t), readPos);
							if (isNull == 256)
							{}
							else
							{}
							char *buf = new char[memSize]();
							ResourcesLoader::ReadFromBuffer(buffer.data(), buf, memSize, readPos);
							const rfk::Field *f = t->getArchetype().getField(Name);
							f->setData(t, ((void *) buf), memSize);
							delete[] buf;

						}

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
							                                readPos);
							std::string Name;
							Name.resize(cmpNameSize / sizeof(std::string::value_type));
							ResourcesLoader::ReadFromBuffer(buffer.data(), Name.data(), cmpNameSize, readPos);

							//Get is Null
							ResourcesLoader::ReadFromBuffer(buffer.data(), &isNull, sizeof(short), readPos);
							std::size_t memSize = 0;
							//Get Field Data
							ResourcesLoader::ReadFromBuffer(buffer.data(), &memSize, sizeof(std::size_t), readPos);
							if (isNull == 256)
							{}
							else
							{}
							char *buf = new char[memSize]();
							ResourcesLoader::ReadFromBuffer(buffer.data(), buf, memSize, readPos);
							const rfk::Field *f = s->getArchetype().getField(Name);
							f->setData(s, ((void *) buf), memSize);
							delete[] buf;

						}
					}
				}

			}
		}
		//get num of Childs
		std::size_t childNum = 0;
		ResourcesLoader::ReadFromBuffer(buffer.data(), &childNum, sizeof(std::size_t), readPos);

		for (int games = 0; games < childNum; ++games)
		{
			GameObject* go = elt->childs.at(games);


			AddAllComps(go);
		}
	};
	AddAllComps(ent);
	return ent;
}
