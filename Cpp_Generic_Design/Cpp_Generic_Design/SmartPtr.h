#pragma once

template <class T> struct NoChecking
{
	static void Check(T*) {}
};


template <class T> struct EnsureNotNull
{
	static void Check(T*& ptr)
	{
		if (!ptr) ptr = GetDefaultValue();
	}
};

template
<
	class T,
	template <class> class CheckingPolicy,
	template <class> class ThreadingModel
>
class SmartPtr
	: public CheckingPolicy<T>
	, public ThreadingModel<SmartPtr>
{
	T* operator->()
	{
		typename ThreadingModel<SmartPtr>::Lock guard(*this);
		CheckingPolicy<T>::Check(pointee_);
		return pointee_;
	}
private:
	T* pointee_;
};