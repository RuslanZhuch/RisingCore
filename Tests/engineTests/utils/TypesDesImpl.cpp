#pragma once
#include "pch.h"

#include <engine/contextUtils.h>
#include <engine/String.h>

#include "TypeUtils.h"

void Type2f::setValueByName(std::string_view name, const rapidjson::Value& value) noexcept
{
	if (name == "x")
	{
		Engine::ContextUtils::assignToVariable(this->x, value);
		return;
	}
	if (name == "y")
	{
		Engine::ContextUtils::assignToVariable(this->y, value);
		return;
	}
}

void TypeIntWith2f::setValueByName(std::string_view name, const rapidjson::Value& value) noexcept
{
	if (name == "int1")
	{
		Engine::ContextUtils::assignToVariable(this->int1, value);
		return;
	}
	if (name == "inner")
	{
		Engine::ContextUtils::assignToVariable(this->inner, value);
		return;
	}
}

void TypeComplex1::setValueByName(std::string_view name, const rapidjson::Value& value) noexcept
{
	if (name == "int1")
	{
		Engine::ContextUtils::assignToVariable(this->int1, value);
		return;
	}
	if (name == "float1")
	{
		Engine::ContextUtils::assignToVariable(this->float1, value);
		return;
	}
	if (name == "string1")
	{
		Engine::ContextUtils::assignToVariable(this->string1, value);
		return;
	}
	if (name == "inner")
	{
		Engine::ContextUtils::assignToVariable(this->inner, value);
		return;
	}
}