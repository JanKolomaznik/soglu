#pragma once

#include <soglu/ErrorHandling.hpp>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <string>

namespace soglu {
	
template <typename TValue, typename TTag, TValue tInvalidValue>
struct GLValueWrapper
{
	typedef TTag Tag;
	GLValueWrapper() : value(tInvalidValue) {}
	GLValueWrapper(const TValue &aValue) : value(aValue) {}
	operator bool() const { return tInvalidValue != value; }

	TValue value;
};

struct TextureTag {};
struct TextureUnitTag {};

typedef GLValueWrapper<unsigned int, TextureTag, 0> TextureId;
typedef GLValueWrapper<unsigned int, TextureUnitTag, 0> TextureUnitId;

} //namespace soglu
