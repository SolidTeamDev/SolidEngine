#pragma once

/**
*	Source file: D:\Project\SolidEngine\SolidEngine\Include\Core\Maths\Vector\vector4.hpp
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
#define __RFK3698706698196451833u_DeclareGetNamespaceFragment inline rfk::NamespaceFragment const& getNamespaceFragment3698706698196451833u_5757173273107883566() noexcept;

#ifdef __RFK3698706698196451833u_DefineGetNamespaceFragment
	#undef __RFK3698706698196451833u_DefineGetNamespaceFragment
#endif
#define __RFK3698706698196451833u_DefineGetNamespaceFragment	\
	inline rfk::NamespaceFragment const& getNamespaceFragment3698706698196451833u_5757173273107883566() noexcept	\
	{	\
		static rfk::NamespaceFragment	fragment("Solid", 3698706698196451833u);	\
		static bool						initialized = false;	\
		if (!initialized)	\
		{	\
			initialized = true;	\
			fragment.nestedEntities.reserve(2u); rfk::NamespaceFragment* fragmentPtr = &fragment; fragmentPtr->addNestedEntity(&Solid::Vec4i::staticGetArchetype())->addNestedEntity(&Solid::Vec4::staticGetArchetype());	\
		}	\
	return fragment;	\
	}

#ifdef __RFK3698706698196451833u_RegisterNamespace
	#undef __RFK3698706698196451833u_RegisterNamespace
#endif
#define __RFK3698706698196451833u_RegisterNamespace	\
	inline rfk::NamespaceFragmentRegisterer namespaceFragmentRegisterer3698706698196451833u_5757173273107883566 = rfk::NamespaceFragmentRegisterer("Solid", 3698706698196451833u, &getNamespaceFragment3698706698196451833u_5757173273107883566(), true);

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

#define __RFK17241326416511343721u_GenerateFieldsMetadata	\
	registerChild<Vec4i>(&type);	\

#define __RFK17241326416511343721u_GenerateFieldHelperMethods	\
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
		field = childArchetype->addField("x", 10442861051043893602u, rfk::Type::getType<int>(), static_cast<rfk::EFieldFlags>(1), &thisArchetype, offsetof(ChildType, x));	\
		field = childArchetype->addField("y", 10442862150555521813u, rfk::Type::getType<int>(), static_cast<rfk::EFieldFlags>(1), &thisArchetype, offsetof(ChildType, y));	\
		field = childArchetype->addField("z", 10442858852020637180u, rfk::Type::getType<int>(), static_cast<rfk::EFieldFlags>(1), &thisArchetype, offsetof(ChildType, z));	\
		field = childArchetype->addField("w", 10442864349578778235u, rfk::Type::getType<int>(), static_cast<rfk::EFieldFlags>(1), &thisArchetype, offsetof(ChildType, w));	\
		__RFK_DISABLE_WARNING_POP	\
	}

#define __RFK17241326416511343721u_GenerateMethodsMetadata	\

#define __RFK17241326416511343721u_GenerateArchetypeProperties	\
	

#define __RFK17241326416511343721u_GenerateDefaultInstantiatorSetup	\
	type.setDefaultInstantiator(&rfk::defaultInstantiator<Vec4i>);

#define __RFK17241326416511343721u_GetTypeDeclaration	\
	__RFK17241326416511343721u_GenerateFieldHelperMethods	\
	public:	\
		inline static rfk::Class const& staticGetArchetype() noexcept;	\
	

#ifdef KODGEN_PARSING
#define __RFK17241326416511343721u_GetTypeDefinition
#else
#define __RFK17241326416511343721u_GetTypeDefinition	\
		inline rfk::Class const& Solid::Vec4i::staticGetArchetype() noexcept	\
		{	\
			static bool			initialized = false;	\
			static rfk::Class	type("Vec4i", 17241326416511343721u, sizeof(Vec4i));	\
			\
			if (!initialized)	\
			{	\
				initialized = true;	\
			\
				__RFK17241326416511343721u_GenerateArchetypeProperties	\
					\
					\
				__RFK17241326416511343721u_GenerateFieldsMetadata	\
				__RFK17241326416511343721u_GenerateDefaultInstantiatorSetup	\
				__RFK17241326416511343721u_GenerateMethodsMetadata	\
			}	\
			\
			return type;	\
		}

#endif
#define __RFK17241326416511343721u_RegisterArchetype	\
	private:	\
		static inline rfk::ArchetypeRegisterer __rfkArchetypeRegisterer;


#ifdef KODGEN_PARSING
#define Vec4i_GENERATED
#else
#define Vec4i_GENERATED	\
	friend rfk::Struct;	\
	friend rfk::hasField___rfkArchetypeRegisterer<Vec4i, rfk::ArchetypeRegisterer>;	\
	__RFK17241326416511343721u_GetTypeDeclaration	\
	__RFK17241326416511343721u_RegisterArchetype	\
		\
	private:

#endif

#define __RFK457194334985863962u_GenerateFieldsMetadata	\
	registerChild<Vec4>(&type);	\

#define __RFK457194334985863962u_GenerateFieldHelperMethods	\
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
		field = childArchetype->addField("x", 3115027379321753299u, rfk::Type::getType<float>(), static_cast<rfk::EFieldFlags>(1), &thisArchetype, offsetof(ChildType, x));	\
		field = childArchetype->addField("y", 3115026279810125088u, rfk::Type::getType<float>(), static_cast<rfk::EFieldFlags>(1), &thisArchetype, offsetof(ChildType, y));	\
		field = childArchetype->addField("z", 3115029578345009721u, rfk::Type::getType<float>(), static_cast<rfk::EFieldFlags>(1), &thisArchetype, offsetof(ChildType, z));	\
		field = childArchetype->addField("w", 3115041672972920042u, rfk::Type::getType<float>(), static_cast<rfk::EFieldFlags>(1), &thisArchetype, offsetof(ChildType, w));	\
		__RFK_DISABLE_WARNING_POP	\
	}

#define __RFK457194334985863962u_GenerateMethodsMetadata	\
	[[maybe_unused]] rfk::Method*		method			= nullptr;	\
	[[maybe_unused]] rfk::StaticMethod*	staticMethod	= nullptr;	\
	staticMethod = type.addStaticMethod("Dot", 13083327814669505498u, rfk::Type::getType<float>(), std::make_unique<rfk::NonMemberFunction<float (const Solid::Vec4 &, const Solid::Vec4 &)>>(static_cast<float (*)(const Solid::Vec4 &, const Solid::Vec4 &) noexcept>(& Vec4::Dot)), static_cast<rfk::EMethodFlags>(9));	\
	staticMethod->parameters.reserve(2); staticMethod->addParameter("_v1", rfk::Type::getType<const Solid::Vec4 &>())->addParameter("_v2", rfk::Type::getType<const Solid::Vec4 &>());	\
	staticMethod = type.addStaticMethod("Cross", 7315735663504210299u, rfk::Type::getType<Solid::Vec4>(), std::make_unique<rfk::NonMemberFunction<Solid::Vec4 (const Solid::Vec4 &, const Solid::Vec4 &)>>(static_cast<Solid::Vec4 (*)(const Solid::Vec4 &, const Solid::Vec4 &) noexcept>(& Vec4::Cross)), static_cast<rfk::EMethodFlags>(9));	\
	staticMethod->parameters.reserve(2); staticMethod->addParameter("_v1", rfk::Type::getType<const Solid::Vec4 &>())->addParameter("_v2", rfk::Type::getType<const Solid::Vec4 &>());	\
	staticMethod = type.addStaticMethod("Lerp", 8594008838622955129u, rfk::Type::getType<Solid::Vec4>(), std::make_unique<rfk::NonMemberFunction<Solid::Vec4 (const Solid::Vec4 &, const Solid::Vec4 &, float)>>(static_cast<Solid::Vec4 (*)(const Solid::Vec4 &, const Solid::Vec4 &, float) noexcept>(& Vec4::Lerp)), static_cast<rfk::EMethodFlags>(9));	\
	staticMethod->parameters.reserve(3); staticMethod->addParameter("_v1", rfk::Type::getType<const Solid::Vec4 &>())->addParameter("_v2", rfk::Type::getType<const Solid::Vec4 &>())->addParameter("_r", rfk::Type::getType<float>());	\
	staticMethod = type.addStaticMethod("Nlerp", 5462263563637434407u, rfk::Type::getType<Solid::Vec4>(), std::make_unique<rfk::NonMemberFunction<Solid::Vec4 (const Solid::Vec4 &, const Solid::Vec4 &, float)>>(static_cast<Solid::Vec4 (*)(const Solid::Vec4 &, const Solid::Vec4 &, float) noexcept>(& Vec4::Nlerp)), static_cast<rfk::EMethodFlags>(9));	\
	staticMethod->parameters.reserve(3); staticMethod->addParameter("_v1", rfk::Type::getType<const Solid::Vec4 &>())->addParameter("_v2", rfk::Type::getType<const Solid::Vec4 &>())->addParameter("_r", rfk::Type::getType<float>());	\
	staticMethod = type.addStaticMethod("Slerp", 8323858690856878982u, rfk::Type::getType<Solid::Vec4>(), std::make_unique<rfk::NonMemberFunction<Solid::Vec4 (const Solid::Vec4 &, const Solid::Vec4 &, float)>>(static_cast<Solid::Vec4 (*)(const Solid::Vec4 &, const Solid::Vec4 &, float) noexcept>(& Vec4::Slerp)), static_cast<rfk::EMethodFlags>(9));	\
	staticMethod->parameters.reserve(3); staticMethod->addParameter("_v1", rfk::Type::getType<const Solid::Vec4 &>())->addParameter("_v2", rfk::Type::getType<const Solid::Vec4 &>())->addParameter("_r", rfk::Type::getType<float>());	\
	method = type.addMethod("SqrtLength", 3155120530901671238u, rfk::Type::getType<float>(), std::make_unique<rfk::MemberFunction<Vec4, float ()>>(static_cast<float (Vec4::*)() const noexcept>(& Vec4::SqrtLength)), static_cast<rfk::EMethodFlags>(513));	\
	method = type.addMethod("Length", 12534383561642291948u, rfk::Type::getType<float>(), std::make_unique<rfk::MemberFunction<Vec4, float ()>>(static_cast<float (Vec4::*)() const noexcept>(& Vec4::Length)), static_cast<rfk::EMethodFlags>(513));	\
	method = type.addMethod("Scale", 18336087423180008028u, rfk::Type::getType<Solid::Vec4 &>(), std::make_unique<rfk::MemberFunction<Vec4, Solid::Vec4 &(float)>>(static_cast<Solid::Vec4 &(Vec4::*)(float) noexcept>(& Vec4::Scale)), static_cast<rfk::EMethodFlags>(1));	\
	method->parameters.reserve(1); method->addParameter("_scale", rfk::Type::getType<float>());	\
	method = type.addMethod("GetScaled", 8609877475723450099u, rfk::Type::getType<Solid::Vec4>(), std::make_unique<rfk::MemberFunction<Vec4, Solid::Vec4 (float)>>(static_cast<Solid::Vec4 (Vec4::*)(float) const noexcept>(& Vec4::GetScaled)), static_cast<rfk::EMethodFlags>(513));	\
	method->parameters.reserve(1); method->addParameter("_scale", rfk::Type::getType<float>());	\
	method = type.addMethod("Unscale", 5229774776651803683u, rfk::Type::getType<Solid::Vec4 &>(), std::make_unique<rfk::MemberFunction<Vec4, Solid::Vec4 &(float)>>(static_cast<Solid::Vec4 &(Vec4::*)(float) noexcept>(& Vec4::Unscale)), static_cast<rfk::EMethodFlags>(1));	\
	method->parameters.reserve(1); method->addParameter("_scale", rfk::Type::getType<float>());	\
	method = type.addMethod("GetUnscaled", 12587597072024918288u, rfk::Type::getType<Solid::Vec4>(), std::make_unique<rfk::MemberFunction<Vec4, Solid::Vec4 (float)>>(static_cast<Solid::Vec4 (Vec4::*)(float) const noexcept>(& Vec4::GetUnscaled)), static_cast<rfk::EMethodFlags>(513));	\
	method->parameters.reserve(1); method->addParameter("_scale", rfk::Type::getType<float>());	\
	method = type.addMethod("Normalize", 16363843472069129792u, rfk::Type::getType<Solid::Vec4 &>(), std::make_unique<rfk::MemberFunction<Vec4, Solid::Vec4 &()>>(static_cast<Solid::Vec4 &(Vec4::*)() noexcept>(& Vec4::Normalize)), static_cast<rfk::EMethodFlags>(1));	\
	method = type.addMethod("GetNormalized", 13043740370961270997u, rfk::Type::getType<Solid::Vec4>(), std::make_unique<rfk::MemberFunction<Vec4, Solid::Vec4 ()>>(static_cast<Solid::Vec4 (Vec4::*)() const noexcept>(& Vec4::GetNormalized)), static_cast<rfk::EMethodFlags>(513));	\
	method = type.addMethod("IsNormalized", 12159802923976027665u, rfk::Type::getType<bool>(), std::make_unique<rfk::MemberFunction<Vec4, bool ()>>(static_cast<bool (Vec4::*)() const noexcept>(& Vec4::IsNormalized)), static_cast<rfk::EMethodFlags>(513));	\
	method = type.addMethod("IsEquals", 6719863729088560269u, rfk::Type::getType<bool>(), std::make_unique<rfk::MemberFunction<Vec4, bool (const Solid::Vec4 &)>>(static_cast<bool (Vec4::*)(const Solid::Vec4 &) const noexcept>(& Vec4::IsEquals)), static_cast<rfk::EMethodFlags>(513));	\
	method->parameters.reserve(1); method->addParameter("vec", rfk::Type::getType<const Solid::Vec4 &>());	\
	method = type.addMethod("Dist", 8456789296188516766u, rfk::Type::getType<float>(), std::make_unique<rfk::MemberFunction<Vec4, float (const Solid::Vec4 &)>>(static_cast<float (Vec4::*)(const Solid::Vec4 &) const noexcept>(& Vec4::Dist)), static_cast<rfk::EMethodFlags>(513));	\
	method->parameters.reserve(1); method->addParameter("vec", rfk::Type::getType<const Solid::Vec4 &>());	\
	method = type.addMethod("SqrtDist", 16654749217375293520u, rfk::Type::getType<float>(), std::make_unique<rfk::MemberFunction<Vec4, float (const Solid::Vec4 &)>>(static_cast<float (Vec4::*)(const Solid::Vec4 &) const noexcept>(& Vec4::SqrtDist)), static_cast<rfk::EMethodFlags>(513));	\
	method->parameters.reserve(1); method->addParameter("vec", rfk::Type::getType<const Solid::Vec4 &>());	\
	method = type.addMethod("ToVector3", 11328750909286344023u, rfk::Type::getType<Solid::Vec3>(), std::make_unique<rfk::MemberFunction<Vec4, Solid::Vec3 ()>>(static_cast<Solid::Vec3 (Vec4::*)() const noexcept>(& Vec4::ToVector3)), static_cast<rfk::EMethodFlags>(513));	\
	method = type.addMethod("ToString", 12687417519863815444u, rfk::Type::getType<const std::string>(), std::make_unique<rfk::MemberFunction<Vec4, const std::string ()>>(static_cast<const std::string (Vec4::*)() const noexcept>(& Vec4::ToString)), static_cast<rfk::EMethodFlags>(513));	\
	method = type.addMethod("operator=", 8680157199429313776u, rfk::Type::getType<Solid::Vec4 &>(), std::make_unique<rfk::MemberFunction<Vec4, Solid::Vec4 &(const Solid::Vec4 &)>>(static_cast<Solid::Vec4 &(Vec4::*)(const Solid::Vec4 &) noexcept>(& Vec4::operator=)), static_cast<rfk::EMethodFlags>(17));	\
	method->parameters.reserve(1); method->addParameter("vec", rfk::Type::getType<const Solid::Vec4 &>());	\
	method = type.addMethod("operator==", 1509355954365929292u, rfk::Type::getType<bool>(), std::make_unique<rfk::MemberFunction<Vec4, bool (const Solid::Vec4 &)>>(static_cast<bool (Vec4::*)(const Solid::Vec4 &) const noexcept>(& Vec4::operator==)), static_cast<rfk::EMethodFlags>(513));	\
	method->parameters.reserve(1); method->addParameter("_vec", rfk::Type::getType<const Solid::Vec4 &>());	\
	method = type.addMethod("operator!=", 12999909788439197448u, rfk::Type::getType<bool>(), std::make_unique<rfk::MemberFunction<Vec4, bool (const Solid::Vec4 &)>>(static_cast<bool (Vec4::*)(const Solid::Vec4 &) const noexcept>(& Vec4::operator!=)), static_cast<rfk::EMethodFlags>(513));	\
	method->parameters.reserve(1); method->addParameter("_vec", rfk::Type::getType<const Solid::Vec4 &>());	\
	method = type.addMethod("operator+", 5162143064355055445u, rfk::Type::getType<Solid::Vec4>(), std::make_unique<rfk::MemberFunction<Vec4, Solid::Vec4 (const Solid::Vec4 &)>>(static_cast<Solid::Vec4 (Vec4::*)(const Solid::Vec4 &) const noexcept>(& Vec4::operator+)), static_cast<rfk::EMethodFlags>(513));	\
	method->parameters.reserve(1); method->addParameter("_vec", rfk::Type::getType<const Solid::Vec4 &>());	\
	method = type.addMethod("operator-", 13735437983347355459u, rfk::Type::getType<Solid::Vec4>(), std::make_unique<rfk::MemberFunction<Vec4, Solid::Vec4 (const Solid::Vec4 &)>>(static_cast<Solid::Vec4 (Vec4::*)(const Solid::Vec4 &) const noexcept>(& Vec4::operator-)), static_cast<rfk::EMethodFlags>(513));	\
	method->parameters.reserve(1); method->addParameter("_vec", rfk::Type::getType<const Solid::Vec4 &>());	\
	method = type.addMethod("operator*", 6926593734186904028u, rfk::Type::getType<Solid::Vec4>(), std::make_unique<rfk::MemberFunction<Vec4, Solid::Vec4 (const Solid::Vec4 &)>>(static_cast<Solid::Vec4 (Vec4::*)(const Solid::Vec4 &) const noexcept>(& Vec4::operator*)), static_cast<rfk::EMethodFlags>(513));	\
	method->parameters.reserve(1); method->addParameter("_vec", rfk::Type::getType<const Solid::Vec4 &>());	\
	method = type.addMethod("operator/", 10823321459138383089u, rfk::Type::getType<Solid::Vec4>(), std::make_unique<rfk::MemberFunction<Vec4, Solid::Vec4 (const Solid::Vec4 &)>>(static_cast<Solid::Vec4 (Vec4::*)(const Solid::Vec4 &) const noexcept>(& Vec4::operator/)), static_cast<rfk::EMethodFlags>(513));	\
	method->parameters.reserve(1); method->addParameter("_vec", rfk::Type::getType<const Solid::Vec4 &>());	\
	method = type.addMethod("operator+=", 4755110937827232183u, rfk::Type::getType<Solid::Vec4 &>(), std::make_unique<rfk::MemberFunction<Vec4, Solid::Vec4 &(const Solid::Vec4 &)>>(static_cast<Solid::Vec4 &(Vec4::*)(const Solid::Vec4 &) noexcept>(& Vec4::operator+=)), static_cast<rfk::EMethodFlags>(1));	\
	method->parameters.reserve(1); method->addParameter("_vec", rfk::Type::getType<const Solid::Vec4 &>());	\
	method = type.addMethod("operator-=", 2219291072226939397u, rfk::Type::getType<Solid::Vec4 &>(), std::make_unique<rfk::MemberFunction<Vec4, Solid::Vec4 &(const Solid::Vec4 &)>>(static_cast<Solid::Vec4 &(Vec4::*)(const Solid::Vec4 &) noexcept>(& Vec4::operator-=)), static_cast<rfk::EMethodFlags>(1));	\
	method->parameters.reserve(1); method->addParameter("_vec", rfk::Type::getType<const Solid::Vec4 &>());	\
	method = type.addMethod("operator*=", 4155374745039212142u, rfk::Type::getType<Solid::Vec4 &>(), std::make_unique<rfk::MemberFunction<Vec4, Solid::Vec4 &(const Solid::Vec4 &)>>(static_cast<Solid::Vec4 &(Vec4::*)(const Solid::Vec4 &) noexcept>(& Vec4::operator*=)), static_cast<rfk::EMethodFlags>(1));	\
	method->parameters.reserve(1); method->addParameter("_vec", rfk::Type::getType<const Solid::Vec4 &>());	\
	method = type.addMethod("operator/=", 7920043065833845163u, rfk::Type::getType<Solid::Vec4 &>(), std::make_unique<rfk::MemberFunction<Vec4, Solid::Vec4 &(const Solid::Vec4 &)>>(static_cast<Solid::Vec4 &(Vec4::*)(const Solid::Vec4 &) noexcept>(& Vec4::operator/=)), static_cast<rfk::EMethodFlags>(1));	\
	method->parameters.reserve(1); method->addParameter("_vec", rfk::Type::getType<const Solid::Vec4 &>());	\
	method = type.addMethod("operator+", 5623072866364536160u, rfk::Type::getType<Solid::Vec4>(), std::make_unique<rfk::MemberFunction<Vec4, Solid::Vec4 (float)>>(static_cast<Solid::Vec4 (Vec4::*)(float) const noexcept>(& Vec4::operator+)), static_cast<rfk::EMethodFlags>(513));	\
	method->parameters.reserve(1); method->addParameter("_value", rfk::Type::getType<float>());	\
	method = type.addMethod("operator-", 3683477500284294378u, rfk::Type::getType<Solid::Vec4>(), std::make_unique<rfk::MemberFunction<Vec4, Solid::Vec4 (float)>>(static_cast<Solid::Vec4 (Vec4::*)(float) const noexcept>(& Vec4::operator-)), static_cast<rfk::EMethodFlags>(513));	\
	method->parameters.reserve(1); method->addParameter("_value", rfk::Type::getType<float>());	\
	method = type.addMethod("operator*", 94661077593167243u, rfk::Type::getType<Solid::Vec4>(), std::make_unique<rfk::MemberFunction<Vec4, Solid::Vec4 (float)>>(static_cast<Solid::Vec4 (Vec4::*)(float) const noexcept>(& Vec4::operator*)), static_cast<rfk::EMethodFlags>(513));	\
	method->parameters.reserve(1); method->addParameter("_value", rfk::Type::getType<float>());	\
	method = type.addMethod("operator/", 8165887739729411780u, rfk::Type::getType<Solid::Vec4>(), std::make_unique<rfk::MemberFunction<Vec4, Solid::Vec4 (float)>>(static_cast<Solid::Vec4 (Vec4::*)(float) const noexcept>(& Vec4::operator/)), static_cast<rfk::EMethodFlags>(513));	\
	method->parameters.reserve(1); method->addParameter("_value", rfk::Type::getType<float>());	\
	method = type.addMethod("operator+=", 12779327112649547350u, rfk::Type::getType<Solid::Vec4 &>(), std::make_unique<rfk::MemberFunction<Vec4, Solid::Vec4 &(float)>>(static_cast<Solid::Vec4 &(Vec4::*)(float) noexcept>(& Vec4::operator+=)), static_cast<rfk::EMethodFlags>(1));	\
	method->parameters.reserve(1); method->addParameter("_value", rfk::Type::getType<float>());	\
	method = type.addMethod("operator-=", 12022757730393756024u, rfk::Type::getType<Solid::Vec4 &>(), std::make_unique<rfk::MemberFunction<Vec4, Solid::Vec4 &(float)>>(static_cast<Solid::Vec4 &(Vec4::*)(float) noexcept>(& Vec4::operator-=)), static_cast<rfk::EMethodFlags>(1));	\
	method->parameters.reserve(1); method->addParameter("_value", rfk::Type::getType<float>());	\
	method = type.addMethod("operator*=", 15319225490224361449u, rfk::Type::getType<Solid::Vec4 &>(), std::make_unique<rfk::MemberFunction<Vec4, Solid::Vec4 &(float)>>(static_cast<Solid::Vec4 &(Vec4::*)(float) noexcept>(& Vec4::operator*=)), static_cast<rfk::EMethodFlags>(1));	\
	method->parameters.reserve(1); method->addParameter("_value", rfk::Type::getType<float>());	\
	method = type.addMethod("operator/=", 1023836847506260178u, rfk::Type::getType<Solid::Vec4 &>(), std::make_unique<rfk::MemberFunction<Vec4, Solid::Vec4 &(float)>>(static_cast<Solid::Vec4 &(Vec4::*)(float) noexcept>(& Vec4::operator/=)), static_cast<rfk::EMethodFlags>(1));	\
	method->parameters.reserve(1); method->addParameter("_value", rfk::Type::getType<float>());	\
	method = type.addMethod("operator-", 6740941589890806788u, rfk::Type::getType<Solid::Vec4>(), std::make_unique<rfk::MemberFunction<Vec4, Solid::Vec4 ()>>(static_cast<Solid::Vec4 (Vec4::*)() noexcept>(& Vec4::operator-)), static_cast<rfk::EMethodFlags>(1));	\

#define __RFK457194334985863962u_GenerateArchetypeProperties	\
	

#define __RFK457194334985863962u_GenerateDefaultInstantiatorSetup	\
	type.setDefaultInstantiator(&rfk::defaultInstantiator<Vec4>);

#define __RFK457194334985863962u_GetTypeDeclaration	\
	__RFK457194334985863962u_GenerateFieldHelperMethods	\
	public:	\
		inline static rfk::Class const& staticGetArchetype() noexcept;	\
	

#ifdef KODGEN_PARSING
#define __RFK457194334985863962u_GetTypeDefinition
#else
#define __RFK457194334985863962u_GetTypeDefinition	\
		inline rfk::Class const& Solid::Vec4::staticGetArchetype() noexcept	\
		{	\
			static bool			initialized = false;	\
			static rfk::Class	type("Vec4", 457194334985863962u, sizeof(Vec4));	\
			\
			if (!initialized)	\
			{	\
				initialized = true;	\
			\
				__RFK457194334985863962u_GenerateArchetypeProperties	\
					\
					\
				__RFK457194334985863962u_GenerateFieldsMetadata	\
				__RFK457194334985863962u_GenerateDefaultInstantiatorSetup	\
				__RFK457194334985863962u_GenerateMethodsMetadata	\
			}	\
			\
			return type;	\
		}

#endif
#define __RFK457194334985863962u_RegisterArchetype	\
	private:	\
		static inline rfk::ArchetypeRegisterer __rfkArchetypeRegisterer;


#ifdef KODGEN_PARSING
#define Vec4_GENERATED
#else
#define Vec4_GENERATED	\
	friend rfk::Struct;	\
	friend rfk::hasField___rfkArchetypeRegisterer<Vec4, rfk::ArchetypeRegisterer>;	\
	__RFK457194334985863962u_GetTypeDeclaration	\
	__RFK457194334985863962u_RegisterArchetype	\
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
	__RFK17241326416511343721u_GetTypeDefinition	\
	__RFK457194334985863962u_GetTypeDefinition	\
	__RFK3698706698196451833u_GENERATED	\
	__RFKNativeProperties_GENERATED	\


