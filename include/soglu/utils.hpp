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
struct SamplerTag {};

typedef GLValueWrapper<unsigned int, TextureTag, 0> TextureId;
typedef GLValueWrapper<unsigned int, TextureUnitTag, 0> TextureUnitId;
typedef GLValueWrapper<unsigned int, SamplerTag, 0> SamplerId;

template<typename TBindable>
struct GLBinder
{
	GLBinder(TBindable &aBindable)
		: mBindable(aBindable)
		, mIsBinded(false)
	{
		bind();		
	}

	GLBinder(GLBinder &&aBinder)
		: mBindable(aBinder.aBindable)
		, mIsBinded(aBinder.mIsBinded)
	{
		aBinder.mIsBinded = false;
	}

	~GLBinder()
	{
		if (mIsBinded) {
			unbind();
		}
	}

	void
	bind()
	{
		SOGLU_ASSERT(!mIsBinded);
		SOGLU_ASSERT(isGLContextActive());
		mBindable.bind();
	}
	
	void
	unbind()
	{
		SOGLU_ASSERT(mIsBinded);
		SOGLU_ASSERT(isGLContextActive());
		mBindable.unbind();
	}

	TBindable &mBindable;
	bool mIsBinded;
};

template<typename TBindable>
GLBinder<TBindable>
getBinder(TBindable &aBindable)
{
	return GLBinder<TBindable>(aBindable);
}

class Sampler : public SamplerId
{
public:
	Sampler(bool aInit = false);

	~Sampler();

	void
	initialize();

	void
	finalize();

	void
	bind(const TextureUnitId &aTextureUnit);

	static void
	unbind(const TextureUnitId &aTextureUnit);

	void
	setParameter(int aParameter, int aParameterValue);

protected:
	Sampler(const Sampler &);
};

} // namespace soglu
