#pragma once

/**
*	Source file: D:\Project\SolidEngine\SolidEngine\Include\Core\Maths\Quaternion\quaternion.hpp
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
#define __RFK3698706698196451833u_DeclareGetNamespaceFragment inline rfk::NamespaceFragment const& getNamespaceFragment3698706698196451833u_15743786790436941404() noexcept;

#ifdef __RFK3698706698196451833u_DefineGetNamespaceFragment
	#undef __RFK3698706698196451833u_DefineGetNamespaceFragment
#endif
#define __RFK3698706698196451833u_DefineGetNamespaceFragment	\
	inline rfk::NamespaceFragment const& getNamespaceFragment3698706698196451833u_15743786790436941404() noexcept	\
	{	\
		static rfk::NamespaceFragment	fragment("Solid", 3698706698196451833u);	\
		static bool						initialized = false;	\
		if (!initialized)	\
		{	\
			initialized = true;	\
			fragment.nestedEntities.reserve(1u); rfk::NamespaceFragment* fragmentPtr = &fragment; fragmentPtr->addNestedEntity(&Solid::Quat::staticGetArchetype());	\
		}	\
	return fragment;	\
	}

#ifdef __RFK3698706698196451833u_RegisterNamespace
	#undef __RFK3698706698196451833u_RegisterNamespace
#endif
#define __RFK3698706698196451833u_RegisterNamespace	\
	inline rfk::NamespaceFragmentRegisterer namespaceFragmentRegisterer3698706698196451833u_15743786790436941404 = rfk::NamespaceFragmentRegisterer("Solid", 3698706698196451833u, &getNamespaceFragment3698706698196451833u_15743786790436941404(), true);

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

#define __RFK5554176018135410655u_GenerateFieldsMetadata	\
	registerChild<Quat>(&type);	\

#define __RFK5554176018135410655u_GenerateFieldHelperMethods	\
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
		field = childArchetype->addField("x", 76580028223098784u, rfk::Type::getType<float>(), static_cast<rfk::EFieldFlags>(1), &thisArchetype, offsetof(ChildType, x));	\
		field = childArchetype->addField("y", 76581127734726995u, rfk::Type::getType<float>(), static_cast<rfk::EFieldFlags>(1), &thisArchetype, offsetof(ChildType, y));	\
		field = childArchetype->addField("z", 76582227246355206u, rfk::Type::getType<float>(), static_cast<rfk::EFieldFlags>(1), &thisArchetype, offsetof(ChildType, z));	\
		field = childArchetype->addField("w", 76596520897521949u, rfk::Type::getType<float>(), static_cast<rfk::EFieldFlags>(1), &thisArchetype, offsetof(ChildType, w));	\
		__RFK_DISABLE_WARNING_POP	\
	}

#define __RFK5554176018135410655u_GenerateMethodsMetadata	\
	[[maybe_unused]] rfk::Method*		method			= nullptr;	\
	[[maybe_unused]] rfk::StaticMethod*	staticMethod	= nullptr;	\
	staticMethod = type.addStaticMethod("Dot", 4948997927945348588u, rfk::Type::getType<float>(), std::make_unique<rfk::NonMemberFunction<float (const Solid::Quat &, const Solid::Quat &)>>(static_cast<float (*)(const Solid::Quat &, const Solid::Quat &) noexcept>(& Quat::Dot)), static_cast<rfk::EMethodFlags>(9));	\
	staticMethod->parameters.reserve(2); staticMethod->addParameter("_q1", rfk::Type::getType<const Solid::Quat &>())->addParameter("_q2", rfk::Type::getType<const Solid::Quat &>());	\
	staticMethod = type.addStaticMethod("Lerp", 11194200692194933293u, rfk::Type::getType<Solid::Quat>(), std::make_unique<rfk::NonMemberFunction<Solid::Quat (const Solid::Quat &, const Solid::Quat &, float)>>(static_cast<Solid::Quat (*)(const Solid::Quat &, const Solid::Quat &, float) noexcept>(& Quat::Lerp)), static_cast<rfk::EMethodFlags>(9));	\
	staticMethod->parameters.reserve(3); staticMethod->addParameter("_q1", rfk::Type::getType<const Solid::Quat &>())->addParameter("_q2", rfk::Type::getType<const Solid::Quat &>())->addParameter("_r", rfk::Type::getType<float>());	\
	staticMethod = type.addStaticMethod("Nlerp", 17478979315751269257u, rfk::Type::getType<Solid::Quat>(), std::make_unique<rfk::NonMemberFunction<Solid::Quat (const Solid::Quat &, const Solid::Quat &, float)>>(static_cast<Solid::Quat (*)(const Solid::Quat &, const Solid::Quat &, float) noexcept>(& Quat::Nlerp)), static_cast<rfk::EMethodFlags>(9));	\
	staticMethod->parameters.reserve(3); staticMethod->addParameter("_q1", rfk::Type::getType<const Solid::Quat &>())->addParameter("_q2", rfk::Type::getType<const Solid::Quat &>())->addParameter("_r", rfk::Type::getType<float>());	\
	staticMethod = type.addStaticMethod("Slerp", 546508199291282768u, rfk::Type::getType<Solid::Quat>(), std::make_unique<rfk::NonMemberFunction<Solid::Quat (const Solid::Quat &, const Solid::Quat &, float)>>(static_cast<Solid::Quat (*)(const Solid::Quat &, const Solid::Quat &, float) noexcept>(& Quat::Slerp)), static_cast<rfk::EMethodFlags>(9));	\
	staticMethod->parameters.reserve(3); staticMethod->addParameter("_q1", rfk::Type::getType<const Solid::Quat &>())->addParameter("_q2", rfk::Type::getType<const Solid::Quat &>())->addParameter("_r", rfk::Type::getType<float>());	\
	method = type.addMethod("IsZero", 813706280614278029u, rfk::Type::getType<bool>(), std::make_unique<rfk::MemberFunction<Quat, bool ()>>(static_cast<bool (Quat::*)() const noexcept>(& Quat::IsZero)), static_cast<rfk::EMethodFlags>(513));	\
	method = type.addMethod("SqrtLength", 11727436490908334985u, rfk::Type::getType<float>(), std::make_unique<rfk::MemberFunction<Quat, float ()>>(static_cast<float (Quat::*)() const noexcept>(& Quat::SqrtLength)), static_cast<rfk::EMethodFlags>(513));	\
	method = type.addMethod("Length", 10467547655122380427u, rfk::Type::getType<float>(), std::make_unique<rfk::MemberFunction<Quat, float ()>>(static_cast<float (Quat::*)() const noexcept>(& Quat::Length)), static_cast<rfk::EMethodFlags>(513));	\
	method = type.addMethod("Scale", 9198770117740095403u, rfk::Type::getType<Solid::Quat &>(), std::make_unique<rfk::MemberFunction<Quat, Solid::Quat &(float)>>(static_cast<Solid::Quat &(Quat::*)(float) noexcept>(& Quat::Scale)), static_cast<rfk::EMethodFlags>(1));	\
	method->parameters.reserve(1); method->addParameter("_scale", rfk::Type::getType<float>());	\
	method = type.addMethod("GetScaled", 13378269403803576994u, rfk::Type::getType<Solid::Quat>(), std::make_unique<rfk::MemberFunction<Quat, Solid::Quat (float)>>(static_cast<Solid::Quat (Quat::*)(float) const noexcept>(& Quat::GetScaled)), static_cast<rfk::EMethodFlags>(513));	\
	method->parameters.reserve(1); method->addParameter("_scale", rfk::Type::getType<float>());	\
	method = type.addMethod("Unscale", 17403139114462062384u, rfk::Type::getType<Solid::Quat &>(), std::make_unique<rfk::MemberFunction<Quat, Solid::Quat &(float)>>(static_cast<Solid::Quat &(Quat::*)(float) noexcept>(& Quat::Unscale)), static_cast<rfk::EMethodFlags>(1));	\
	method->parameters.reserve(1); method->addParameter("_scale", rfk::Type::getType<float>());	\
	method = type.addMethod("GetUnscaled", 17696936245343072333u, rfk::Type::getType<Solid::Quat>(), std::make_unique<rfk::MemberFunction<Quat, Solid::Quat (float)>>(static_cast<Solid::Quat (Quat::*)(float) const noexcept>(& Quat::GetUnscaled)), static_cast<rfk::EMethodFlags>(513));	\
	method->parameters.reserve(1); method->addParameter("_scale", rfk::Type::getType<float>());	\
	method = type.addMethod("Normalize", 14953586986810074135u, rfk::Type::getType<Solid::Quat &>(), std::make_unique<rfk::MemberFunction<Quat, Solid::Quat &()>>(static_cast<Solid::Quat &(Quat::*)() noexcept>(& Quat::Normalize)), static_cast<rfk::EMethodFlags>(1));	\
	method = type.addMethod("GetNormalized", 8529525229170211608u, rfk::Type::getType<Solid::Quat>(), std::make_unique<rfk::MemberFunction<Quat, Solid::Quat ()>>(static_cast<Solid::Quat (Quat::*)() const noexcept>(& Quat::GetNormalized)), static_cast<rfk::EMethodFlags>(513));	\
	method = type.addMethod("Inverse", 5136648026097689458u, rfk::Type::getType<Solid::Quat &>(), std::make_unique<rfk::MemberFunction<Quat, Solid::Quat &()>>(static_cast<Solid::Quat &(Quat::*)()>(& Quat::Inverse)), static_cast<rfk::EMethodFlags>(1));	\
	method = type.addMethod("GetInversed", 16267037214039761217u, rfk::Type::getType<Solid::Quat>(), std::make_unique<rfk::MemberFunction<Quat, Solid::Quat ()>>(static_cast<Solid::Quat (Quat::*)() const>(& Quat::GetInversed)), static_cast<rfk::EMethodFlags>(513));	\
	method = type.addMethod("IsNormalized", 7097393891695980806u, rfk::Type::getType<bool>(), std::make_unique<rfk::MemberFunction<Quat, bool ()>>(static_cast<bool (Quat::*)() const noexcept>(& Quat::IsNormalized)), static_cast<rfk::EMethodFlags>(513));	\
	method = type.addMethod("IsEquals", 16804195902934675777u, rfk::Type::getType<bool>(), std::make_unique<rfk::MemberFunction<Quat, bool (const Solid::Quat &)>>(static_cast<bool (Quat::*)(const Solid::Quat &) const noexcept>(& Quat::IsEquals)), static_cast<rfk::EMethodFlags>(513));	\
	method->parameters.reserve(1); method->addParameter("_quat", rfk::Type::getType<const Solid::Quat &>());	\
	method = type.addMethod("Rotate", 5804665867487750547u, rfk::Type::getType<Solid::Vec3>(), std::make_unique<rfk::MemberFunction<Quat, Solid::Vec3 (const Solid::Vec3 &)>>(static_cast<Solid::Vec3 (Quat::*)(const Solid::Vec3 &) const>(& Quat::Rotate)), static_cast<rfk::EMethodFlags>(513));	\
	method->parameters.reserve(1); method->addParameter("_vec", rfk::Type::getType<const Solid::Vec3 &>());	\
	method = type.addMethod("Rotate", 9926126182113787831u, rfk::Type::getType<Solid::Quat>(), std::make_unique<rfk::MemberFunction<Quat, Solid::Quat (const Solid::Quat &)>>(static_cast<Solid::Quat (Quat::*)(const Solid::Quat &) const>(& Quat::Rotate)), static_cast<rfk::EMethodFlags>(513));	\
	method->parameters.reserve(1); method->addParameter("_quat", rfk::Type::getType<const Solid::Quat &>());	\
	method = type.addMethod("Unrotate", 8386355940530628770u, rfk::Type::getType<Solid::Vec3>(), std::make_unique<rfk::MemberFunction<Quat, Solid::Vec3 (const Solid::Vec3 &)>>(static_cast<Solid::Vec3 (Quat::*)(const Solid::Vec3 &) const>(& Quat::Unrotate)), static_cast<rfk::EMethodFlags>(513));	\
	method->parameters.reserve(1); method->addParameter("_vec", rfk::Type::getType<const Solid::Vec3 &>());	\
	method = type.addMethod("Unrotate", 7009256489959914270u, rfk::Type::getType<Solid::Quat>(), std::make_unique<rfk::MemberFunction<Quat, Solid::Quat (const Solid::Quat &)>>(static_cast<Solid::Quat (Quat::*)(const Solid::Quat &) const>(& Quat::Unrotate)), static_cast<rfk::EMethodFlags>(513));	\
	method->parameters.reserve(1); method->addParameter("_quat", rfk::Type::getType<const Solid::Quat &>());	\
	method = type.addMethod("ToEuler", 8809372870412593943u, rfk::Type::getType<Solid::Vec3>(), std::make_unique<rfk::MemberFunction<Quat, Solid::Vec3 ()>>(static_cast<Solid::Vec3 (Quat::*)() const>(& Quat::ToEuler)), static_cast<rfk::EMethodFlags>(513));	\
	method = type.addMethod("FromEuler", 12902566064812106775u, rfk::Type::getType<Solid::Quat>(), std::make_unique<rfk::MemberFunction<Quat, Solid::Quat (const Solid::Vec3 &)>>(static_cast<Solid::Quat (Quat::*)(const Solid::Vec3 &) const>(& Quat::FromEuler)), static_cast<rfk::EMethodFlags>(513));	\
	method->parameters.reserve(1); method->addParameter("_angles", rfk::Type::getType<const Solid::Vec3 &>());	\
	method = type.addMethod("ToString", 11161196962058126623u, rfk::Type::getType<const std::string>(), std::make_unique<rfk::MemberFunction<Quat, const std::string ()>>(static_cast<const std::string (Quat::*)() const noexcept>(& Quat::ToString)), static_cast<rfk::EMethodFlags>(513));	\
	method = type.addMethod("operator=", 526913328001129178u, rfk::Type::getType<Solid::Quat &>(), std::make_unique<rfk::MemberFunction<Quat, Solid::Quat &(const Solid::Quat &)>>(static_cast<Solid::Quat &(Quat::*)(const Solid::Quat &)>(& Quat::operator=)), static_cast<rfk::EMethodFlags>(17));	\
	method->parameters.reserve(1); method->addParameter("_quat", rfk::Type::getType<const Solid::Quat &>());	\
	method = type.addMethod("operator=", 2934971350601411013u, rfk::Type::getType<Solid::Quat &>(), std::make_unique<rfk::MemberFunction<Quat, Solid::Quat &(Solid::Quat &&)>>(static_cast<Solid::Quat &(Quat::*)(Solid::Quat &&)>(& Quat::operator=)), static_cast<rfk::EMethodFlags>(17));	\
	method->parameters.reserve(1); method->addParameter("_quat", rfk::Type::getType<Solid::Quat &&>());	\
	method = type.addMethod("operator*", 12384846814120975454u, rfk::Type::getType<Solid::Quat>(), std::make_unique<rfk::MemberFunction<Quat, Solid::Quat (float)>>(static_cast<Solid::Quat (Quat::*)(float) const noexcept>(& Quat::operator*)), static_cast<rfk::EMethodFlags>(513));	\
	method->parameters.reserve(1); method->addParameter("_scale", rfk::Type::getType<float>());	\
	method = type.addMethod("operator/", 5357271820397166317u, rfk::Type::getType<Solid::Quat>(), std::make_unique<rfk::MemberFunction<Quat, Solid::Quat (float)>>(static_cast<Solid::Quat (Quat::*)(float) const noexcept>(& Quat::operator/)), static_cast<rfk::EMethodFlags>(513));	\
	method->parameters.reserve(1); method->addParameter("_scale", rfk::Type::getType<float>());	\
	method = type.addMethod("operator+", 3737780525495944537u, rfk::Type::getType<Solid::Quat>(), std::make_unique<rfk::MemberFunction<Quat, Solid::Quat (float)>>(static_cast<Solid::Quat (Quat::*)(float) const noexcept>(& Quat::operator+)), static_cast<rfk::EMethodFlags>(513));	\
	method->parameters.reserve(1); method->addParameter("_scale", rfk::Type::getType<float>());	\
	method = type.addMethod("operator-", 6662691329784941143u, rfk::Type::getType<Solid::Quat>(), std::make_unique<rfk::MemberFunction<Quat, Solid::Quat (float)>>(static_cast<Solid::Quat (Quat::*)(float) const noexcept>(& Quat::operator-)), static_cast<rfk::EMethodFlags>(513));	\
	method->parameters.reserve(1); method->addParameter("_scale", rfk::Type::getType<float>());	\
	method = type.addMethod("operator*", 17425066243143884758u, rfk::Type::getType<Solid::Quat>(), std::make_unique<rfk::MemberFunction<Quat, Solid::Quat (const Solid::Quat &)>>(static_cast<Solid::Quat (Quat::*)(const Solid::Quat &) const noexcept>(& Quat::operator*)), static_cast<rfk::EMethodFlags>(513));	\
	method->parameters.reserve(1); method->addParameter("_quat", rfk::Type::getType<const Solid::Quat &>());	\
	method = type.addMethod("operator*", 12471830964526584570u, rfk::Type::getType<Solid::Vec3>(), std::make_unique<rfk::MemberFunction<Quat, Solid::Vec3 (const Solid::Vec3 &)>>(static_cast<Solid::Vec3 (Quat::*)(const Solid::Vec3 &) const noexcept>(& Quat::operator*)), static_cast<rfk::EMethodFlags>(513));	\
	method->parameters.reserve(1); method->addParameter("_vec", rfk::Type::getType<const Solid::Vec3 &>());	\
	method = type.addMethod("operator/", 6280177505657493299u, rfk::Type::getType<Solid::Quat>(), std::make_unique<rfk::MemberFunction<Quat, Solid::Quat (const Solid::Quat &)>>(static_cast<Solid::Quat (Quat::*)(const Solid::Quat &) const noexcept>(& Quat::operator/)), static_cast<rfk::EMethodFlags>(513));	\
	method->parameters.reserve(1); method->addParameter("_quat", rfk::Type::getType<const Solid::Quat &>());	\
	method = type.addMethod("operator/", 9216703360848273247u, rfk::Type::getType<Solid::Vec3>(), std::make_unique<rfk::MemberFunction<Quat, Solid::Vec3 (const Solid::Vec3 &)>>(static_cast<Solid::Vec3 (Quat::*)(const Solid::Vec3 &) const noexcept>(& Quat::operator/)), static_cast<rfk::EMethodFlags>(513));	\
	method->parameters.reserve(1); method->addParameter("_vec", rfk::Type::getType<const Solid::Vec3 &>());	\
	method = type.addMethod("operator+", 9623659720336488463u, rfk::Type::getType<Solid::Quat>(), std::make_unique<rfk::MemberFunction<Quat, Solid::Quat (const Solid::Quat &)>>(static_cast<Solid::Quat (Quat::*)(const Solid::Quat &) const noexcept>(& Quat::operator+)), static_cast<rfk::EMethodFlags>(513));	\
	method->parameters.reserve(1); method->addParameter("_quat", rfk::Type::getType<const Solid::Quat &>());	\
	method = type.addMethod("operator-", 11800632414543256481u, rfk::Type::getType<Solid::Quat>(), std::make_unique<rfk::MemberFunction<Quat, Solid::Quat (const Solid::Quat &)>>(static_cast<Solid::Quat (Quat::*)(const Solid::Quat &) const noexcept>(& Quat::operator-)), static_cast<rfk::EMethodFlags>(513));	\
	method->parameters.reserve(1); method->addParameter("_quat", rfk::Type::getType<const Solid::Quat &>());	\
	method = type.addMethod("operator*=", 3385822929042262564u, rfk::Type::getType<Solid::Quat &>(), std::make_unique<rfk::MemberFunction<Quat, Solid::Quat &(float)>>(static_cast<Solid::Quat &(Quat::*)(float)>(& Quat::operator*=)), static_cast<rfk::EMethodFlags>(1));	\
	method->parameters.reserve(1); method->addParameter("_scale", rfk::Type::getType<float>());	\
	method = type.addMethod("operator/=", 12314499365043478763u, rfk::Type::getType<Solid::Quat &>(), std::make_unique<rfk::MemberFunction<Quat, Solid::Quat &(float)>>(static_cast<Solid::Quat &(Quat::*)(float)>(& Quat::operator/=)), static_cast<rfk::EMethodFlags>(1));	\
	method->parameters.reserve(1); method->addParameter("_scale", rfk::Type::getType<float>());	\
	method = type.addMethod("operator+=", 5815206907413001711u, rfk::Type::getType<Solid::Quat &>(), std::make_unique<rfk::MemberFunction<Quat, Solid::Quat &(float)>>(static_cast<Solid::Quat &(Quat::*)(float)>(& Quat::operator+=)), static_cast<rfk::EMethodFlags>(1));	\
	method->parameters.reserve(1); method->addParameter("_scale", rfk::Type::getType<float>());	\
	method = type.addMethod("operator-=", 16926743921708553149u, rfk::Type::getType<Solid::Quat &>(), std::make_unique<rfk::MemberFunction<Quat, Solid::Quat &(float)>>(static_cast<Solid::Quat &(Quat::*)(float)>(& Quat::operator-=)), static_cast<rfk::EMethodFlags>(1));	\
	method->parameters.reserve(1); method->addParameter("_scale", rfk::Type::getType<float>());	\
	method = type.addMethod("operator*=", 15939248172943653026u, rfk::Type::getType<Solid::Quat &>(), std::make_unique<rfk::MemberFunction<Quat, Solid::Quat &(const Solid::Quat &)>>(static_cast<Solid::Quat &(Quat::*)(const Solid::Quat &)>(& Quat::operator*=)), static_cast<rfk::EMethodFlags>(1));	\
	method->parameters.reserve(1); method->addParameter("_quat", rfk::Type::getType<const Solid::Quat &>());	\
	method = type.addMethod("operator/=", 4640339353718201107u, rfk::Type::getType<Solid::Quat &>(), std::make_unique<rfk::MemberFunction<Quat, Solid::Quat &(const Solid::Quat &)>>(static_cast<Solid::Quat &(Quat::*)(const Solid::Quat &)>(& Quat::operator/=)), static_cast<rfk::EMethodFlags>(1));	\
	method->parameters.reserve(1); method->addParameter("_quat", rfk::Type::getType<const Solid::Quat &>());	\
	method = type.addMethod("operator+=", 11658836517280606519u, rfk::Type::getType<Solid::Quat &>(), std::make_unique<rfk::MemberFunction<Quat, Solid::Quat &(const Solid::Quat &)>>(static_cast<Solid::Quat &(Quat::*)(const Solid::Quat &)>(& Quat::operator+=)), static_cast<rfk::EMethodFlags>(1));	\
	method->parameters.reserve(1); method->addParameter("_quat", rfk::Type::getType<const Solid::Quat &>());	\
	method = type.addMethod("operator-=", 10675823934867694137u, rfk::Type::getType<Solid::Quat &>(), std::make_unique<rfk::MemberFunction<Quat, Solid::Quat &(const Solid::Quat &)>>(static_cast<Solid::Quat &(Quat::*)(const Solid::Quat &)>(& Quat::operator-=)), static_cast<rfk::EMethodFlags>(1));	\
	method->parameters.reserve(1); method->addParameter("_quat", rfk::Type::getType<const Solid::Quat &>());	\
	method = type.addMethod("operator==", 17459172164060668777u, rfk::Type::getType<bool>(), std::make_unique<rfk::MemberFunction<Quat, bool (const Solid::Quat &)>>(static_cast<bool (Quat::*)(const Solid::Quat &)>(& Quat::operator==)), static_cast<rfk::EMethodFlags>(1));	\
	method->parameters.reserve(1); method->addParameter("_quat", rfk::Type::getType<const Solid::Quat &>());	\
	method = type.addMethod("operator!=", 10073137874439736949u, rfk::Type::getType<bool>(), std::make_unique<rfk::MemberFunction<Quat, bool (const Solid::Quat &)>>(static_cast<bool (Quat::*)(const Solid::Quat &)>(& Quat::operator!=)), static_cast<rfk::EMethodFlags>(1));	\
	method->parameters.reserve(1); method->addParameter("_quat", rfk::Type::getType<const Solid::Quat &>());	\
	method = type.addMethod("operator-", 14871021773249301646u, rfk::Type::getType<Solid::Quat>(), std::make_unique<rfk::MemberFunction<Quat, Solid::Quat ()>>(static_cast<Solid::Quat (Quat::*)() const noexcept>(& Quat::operator-)), static_cast<rfk::EMethodFlags>(513));	\

#define __RFK5554176018135410655u_GenerateArchetypeProperties	\
	

#define __RFK5554176018135410655u_GenerateDefaultInstantiatorSetup	\
	type.setDefaultInstantiator(&rfk::defaultInstantiator<Quat>);

#define __RFK5554176018135410655u_GetTypeDeclaration	\
	__RFK5554176018135410655u_GenerateFieldHelperMethods	\
	public:	\
		inline static rfk::Class const& staticGetArchetype() noexcept;	\
	

#ifdef KODGEN_PARSING
#define __RFK5554176018135410655u_GetTypeDefinition
#else
#define __RFK5554176018135410655u_GetTypeDefinition	\
		inline rfk::Class const& Solid::Quat::staticGetArchetype() noexcept	\
		{	\
			static bool			initialized = false;	\
			static rfk::Class	type("Quat", 5554176018135410655u, sizeof(Quat));	\
			\
			if (!initialized)	\
			{	\
				initialized = true;	\
			\
				__RFK5554176018135410655u_GenerateArchetypeProperties	\
					\
					\
				__RFK5554176018135410655u_GenerateFieldsMetadata	\
				__RFK5554176018135410655u_GenerateDefaultInstantiatorSetup	\
				__RFK5554176018135410655u_GenerateMethodsMetadata	\
			}	\
			\
			return type;	\
		}

#endif
#define __RFK5554176018135410655u_RegisterArchetype	\
	private:	\
		static inline rfk::ArchetypeRegisterer __rfkArchetypeRegisterer;


#ifdef KODGEN_PARSING
#define Quat_GENERATED
#else
#define Quat_GENERATED	\
	friend rfk::Struct;	\
	friend rfk::hasField___rfkArchetypeRegisterer<Quat, rfk::ArchetypeRegisterer>;	\
	__RFK5554176018135410655u_GetTypeDeclaration	\
	__RFK5554176018135410655u_RegisterArchetype	\
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
	__RFK5554176018135410655u_GetTypeDefinition	\
	__RFK3698706698196451833u_GENERATED	\
	__RFKNativeProperties_GENERATED	\


