#pragma once

/**
*	Source file: D:\Project\SolidEngine\SolidEngine\Include\ECS\Components\camera.hpp
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
#define __RFK3698706698196451833u_DeclareGetNamespaceFragment inline rfk::NamespaceFragment const& getNamespaceFragment3698706698196451833u_8418644301001383486() noexcept;

#ifdef __RFK3698706698196451833u_DefineGetNamespaceFragment
	#undef __RFK3698706698196451833u_DefineGetNamespaceFragment
#endif
#define __RFK3698706698196451833u_DefineGetNamespaceFragment	\
	inline rfk::NamespaceFragment const& getNamespaceFragment3698706698196451833u_8418644301001383486() noexcept	\
	{	\
		static rfk::NamespaceFragment	fragment("Solid", 3698706698196451833u);	\
		static bool						initialized = false;	\
		if (!initialized)	\
		{	\
			initialized = true;	\
			fragment.nestedEntities.reserve(1u); rfk::NamespaceFragment* fragmentPtr = &fragment; fragmentPtr->addNestedEntity(&Solid::Camera::staticGetArchetype());	\
		}	\
	return fragment;	\
	}

#ifdef __RFK3698706698196451833u_RegisterNamespace
	#undef __RFK3698706698196451833u_RegisterNamespace
#endif
#define __RFK3698706698196451833u_RegisterNamespace	\
	inline rfk::NamespaceFragmentRegisterer namespaceFragmentRegisterer3698706698196451833u_8418644301001383486 = rfk::NamespaceFragmentRegisterer("Solid", 3698706698196451833u, &getNamespaceFragment3698706698196451833u_8418644301001383486(), true);

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

#define __RFK1670407136954520295u_GenerateParentsMetadata	\
	type.directParents.reserve(1);	\
	type.addToParents<Solid::Components>(static_cast<rfk::EAccessSpecifier>(1));	\

#define __RFK1670407136954520295u_GenerateFieldsMetadata	\
	registerChild<Camera>(&type);	\

#define __RFK1670407136954520295u_GenerateFieldHelperMethods	\
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
		field = childArchetype->addField("isActiveCamera", 3285293707122663665u, rfk::Type::getType<bool>(), static_cast<rfk::EFieldFlags>(4), &thisArchetype, offsetof(ChildType, isActiveCamera));	\
		field = childArchetype->addField("isDefaultCamera", 3317188027178987390u, rfk::Type::getType<bool>(), static_cast<rfk::EFieldFlags>(4), &thisArchetype, offsetof(ChildType, isDefaultCamera));	\
		field = childArchetype->addField("fov", 13929501697870170739u, rfk::Type::getType<float>(), static_cast<rfk::EFieldFlags>(1), &thisArchetype, offsetof(ChildType, fov));	\
		field = childArchetype->addField("_near", 15647780023435228343u, rfk::Type::getType<float>(), static_cast<rfk::EFieldFlags>(1), &thisArchetype, offsetof(ChildType, _near));	\
		field = childArchetype->addField("_far", 4669005190604552310u, rfk::Type::getType<float>(), static_cast<rfk::EFieldFlags>(1), &thisArchetype, offsetof(ChildType, _far));	\
		field = childArchetype->addField("Right", 12888843067222633458u, rfk::Type::getType<Solid::Vec3>(), static_cast<rfk::EFieldFlags>(1), &thisArchetype, offsetof(ChildType, Right));	\
		field = childArchetype->addField("Front", 1821305778360589943u, rfk::Type::getType<Solid::Vec3>(), static_cast<rfk::EFieldFlags>(1), &thisArchetype, offsetof(ChildType, Front));	\
		field = childArchetype->addField("Up", 8228860089298804413u, rfk::Type::getType<Solid::Vec3>(), static_cast<rfk::EFieldFlags>(1), &thisArchetype, offsetof(ChildType, Up));	\
		field = childArchetype->addField("position", 444306713112828243u, rfk::Type::getType<Solid::Vec3>(), static_cast<rfk::EFieldFlags>(1), &thisArchetype, offsetof(ChildType, position));	\
		field = childArchetype->addField("Euler", 11116844893687341585u, rfk::Type::getType<Solid::Vec3>(), static_cast<rfk::EFieldFlags>(1), &thisArchetype, offsetof(ChildType, Euler));	\
		__RFK_DISABLE_WARNING_POP	\
	}

#define __RFK1670407136954520295u_GenerateMethodsMetadata	\

#define __RFK1670407136954520295u_GenerateArchetypeProperties	\
	

#define __RFK1670407136954520295u_GenerateDefaultInstantiatorSetup	\
	type.setDefaultInstantiator(&rfk::defaultInstantiator<Camera>);

#define __RFK1670407136954520295u_GetTypeDeclaration	\
	__RFK1670407136954520295u_GenerateFieldHelperMethods	\
	public:	\
		inline static rfk::Class const& staticGetArchetype() noexcept;	\
		rfk::Class const& getArchetype() const noexcept override { return Camera::staticGetArchetype(); }

#ifdef KODGEN_PARSING
#define __RFK1670407136954520295u_GetTypeDefinition
#else
#define __RFK1670407136954520295u_GetTypeDefinition	\
		inline rfk::Class const& Solid::Camera::staticGetArchetype() noexcept	\
		{	\
			static bool			initialized = false;	\
			static rfk::Class	type("Camera", 1670407136954520295u, sizeof(Camera));	\
			\
			if (!initialized)	\
			{	\
				initialized = true;	\
			\
				__RFK1670407136954520295u_GenerateArchetypeProperties	\
				__RFK1670407136954520295u_GenerateParentsMetadata	\
					\
				__RFK1670407136954520295u_GenerateFieldsMetadata	\
				__RFK1670407136954520295u_GenerateDefaultInstantiatorSetup	\
				__RFK1670407136954520295u_GenerateMethodsMetadata	\
			}	\
			\
			return type;	\
		}

#endif
#define __RFK1670407136954520295u_RegisterArchetype	\
	private:	\
		static inline rfk::ArchetypeRegisterer __rfkArchetypeRegisterer;


#ifdef KODGEN_PARSING
#define Camera_GENERATED
#else
#define Camera_GENERATED	\
	friend rfk::Struct;	\
	friend rfk::hasField___rfkArchetypeRegisterer<Camera, rfk::ArchetypeRegisterer>;	\
	__RFK1670407136954520295u_GetTypeDeclaration	\
	__RFK1670407136954520295u_RegisterArchetype	\
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
	__RFK1670407136954520295u_GetTypeDefinition	\
	__RFK3698706698196451833u_GENERATED	\
	__RFKNativeProperties_GENERATED	\


