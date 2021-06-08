#pragma once

/**
*	Source file: D:\Project\SolidEngine\SolidEngine\Include\ECS\Components\audioSource.hpp
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
#define __RFK3698706698196451833u_DeclareGetNamespaceFragment inline rfk::NamespaceFragment const& getNamespaceFragment3698706698196451833u_3308647471198535408() noexcept;

#ifdef __RFK3698706698196451833u_DefineGetNamespaceFragment
	#undef __RFK3698706698196451833u_DefineGetNamespaceFragment
#endif
#define __RFK3698706698196451833u_DefineGetNamespaceFragment	\
	inline rfk::NamespaceFragment const& getNamespaceFragment3698706698196451833u_3308647471198535408() noexcept	\
	{	\
		static rfk::NamespaceFragment	fragment("Solid", 3698706698196451833u);	\
		static bool						initialized = false;	\
		if (!initialized)	\
		{	\
			initialized = true;	\
			fragment.nestedEntities.reserve(1u); rfk::NamespaceFragment* fragmentPtr = &fragment; fragmentPtr->addNestedEntity(&Solid::AudioSource::staticGetArchetype());	\
		}	\
	return fragment;	\
	}

#ifdef __RFK3698706698196451833u_RegisterNamespace
	#undef __RFK3698706698196451833u_RegisterNamespace
#endif
#define __RFK3698706698196451833u_RegisterNamespace	\
	inline rfk::NamespaceFragmentRegisterer namespaceFragmentRegisterer3698706698196451833u_3308647471198535408 = rfk::NamespaceFragmentRegisterer("Solid", 3698706698196451833u, &getNamespaceFragment3698706698196451833u_3308647471198535408(), true);

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

#define __RFK3133138418174186223u_GenerateParentsMetadata	\
	type.directParents.reserve(1);	\
	type.addToParents<Solid::Components>(static_cast<rfk::EAccessSpecifier>(1));	\

#define __RFK3133138418174186223u_GenerateFieldsMetadata	\
	registerChild<AudioSource>(&type);	\

#define __RFK3133138418174186223u_GenerateFieldHelperMethods	\
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
		field = childArchetype->addField("name", 1082712505782278795u, rfk::Type::getType<String>(), static_cast<rfk::EFieldFlags>(4), &thisArchetype, offsetof(ChildType, name));	\
		field = childArchetype->addField("loop", 17901538298485596118u, rfk::Type::getType<bool>(), static_cast<rfk::EFieldFlags>(4), &thisArchetype, offsetof(ChildType, loop));	\
		field = childArchetype->addField("pitch", 3588574026796702158u, rfk::Type::getType<float>(), static_cast<rfk::EFieldFlags>(4), &thisArchetype, offsetof(ChildType, pitch));	\
		field = childArchetype->addField("volume", 18351019265825224394u, rfk::Type::getType<float>(), static_cast<rfk::EFieldFlags>(4), &thisArchetype, offsetof(ChildType, volume));	\
		field = childArchetype->addField("maxDistance", 18232469590400909101u, rfk::Type::getType<float>(), static_cast<rfk::EFieldFlags>(4), &thisArchetype, offsetof(ChildType, maxDistance));	\
		field = childArchetype->addField("velocity", 12423396970938447547u, rfk::Type::getType<Solid::Vec3>(), static_cast<rfk::EFieldFlags>(4), &thisArchetype, offsetof(ChildType, velocity));	\
		__RFK_DISABLE_WARNING_POP	\
	}

#define __RFK3133138418174186223u_GenerateMethodsMetadata	\
	[[maybe_unused]] rfk::Method*		method			= nullptr;	\
	[[maybe_unused]] rfk::StaticMethod*	staticMethod	= nullptr;	\
	method = type.addMethod("Init", 18298368011246378570u, rfk::Type::getType<void>(), std::make_unique<rfk::MemberFunction<AudioSource, void ()>>(static_cast<void (AudioSource::*)()>(& AudioSource::Init)), static_cast<rfk::EMethodFlags>(161));	\
	method->inheritBaseMethodProperties();	\
	method = type.addMethod("Release", 6465912359747199993u, rfk::Type::getType<void>(), std::make_unique<rfk::MemberFunction<AudioSource, void ()>>(static_cast<void (AudioSource::*)()>(& AudioSource::Release)), static_cast<rfk::EMethodFlags>(161));	\
	method->inheritBaseMethodProperties();	\
	method = type.addMethod("SetAudio", 10738512231165870711u, rfk::Type::getType<void>(), std::make_unique<rfk::MemberFunction<AudioSource, void (Solid::AudioResource *)>>(static_cast<void (AudioSource::*)(Solid::AudioResource *)>(& AudioSource::SetAudio)), static_cast<rfk::EMethodFlags>(1));	\
	method->parameters.reserve(1); method->addParameter("_audioResource", rfk::Type::getType<Solid::AudioResource *>());	\
	method = type.addMethod("SetLoop", 4435344529645949747u, rfk::Type::getType<void>(), std::make_unique<rfk::MemberFunction<AudioSource, void (bool)>>(static_cast<void (AudioSource::*)(bool)>(& AudioSource::SetLoop)), static_cast<rfk::EMethodFlags>(1));	\
	method->parameters.reserve(1); method->addParameter("_loop", rfk::Type::getType<bool>());	\
	method = type.addMethod("SetVolume", 2627394686812386315u, rfk::Type::getType<void>(), std::make_unique<rfk::MemberFunction<AudioSource, void (float)>>(static_cast<void (AudioSource::*)(float)>(& AudioSource::SetVolume)), static_cast<rfk::EMethodFlags>(1));	\
	method->parameters.reserve(1); method->addParameter("_vol", rfk::Type::getType<float>());	\
	method = type.addMethod("SetPitch", 8506531981794292389u, rfk::Type::getType<void>(), std::make_unique<rfk::MemberFunction<AudioSource, void (float)>>(static_cast<void (AudioSource::*)(float)>(& AudioSource::SetPitch)), static_cast<rfk::EMethodFlags>(1));	\
	method->parameters.reserve(1); method->addParameter("_pitch", rfk::Type::getType<float>());	\
	method = type.addMethod("SetMaxDistance", 14375922090066646740u, rfk::Type::getType<void>(), std::make_unique<rfk::MemberFunction<AudioSource, void (float)>>(static_cast<void (AudioSource::*)(float)>(& AudioSource::SetMaxDistance)), static_cast<rfk::EMethodFlags>(1));	\
	method->parameters.reserve(1); method->addParameter("_maxDistance", rfk::Type::getType<float>());	\
	method = type.addMethod("SetMusicVelocity", 385600790483320271u, rfk::Type::getType<void>(), std::make_unique<rfk::MemberFunction<AudioSource, void (const Solid::Vec3 &)>>(static_cast<void (AudioSource::*)(const Solid::Vec3 &)>(& AudioSource::SetMusicVelocity)), static_cast<rfk::EMethodFlags>(1));	\
	method->parameters.reserve(1); method->addParameter("_velocity", rfk::Type::getType<const Solid::Vec3 &>());	\
	method = type.addMethod("SetPosition", 7301755559458985270u, rfk::Type::getType<void>(), std::make_unique<rfk::MemberFunction<AudioSource, void (const Solid::Vec3 &)>>(static_cast<void (AudioSource::*)(const Solid::Vec3 &)>(& AudioSource::SetPosition)), static_cast<rfk::EMethodFlags>(1));	\
	method->parameters.reserve(1); method->addParameter("_position", rfk::Type::getType<const Solid::Vec3 &>());	\
	method = type.addMethod("GetName", 16695109917678390050u, rfk::Type::getType<std::string>(), std::make_unique<rfk::MemberFunction<AudioSource, std::string ()>>(static_cast<std::string (AudioSource::*)() const>(& AudioSource::GetName)), static_cast<rfk::EMethodFlags>(513));	\
	method = type.addMethod("GetVolume", 437583561317441559u, rfk::Type::getType<float>(), std::make_unique<rfk::MemberFunction<AudioSource, float ()>>(static_cast<float (AudioSource::*)() const>(& AudioSource::GetVolume)), static_cast<rfk::EMethodFlags>(513));	\
	method = type.addMethod("GetPitch", 16328048710465966477u, rfk::Type::getType<float>(), std::make_unique<rfk::MemberFunction<AudioSource, float ()>>(static_cast<float (AudioSource::*)() const>(& AudioSource::GetPitch)), static_cast<rfk::EMethodFlags>(513));	\
	method = type.addMethod("GetMaxDistance", 2198768468983315266u, rfk::Type::getType<float>(), std::make_unique<rfk::MemberFunction<AudioSource, float ()>>(static_cast<float (AudioSource::*)() const>(& AudioSource::GetMaxDistance)), static_cast<rfk::EMethodFlags>(513));	\
	method = type.addMethod("GetMusicVelocity", 13873611640292728919u, rfk::Type::getType<Solid::Vec3>(), std::make_unique<rfk::MemberFunction<AudioSource, Solid::Vec3 ()>>(static_cast<Solid::Vec3 (AudioSource::*)() const>(& AudioSource::GetMusicVelocity)), static_cast<rfk::EMethodFlags>(513));	\
	method = type.addMethod("Play", 13973612297271953566u, rfk::Type::getType<void>(), std::make_unique<rfk::MemberFunction<AudioSource, void ()>>(static_cast<void (AudioSource::*)()>(& AudioSource::Play)), static_cast<rfk::EMethodFlags>(1));	\
	method = type.addMethod("Pause", 18064273411278020008u, rfk::Type::getType<void>(), std::make_unique<rfk::MemberFunction<AudioSource, void ()>>(static_cast<void (AudioSource::*)()>(& AudioSource::Pause)), static_cast<rfk::EMethodFlags>(1));	\
	method = type.addMethod("Stop", 225430254939685704u, rfk::Type::getType<void>(), std::make_unique<rfk::MemberFunction<AudioSource, void ()>>(static_cast<void (AudioSource::*)()>(& AudioSource::Stop)), static_cast<rfk::EMethodFlags>(1));	\
	method = type.addMethod("IsPlaying", 1420905897960193560u, rfk::Type::getType<bool>(), std::make_unique<rfk::MemberFunction<AudioSource, bool ()>>(static_cast<bool (AudioSource::*)()>(& AudioSource::IsPlaying)), static_cast<rfk::EMethodFlags>(1));	\
	method = type.addMethod("IsPaused", 17150999183712692126u, rfk::Type::getType<bool>(), std::make_unique<rfk::MemberFunction<AudioSource, bool ()>>(static_cast<bool (AudioSource::*)()>(& AudioSource::IsPaused)), static_cast<rfk::EMethodFlags>(1));	\
	method = type.addMethod("IsLooping", 4981496751501358672u, rfk::Type::getType<bool>(), std::make_unique<rfk::MemberFunction<AudioSource, bool ()>>(static_cast<bool (AudioSource::*)()>(& AudioSource::IsLooping)), static_cast<rfk::EMethodFlags>(1));	\

#define __RFK3133138418174186223u_GenerateArchetypeProperties	\
	

#define __RFK3133138418174186223u_GenerateDefaultInstantiatorSetup	\
	type.setDefaultInstantiator(&rfk::defaultInstantiator<AudioSource>);

#define __RFK3133138418174186223u_GetTypeDeclaration	\
	__RFK3133138418174186223u_GenerateFieldHelperMethods	\
	public:	\
		inline static rfk::Class const& staticGetArchetype() noexcept;	\
		rfk::Class const& getArchetype() const noexcept override { return AudioSource::staticGetArchetype(); }

#ifdef KODGEN_PARSING
#define __RFK3133138418174186223u_GetTypeDefinition
#else
#define __RFK3133138418174186223u_GetTypeDefinition	\
		inline rfk::Class const& Solid::AudioSource::staticGetArchetype() noexcept	\
		{	\
			static bool			initialized = false;	\
			static rfk::Class	type("AudioSource", 3133138418174186223u, sizeof(AudioSource));	\
			\
			if (!initialized)	\
			{	\
				initialized = true;	\
			\
				__RFK3133138418174186223u_GenerateArchetypeProperties	\
				__RFK3133138418174186223u_GenerateParentsMetadata	\
					\
				__RFK3133138418174186223u_GenerateFieldsMetadata	\
				__RFK3133138418174186223u_GenerateDefaultInstantiatorSetup	\
				__RFK3133138418174186223u_GenerateMethodsMetadata	\
			}	\
			\
			return type;	\
		}

#endif
#define __RFK3133138418174186223u_RegisterArchetype	\
	private:	\
		static inline rfk::ArchetypeRegisterer __rfkArchetypeRegisterer;


#ifdef KODGEN_PARSING
#define AudioSource_GENERATED
#else
#define AudioSource_GENERATED	\
	friend rfk::Struct;	\
	friend rfk::hasField___rfkArchetypeRegisterer<AudioSource, rfk::ArchetypeRegisterer>;	\
	__RFK3133138418174186223u_GetTypeDeclaration	\
	__RFK3133138418174186223u_RegisterArchetype	\
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
	__RFK3133138418174186223u_GetTypeDefinition	\
	__RFK3698706698196451833u_GENERATED	\
	__RFKNativeProperties_GENERATED	\


