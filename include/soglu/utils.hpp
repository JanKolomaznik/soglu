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
	typedef TValue ValueType;
	GLValueWrapper() : value(tInvalidValue) {}
	GLValueWrapper(const ValueType &aValue) : value(aValue) {}
	GLValueWrapper(const GLValueWrapper<TValue, TTag, tInvalidValue> &aValue) : value(aValue.value) {}
	GLValueWrapper(GLValueWrapper<TValue, TTag, tInvalidValue> &&aValue)
		: value(aValue.value)
	{
		aValue.value = tInvalidValue;
	}
	operator bool() const { return tInvalidValue != value; }

	GLValueWrapper<TValue, TTag, tInvalidValue> &
	operator=(const GLValueWrapper<TValue, TTag, tInvalidValue> &aValue)
	{
		value = aValue.value;
		return *this;
	}

	ValueType value;
};

template <typename TValueWrapper, typename TClass>
struct GLGeneratedValueWrapperCRTP : TValueWrapper
{
	GLGeneratedValueWrapperCRTP() : TValueWrapper() {}
	GLGeneratedValueWrapperCRTP(const typename TValueWrapper::ValueType &aValue) : TValueWrapper(aValue) {}
	GLGeneratedValueWrapperCRTP(GLGeneratedValueWrapperCRTP &&aWrapper)
		: TValueWrapper(std::move(aWrapper))
	{
	}

	~GLGeneratedValueWrapperCRTP() {}

	void
	initialize()
	{
		static_cast<TClass *>(this)->initialize();
	}

	void
	finalize()
	{
		static_cast<TClass *>(this)->finalize();
	}
private:
	GLGeneratedValueWrapperCRTP(const GLGeneratedValueWrapperCRTP &);
};

struct TextureTag {};
struct TextureUnitTag {};
struct SamplerTag {};
struct FramebufferTag {};
struct RenderbufferTag {};
struct BufferTag {};
struct VertexArrayTag {};

typedef GLValueWrapper<uint32_t, TextureTag, 0> TextureId;
typedef GLValueWrapper<uint32_t, TextureUnitTag, 0> TextureUnitId;
typedef GLValueWrapper<uint32_t, SamplerTag, 0> SamplerId;
typedef GLValueWrapper<uint32_t, FramebufferTag, 0> FramebufferId;
typedef GLValueWrapper<uint32_t, RenderbufferTag, 0> RenderbufferId;
typedef GLValueWrapper<uint32_t, BufferTag, 0> BufferId;
typedef GLValueWrapper<uint32_t, VertexArrayTag, 0> VertexArrayId;

struct TextureObject
	: GLGeneratedValueWrapperCRTP<TextureId, TextureObject>
{
	void
	initialize();

	void
	finalize();

	void
	bind(uint32_t aTextureTarget);

	void
	unbind(uint32_t aTextureTarget);
};

struct FramebufferObject
	: GLGeneratedValueWrapperCRTP<FramebufferId, FramebufferObject>
{
	void
	initialize();

	void
	finalize();

	void
	bind(uint32_t aFramebufferType);

	void
	unbind(uint32_t aFramebufferType);
};

struct RenderbufferObject
	: GLGeneratedValueWrapperCRTP<RenderbufferId, RenderbufferObject>
{
	void
	initialize();

	void
	finalize();

	void
	bind();

	void
	unbind();
};


struct BufferObject
	: GLGeneratedValueWrapperCRTP<BufferId, BufferObject>
{
	void
	initialize();

	void
	finalize();

	void
	bind(uint32_t aBufferType);

	void
	unbind(uint32_t aBufferType);
};


struct VertexArrayObject
	: GLGeneratedValueWrapperCRTP<VertexArrayId, VertexArrayObject>
{
	void
	initialize();

	void
	finalize();

	void
	bind();

	void
	unbind();
};

// TODO make objects noncopyable

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
		: mBindable(aBinder.mBindable)
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
		mIsBinded = true;
	}

	void
	unbind()
	{
		SOGLU_ASSERT(mIsBinded);
		SOGLU_ASSERT(isGLContextActive());
		mBindable.unbind();
		mIsBinded = false;
	}

	TBindable &mBindable;
	bool mIsBinded;
};

template<typename TBindable, typename TTarget>
struct GLTargetBinder
{
	GLTargetBinder(TBindable &aBindable, TTarget aTarget)
		: mBindable(aBindable)
		, mTarget(aTarget)
		, mIsBinded(false)
	{
		bind();
	}

	GLTargetBinder(GLTargetBinder &&aBinder)
		: mBindable(aBinder.mBindable)
		, mTarget(aBinder.mTarget)
		, mIsBinded(aBinder.mIsBinded)
	{
		aBinder.mIsBinded = false;
	}

	~GLTargetBinder()
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
		mBindable.bind(mTarget);
		mIsBinded = true;
	}

	void
	unbind()
	{
		SOGLU_ASSERT(mIsBinded);
		SOGLU_ASSERT(isGLContextActive());
		mBindable.unbind(mTarget);
		mIsBinded = false;
	}

	TBindable &mBindable;
	TTarget mTarget;
	bool mIsBinded;
};

template<typename TBindable>
GLBinder<TBindable>
getBinder(TBindable &aBindable)
{
	return GLBinder<TBindable>(aBindable);
}

template<typename TBindable, typename TTarget>
GLTargetBinder<TBindable, TTarget>
getBinder(TBindable &aBindable, TTarget aTarget)
{
	return GLTargetBinder<TBindable, TTarget>(aBindable, aTarget);
}

struct GLEnabler
{
	GLEnabler(int aValue);
	~GLEnabler();

	int mValue;
};

struct GLDisabler
{
	GLDisabler(int aValue);
	~GLDisabler();

	int mValue;
};

inline GLEnabler
enable(int aValue)
{
	return GLEnabler(aValue);
}

inline GLDisabler
disable(int aValue)
{
	return GLDisabler(aValue);
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

	void
	setParameter(int aParameter, const glm::fvec4 &aParameterValues);
protected:
	Sampler(const Sampler &);
};

} // namespace soglu
