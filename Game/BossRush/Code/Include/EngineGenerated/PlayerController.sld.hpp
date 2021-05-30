#pragma once

/**
*	Source file: C:\Users\gu.lefebvre\Documents\cpp\SolidEngine\Game\BossRush\Code\Include\PlayerController.hpp
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
#define __RFK3698706698196451833u_DeclareGetNamespaceFragment inline rfk::NamespaceFragment const& getNamespaceFragment3698706698196451833u_11296385836305945978() noexcept;

#ifdef __RFK3698706698196451833u_DefineGetNamespaceFragment
	#undef __RFK3698706698196451833u_DefineGetNamespaceFragment
#endif
#define __RFK3698706698196451833u_DefineGetNamespaceFragment	\
	inline rfk::NamespaceFragment const& getNamespaceFragment3698706698196451833u_11296385836305945978() noexcept	\
	{	\
		static rfk::NamespaceFragment	fragment("Solid", 3698706698196451833u);	\
		static bool						initialized = false;	\
		if (!initialized)	\
		{	\
			initialized = true;	\
			fragment.nestedEntities.reserve(1u); rfk::NamespaceFragment* fragmentPtr = &fragment; fragmentPtr->addNestedEntity(&Solid::PlayerController::staticGetArchetype());	\
		}	\
	return fragment;	\
	}

#ifdef __RFK3698706698196451833u_RegisterNamespace
	#undef __RFK3698706698196451833u_RegisterNamespace
#endif
#define __RFK3698706698196451833u_RegisterNamespace	\
	inline rfk::NamespaceFragmentRegisterer namespaceFragmentRegisterer3698706698196451833u_11296385836305945978 = rfk::NamespaceFragmentRegisterer("Solid", 3698706698196451833u, &getNamespaceFragment3698706698196451833u_11296385836305945978(), true);

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

#define __RFK7977671938205726473u_GenerateParentsMetadata	\
	type.directParents.reserve(1);	\
	type.addToParents<Solid::Script>(static_cast<rfk::EAccessSpecifier>(1));	\

#define __RFK7977671938205726473u_GenerateFieldsMetadata	\
	registerChild<PlayerController>(&type);	\

#define __RFK7977671938205726473u_GenerateFieldHelperMethods	\
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
		recurseRegisterChild<Solid::Script, ChildType>(childArchetype);	\
		rfk::Struct const& thisArchetype = staticGetArchetype();	\
		if (childArchetype != &thisArchetype)	\
		{	\
			const_cast<rfk::Struct&>(thisArchetype).children.insert(childArchetype);	\
		}	\
		[[maybe_unused]] rfk::Field*		field		= nullptr; 	\
		[[maybe_unused]] rfk::StaticField*	staticField = nullptr; 	\
		__RFK_DISABLE_WARNING_PUSH	\
		__RFK_DISABLE_WARNING_OFFSETOF	\
		field = childArchetype->addField("moveSpeed", 5692696800363440678u, rfk::Type::getType<float>(), static_cast<rfk::EFieldFlags>(1), &thisArchetype, offsetof(ChildType, moveSpeed));	\
		field = childArchetype->addField("jumpForce", 11909141086532344541u, rfk::Type::getType<float>(), static_cast<rfk::EFieldFlags>(1), &thisArchetype, offsetof(ChildType, jumpForce));	\
		field = childArchetype->addField("health", 1657424245421476716u, rfk::Type::getType<int>(), static_cast<rfk::EFieldFlags>(1), &thisArchetype, offsetof(ChildType, health));	\
		__RFK_DISABLE_WARNING_POP	\
	}

#define __RFK7977671938205726473u_GenerateMethodsMetadata	\

#define __RFK7977671938205726473u_GenerateArchetypeProperties	\
	

#define __RFK7977671938205726473u_GenerateDefaultInstantiatorSetup	\
	type.setDefaultInstantiator(&rfk::defaultInstantiator<PlayerController>);

#define __RFK7977671938205726473u_GetTypeDeclaration	\
	__RFK7977671938205726473u_GenerateFieldHelperMethods	\
	public:	\
		inline static rfk::Class const& staticGetArchetype() noexcept;	\
		rfk::Class const& getArchetype() const noexcept override { return PlayerController::staticGetArchetype(); }

#ifdef KODGEN_PARSING
#define __RFK7977671938205726473u_GetTypeDefinition
#else
#define __RFK7977671938205726473u_GetTypeDefinition	\
		inline rfk::Class const& Solid::PlayerController::staticGetArchetype() noexcept	\
		{	\
			static bool			initialized = false;	\
			static rfk::Class	type("PlayerController", 7977671938205726473u, sizeof(PlayerController));	\
			\
			if (!initialized)	\
			{	\
				initialized = true;	\
			\
				__RFK7977671938205726473u_GenerateArchetypeProperties	\
				__RFK7977671938205726473u_GenerateParentsMetadata	\
					\
				__RFK7977671938205726473u_GenerateFieldsMetadata	\
				__RFK7977671938205726473u_GenerateDefaultInstantiatorSetup	\
				__RFK7977671938205726473u_GenerateMethodsMetadata	\
			}	\
			\
			return type;	\
		}

#endif
#define __RFK7977671938205726473u_RegisterArchetype	\
	private:	\
		static inline rfk::ArchetypeRegisterer __rfkArchetypeRegisterer;


#ifdef KODGEN_PARSING
#define PlayerController_GENERATED
#else
#define PlayerController_GENERATED	\
	friend rfk::Struct;	\
	friend rfk::hasField___rfkArchetypeRegisterer<PlayerController, rfk::ArchetypeRegisterer>;	\
	__RFK7977671938205726473u_GetTypeDeclaration	\
	__RFK7977671938205726473u_RegisterArchetype	\
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
	__RFK7977671938205726473u_GetTypeDefinition	\
	__RFK3698706698196451833u_GENERATED	\
	__RFKNativeProperties_GENERATED	\


