#pragma once

/**
*	Source file: D:\Project\SolidEngine\SolidEngine\Include\Core\std_wrapper.hpp
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


#define __RFK12346035822620291350u_GenerateParentsMetadata	\
	type.directParents.reserve(1);	\
	type.addToParents<std::string>(static_cast<rfk::EAccessSpecifier>(1));	\

#define __RFK12346035822620291350u_GenerateFieldsMetadata	\
	registerChild<String>(&type);	\

#define __RFK12346035822620291350u_GenerateFieldHelperMethods	\
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
		recurseRegisterChild<std::string, ChildType>(childArchetype);	\
		rfk::Struct const& thisArchetype = staticGetArchetype();	\
		if (childArchetype != &thisArchetype)	\
		{	\
			const_cast<rfk::Struct&>(thisArchetype).children.insert(childArchetype);	\
		}	\
	}

#define __RFK12346035822620291350u_GenerateMethodsMetadata	\

#define __RFK12346035822620291350u_GenerateArchetypeProperties	\
	

#define __RFK12346035822620291350u_GenerateDefaultInstantiatorSetup	\
	type.setDefaultInstantiator(&rfk::defaultInstantiator<String>);

#define __RFK12346035822620291350u_GetTypeDeclaration	\
	__RFK12346035822620291350u_GenerateFieldHelperMethods	\
	public:	\
		inline static rfk::Class const& staticGetArchetype() noexcept;	\
	

#ifdef KODGEN_PARSING
#define __RFK12346035822620291350u_GetTypeDefinition
#else
#define __RFK12346035822620291350u_GetTypeDefinition	\
		inline rfk::Class const& String::staticGetArchetype() noexcept	\
		{	\
			static bool			initialized = false;	\
			static rfk::Class	type("String", 12346035822620291350u, sizeof(String));	\
			\
			if (!initialized)	\
			{	\
				initialized = true;	\
			\
				__RFK12346035822620291350u_GenerateArchetypeProperties	\
				__RFK12346035822620291350u_GenerateParentsMetadata	\
					\
				__RFK12346035822620291350u_GenerateFieldsMetadata	\
				__RFK12346035822620291350u_GenerateDefaultInstantiatorSetup	\
				__RFK12346035822620291350u_GenerateMethodsMetadata	\
			}	\
			\
			return type;	\
		}

#endif
#define __RFK12346035822620291350u_RegisterArchetype	\
	private:	\
		static inline rfk::ArchetypeRegisterer __rfkArchetypeRegisterer = &staticGetArchetype();


#ifdef KODGEN_PARSING
#define String_GENERATED
#else
#define String_GENERATED	\
	friend rfk::Struct;	\
	friend rfk::hasField___rfkArchetypeRegisterer<String, rfk::ArchetypeRegisterer>;	\
	__RFK12346035822620291350u_GetTypeDeclaration	\
	__RFK12346035822620291350u_RegisterArchetype	\
		\
	private:

#endif

#define __RFK12397126941405399743u_GenerateParentsMetadata	\
	type.directParents.reserve(1);	\
	type.addToParents<std::vector<String>>(static_cast<rfk::EAccessSpecifier>(1));	\

#define __RFK12397126941405399743u_GenerateFieldsMetadata	\
	registerChild<vectorStr>(&type);	\

#define __RFK12397126941405399743u_GenerateFieldHelperMethods	\
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
		recurseRegisterChild<std::vector<String>, ChildType>(childArchetype);	\
		rfk::Struct const& thisArchetype = staticGetArchetype();	\
		if (childArchetype != &thisArchetype)	\
		{	\
			const_cast<rfk::Struct&>(thisArchetype).children.insert(childArchetype);	\
		}	\
	}

#define __RFK12397126941405399743u_GenerateMethodsMetadata	\

#define __RFK12397126941405399743u_GenerateArchetypeProperties	\
	

#define __RFK12397126941405399743u_GenerateDefaultInstantiatorSetup	\
	type.setDefaultInstantiator(&rfk::defaultInstantiator<vectorStr>);

#define __RFK12397126941405399743u_GetTypeDeclaration	\
	__RFK12397126941405399743u_GenerateFieldHelperMethods	\
	public:	\
		inline static rfk::Class const& staticGetArchetype() noexcept;	\
	

#ifdef KODGEN_PARSING
#define __RFK12397126941405399743u_GetTypeDefinition
#else
#define __RFK12397126941405399743u_GetTypeDefinition	\
		inline rfk::Class const& vectorStr::staticGetArchetype() noexcept	\
		{	\
			static bool			initialized = false;	\
			static rfk::Class	type("vectorStr", 12397126941405399743u, sizeof(vectorStr));	\
			\
			if (!initialized)	\
			{	\
				initialized = true;	\
			\
				__RFK12397126941405399743u_GenerateArchetypeProperties	\
				__RFK12397126941405399743u_GenerateParentsMetadata	\
					\
				__RFK12397126941405399743u_GenerateFieldsMetadata	\
				__RFK12397126941405399743u_GenerateDefaultInstantiatorSetup	\
				__RFK12397126941405399743u_GenerateMethodsMetadata	\
			}	\
			\
			return type;	\
		}

#endif
#define __RFK12397126941405399743u_RegisterArchetype	\
	private:	\
		static inline rfk::ArchetypeRegisterer __rfkArchetypeRegisterer = &staticGetArchetype();


#ifdef KODGEN_PARSING
#define vectorStr_GENERATED
#else
#define vectorStr_GENERATED	\
	friend rfk::Struct;	\
	friend rfk::hasField___rfkArchetypeRegisterer<vectorStr, rfk::ArchetypeRegisterer>;	\
	__RFK12397126941405399743u_GetTypeDeclaration	\
	__RFK12397126941405399743u_RegisterArchetype	\
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
	__RFK12346035822620291350u_GetTypeDefinition	\
	__RFK12397126941405399743u_GetTypeDefinition	\
	__RFKNativeProperties_GENERATED	\


