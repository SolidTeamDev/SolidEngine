#pragma once

/**
*	Source file: D:\Project\SolidEngine\SolidEngine\Include\ECS\Components\light.hpp
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
#define __RFK3698706698196451833u_DeclareGetNamespaceFragment inline rfk::NamespaceFragment const& getNamespaceFragment3698706698196451833u_10506963928207784167() noexcept;

#ifdef __RFK3698706698196451833u_DefineGetNamespaceFragment
	#undef __RFK3698706698196451833u_DefineGetNamespaceFragment
#endif
#define __RFK3698706698196451833u_DefineGetNamespaceFragment	\
	inline rfk::NamespaceFragment const& getNamespaceFragment3698706698196451833u_10506963928207784167() noexcept	\
	{	\
		static rfk::NamespaceFragment	fragment("Solid", 3698706698196451833u);	\
		static bool						initialized = false;	\
		if (!initialized)	\
		{	\
			initialized = true;	\
			fragment.nestedEntities.reserve(1u); rfk::NamespaceFragment* fragmentPtr = &fragment; fragmentPtr->addNestedEntity(&Solid::Light::staticGetArchetype());	\
		}	\
	return fragment;	\
	}

#ifdef __RFK3698706698196451833u_RegisterNamespace
	#undef __RFK3698706698196451833u_RegisterNamespace
#endif
#define __RFK3698706698196451833u_RegisterNamespace	\
	inline rfk::NamespaceFragmentRegisterer namespaceFragmentRegisterer3698706698196451833u_10506963928207784167 = rfk::NamespaceFragmentRegisterer("Solid", 3698706698196451833u, &getNamespaceFragment3698706698196451833u_10506963928207784167(), true);

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

#define __RFK6641605946018490268u_GenerateParentsMetadata	\
	type.directParents.reserve(1);	\
	type.addToParents<Solid::Components>(static_cast<rfk::EAccessSpecifier>(1));	\

#define __RFK6641605946018490268u_GenerateFieldsMetadata	\
	registerChild<Light>(&type);	\

#define __RFK6641605946018490268u_GenerateFieldHelperMethods	\
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
		field = childArchetype->addField("type", 16033048078834940207u, rfk::Type::getType<Solid::Light::ELightType>(), static_cast<rfk::EFieldFlags>(1), &thisArchetype, offsetof(ChildType, type));	\
		field = childArchetype->addField("color", 15015620519318429190u, rfk::Type::getType<Solid::Vec3>(), static_cast<rfk::EFieldFlags>(1), &thisArchetype, offsetof(ChildType, color));	\
		field = childArchetype->addField("intensity", 9417245124466279316u, rfk::Type::getType<float>(), static_cast<rfk::EFieldFlags>(1), &thisArchetype, offsetof(ChildType, intensity));	\
		field = childArchetype->addField("dir", 17571421819883653014u, rfk::Type::getType<Solid::Vec3>(), static_cast<rfk::EFieldFlags>(1), &thisArchetype, offsetof(ChildType, dir));	\
		__RFK_DISABLE_WARNING_POP	\
	}

#define __RFK6641605946018490268u_GenerateMethodsMetadata	\

#define __RFK6641605946018490268u_GenerateArchetypeProperties	\
	

#define __RFK6641605946018490268u_GenerateNestedArchetypesMetadata	\
	rfk::Archetype* archetype = nullptr;	\
	type.nestedArchetypes.reserve(1);	\
	archetype = type.addNestedArchetype(rfk::getEnum<Solid::Light::ELightType>(), static_cast<rfk::EAccessSpecifier>(1));	\

#define __RFK6641605946018490268u_GenerateDefaultInstantiatorSetup	\
	type.setDefaultInstantiator(&rfk::defaultInstantiator<Light>);

#define __RFK6641605946018490268u_GetTypeDeclaration	\
	__RFK6641605946018490268u_GenerateFieldHelperMethods	\
	public:	\
		inline static rfk::Class const& staticGetArchetype() noexcept;	\
		rfk::Class const& getArchetype() const noexcept override { return Light::staticGetArchetype(); }

#ifdef KODGEN_PARSING
#define __RFK6641605946018490268u_GetTypeDefinition
#else
#define __RFK6641605946018490268u_GetTypeDefinition	\
		inline rfk::Class const& Solid::Light::staticGetArchetype() noexcept	\
		{	\
			static bool			initialized = false;	\
			static rfk::Class	type("Light", 6641605946018490268u, sizeof(Light));	\
			\
			if (!initialized)	\
			{	\
				initialized = true;	\
			\
				__RFK6641605946018490268u_GenerateArchetypeProperties	\
				__RFK6641605946018490268u_GenerateParentsMetadata	\
				__RFK6641605946018490268u_GenerateNestedArchetypesMetadata	\
				__RFK6641605946018490268u_GenerateFieldsMetadata	\
				__RFK6641605946018490268u_GenerateDefaultInstantiatorSetup	\
				__RFK6641605946018490268u_GenerateMethodsMetadata	\
			}	\
			\
			return type;	\
		}

#endif
#define __RFK6641605946018490268u_RegisterArchetype	\
	private:	\
		static inline rfk::ArchetypeRegisterer __rfkArchetypeRegisterer;


#ifdef KODGEN_PARSING
#define Light_GENERATED
#else
#define Light_GENERATED	\
	friend rfk::Struct;	\
	friend rfk::hasField___rfkArchetypeRegisterer<Light, rfk::ArchetypeRegisterer>;	\
	__RFK6641605946018490268u_GetTypeDeclaration	\
	__RFK6641605946018490268u_RegisterArchetype	\
		\
	private:

#endif

#define __RFK13874222094267739888u_GenerateGetEnumSpecialization	\
	template <>	\
	inline rfk::Enum const* getEnum<Solid::Light::ELightType>() noexcept	\
	{	\
		static bool			initialized = false;	\
		static rfk::Enum	type("ELightType", 13874222094267739888u, sizeof(Solid::Light::ELightType), rfk::Type::getType<int>());	\
		if (!initialized)	\
		{	\
			initialized = true;	\
			rfk::EnumValue*	enumValue = nullptr;	\
			type.values.reserve(3);	\
			enumValue = type.addEnumValue("DIRECTIONAL", 2785486939851002516u, 0);	\
			enumValue = type.addEnumValue("POINT", 3505893960370790950u, 1);	\
			enumValue = type.addEnumValue("SPOT", 7705631812001943752u, 2);	\
		}	\
		return &type;	\
	}

#ifdef KODGEN_PARSING
#define __RFK13874222094267739888u_GENERATED
#else
#define __RFK13874222094267739888u_GENERATED	\
	namespace rfk {	\
	__RFK13874222094267739888u_GenerateGetEnumSpecialization	\
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
	__RFK13874222094267739888u_GENERATED	\
	__RFK6641605946018490268u_GetTypeDefinition	\
	__RFK3698706698196451833u_GENERATED	\
	__RFKNativeProperties_GENERATED	\


