//
// Created by ryan1 on 14/04/2021.
//

#ifndef SOLIDEDITOR_STD_WRAPPER_HPP
#define SOLIDEDITOR_STD_WRAPPER_HPP

#include <string>
#include "ECS/types.hpp"
#include "Build/solidAPI.hpp"
#include "EngineGenerated/std_wrapper.sld.hpp"



class SLDClass() String : public std::string
	{
	public:
	//public members

	protected:
	//protected members


	public:
	//public func
	using std::string::string;
	using std::string::operator=;


		String_GENERATED

	};


class SLDClass() vectorStr : public std::vector<String>
{
public:
	//public members

protected:
	//protected members


public:
	//public func
	using std::vector<String>::vector;
	using std::vector<String>::operator=;


	vectorStr_GENERATED

};



File_GENERATED


#endif //SOLIDEDITOR_STD_WRAPPER_HPP
