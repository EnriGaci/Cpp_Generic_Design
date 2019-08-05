#pragma once

#include <iostream>

template <class T> class Widget
{
public:
	void Fun();
	void Clone() { return new Widget<T>(); };
};
// OK: specialization of a member function of Widget
template <> void Widget<char>::Fun()
{
	std::cout << "Fun from Widget class\n";
}

template <class T, class U> class Gadget
{
	void Fun() { "Fun from Gadget class\n"; }
};
//// Error! Cannot partially specialize a member class of Gadget
//template <class U> void Gadget<char, U>::Fun()
//{
//	std::cout << "Fun from Gadget class\n";
//}


template <class T>
struct OpNewCreator
{
	static T* Create()
	{
		return new T;
	}
};
template <class T>
struct MallocCreator
{
	static T* Create()
	{
		void* buf = std::malloc(sizeof(T));
		if (!buf) return 0;
		return new(buf) T;
	}
};
template <class T>
struct PrototypeCreator
{
	PrototypeCreator(T* pObj = 0)
		:pPrototype_(pObj)
	{}
	T* Create()
	{
		return pPrototype_ ? pPrototype_->Clone() : nullptr;
	}
	T* GetPrototype() { return pPrototype_; }
	void SetPrototype(T* pObj) { pPrototype_ = pObj; }
private:
	T* pPrototype_;
};

// Library code
template <template <class Created> class CreationPolicy = OpNewCreator>
class WidgetManager : public CreationPolicy<Widget<char>>
{
	void DoSomething()
	{
		Gadget* pW = CreationPolicy<Gadget>().Create();
	}
};