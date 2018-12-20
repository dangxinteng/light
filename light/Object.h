#pragma once
#include<iostream>
#include<memory>
#include"Exception.h"

//class Interpreter;
class Callable;
class Function;
class Class;
class Instance;

struct Object
{
	enum ValueType { STRINGOBJ, NUMBEROBJ, NILOBJ, BOOLOBJ, FUNOBJ, CLASSOBJ, INSTANCEOBJ };
	ValueType t;
	double d;
	std::string s;
	bool b;
	nullptr_t nil;
	std::shared_ptr<Function> fun = nullptr;
	std::shared_ptr<Class> cls = nullptr;
	std::shared_ptr<Instance> ins = nullptr;
	Object() = default;
	~Object() = default;
	explicit Object(nullptr_t nil)
		:t(NILOBJ), nil(nil)
	{
	}
	explicit Object(bool b) 
		:t(BOOLOBJ), b(b)
	{
	}
	explicit Object(double d) 
		:t(NUMBEROBJ), d(d)
	{
	}
	explicit Object(std::string s) 
		:t(STRINGOBJ), s(s)
	{
	}
	explicit Object(std::shared_ptr<Function> fun)
		:t(FUNOBJ), fun(fun)
	{
	}
	explicit Object(std::shared_ptr<Class> cls)
		:t(CLASSOBJ), cls(cls)
	{
	}
	explicit Object(std::shared_ptr<Instance> ins)
		:t(INSTANCEOBJ), ins(ins)
	{
	}
	operator bool() const;
	operator double() const;	
	operator std::string()const;	
	operator std::shared_ptr<Callable>()const;

	double operator-()const;
	bool operator!()const ;

	bool operator>(const Object& obj)const;
	bool operator==(const Object& obj)const;

	Object operator+(const Object& obj) const;
	Object operator-(const Object& obj)const;
	Object operator*(const Object& obj)const;
	Object operator/(const Object& obj)const;

	inline bool IsInstace()const
	{
		return ins != nullptr;
	}
	inline Object& operator+=(const Object& obj)
	{
		return *this = *this + obj;
	}
	inline Object& operator-=(const Object& obj)
	{
		return *this = *this - obj;
	}
	inline Object& operator*=(const Object& obj)
	{
		return *this = *this * obj;	
	}
	inline Object& operator/=(const Object& obj)
	{
		return *this = *this / obj;
	}

	inline bool operator>=(const Object& obj)const
	{
		return (*this > obj || *this == obj);
	}
	inline bool operator<(const Object& obj)const
	{
		return !(*this >=  obj);
	}	
	inline bool operator<=(const Object& obj)const
	{
		return !(*this > obj);
	}
	inline bool operator!=(const Object& obj)const
	{
		return !(*this == obj);
	}
	friend std::ostream& operator<<(std::ostream &os, const Object &obj)
	{
		os << (std::string)obj;
		return os;
	}
};