#pragma once

/**
*	Source file: D:\Project\SolidEngine\SolidEngine\Include\ECS\Components\rigidBody.hpp
*/

#include "EntityMacros.h"

#include <Refureku/Misc/DisableWarningMacros.h>
#include <Refureku/TypeInfo/Namespaces/Namespace.h>
#include <Refureku/TypeInfo/Namespaces/NamespaceFragment.h>
#include <Refureku/TypeInfo/Namespaces/NamespaceFragmentRegisterer.h>
#include <Refureku/TypeInfo/Archetypes/Class.h>
#include <Refureku/TypeInfo/Archetypes/Enum.h>
#include <Refureku/TypeInfo/Archetypes/ArchetypeRegisterer.h>
#include <Refureku/TypeInfo/Entity/DefaultEntityRegisterer.h>


#ifdef __RFK3698706698196451833u_DeclareGetNamespaceFragment
	#undef __RFK3698706698196451833u_DeclareGetNamespaceFragment
#endif
#define __RFK3698706698196451833u_DeclareGetNamespaceFragment inline rfk::NamespaceFragment const& getNamespaceFragment3698706698196451833u_16729104821330039792() noexcept;

#ifdef __RFK3698706698196451833u_DefineGetNamespaceFragment
	#undef __RFK3698706698196451833u_DefineGetNamespaceFragment
#endif
#define __RFK3698706698196451833u_DefineGetNamespaceFragment	\
	inline rfk::NamespaceFragment const& getNamespaceFragment3698706698196451833u_16729104821330039792() noexcept	\
	{	\
		static rfk::NamespaceFragment	fragment("Solid", 3698706698196451833u);	\
		static bool						initialized = false;	\
		if (!initialized)	\
		{	\
			initialized = true;	\
			fragment.nestedEntities.reserve(1u); rfk::NamespaceFragment* fragmentPtr = &fragment; fragmentPtr->addNestedEntity(&Solid::RigidBody::staticGetArchetype());	\
		}	\
	return fragment;	\
	}

#ifdef __RFK3698706698196451833u_RegisterNamespace
	#undef __RFK3698706698196451833u_RegisterNamespace
#endif
#define __RFK3698706698196451833u_RegisterNamespace	\
	inline rfk::NamespaceFragmentRegisterer namespaceFragmentRegisterer3698706698196451833u_16729104821330039792 = rfk::NamespaceFragmentRegisterer("Solid", 3698706698196451833u, &getNamespaceFragment3698706698196451833u_16729104821330039792(), true);

#ifdef __RFK3698706698196451833u_GENERATED
	#undef __RFK3698706698196451833u_GENERATED
#endif
#ifdef KODGEN_PARSING
#define __RFK3698706698196451833u_GENERATED
#else
#define __RFK3698706698196451833u_GENERATED	\
	namespace rfk::generated {	\
	__RFK3698706698196451833u_DeclareGetNamespaceFragment	\
	__RFK3698706698196451833u_RegisterNamespace	\
	__RFK3698706698196451833u_DefineGetNamespaceFragment	\
	}

#endif

#define __RFK9721541611162158115u_GenerateParentsMetadata	\
	type.directParents.reserve(1);	\
	type.addToParents<Solid::Components>(static_cast<rfk::EAccessSpecifier>(1));	\

#define __RFK9721541611162158115u_GenerateFieldsMetadata	\
	registerChild<RigidBody>(&type);	\

#define __RFK9721541611162158115u_GenerateFieldHelperMethods	\
private:	\
	template <typename ParentType, typename ChildType>	\
	static constexpr void recurseRegisterChild([[maybe_unused]] rfk::Struct* childArchetype)	\
	{	\
		if constexpr (rfk::isReflectedClass<ParentType>)	\
		{	\
			ParentType::template registerChild<ChildType>(childArchetype);	\
		}	\
	}	\
