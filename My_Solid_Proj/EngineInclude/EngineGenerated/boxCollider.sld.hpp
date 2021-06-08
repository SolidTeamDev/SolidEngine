#pragma once

/**
*	Source file: D:\Project\SolidEngine\SolidEngine\Include\ECS\Components\boxCollider.hpp
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
#define __RFK3698706698196451833u_DeclareGetNamespaceFragment inline rfk::NamespaceFragment const& getNamespaceFragment3698706698196451833u_17012662870255190266() noexcept;

#ifdef __RFK3698706698196451833u_DefineGetNamespaceFragment
	#undef __RFK3698706698196451833u_DefineGetNamespaceFragment
#endif
#define __RFK3698706698196451833u_DefineGetNamespaceFragment	\
	inline rfk::NamespaceFragment const& getNamespaceFragment3698706698196451833u_17012662870255190266() noexcept	\
	{	\
		static rfk::NamespaceFragment	fragment("Solid", 3698706698196451833u);	\
		static bool						initialized = false;	\
		if (!initialized)	\
		{	\
			initialized = true;	\
			fragment.nestedEntities.reserve(1u); rfk::NamespaceFragment* fragmentPtr = &fragment; fragmentPtr->addNestedEntity(&Solid::BoxCollider::staticGetArchetype());	\
		}	\
	return fragment;	\
	}

#ifdef __RFK3698706698196451833u_RegisterNamespace
	#undef __RFK3698706698196451833u_RegisterNamespace
#endif
#define __RFK3698706698196451833u_RegisterNamespace	\
	inline rfk::NamespaceFragmentRegisterer namespaceFragmentRegisterer3698706698196451833u_17012662870255190266 = rfk::NamespaceFragmentRegisterer("Solid", 3698706698196451833u, &getNamespaceFragment3698706698196451833u_17012662870255190266(), true);

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

#define __RFK17798290820717880881u_GenerateParentsMetadata	\
	type.directParents.reserve(1);	\
	type.addToParents<Solid::Components>(static_cast<rfk::EAccessSpecifier>(1));	\

#define __RFK17798290820717880881u_GenerateFieldsMetadata	\
	registerChild<BoxCollider>(&type);	\

#define __RFK17798290820717880881u_GenerateFieldHelperMethods	\
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
		field = childArchetype->addField("isTrigger", 13984266076773327818u, rfk::Type::getType<bool>(), static_cast<rfk::EFieldFlags>(4), &thisArchetype, offsetof(ChildType, isTrigger));	\
		field = childArchetype->addField("center", 13251233837928656875u, rfk::Type::getType<Solid::Vec3>(), static_cast<rfk::EFieldFlags>(4), &thisArchetype, offsetof(ChildType, center));	\
		field = childArchetype->addField("size", 14403981717762911271u, rfk::Type::getType<Solid::Vec3>(), static_cast<rfk::EFieldFlags>(4), &thisArchetype, offsetof(ChildType, size));	\
		field = childArchetype->addField("staticFriction", 3332567542279729814u, rfk::Type::getType<float>(), static_cast<rfk::EFieldFlags>(4), &thisArchetype, offsetof(ChildType, staticFriction));	\
		field = childArchetype->addField("dynamicFriction", 11295727859194916821u, rfk::Type::getType<float>(), static_cast<rfk::EFieldFlags>(4), &thisArchetype, offsetof(ChildType, dynamicFriction));	\
		field = childArchetype->addField("restitution", 15868875410726708342u, rfk::Type::getType<float>(), static_cast<rfk::EFieldFlags>(4), &thisArchetype, offsetof(ChildType, restitution));	\
		__RFK_DISABLE_WARNING_POP	\
	}

#define __RFK17798290820717880881u_GenerateMethodsMetadata	\

#define __RFK17798290820717880881u_GenerateArchetypeProperties	\
	

#define __RFK17798290820717880881u_GenerateDefaultInstantiatorSetup	\
	type.setDefaultInstantiator(&rfk::defaultInstantiator<BoxCollider>);

#define __RFK17798290820717880881u_GetTypeDeclaration	\
	__RFK17798290820717880881u_GenerateFieldHelperMethods	\
	public:	\
		inline static rfk::Class const& staticGetArchetype() noexcept;	\
		rfk::Class const& getArchetype() const noexcept override { return BoxCollider::staticGetArchetype(); }

#ifdef KODGEN_PARSING
#define __RFK17798290820717880881u_GetTypeDefinition
#else
#define __RFK17798290820717880881u_GetTypeDefinition	\
		inline rfk::Class const& Solid::BoxCollider::staticGetArchetype() noexcept	\
		{	\
			static bool			initialized = false;	\
			static rfk::Class	type("BoxCollider", 17798290820717880881u, sizeof(BoxCollider));	\
			\
			if (!initialized)	\
			{	\
				initialized = true;	\
			\
				__RFK17798290820717880881u_GenerateArchetypeProperties	\
				__RFK17798290820717880881u_GenerateParentsMetadata	\
					\
				__RFK17798290820717880881u_GenerateFieldsMetadata	\
				__RFK17798290820717880881u_GenerateDefaultInstantiatorSetup	\
				__RFK17798290820717880881u_GenerateMethodsMetadata	\
			}	\
			\
			return type;	\
		}

#endif
#define __RFK17798290820717880881u_RegisterArchetype	\
	private:	\
		static inline rfk::ArchetypeRegisterer __rfkArchetypeRegisterer;


#ifdef KODGEN_PARSING
#define BoxCollider_GENERATED
#else
#define BoxCollider_GENERATED	\
	friend rfk::Struct;	\
	friend rfk::hasField___rfkArchetypeRegisterer<BoxCollider, rfk::ArchetypeRegisterer>;	\
	__RFK17798290820717880881u_GetTypeDeclaration	\
	__RFK17798290820717880881u_RegisterArchetype	\
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
	__RFK17798290820717880881u_GetTypeDefinition	\
	__RFK3698706698196451833u_GENERATED	\
	__RFKNativeProperties_GENERATED	\


