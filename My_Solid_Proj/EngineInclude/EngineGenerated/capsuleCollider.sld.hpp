#pragma once

/**
*	Source file: D:\Project\SolidEngine\SolidEngine\Include\ECS\Components\capsuleCollider.hpp
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
#define __RFK3698706698196451833u_DeclareGetNamespaceFragment inline rfk::NamespaceFragment const& getNamespaceFragment3698706698196451833u_15499281643140669044() noexcept;

#ifdef __RFK3698706698196451833u_DefineGetNamespaceFragment
	#undef __RFK3698706698196451833u_DefineGetNamespaceFragment
#endif
#define __RFK3698706698196451833u_DefineGetNamespaceFragment	\
	inline rfk::NamespaceFragment const& getNamespaceFragment3698706698196451833u_15499281643140669044() noexcept	\
	{	\
		static rfk::NamespaceFragment	fragment("Solid", 3698706698196451833u);	\
		static bool						initialized = false;	\
		if (!initialized)	\
		{	\
			initialized = true;	\
			fragment.nestedEntities.reserve(1u); rfk::NamespaceFragment* fragmentPtr = &fragment; fragmentPtr->addNestedEntity(&Solid::CapsuleCollider::staticGetArchetype());	\
		}	\
	return fragment;	\
	}

#ifdef __RFK3698706698196451833u_RegisterNamespace
	#undef __RFK3698706698196451833u_RegisterNamespace
#endif
#define __RFK3698706698196451833u_RegisterNamespace	\
	inline rfk::NamespaceFragmentRegisterer namespaceFragmentRegisterer3698706698196451833u_15499281643140669044 = rfk::NamespaceFragmentRegisterer("Solid", 3698706698196451833u, &getNamespaceFragment3698706698196451833u_15499281643140669044(), true);

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

#define __RFK9281368411752454939u_GenerateParentsMetadata	\
	type.directParents.reserve(1);	\
	type.addToParents<Solid::Components>(static_cast<rfk::EAccessSpecifier>(1));	\

#define __RFK9281368411752454939u_GenerateFieldsMetadata	\
	registerChild<CapsuleCollider>(&type);	\

#define __RFK9281368411752454939u_GenerateFieldHelperMethods	\
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
		field = childArchetype->addField("isTrigger", 529915094729379688u, rfk::Type::getType<bool>(), static_cast<rfk::EFieldFlags>(4), &thisArchetype, offsetof(ChildType, isTrigger));	\
		field = childArchetype->addField("center", 12678134364613282533u, rfk::Type::getType<Solid::Vec3>(), static_cast<rfk::EFieldFlags>(4), &thisArchetype, offsetof(ChildType, center));	\
		field = childArchetype->addField("radius", 4921818787176289874u, rfk::Type::getType<float>(), static_cast<rfk::EFieldFlags>(4), &thisArchetype, offsetof(ChildType, radius));	\
		field = childArchetype->addField("height", 3086876312097772847u, rfk::Type::getType<float>(), static_cast<rfk::EFieldFlags>(4), &thisArchetype, offsetof(ChildType, height));	\
		field = childArchetype->addField("staticFriction", 11794392754262846096u, rfk::Type::getType<float>(), static_cast<rfk::EFieldFlags>(4), &thisArchetype, offsetof(ChildType, staticFriction));	\
		field = childArchetype->addField("dynamicFriction", 4933846471799886323u, rfk::Type::getType<float>(), static_cast<rfk::EFieldFlags>(4), &thisArchetype, offsetof(ChildType, dynamicFriction));	\
		field = childArchetype->addField("restitution", 10207598569352052824u, rfk::Type::getType<float>(), static_cast<rfk::EFieldFlags>(4), &thisArchetype, offsetof(ChildType, restitution));	\
		field = childArchetype->addField("direction", 13835668128423135133u, rfk::Type::getType<Solid::CapsuleCollider::ECapsuleDirection>(), static_cast<rfk::EFieldFlags>(4), &thisArchetype, offsetof(ChildType, direction));	\
		__RFK_DISABLE_WARNING_POP	\
	}

#define __RFK9281368411752454939u_GenerateMethodsMetadata	\

#define __RFK9281368411752454939u_GenerateArchetypeProperties	\
	

#define __RFK9281368411752454939u_GenerateNestedArchetypesMetadata	\
	rfk::Archetype* archetype = nullptr;	\
	type.nestedArchetypes.reserve(1);	\
	archetype = type.addNestedArchetype(rfk::getEnum<Solid::CapsuleCollider::ECapsuleDirection>(), static_cast<rfk::EAccessSpecifier>(1));	\

#define __RFK9281368411752454939u_GenerateDefaultInstantiatorSetup	\
	type.setDefaultInstantiator(&rfk::defaultInstantiator<CapsuleCollider>);

#define __RFK9281368411752454939u_GetTypeDeclaration	\
	__RFK9281368411752454939u_GenerateFieldHelperMethods	\
	public:	\
		inline static rfk::Class const& staticGetArchetype() noexcept;	\
		rfk::Class const& getArchetype() const noexcept override { return CapsuleCollider::staticGetArchetype(); }

#ifdef KODGEN_PARSING
#define __RFK9281368411752454939u_GetTypeDefinition
#else
#define __RFK9281368411752454939u_GetTypeDefinition	\
		inline rfk::Class const& Solid::CapsuleCollider::staticGetArchetype() noexcept	\
		{	\
			static bool			initialized = false;	\
			static rfk::Class	type("CapsuleCollider", 9281368411752454939u, sizeof(CapsuleCollider));	\
			\
			if (!initialized)	\
			{	\
				initialized = true;	\
			\
				__RFK9281368411752454939u_GenerateArchetypeProperties	\
				__RFK9281368411752454939u_GenerateParentsMetadata	\
				__RFK9281368411752454939u_GenerateNestedArchetypesMetadata	\
				__RFK9281368411752454939u_GenerateFieldsMetadata	\
				__RFK9281368411752454939u_GenerateDefaultInstantiatorSetup	\
				__RFK9281368411752454939u_GenerateMethodsMetadata	\
			}	\
			\
			return type;	\
		}

#endif
#define __RFK9281368411752454939u_RegisterArchetype	\
	private:	\
		static inline rfk::ArchetypeRegisterer __rfkArchetypeRegisterer;


#ifdef KODGEN_PARSING
#define CapsuleCollider_GENERATED
#else
#define CapsuleCollider_GENERATED	\
	friend rfk::Struct;	\
	friend rfk::hasField___rfkArchetypeRegisterer<CapsuleCollider, rfk::ArchetypeRegisterer>;	\
	__RFK9281368411752454939u_GetTypeDeclaration	\
	__RFK9281368411752454939u_RegisterArchetype	\
		\
	private:

#endif

#define __RFK11608466684729305483u_GenerateGetEnumSpecialization	\
	template <>	\
	inline rfk::Enum const* getEnum<Solid::CapsuleCollider::ECapsuleDirection>() noexcept	\
	{	\
		static bool			initialized = false;	\
		static rfk::Enum	type("ECapsuleDirection", 11608466684729305483u, sizeof(Solid::CapsuleCollider::ECapsuleDirection), rfk::Type::getType<int>());	\
		if (!initialized)	\
		{	\
			initialized = true;	\
			rfk::EnumValue*	enumValue = nullptr;	\
			type.values.reserve(3);	\
			enumValue = type.addEnumValue("X_AXIS", 15241905274787335885u, 0);	\
			enumValue = type.addEnumValue("Y_AXIS", 4116657394184815444u, 1);	\
			enumValue = type.addEnumValue("Z_AXIS", 10107870302772330863u, 2);	\
		}	\
		return &type;	\
	}

#ifdef KODGEN_PARSING
#define __RFK11608466684729305483u_GENERATED
#else
#define __RFK11608466684729305483u_GENERATED	\
	namespace rfk {	\
	__RFK11608466684729305483u_GenerateGetEnumSpecialization	\
		\
	}

#endif

#ifdef __RFKNativeProperties_GENERATED
	#undef __RFKNativeProperties_GENERATED
#endif

#define __RFKNativeProperties_GENERATED	\
	

#ifdef File_GENERATED
	#undef File_GENERATED
#endif
#define File_GENERATED	\
	__RFK11608466684729305483u_GENERATED	\
	__RFK9281368411752454939u_GetTypeDefinition	\
	__RFK3698706698196451833u_GENERATED	\
	__RFKNativeProperties_GENERATED	\


