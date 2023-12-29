#pragma once

#include <engine/String.h>

#include <engine/types.h>

struct Type2f
{
	ENGINE_TYPES_DESERIALIZE

	float x{};
	float y{};

};

struct TypeIntWith2f
{
	ENGINE_TYPES_DESERIALIZE

	int int1{};
	Type2f inner;

};

struct TypeComplex1
{
	ENGINE_TYPES_DESERIALIZE

	int int1{};
	float float1{};
	Engine::String string1{};
	Type2f inner;

};

struct TypeNoSetValueByName
{

	float x{};
	float y{};
	float z{};

};
