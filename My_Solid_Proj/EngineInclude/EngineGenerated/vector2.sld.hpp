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
		[[maybe_unused]] rfk::Field*		field		= nullptr; 	\
		[[maybe_unused]] rfk::StaticField*	staticField = nullptr; 	\
		__RFK_DISABLE_WARNING_PUSH	\
		__RFK_DISABLE_WARNING_OFFSETOF	\
		field = childArchetype->addField("x", 8732607830096158948u, rfk::Type::getType<int>(), static_cast<rfk::EFieldFlags>(1), &thisArchetype, offsetof(ChildType, x));	\
		field = childArchetype->addField("y", 8732608929607787159u, rfk::Type::getType<int>(), static_cast<rfk::EFieldFlags>(1), &thisArchetype, offsetof(ChildType, y));	\
		__RFK_DISABLE_WARNING_POP	\
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
		[[maybe_unused]] rfk::Field*		field		= nullptr; 	\
		[[maybe_unused]] rfk::StaticField*	staticField = nullptr; 	\
		__RFK_DISABLE_WARNING_PUSH	\
		__RFK_DISABLE_WARNING_OFFSETOF	\
		field = childArchetype->addField("x", 18430655376742162261u, rfk::Type::getType<double>(), static_cast<rfk::EFieldFlags>(1), &thisArchetype, offsetof(ChildType, x));	\
		field = childArchetype->addField("y", 18430654277230534050u, rfk::Type::getType<double>(), static_cast<rfk::EFieldFlags>(1), &thisArchetype, offsetof(ChildType, y));	\
		__RFK_DISABLE_WARNING_POP	\
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
		[[maybe_unused]] rfk::Field*		field		= nullptr; 	\
		[[maybe_unused]] rfk::StaticField*	staticField = nullptr; 	\
		__RFK_DISABLE_WARNING_PUSH	\
		__RFK_DISABLE_WARNING_OFFSETOF	\
		field = childArchetype->addField("x", 2883023776431942569u, rfk::Type::getType<float>(), static_cast<rfk::EFieldFlags>(1), &thisArchetype, offsetof(ChildType, x));	\
		field = childArchetype->addField("y", 2883022676920314358u, rfk::Type::getType<float>(), static_cast<rfk::EFieldFlags>(1), &thisArchetype, offsetof(ChildType, y));	\
		__RFK_DISABLE_WARNING_POP	\
	}