public:	\
	template <typename ChildType>	\
	static void registerChild(rfk::Struct* childArchetype) noexcept	\
	{	\
		recurseRegisterChild<Solid::Components, ChildType>(childArchetype);	\
		rfk::Struct const& thisArchetype = staticGetArchetype();	\
		if (childArchetype != &thisArchetype)	\
		{	\
			const_cast<rfk::Struct&>(thisArchetype).children.insert(childArchetype);	\
		}	\
		[[maybe_unused]] rfk::Field*		field		= nullptr; 	\
		[[maybe_unused]] rfk::StaticField*	staticField = nullptr; 	\
		__RFK_DISABLE_WARNING_PUSH	\
		__RFK_DISABLE_WARNING_OFFSETOF	\
		field = childArchetype->addField("gravity", 15862074580564247938u, rfk::Type::getType<bool>(), static_cast<rfk::EFieldFlags>(4), &thisArchetype, offsetof(ChildType, gravity));	\
		field = childArchetype->addField("isKinematic", 2476124050480900145u, rfk::Type::getType<bool>(), static_cast<rfk::EFieldFlags>(4), &thisArchetype, offsetof(ChildType, isKinematic));	\
		field = childArchetype->addField("mass", 4790624427714371020u, rfk::Type::getType<float>(), static_cast<rfk::EFieldFlags>(4), &thisArchetype, offsetof(ChildType, mass));	\
		field = childArchetype->addField("drag", 11318751206634570742u, rfk::Type::getType<float>(), static_cast<rfk::EFieldFlags>(4), &thisArchetype, offsetof(ChildType, drag));	\
		field = childArchetype->addField("angularDrag", 15619724461819505796u, rfk::Type::getType<float>(), static_cast<rfk::EFieldFlags>(4), &thisArchetype, offsetof(ChildType, angularDrag));	\
		field = childArchetype->addField("freezePosX", 15257040038834533861u, rfk::Type::getType<bool>(), static_cast<rfk::EFieldFlags>(4), &thisArchetype, offsetof(ChildType, freezePosX));	\
		field = childArchetype->addField("freezePosY", 15257038939322905650u, rfk::Type::getType<bool>(), static_cast<rfk::EFieldFlags>(4), &thisArchetype, offsetof(ChildType, freezePosY));	\
		field = childArchetype->addField("freezePosZ", 15257037839811277439u, rfk::Type::getType<bool>(), static_cast<rfk::EFieldFlags>(4), &thisArchetype, offsetof(ChildType, freezePosZ));	\
		field = childArchetype->addField("freezeRotX", 15181879547109669988u, rfk::Type::getType<bool>(), static_cast<rfk::EFieldFlags>(4), &thisArchetype, offsetof(ChildType, freezeRotX));	\
		field = childArchetype->addField("freezeRotY", 15181880646621298199u, rfk::Type::getType<bool>(), static_cast<rfk::EFieldFlags>(4), &thisArchetype, offsetof(ChildType, freezeRotY));	\
		field = childArchetype->addField("freezeRotZ", 15181881746132926410u, rfk::Type::getType<bool>(), static_cast<rfk::EFieldFlags>(4), &thisArchetype, offsetof(ChildType, freezeRotZ));	\
		__RFK_DISABLE_WARNING_POP	\
	}

#define __RFK9721541611162158115u_GenerateMethodsMetadata	\

#define __RFK9721541611162158115u_GenerateArchetypeProperties	\
	

#define __RFK9721541611162158115u_GenerateDefaultInstantiatorSetup	\
	type.setDefaultInstantiator(&rfk::defaultInstantiator<RigidBody>);

#define __RFK9721541611162158115u_GetTypeDeclaration	\
	__RFK9721541611162158115u_GenerateFieldHelperMethods	\
	public:	\
		inline static rfk::Class const& staticGetArchetype() noexcept;	\
		rfk::Class const& getArchetype() const noexcept override { return RigidBody::staticGetArchetype(); }

#ifdef KODGEN_PARSING
#define __RFK9721541611162158115u_GetTypeDefinition
#else
#define __RFK9721541611162158115u_GetTypeDefinition	\
		inline rfk::Class const& Solid::RigidBody::staticGetArchetype() noexcept	\
		{	\
			static bool			initialized = false;	\
			static rfk::Class	type("RigidBody", 9721541611162158115u, sizeof(RigidBody));	\
			\
			if (!initialized)	\
			{	\
				initialized = true;	\
			\
				__RFK9721541611162158115u_GenerateArchetypeProperties	\
				__RFK9721541611162158115u_GenerateParentsMetadata	\
					\
				__RFK9721541611162158115u_GenerateFieldsMetadata	\
				__RFK9721541611162158115u_GenerateDefaultInstantiatorSetup	\
				__RFK9721541611162158115u_GenerateMethodsMetadata	\
			}	\
			\
			return type;	\
		}

#endif
#define __RFK9721541611162158115u_RegisterArchetype	\
	private:	\
		static inline rfk::ArchetypeRegisterer __rfkArchetypeRegisterer;


#ifdef KODGEN_PARSING
#define RigidBody_GENERATED
#else
#define RigidBody_GENERATED	\
	friend rfk::Struct;	\
	friend rfk::hasField___rfkArchetypeRegisterer<RigidBody, rfk::ArchetypeRegisterer>;	\
	__RFK9721541611162158115u_GetTypeDeclaration	\
	__RFK9721541611162158115u_RegisterArchetype	\
		\
	private:

#endif

#ifdef __RFKNativeProperties_GENERATED
	#undef __RFKNativeProperties_GENERATED
#endif

#define __RFKNativeProperties_GENERATED	\
	

#ifdef File_GENERATED
	#undef File_GENERATED
#endif
#define File_GENERATED	\
	__RFK9721541611162158115u_GetTypeDefinition	\
	__RFK3698706698196451833u_GENERATED	\
	__RFKNativeProperties_GENERATED	\


