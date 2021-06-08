#pragma once

/**
*	Source file: D:\Project\SolidEngine\SolidEngine\Include\ECS\Components\sphereCollider.hpp
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
#define __RFK3698706698196451833u_DeclareGetNamespaceFragment inline rfk::NamespaceFragment const& getNamespaceFragment3698706698196451833u_13374959943706615964() noexcept;

#ifdef __RFK3698706698196451833u_DefineGetNamespaceFragment
	#undef __RFK3698706698196451833u_DefineGetNamespaceFragment
#endif
#define __RFK3698706698196451833u_DefineGetNamespaceFragment	\
	inline rfk::NamespaceFragment const& getNamespaceFragment3698706698196451833u_13374959943706615964() noexcept	\
	{	\
		static rfk::NamespaceFragment	fragment("Solid", 3698706698196451833u);	\
		static bool						initialized = false;	\
		if (!initialized)	\
		{	\
			initialized = true;	\
			fragment.nestedEntities.reserve(1u); rfk::NamespaceFragment* fragmentPtr = &fragment; fragmentPtr->addNestedEntity(&Solid::SphereCollider::staticGetArchetype());	\
		}	\
	return fragment;	\
	}

#ifdef __RFK3698706698196451833u_RegisterNamespace
	#undef __RFK3698706698196451833u_RegisterNamespace
#endif
#define __RFK3698706698196451833u_RegisterNamespace	\
	inline rfk::NamespaceFragmentRegisterer namespaceFragmentRegisterer3698706698196451833u_13374959943706615964 = rfk::NamespaceFragmentRegisterer("Solid", 3698706698196451833u, &getNamespaceFragment3698706698196451833u_13374959943706615964(), true);

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

#define __RFK16300895958427056117u_GenerateParentsMetadata	\
	type.directParents.reserve(1);	\
	type.addToParents<Solid::Components>(static_cast<rfk::EAccessSpecifier>(1));	\

#define __RFK16300895958427056117u_GenerateFieldsMetadata	\
	registerChild<SphereCollider>(&type);	\

#define __RFK16300895958427056117u_GenerateFieldHelperMethods	\
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
		field = childArchetype->addField("isTrigger", 10831053466185493406u, rfk::Type::getType<bool>(), static_cast<rfk::EFieldFlags>(4), &thisArchetype, offsetof(ChildType, isTrigger));	\
		field = childArchetype->addField("center", 11325819432530636791u, rfk::Type::getType<Solid::Vec3>(), static_cast<rfk::EFieldFlags>(4), &thisArchetype, offsetof(ChildType, center));	\
		field = childArchetype->addField("radius", 12818803810150764352u, rfk::Type::getType<float>(), static_cast<rfk::EFieldFlags>(4), &thisArchetype, offsetof(ChildType, radius));	\
		field = childArchetype->addField("staticFriction", 16256472132574340642u, rfk::Type::getType<float>(), static_cast<rfk::EFieldFlags>(4), &thisArchetype, offsetof(ChildType, staticFriction));	\
		field = childArchetype->addField("dynamicFriction", 4283536980477759457u, rfk::Type::getType<float>(), static_cast<rfk::EFieldFlags>(4), &thisArchetype, offsetof(ChildType, dynamicFriction));	\
		field = childArchetype->addField("restitution", 14154565729214963474u, rfk::Type::getType<float>(), static_cast<rfk::EFieldFlags>(4), &thisArchetype, offsetof(ChildType, restitution));	\
		__RFK_DISABLE_WARNING_POP	\
	}

#define __RFK16300895958427056117u_GenerateMethodsMetadata	\

#define __RFK16300895958427056117u_GenerateArchetypeProperties	\
	

#define __RFK16300895958427056117u_GenerateDefaultInstantiatorSetup	\
	type.setDefaultInstantiator(&rfk::defaultInstantiator<SphereCollider>);

#define __RFK16300895958427056117u_GetTypeDeclaration	\
	__RFK16300895958427056117u_GenerateFieldHelperMethods	\
	public:	\
		inline static rfk::Class const& staticGetArchetype() noexcept;	\
		rfk::Class const& getArchetype() const noexcept override { return SphereCollider::staticGetArchetype(); }

#ifdef KODGEN_PARSING
#define __RFK16300895958427056117u_GetTypeDefinition
#else
#define __RFK16300895958427056117u_GetTypeDefinition	\
		inline rfk::Class const& Solid::SphereCollider::staticGetArchetype() noexcept	\
		{	\
			static bool			initialized = false;	\
			static rfk::Class	type("SphereCollider", 16300895958427056117u, sizeof(SphereCollider));	\
			\
			if (!initialized)	\
			{	\
				initialized = true;	\
			\
				__RFK16300895958427056117u_GenerateArchetypeProperties	\
				__RFK16300895958427056117u_GenerateParentsMetadata	\
					\
				__RFK16300895958427056117u_GenerateFieldsMetadata	\
				__RFK16300895958427056117u_GenerateDefaultInstantiatorSetup	\
				__RFK16300895958427056117u_GenerateMethodsMetadata	\
			}	\
			\
			return type;	\
		}

#endif
#define __RFK16300895958427056117u_RegisterArchetype	\
	private:	\
		static inline rfk::ArchetypeRegisterer __rfkArchetypeRegisterer;


#ifdef KODGEN_PARSING
#define SphereCollider_GENERATED
#else
#define SphereCollider_GENERATED	\
	friend rfk::Struct;	\
	friend rfk::hasField___rfkArchetypeRegisterer<SphereCollider, rfk::ArchetypeRegisterer>;	\
	__RFK16300895958427056117u_GetTypeDeclaration	\
	__RFK16300895958427056117u_RegisterArchetype	\
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
	__RFK16300895958427056117u_GetTypeDefinition	\
	__RFK3698706698196451833u_GENERATED	\
	__RFKNativeProperties_GENERATED	\