#define __RFK457187737916094696u_GenerateMethodsMetadata	\
	[[maybe_unused]] rfk::Method*		method			= nullptr;	\
	[[maybe_unused]] rfk::StaticMethod*	staticMethod	= nullptr;	\
	staticMethod = type.addStaticMethod("Dot", 12101278909510278594u, rfk::Type::getType<float>(), std::make_unique<rfk::NonMemberFunction<float (const Solid::Vec2 &, const Solid::Vec2 &)>>(static_cast<float (*)(const Solid::Vec2 &, const Solid::Vec2 &) noexcept>(& Vec2::Dot)), static_cast<rfk::EMethodFlags>(9));	\
	staticMethod->parameters.reserve(2); staticMethod->addParameter("_v1", rfk::Type::getType<const Solid::Vec2 &>())->addParameter("_v2", rfk::Type::getType<const Solid::Vec2 &>());	\
	staticMethod = type.addStaticMethod("Cross", 15634921673735646903u, rfk::Type::getType<float>(), std::make_unique<rfk::NonMemberFunction<float (const Solid::Vec2 &, const Solid::Vec2 &)>>(static_cast<float (*)(const Solid::Vec2 &, const Solid::Vec2 &) noexcept>(& Vec2::Cross)), static_cast<rfk::EMethodFlags>(9));	\
	staticMethod->parameters.reserve(2); staticMethod->addParameter("_v1", rfk::Type::getType<const Solid::Vec2 &>())->addParameter("_v2", rfk::Type::getType<const Solid::Vec2 &>());	\
	staticMethod = type.addStaticMethod("Lerp", 6612827195457751697u, rfk::Type::getType<Solid::Vec2>(), std::make_unique<rfk::NonMemberFunction<Solid::Vec2 (const Solid::Vec2 &, const Solid::Vec2 &, float)>>(static_cast<Solid::Vec2 (*)(const Solid::Vec2 &, const Solid::Vec2 &, float) noexcept>(& Vec2::Lerp)), static_cast<rfk::EMethodFlags>(9));	\
	staticMethod->parameters.reserve(3); staticMethod->addParameter("_v1", rfk::Type::getType<const Solid::Vec2 &>())->addParameter("_v2", rfk::Type::getType<const Solid::Vec2 &>())->addParameter("_r", rfk::Type::getType<float>());	\
	staticMethod = type.addStaticMethod("Nlerp", 16078846767050564179u, rfk::Type::getType<Solid::Vec2>(), std::make_unique<rfk::NonMemberFunction<Solid::Vec2 (const Solid::Vec2 &, const Solid::Vec2 &, float)>>(static_cast<Solid::Vec2 (*)(const Solid::Vec2 &, const Solid::Vec2 &, float) noexcept>(& Vec2::Nlerp)), static_cast<rfk::EMethodFlags>(9));	\
	staticMethod->parameters.reserve(3); staticMethod->addParameter("_v1", rfk::Type::getType<const Solid::Vec2 &>())->addParameter("_v2", rfk::Type::getType<const Solid::Vec2 &>())->addParameter("_r", rfk::Type::getType<float>());	\
	staticMethod = type.addStaticMethod("Slerp", 7372834151964589462u, rfk::Type::getType<Solid::Vec2>(), std::make_unique<rfk::NonMemberFunction<Solid::Vec2 (const Solid::Vec2 &, const Solid::Vec2 &, float)>>(static_cast<Solid::Vec2 (*)(const Solid::Vec2 &, const Solid::Vec2 &, float) noexcept>(& Vec2::Slerp)), static_cast<rfk::EMethodFlags>(9));	\
	staticMethod->parameters.reserve(3); staticMethod->addParameter("_v1", rfk::Type::getType<const Solid::Vec2 &>())->addParameter("_v2", rfk::Type::getType<const Solid::Vec2 &>())->addParameter("_r", rfk::Type::getType<float>());	\
	method = type.addMethod("SqrtLength", 7753399006788193784u, rfk::Type::getType<float>(), std::make_unique<rfk::MemberFunction<Vec2, float ()>>(static_cast<float (Vec2::*)() const noexcept>(& Vec2::SqrtLength)), static_cast<rfk::EMethodFlags>(513));	\
	method = type.addMethod("Length", 7208936942136604142u, rfk::Type::getType<float>(), std::make_unique<rfk::MemberFunction<Vec2, float ()>>(static_cast<float (Vec2::*)() const noexcept>(& Vec2::Length)), static_cast<rfk::EMethodFlags>(513));	\
	method = type.addMethod("Scale", 15106486385941643462u, rfk::Type::getType<Solid::Vec2 &>(), std::make_unique<rfk::MemberFunction<Vec2, Solid::Vec2 &(float)>>(static_cast<Solid::Vec2 &(Vec2::*)(float) noexcept>(& Vec2::Scale)), static_cast<rfk::EMethodFlags>(1));	\
	method->parameters.reserve(1); method->addParameter("_scale", rfk::Type::getType<float>());	\
	method = type.addMethod("GetScaled", 9003906959738626725u, rfk::Type::getType<Solid::Vec2>(), std::make_unique<rfk::MemberFunction<Vec2, Solid::Vec2 (float)>>(static_cast<Solid::Vec2 (Vec2::*)(float) const noexcept>(& Vec2::GetScaled)), static_cast<rfk::EMethodFlags>(513));	\
	method->parameters.reserve(1); method->addParameter("_scale", rfk::Type::getType<float>());	\
	method = type.addMethod("Unscale", 14091610031337522545u, rfk::Type::getType<Solid::Vec2 &>(), std::make_unique<rfk::MemberFunction<Vec2, Solid::Vec2 &(float)>>(static_cast<Solid::Vec2 &(Vec2::*)(float) noexcept>(& Vec2::Unscale)), static_cast<rfk::EMethodFlags>(1));	\
	method->parameters.reserve(1); method->addParameter("_scale", rfk::Type::getType<float>());	\
	method = type.addMethod("GetUnscaled", 17610633803277118638u, rfk::Type::getType<Solid::Vec2>(), std::make_unique<rfk::MemberFunction<Vec2, Solid::Vec2 (float)>>(static_cast<Solid::Vec2 (Vec2::*)(float) const noexcept>(& Vec2::GetUnscaled)), static_cast<rfk::EMethodFlags>(513));	\
	method->parameters.reserve(1); method->addParameter("_scale", rfk::Type::getType<float>());	\
	method = type.addMethod("Normalize", 7518687596643189890u, rfk::Type::getType<Solid::Vec2 &>(), std::make_unique<rfk::MemberFunction<Vec2, Solid::Vec2 &()>>(static_cast<Solid::Vec2 &(Vec2::*)() noexcept>(& Vec2::Normalize)), static_cast<rfk::EMethodFlags>(1));	\
	method = type.addMethod("GetNormalized", 11024118545609933647u, rfk::Type::getType<Solid::Vec2>(), std::make_unique<rfk::MemberFunction<Vec2, Solid::Vec2 ()>>(static_cast<Solid::Vec2 (Vec2::*)() const noexcept>(& Vec2::GetNormalized)), static_cast<rfk::EMethodFlags>(513));	\
	method = type.addMethod("IsNormalized", 15159715434575469399u, rfk::Type::getType<bool>(), std::make_unique<rfk::MemberFunction<Vec2, bool ()>>(static_cast<bool (Vec2::*)() const noexcept>(& Vec2::IsNormalized)), static_cast<rfk::EMethodFlags>(513));	\
	method = type.addMethod("IsEquals", 6134438974077251769u, rfk::Type::getType<bool>(), std::make_unique<rfk::MemberFunction<Vec2, bool (const Solid::Vec2 &)>>(static_cast<bool (Vec2::*)(const Solid::Vec2 &) const noexcept>(& Vec2::IsEquals)), static_cast<rfk::EMethodFlags>(513));	\
	method->parameters.reserve(1); method->addParameter("vec", rfk::Type::getType<const Solid::Vec2 &>());	\
	method = type.addMethod("Dist", 263679113256326246u, rfk::Type::getType<float>(), std::make_unique<rfk::MemberFunction<Vec2, float (const Solid::Vec2 &)>>(static_cast<float (Vec2::*)(const Solid::Vec2 &) const noexcept>(& Vec2::Dist)), static_cast<rfk::EMethodFlags>(513));	\
	method->parameters.reserve(1); method->addParameter("vec", rfk::Type::getType<const Solid::Vec2 &>());	\
	method = type.addMethod("SqrtDist", 6850231261257716348u, rfk::Type::getType<float>(), std::make_unique<rfk::MemberFunction<Vec2, float (const Solid::Vec2 &)>>(static_cast<float (Vec2::*)(const Solid::Vec2 &) const noexcept>(& Vec2::SqrtDist)), static_cast<rfk::EMethodFlags>(513));	\
	method->parameters.reserve(1); method->addParameter("vec", rfk::Type::getType<const Solid::Vec2 &>());	\
	method = type.addMethod("ToString", 17339381511605718746u, rfk::Type::getType<const std::string>(), std::make_unique<rfk::MemberFunction<Vec2, const std::string ()>>(static_cast<const std::string (Vec2::*)() const noexcept>(& Vec2::ToString)), static_cast<rfk::EMethodFlags>(513));	\

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


