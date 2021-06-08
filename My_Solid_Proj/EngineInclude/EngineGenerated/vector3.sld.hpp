#pragma once

/**
*	Source file: D:\Project\SolidEngine\SolidEngine\Include\Core\Maths\Vector\vector3.hpp
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
#define __RFK3698706698196451833u_DeclareGetNamespaceFragment inline rfk::NamespaceFragment const& getNamespaceFragment3698706698196451833u_14616602350178543883() noexcept;

#ifdef __RFK3698706698196451833u_DefineGetNamespaceFragment
	#undef __RFK3698706698196451833u_DefineGetNamespaceFragment
#endif
#define __RFK3698706698196451833u_DefineGetNamespaceFragment	\
	inline rfk::NamespaceFragment const& getNamespaceFragment3698706698196451833u_14616602350178543883() noexcept	\
	{	\
		static rfk::NamespaceFragment	fragment("Solid", 3698706698196451833u);	\
		static bool						initialized = false;	\
		if (!initialized)	\
		{	\
			initialized = true;	\
			fragment.nestedEntities.reserve(3u); rfk::NamespaceFragment* fragmentPtr = &fragment; fragmentPtr->addNestedEntity(&Solid::Vec3i::staticGetArchetype())->addNestedEntity(&Solid::Vec3ui::staticGetArchetype())->addNestedEntity(&Solid::Vec3::staticGetArchetype());	\
		}	\
	return fragment;	\
	}

#ifdef __RFK3698706698196451833u_RegisterNamespace
	#undef __RFK3698706698196451833u_RegisterNamespace
#endif
#define __RFK3698706698196451833u_RegisterNamespace	\
	inline rfk::NamespaceFragmentRegisterer namespaceFragmentRegisterer3698706698196451833u_14616602350178543883 = rfk::NamespaceFragmentRegisterer("Solid", 3698706698196451833u, &getNamespaceFragment3698706698196451833u_14616602350178543883(), true);

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

#define __RFK17236541341906315574u_GenerateFieldsMetadata	\
	registerChild<Vec3i>(&type);	\

#define __RFK17236541341906315574u_GenerateFieldHelperMethods	\
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
		field = childArchetype->addField("x", 15803245377365923727u, rfk::Type::getType<int>(), static_cast<rfk::EFieldFlags>(1), &thisArchetype, offsetof(ChildType, x));	\
		field = childArchetype->addField("y", 15803244277854295516u, rfk::Type::getType<int>(), static_cast<rfk::EFieldFlags>(1), &thisArchetype, offsetof(ChildType, y));	\
		field = childArchetype->addField("z", 15803247576389180149u, rfk::Type::getType<int>(), static_cast<rfk::EFieldFlags>(1), &thisArchetype, offsetof(ChildType, z));	\
		__RFK_DISABLE_WARNING_POP	\
	}

#define __RFK17236541341906315574u_GenerateMethodsMetadata	\

#define __RFK17236541341906315574u_GenerateArchetypeProperties	\
	

#define __RFK17236541341906315574u_GenerateDefaultInstantiatorSetup	\
	type.setDefaultInstantiator(&rfk::defaultInstantiator<Vec3i>);

#define __RFK17236541341906315574u_GetTypeDeclaration	\
	__RFK17236541341906315574u_GenerateFieldHelperMethods	\
	public:	\
		inline static rfk::Struct const& staticGetArchetype() noexcept;	\
	

#ifdef KODGEN_PARSING
#define __RFK17236541341906315574u_GetTypeDefinition
#else
#define __RFK17236541341906315574u_GetTypeDefinition	\
		inline rfk::Struct const& Solid::Vec3i::staticGetArchetype() noexcept	\
		{	\
			static bool			initialized = false;	\
			static rfk::Struct	type("Vec3i", 17236541341906315574u, sizeof(Vec3i));	\
			\
			if (!initialized)	\
			{	\
				initialized = true;	\
			\
				__RFK17236541341906315574u_GenerateArchetypeProperties	\
					\
					\
				__RFK17236541341906315574u_GenerateFieldsMetadata	\
				__RFK17236541341906315574u_GenerateDefaultInstantiatorSetup	\
				__RFK17236541341906315574u_GenerateMethodsMetadata	\
			}	\
			\
			return type;	\
		}

#endif
#define __RFK17236541341906315574u_RegisterArchetype	\
	private:	\
		static inline rfk::ArchetypeRegisterer __rfkArchetypeRegisterer;


#ifdef KODGEN_PARSING
#define Vec3i_GENERATED
#else
#define Vec3i_GENERATED	\
	friend rfk::Struct;	\
	friend rfk::hasField___rfkArchetypeRegisterer<Vec3i, rfk::ArchetypeRegisterer>;	\
	__RFK17236541341906315574u_GetTypeDeclaration	\
	__RFK17236541341906315574u_RegisterArchetype	\
		\
	private:

#endif

#define __RFK2256039419379501849u_GenerateFieldsMetadata	\
	registerChild<Vec3ui>(&type);	\

#define __RFK2256039419379501849u_GenerateFieldHelperMethods	\
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
		field = childArchetype->addField("x", 6383110448426822482u, rfk::Type::getType<uint>(), static_cast<rfk::EFieldFlags>(1), &thisArchetype, offsetof(ChildType, x));	\
		field = childArchetype->addField("y", 6383111547938450693u, rfk::Type::getType<uint>(), static_cast<rfk::EFieldFlags>(1), &thisArchetype, offsetof(ChildType, y));	\
		field = childArchetype->addField("z", 6383108249403566060u, rfk::Type::getType<uint>(), static_cast<rfk::EFieldFlags>(1), &thisArchetype, offsetof(ChildType, z));	\
		__RFK_DISABLE_WARNING_POP	\
	}

#define __RFK2256039419379501849u_GenerateMethodsMetadata	\

#define __RFK2256039419379501849u_GenerateArchetypeProperties	\
	

#define __RFK2256039419379501849u_GenerateDefaultInstantiatorSetup	\
	type.setDefaultInstantiator(&rfk::defaultInstantiator<Vec3ui>);

#define __RFK2256039419379501849u_GetTypeDeclaration	\
	__RFK2256039419379501849u_GenerateFieldHelperMethods	\
	public:	\
		inline static rfk::Struct const& staticGetArchetype() noexcept;	\
	

#ifdef KODGEN_PARSING
#define __RFK2256039419379501849u_GetTypeDefinition
#else
#define __RFK2256039419379501849u_GetTypeDefinition	\
		inline rfk::Struct const& Solid::Vec3ui::staticGetArchetype() noexcept	\
		{	\
			static bool			initialized = false;	\
			static rfk::Struct	type("Vec3ui", 2256039419379501849u, sizeof(Vec3ui));	\
			\
			if (!initialized)	\
			{	\
				initialized = true;	\
			\
				__RFK2256039419379501849u_GenerateArchetypeProperties	\
					\
					\
				__RFK2256039419379501849u_GenerateFieldsMetadata	\
				__RFK2256039419379501849u_GenerateDefaultInstantiatorSetup	\
				__RFK2256039419379501849u_GenerateMethodsMetadata	\
			}	\
			\
			return type;	\
		}

#endif
#define __RFK2256039419379501849u_RegisterArchetype	\
	private:	\
		static inline rfk::ArchetypeRegisterer __rfkArchetypeRegisterer;


#ifdef KODGEN_PARSING
#define Vec3ui_GENERATED
#else
#define Vec3ui_GENERATED	\
	friend rfk::Struct;	\
	friend rfk::hasField___rfkArchetypeRegisterer<Vec3ui, rfk::ArchetypeRegisterer>;	\
	__RFK2256039419379501849u_GetTypeDeclaration	\
	__RFK2256039419379501849u_RegisterArchetype	\
		\
	private:

#endif

#define __RFK457188837427722907u_GenerateFieldsMetadata	\
	registerChild<Vec3>(&type);	\

#define __RFK457188837427722907u_GenerateFieldHelperMethods	\
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
		field = childArchetype->addField("x", 2960420184111765116u, rfk::Type::getType<float>(), static_cast<rfk::EFieldFlags>(1), &thisArchetype, offsetof(ChildType, x));	\
		field = childArchetype->addField("y", 2960421283623393327u, rfk::Type::getType<float>(), static_cast<rfk::EFieldFlags>(1), &thisArchetype, offsetof(ChildType, y));	\
		field = childArchetype->addField("z", 2960422383135021538u, rfk::Type::getType<float>(), static_cast<rfk::EFieldFlags>(1), &thisArchetype, offsetof(ChildType, z));	\
		__RFK_DISABLE_WARNING_POP	\
	}

#define __RFK457188837427722907u_GenerateMethodsMetadata	\
	[[maybe_unused]] rfk::Method*		method			= nullptr;	\
	[[maybe_unused]] rfk::StaticMethod*	staticMethod	= nullptr;	\
	staticMethod = type.addStaticMethod("Dot", 1117499686329169608u, rfk::Type::getType<float>(), std::make_unique<rfk::NonMemberFunction<float (const Solid::Vec3 &, const Solid::Vec3 &)>>(static_cast<float (*)(const Solid::Vec3 &, const Solid::Vec3 &) noexcept>(& Vec3::Dot)), static_cast<rfk::EMethodFlags>(9));	\
	staticMethod->parameters.reserve(2); staticMethod->addParameter("_v1", rfk::Type::getType<const Solid::Vec3 &>())->addParameter("_v2", rfk::Type::getType<const Solid::Vec3 &>());	\
	staticMethod = type.addStaticMethod("Cross", 9640493273862892389u, rfk::Type::getType<Solid::Vec3>(), std::make_unique<rfk::NonMemberFunction<Solid::Vec3 (const Solid::Vec3 &, const Solid::Vec3 &)>>(static_cast<Solid::Vec3 (*)(const Solid::Vec3 &, const Solid::Vec3 &) noexcept>(& Vec3::Cross)), static_cast<rfk::EMethodFlags>(9));	\
	staticMethod->parameters.reserve(2); staticMethod->addParameter("_v1", rfk::Type::getType<const Solid::Vec3 &>())->addParameter("_v2", rfk::Type::getType<const Solid::Vec3 &>());	\
	staticMethod = type.addStaticMethod("Lerp", 10892778443901000769u, rfk::Type::getType<Solid::Vec3>(), std::make_unique<rfk::NonMemberFunction<Solid::Vec3 (const Solid::Vec3 &, const Solid::Vec3 &, float)>>(static_cast<Solid::Vec3 (*)(const Solid::Vec3 &, const Solid::Vec3 &, float) noexcept>(& Vec3::Lerp)), static_cast<rfk::EMethodFlags>(9));	\
	staticMethod->parameters.reserve(3); staticMethod->addParameter("_v1", rfk::Type::getType<const Solid::Vec3 &>())->addParameter("_v2", rfk::Type::getType<const Solid::Vec3 &>())->addParameter("_r", rfk::Type::getType<float>());	\
	staticMethod = type.addStaticMethod("Nlerp", 13479637576480160733u, rfk::Type::getType<Solid::Vec3>(), std::make_unique<rfk::NonMemberFunction<Solid::Vec3 (const Solid::Vec3 &, const Solid::Vec3 &, float)>>(static_cast<Solid::Vec3 (*)(const Solid::Vec3 &, const Solid::Vec3 &, float) noexcept>(& Vec3::Nlerp)), static_cast<rfk::EMethodFlags>(9));	\
	staticMethod->parameters.reserve(3); staticMethod->addParameter("_v1", rfk::Type::getType<const Solid::Vec3 &>())->addParameter("_v2", rfk::Type::getType<const Solid::Vec3 &>())->addParameter("_r", rfk::Type::getType<float>());	\
	staticMethod = type.addStaticMethod("Slerp", 5192549440138952164u, rfk::Type::getType<Solid::Vec3>(), std::make_unique<rfk::NonMemberFunction<Solid::Vec3 (const Solid::Vec3 &, const Solid::Vec3 &, float)>>(static_cast<Solid::Vec3 (*)(const Solid::Vec3 &, const Solid::Vec3 &, float) noexcept>(& Vec3::Slerp)), static_cast<rfk::EMethodFlags>(9));	\
	staticMethod->parameters.reserve(3); staticMethod->addParameter("_v1", rfk::Type::getType<const Solid::Vec3 &>())->addParameter("_v2", rfk::Type::getType<const Solid::Vec3 &>())->addParameter("_r", rfk::Type::getType<float>());	\
	method = type.addMethod("SqrtLength", 5956780393558970597u, rfk::Type::getType<float>(), std::make_unique<rfk::MemberFunction<Vec3, float ()>>(static_cast<float (Vec3::*)() const noexcept>(& Vec3::SqrtLength)), static_cast<rfk::EMethodFlags>(513));	\
	method = type.addMethod("Length", 2924266761445671791u, rfk::Type::getType<float>(), std::make_unique<rfk::MemberFunction<Vec3, float ()>>(static_cast<float (Vec3::*)() const noexcept>(& Vec3::Length)), static_cast<rfk::EMethodFlags>(513));	\
	method = type.addMethod("Scale", 8672222127300255631u, rfk::Type::getType<Solid::Vec3 &>(), std::make_unique<rfk::MemberFunction<Vec3, Solid::Vec3 &(float)>>(static_cast<Solid::Vec3 &(Vec3::*)(float) noexcept>(& Vec3::Scale)), static_cast<rfk::EMethodFlags>(1));	\
	method->parameters.reserve(1); method->addParameter("_scale", rfk::Type::getType<float>());	\
	method = type.addMethod("GetScaled", 3953381960707688894u, rfk::Type::getType<Solid::Vec3>(), std::make_unique<rfk::MemberFunction<Vec3, Solid::Vec3 (float)>>(static_cast<Solid::Vec3 (Vec3::*)(float) const noexcept>(& Vec3::GetScaled)), static_cast<rfk::EMethodFlags>(513));	\
	method->parameters.reserve(1); method->addParameter("_scale", rfk::Type::getType<float>());	\
	method = type.addMethod("Unscale", 3246223314887295508u, rfk::Type::getType<Solid::Vec3 &>(), std::make_unique<rfk::MemberFunction<Vec3, Solid::Vec3 &(float)>>(static_cast<Solid::Vec3 &(Vec3::*)(float) noexcept>(& Vec3::Unscale)), static_cast<rfk::EMethodFlags>(1));	\
	method->parameters.reserve(1); method->addParameter("_scale", rfk::Type::getType<float>());	\
	method = type.addMethod("GetUnscaled", 1284838920733307689u, rfk::Type::getType<Solid::Vec3>(), std::make_unique<rfk::MemberFunction<Vec3, Solid::Vec3 (float)>>(static_cast<Solid::Vec3 (Vec3::*)(float) const noexcept>(& Vec3::GetUnscaled)), static_cast<rfk::EMethodFlags>(513));	\
	method->parameters.reserve(1); method->addParameter("_scale", rfk::Type::getType<float>());	\
	method = type.addMethod("Normalize", 4315468786635418795u, rfk::Type::getType<Solid::Vec3 &>(), std::make_unique<rfk::MemberFunction<Vec3, Solid::Vec3 &()>>(static_cast<Solid::Vec3 &(Vec3::*)() noexcept>(& Vec3::Normalize)), static_cast<rfk::EMethodFlags>(1));	\
	method = type.addMethod("GetNormalized", 10838364964325153732u, rfk::Type::getType<Solid::Vec3>(), std::make_unique<rfk::MemberFunction<Vec3, Solid::Vec3 ()>>(static_cast<Solid::Vec3 (Vec3::*)() const noexcept>(& Vec3::GetNormalized)), static_cast<rfk::EMethodFlags>(513));	\
	method = type.addMethod("IsNormalized", 3150800047109792994u, rfk::Type::getType<bool>(), std::make_unique<rfk::MemberFunction<Vec3, bool ()>>(static_cast<bool (Vec3::*)() const noexcept>(& Vec3::IsNormalized)), static_cast<rfk::EMethodFlags>(513));	\
	method = type.addMethod("IsEquals", 18182574237998154701u, rfk::Type::getType<bool>(), std::make_unique<rfk::MemberFunction<Vec3, bool (const Solid::Vec3 &)>>(static_cast<bool (Vec3::*)(const Solid::Vec3 &) const noexcept>(& Vec3::IsEquals)), static_cast<rfk::EMethodFlags>(513));	\
	method->parameters.reserve(1); method->addParameter("vec", rfk::Type::getType<const Solid::Vec3 &>());	\
	method = type.addMethod("Dist", 9038260175200964854u, rfk::Type::getType<float>(), std::make_unique<rfk::MemberFunction<Vec3, float (const Solid::Vec3 &)>>(static_cast<float (Vec3::*)(const Solid::Vec3 &) const noexcept>(& Vec3::Dist)), static_cast<rfk::EMethodFlags>(513));	\
	method->parameters.reserve(1); method->addParameter("vec", rfk::Type::getType<const Solid::Vec3 &>());	\
	method = type.addMethod("SqrtDist", 5588852124869503864u, rfk::Type::getType<float>(), std::make_unique<rfk::MemberFunction<Vec3, float (const Solid::Vec3 &)>>(static_cast<float (Vec3::*)(const Solid::Vec3 &) const noexcept>(& Vec3::SqrtDist)), static_cast<rfk::EMethodFlags>(513));	\
	method->parameters.reserve(1); method->addParameter("vec", rfk::Type::getType<const Solid::Vec3 &>());	\
	method = type.addMethod("ToString", 7509060944406244339u, rfk::Type::getType<const std::string>(), std::make_unique<rfk::MemberFunction<Vec3, const std::string ()>>(static_cast<const std::string (Vec3::*)() const noexcept>(& Vec3::ToString)), static_cast<rfk::EMethodFlags>(513));	\
	method = type.addMethod("operator=", 12418719839771213990u, rfk::Type::getType<Solid::Vec3 &>(), std::make_unique<rfk::MemberFunction<Vec3, Solid::Vec3 &(const Solid::Vec3 &)>>(static_cast<Solid::Vec3 &(Vec3::*)(const Solid::Vec3 &) noexcept>(& Vec3::operator=)), static_cast<rfk::EMethodFlags>(17));	\
	method->parameters.reserve(1); method->addParameter("vec", rfk::Type::getType<const Solid::Vec3 &>());	\
	method = type.addMethod("operator==", 7617415144673152148u, rfk::Type::getType<bool>(), std::make_unique<rfk::MemberFunction<Vec3, bool (const Solid::Vec3 &)>>(static_cast<bool (Vec3::*)(const Solid::Vec3 &) const noexcept>(& Vec3::operator==)), static_cast<rfk::EMethodFlags>(513));	\
	method->parameters.reserve(1); method->addParameter("_vec", rfk::Type::getType<const Solid::Vec3 &>());	\
	method = type.addMethod("operator!=", 4611420084985619456u, rfk::Type::getType<bool>(), std::make_unique<rfk::MemberFunction<Vec3, bool (const Solid::Vec3 &)>>(static_cast<bool (Vec3::*)(const Solid::Vec3 &) const noexcept>(& Vec3::operator!=)), static_cast<rfk::EMethodFlags>(513));	\
	method->parameters.reserve(1); method->addParameter("_vec", rfk::Type::getType<const Solid::Vec3 &>());	\
	method = type.addMethod("operator+", 7227352792311120903u, rfk::Type::getType<Solid::Vec3>(), std::make_unique<rfk::MemberFunction<Vec3, Solid::Vec3 (const Solid::Vec3 &)>>(static_cast<Solid::Vec3 (Vec3::*)(const Solid::Vec3 &) const noexcept>(& Vec3::operator+)), static_cast<rfk::EMethodFlags>(513));	\
	method->parameters.reserve(1); method->addParameter("_vec", rfk::Type::getType<const Solid::Vec3 &>());	\
	method = type.addMethod("operator-", 14877630687957603365u, rfk::Type::getType<Solid::Vec3>(), std::make_unique<rfk::MemberFunction<Vec3, Solid::Vec3 (const Solid::Vec3 &)>>(static_cast<Solid::Vec3 (Vec3::*)(const Solid::Vec3 &) const noexcept>(& Vec3::operator-)), static_cast<rfk::EMethodFlags>(513));	\
	method->parameters.reserve(1); method->addParameter("_vec", rfk::Type::getType<const Solid::Vec3 &>());	\
	method = type.addMethod("operator*", 6988544346107729766u, rfk::Type::getType<Solid::Vec3>(), std::make_unique<rfk::MemberFunction<Vec3, Solid::Vec3 (const Solid::Vec3 &)>>(static_cast<Solid::Vec3 (Vec3::*)(const Solid::Vec3 &) const noexcept>(& Vec3::operator*)), static_cast<rfk::EMethodFlags>(513));	\
	method->parameters.reserve(1); method->addParameter("_vec", rfk::Type::getType<const Solid::Vec3 &>());	\
	method = type.addMethod("operator/", 16930161902357858163u, rfk::Type::getType<Solid::Vec3>(), std::make_unique<rfk::MemberFunction<Vec3, Solid::Vec3 (const Solid::Vec3 &)>>(static_cast<Solid::Vec3 (Vec3::*)(const Solid::Vec3 &) const noexcept>(& Vec3::operator/)), static_cast<rfk::EMethodFlags>(513));	\
	method->parameters.reserve(1); method->addParameter("_vec", rfk::Type::getType<const Solid::Vec3 &>());	\
	method = type.addMethod("operator+=", 14984821058012562967u, rfk::Type::getType<Solid::Vec3 &>(), std::make_unique<rfk::MemberFunction<Vec3, Solid::Vec3 &(const Solid::Vec3 &)>>(static_cast<Solid::Vec3 &(Vec3::*)(const Solid::Vec3 &) noexcept>(& Vec3::operator+=)), static_cast<rfk::EMethodFlags>(1));	\
	method->parameters.reserve(1); method->addParameter("_vec", rfk::Type::getType<const Solid::Vec3 &>());	\
	method = type.addMethod("operator-=", 12733552240655336445u, rfk::Type::getType<Solid::Vec3 &>(), std::make_unique<rfk::MemberFunction<Vec3, Solid::Vec3 &(const Solid::Vec3 &)>>(static_cast<Solid::Vec3 &(Vec3::*)(const Solid::Vec3 &) noexcept>(& Vec3::operator-=)), static_cast<rfk::EMethodFlags>(1));	\
	method->parameters.reserve(1); method->addParameter("_vec", rfk::Type::getType<const Solid::Vec3 &>());	\
	method = type.addMethod("operator*=", 10795551019563842166u, rfk::Type::getType<Solid::Vec3 &>(), std::make_unique<rfk::MemberFunction<Vec3, Solid::Vec3 &(const Solid::Vec3 &)>>(static_cast<Solid::Vec3 &(Vec3::*)(const Solid::Vec3 &) noexcept>(& Vec3::operator*=)), static_cast<rfk::EMethodFlags>(1));	\
	method->parameters.reserve(1); method->addParameter("_vec", rfk::Type::getType<const Solid::Vec3 &>());	\
	method = type.addMethod("operator/=", 13541396342218465851u, rfk::Type::getType<Solid::Vec3 &>(), std::make_unique<rfk::MemberFunction<Vec3, Solid::Vec3 &(const Solid::Vec3 &)>>(static_cast<Solid::Vec3 &(Vec3::*)(const Solid::Vec3 &) noexcept>(& Vec3::operator/=)), static_cast<rfk::EMethodFlags>(1));	\
	method->parameters.reserve(1); method->addParameter("_vec", rfk::Type::getType<const Solid::Vec3 &>());	\
	method = type.addMethod("operator+", 4385152115625699301u, rfk::Type::getType<Solid::Vec3>(), std::make_unique<rfk::MemberFunction<Vec3, Solid::Vec3 (float)>>(static_cast<Solid::Vec3 (Vec3::*)(float) const noexcept>(& Vec3::operator+)), static_cast<rfk::EMethodFlags>(513));	\
	method->parameters.reserve(1); method->addParameter("_value", rfk::Type::getType<float>());	\
	method = type.addMethod("operator-", 6329092465208593035u, rfk::Type::getType<Solid::Vec3>(), std::make_unique<rfk::MemberFunction<Vec3, Solid::Vec3 (float)>>(static_cast<Solid::Vec3 (Vec3::*)(float) const noexcept>(& Vec3::operator-)), static_cast<rfk::EMethodFlags>(513));	\
	method->parameters.reserve(1); method->addParameter("_value", rfk::Type::getType<float>());	\
	method = type.addMethod("operator*", 9917908887899720170u, rfk::Type::getType<Solid::Vec3>(), std::make_unique<rfk::MemberFunction<Vec3, Solid::Vec3 (float)>>(static_cast<Solid::Vec3 (Vec3::*)(float) const noexcept>(& Vec3::operator*)), static_cast<rfk::EMethodFlags>(513));	\
	method->parameters.reserve(1); method->addParameter("_value", rfk::Type::getType<float>());	\
	method = type.addMethod("operator/", 14949061178082896177u, rfk::Type::getType<Solid::Vec3>(), std::make_unique<rfk::MemberFunction<Vec3, Solid::Vec3 (float)>>(static_cast<Solid::Vec3 (Vec3::*)(float) const noexcept>(& Vec3::operator/)), static_cast<rfk::EMethodFlags>(513));	\
	method->parameters.reserve(1); method->addParameter("_value", rfk::Type::getType<float>());	\
	method = type.addMethod("operator+=", 5430664548329726995u, rfk::Type::getType<Solid::Vec3 &>(), std::make_unique<rfk::MemberFunction<Vec3, Solid::Vec3 &(float)>>(static_cast<Solid::Vec3 &(Vec3::*)(float) noexcept>(& Vec3::operator+=)), static_cast<rfk::EMethodFlags>(1));	\
	method->parameters.reserve(1); method->addParameter("_value", rfk::Type::getType<float>());	\
	method = type.addMethod("operator-=", 3106912804130235625u, rfk::Type::getType<Solid::Vec3 &>(), std::make_unique<rfk::MemberFunction<Vec3, Solid::Vec3 &(float)>>(static_cast<Solid::Vec3 &(Vec3::*)(float) noexcept>(& Vec3::operator-=)), static_cast<rfk::EMethodFlags>(1));	\
	method->parameters.reserve(1); method->addParameter("_value", rfk::Type::getType<float>());	\
	method = type.addMethod("operator*=", 18257189118009181816u, rfk::Type::getType<Solid::Vec3 &>(), std::make_unique<rfk::MemberFunction<Vec3, Solid::Vec3 &(float)>>(static_cast<Solid::Vec3 &(Vec3::*)(float) noexcept>(& Vec3::operator*=)), static_cast<rfk::EMethodFlags>(1));	\
	method->parameters.reserve(1); method->addParameter("_value", rfk::Type::getType<float>());	\
	method = type.addMethod("operator/=", 16941271583686301847u, rfk::Type::getType<Solid::Vec3 &>(), std::make_unique<rfk::MemberFunction<Vec3, Solid::Vec3 &(float)>>(static_cast<Solid::Vec3 &(Vec3::*)(float) noexcept>(& Vec3::operator/=)), static_cast<rfk::EMethodFlags>(1));	\
	method->parameters.reserve(1); method->addParameter("_value", rfk::Type::getType<float>());	\
	method = type.addMethod("operator-", 4793718344322842671u, rfk::Type::getType<Solid::Vec3>(), std::make_unique<rfk::MemberFunction<Vec3, Solid::Vec3 ()>>(static_cast<Solid::Vec3 (Vec3::*)() noexcept>(& Vec3::operator-)), static_cast<rfk::EMethodFlags>(1));	\

#define __RFK457188837427722907u_GenerateArchetypeProperties	\
	

#define __RFK457188837427722907u_GenerateDefaultInstantiatorSetup	\
	type.setDefaultInstantiator(&rfk::defaultInstantiator<Vec3>);

#define __RFK457188837427722907u_GetTypeDeclaration	\
	__RFK457188837427722907u_GenerateFieldHelperMethods	\
	public:	\
		inline static rfk::Class const& staticGetArchetype() noexcept;	\
	

#ifdef KODGEN_PARSING
#define __RFK457188837427722907u_GetTypeDefinition
#else
#define __RFK457188837427722907u_GetTypeDefinition	\
		inline rfk::Class const& Solid::Vec3::staticGetArchetype() noexcept	\
		{	\
			static bool			initialized = false;	\
			static rfk::Class	type("Vec3", 457188837427722907u, sizeof(Vec3));	\
			\
			if (!initialized)	\
			{	\
				initialized = true;	\
			\
				__RFK457188837427722907u_GenerateArchetypeProperties	\
					\
					\
				__RFK457188837427722907u_GenerateFieldsMetadata	\
				__RFK457188837427722907u_GenerateDefaultInstantiatorSetup	\
				__RFK457188837427722907u_GenerateMethodsMetadata	\
			}	\
			\
			return type;	\
		}

#endif
#define __RFK457188837427722907u_RegisterArchetype	\
	private:	\
		static inline rfk::ArchetypeRegisterer __rfkArchetypeRegisterer;


#ifdef KODGEN_PARSING
#define Vec3_GENERATED
#else
#define Vec3_GENERATED	\
	friend rfk::Struct;	\
	friend rfk::hasField___rfkArchetypeRegisterer<Vec3, rfk::ArchetypeRegisterer>;	\
	__RFK457188837427722907u_GetTypeDeclaration	\
	__RFK457188837427722907u_RegisterArchetype	\
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
	__RFK17236541341906315574u_GetTypeDefinition	\
	__RFK2256039419379501849u_GetTypeDefinition	\
	__RFK457188837427722907u_GetTypeDefinition	\
	__RFK3698706698196451833u_GENERATED	\
	__RFKNativeProperties_GENERATED	\


