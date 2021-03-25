#pragma once

/**
*	Source file: D:\Project\SolidEngine\SolidEngine\Include\Core\Maths\Vector\vector2.hpp
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
#define __RFK3698706698196451833u_DeclareGetNamespaceFragment inline rfk::NamespaceFragment const& getNamespaceFragment3698706698196451833u_7461032412233524492() noexcept;

#ifdef __RFK3698706698196451833u_DefineGetNamespaceFragment
	#undef __RFK3698706698196451833u_DefineGetNamespaceFragment
#endif
#define __RFK3698706698196451833u_DefineGetNamespaceFragment	\
	inline rfk::NamespaceFragment const& getNamespaceFragment3698706698196451833u_7461032412233524492() noexcept	\
	{	\
		static rfk::NamespaceFragment	fragment("Solid", 3698706698196451833u);	\
		static bool						initialized = false;	\
		if (!initialized)	\
		{	\
			initialized = true;	\
			fragment.nestedEntities.reserve(3u); rfk::NamespaceFragment* fragmentPtr = &fragment; fragmentPtr->addNestedEntity(&Solid::Vec2i::staticGetArchetype())->addNestedEntity(&Solid::Vec2d::staticGetArchetype())->addNestedEntity(&Solid::Vec2::staticGetArchetype());	\
		}	\
	return fragment;	\
	}

#ifdef __RFK3698706698196451833u_RegisterNamespace
	#undef __RFK3698706698196451833u_RegisterNamespace
#endif
#define __RFK3698706698196451833u_RegisterNamespace	\
	inline rfk::NamespaceFragmentRegisterer namespaceFragmentRegisterer3698706698196451833u_7461032412233524492 = rfk::NamespaceFragmentRegisterer("Solid", 3698706698196451833u, &getNamespaceFragment3698706698196451833u_7461032412233524492(), true);

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

#define __RFK17235375859580601139u_GenerateFieldsMetadata	\
	registerChild<Vec2i>(&type);	\

#define __RFK17235375859580601139u_GenerateFieldHelperMethods	\
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
		rfk::Struct const& thisArchetype = staticGetArchetype();	\
		if (childArchetype != &thisArchetype)	\
		{	\
			const_cast<rfk::Struct&>(thisArchetype).children.insert(childArchetype);	\
		}	\
	}

#define __RFK17235375859580601139u_GenerateMethodsMetadata	\

#define __RFK17235375859580601139u_GenerateArchetypeProperties	\
	

#define __RFK17235375859580601139u_GenerateDefaultInstantiatorSetup	\
	type.setDefaultInstantiator(&rfk::defaultInstantiator<Vec2i>);

#define __RFK17235375859580601139u_GetTypeDeclaration	\
	__RFK17235375859580601139u_GenerateFieldHelperMethods	\
	public:	\
		inline static rfk::Struct const& staticGetArchetype() noexcept;	\
	

#ifdef KODGEN_PARSING
#define __RFK17235375859580601139u_GetTypeDefinition
#else
#define __RFK17235375859580601139u_GetTypeDefinition	\
		inline rfk::Struct const& Solid::Vec2i::staticGetArchetype() noexcept	\
		{	\
			static bool			initialized = false;	\
			static rfk::Struct	type("Vec2i", 17235375859580601139u, sizeof(Vec2i));	\
			\
			if (!initialized)	\
			{	\
				initialized = true;	\
			\
				__RFK17235375859580601139u_GenerateArchetypeProperties	\
					\
					\
				__RFK17235375859580601139u_GenerateFieldsMetadata	\
				__RFK17235375859580601139u_GenerateDefaultInstantiatorSetup	\
				__RFK17235375859580601139u_GenerateMethodsMetadata	\
			}	\
			\
			return type;	\
		}

#endif
#define __RFK17235375859580601139u_RegisterArchetype	\
	private:	\
		static inline rfk::ArchetypeRegisterer __rfkArchetypeRegisterer;


#ifdef KODGEN_PARSING
#define Vec2i_GENERATED
#else
#define Vec2i_GENERATED	\
	friend rfk::Struct;	\
	friend rfk::hasField___rfkArchetypeRegisterer<Vec2i, rfk::ArchetypeRegisterer>;	\
	__RFK17235375859580601139u_GetTypeDeclaration	\
	__RFK17235375859580601139u_RegisterArchetype	\
		\
	private:

#endif

#define __RFK17235387954208511460u_GenerateFieldsMetadata	\
	registerChild<Vec2d>(&type);	\

#define __RFK17235387954208511460u_GenerateFieldHelperMethods	\
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
		rfk::Struct const& thisArchetype = staticGetArchetype();	\
		if (childArchetype != &thisArchetype)	\
		{	\
			const_cast<rfk::Struct&>(thisArchetype).children.insert(childArchetype);	\
		}	\
	}

#define __RFK17235387954208511460u_GenerateMethodsMetadata	\

#define __RFK17235387954208511460u_GenerateArchetypeProperties	\
	

#define __RFK17235387954208511460u_GenerateDefaultInstantiatorSetup	\
	type.setDefaultInstantiator(&rfk::defaultInstantiator<Vec2d>);

#define __RFK17235387954208511460u_GetTypeDeclaration	\
	__RFK17235387954208511460u_GenerateFieldHelperMethods	\
	public:	\
		inline static rfk::Struct const& staticGetArchetype() noexcept;	\
	

#ifdef KODGEN_PARSING
#define __RFK17235387954208511460u_GetTypeDefinition
#else
#define __RFK17235387954208511460u_GetTypeDefinition	\
		inline rfk::Struct const& Solid::Vec2d::staticGetArchetype() noexcept	\
		{	\
			static bool			initialized = false;	\
			static rfk::Struct	type("Vec2d", 17235387954208511460u, sizeof(Vec2d));	\
			\
			if (!initialized)	\
			{	\
				initialized = true;	\
			\
				__RFK17235387954208511460u_GenerateArchetypeProperties	\
					\
					\
				__RFK17235387954208511460u_GenerateFieldsMetadata	\
				__RFK17235387954208511460u_GenerateDefaultInstantiatorSetup	\
				__RFK17235387954208511460u_GenerateMethodsMetadata	\
			}	\
			\
			return type;	\
		}

#endif
#define __RFK17235387954208511460u_RegisterArchetype	\
	private:	\
		static inline rfk::ArchetypeRegisterer __rfkArchetypeRegisterer;


#ifdef KODGEN_PARSING
#define Vec2d_GENERATED
#else
#define Vec2d_GENERATED	\
	friend rfk::Struct;	\
	friend rfk::hasField___rfkArchetypeRegisterer<Vec2d, rfk::ArchetypeRegisterer>;	\
	__RFK17235387954208511460u_GetTypeDeclaration	\
	__RFK17235387954208511460u_RegisterArchetype	\
		\
	private:

#endif

#define __RFK457187737916094696u_GenerateFieldsMetadata	\
	registerChild<Vec2>(&type);	\

#define __RFK457187737916094696u_GenerateFieldHelperMethods	\
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
		rfk::Struct const& thisArchetype = staticGetArchetype();	\
		if (childArchetype != &thisArchetype)	\
		{	\
			const_cast<rfk::Struct&>(thisArchetype).children.insert(childArchetype);	\
		}	\
	}

#define __RFK457187737916094696u_GenerateMethodsMetadata	\

#define __RFK457187737916094696u_GenerateArchetypeProperties	\
	

#define __RFK457187737916094696u_GenerateDefaultInstantiatorSetup	\
	type.setDefaultInstantiator(&rfk::defaultInstantiator<Vec2>);

#define __RFK457187737916094696u_GetTypeDeclaration	\
	__RFK457187737916094696u_GenerateFieldHelperMethods	\
	public:	\
		inline static rfk::Class const& staticGetArchetype() noexcept;	\
	

#ifdef KODGEN_PARSING
#define __RFK457187737916094696u_GetTypeDefinition
#else
#define __RFK457187737916094696u_GetTypeDefinition	\
		inline rfk::Class const& Solid::Vec2::staticGetArchetype() noexcept	\
		{	\
			static bool			initialized = false;	\
			static rfk::Class	type("Vec2", 457187737916094696u, sizeof(Vec2));	\
			\
			if (!initialized)	\
			{	\
				initialized = true;	\
			\
				__RFK457187737916094696u_GenerateArchetypeProperties	\
					\
					\
				__RFK457187737916094696u_GenerateFieldsMetadata	\
				__RFK457187737916094696u_GenerateDefaultInstantiatorSetup	\
				__RFK457187737916094696u_GenerateMethodsMetadata	\
			}	\
			\
			return type;	\
		}

#endif
#define __RFK457187737916094696u_RegisterArchetype	\
	private:	\
		static inline rfk::ArchetypeRegisterer __rfkArchetypeRegisterer;


#ifdef KODGEN_PARSING
#define Vec2_GENERATED
#else
#define Vec2_GENERATED	\
	friend rfk::Struct;	\
	friend rfk::hasField___rfkArchetypeRegisterer<Vec2, rfk::ArchetypeRegisterer>;	\
	__RFK457187737916094696u_GetTypeDeclaration	\
	__RFK457187737916094696u_RegisterArchetype	\
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
	__RFK17235375859580601139u_GetTypeDefinition	\
	__RFK17235387954208511460u_GetTypeDefinition	\
	__RFK457187737916094696u_GetTypeDefinition	\
	__RFK3698706698196451833u_GENERATED	\
	__RFKNativeProperties_GENERATED	\


