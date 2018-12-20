#include"Object.h"
#include"Callable.h"
#include<sstream>
Object::operator bool() const
{
	switch (t)
	{
	case NILOBJ:    return false;
	case BOOLOBJ:   return b;
	case NUMBEROBJ: return d != 0.;
	case STRINGOBJ: return s != "";
	}
	throw Error("Could not convert object to bool.");
}

Object::operator double()const
{
	switch (t)
	{
	case NILOBJ:    return 0.;
	case BOOLOBJ:   return b ? 1. : 0.;
	case NUMBEROBJ: return d;
	case STRINGOBJ: return std::stod(s);
	}
	throw Error("Could not convert object to number!");
}

Object::operator std::string()const
{
	switch (t)
	{
	case NILOBJ:    return "nil";
	case BOOLOBJ:   return b ? "true" : "false";
	case NUMBEROBJ: 
	{
		std::ostringstream ostr;
		ostr << std::noshowpoint << d;
		return ostr.str();
	}	
	case STRINGOBJ: return s;
	case FUNOBJ:
		return "<fun " + fun->Name() + ">";
	case CLASSOBJ:
		return "<class " + cls->Name() + ">";
	case INSTANCEOBJ:
		return "<instance " + ins->Name() + ">";
	}
	throw Error("Could not convert object to string.");
}

Object::operator std::shared_ptr<Callable>() const
{

	switch (t)
	{
	case NILOBJ:
		throw Error("Could not convert nil to callable..");
	case BOOLOBJ:
		throw Error("Could not convert bools to callable.");
	case STRINGOBJ:
		throw Error("Could not convert string to callable.");
	case NUMBEROBJ:	
		throw Error("Could not convert number to callable.");
	case FUNOBJ:      return std::static_pointer_cast<Callable>(fun);
	case CLASSOBJ:    return std::static_pointer_cast<Callable>(cls);
	case INSTANCEOBJ: return std::static_pointer_cast<Callable>(ins);
	}
	throw Error("Could not convert object to callable..");
}



double Object::operator-() const
{
	if (t == NUMBEROBJ)
		return  -d;
	throw Error("only number can execute -(negative) operator!");
}

bool Object::operator!() const
{
	if (t == BOOLOBJ)
		return  !b;
	if (t == NUMBEROBJ)
		return  d == 0.;
	if (t == STRINGOBJ)
		return  s == "";
	if (t == NILOBJ)
		return  true;
	throw Error("only boolen¡¢number¡¢string¡¢nil can execute ! operator!");
}

bool Object::operator>(const Object & obj)const
{
	if (t== obj.t)
	{
		switch (obj.t)
		{
		case NILOBJ: case BOOLOBJ:
			throw Error("Nil and bool are not well-ordered.");
		case NUMBEROBJ:
			return d > obj.d;
		case STRINGOBJ:
			return s > obj.s;
		default:
			throw Error("Objects are not well-ordered.");
		}
	}
	throw Error("Only objects of the same type can be well-ordered.");
}

bool Object::operator==(const Object & obj)const
{
	if (t == obj.t)
	{
		switch (obj.t)
		{
		case NILOBJ:
			return true;
		case BOOLOBJ:
			return b == obj.b;
		case NUMBEROBJ:
			return d == obj.d;
		case STRINGOBJ:
			return s == obj.s;
		default:
			throw Error("Cannot compare objects for equality.");
		}
	}
	if (t == NILOBJ || obj.t == NILOBJ) return false;
	switch (t)
	{
	case BOOLOBJ: return b == (bool)obj;
	case NUMBEROBJ: return d == (double)obj;
	case STRINGOBJ: return s == (std::string)obj;
	default:
		throw Error("Cannot compare objects for equality.");
	}
}

Object Object::operator+(const Object & obj) const
{
	if (t == obj.t)
	{
		switch (t)
		{
		case NILOBJ:
			throw Error("Cannot add nil.");
		case BOOLOBJ:
			throw Error("Cannot add bools.");
		case NUMBEROBJ:	return Object(d + obj.d);
		case STRINGOBJ: return Object(s + obj.s);
		default:
			throw Error("Cannot add objects.");
		}
	}
	throw Error("Only objects of the same type can be add.");
}

Object Object::operator-(const Object & obj) const
{
	if (t == obj.t)
	{
		switch (t)
		{
		case NILOBJ:
			throw Error("Cannot minus nil.");
		case BOOLOBJ:
			throw Error("Cannot minus bools.");		
		case STRINGOBJ: 
			throw Error("Cannot minus strings.");
		case NUMBEROBJ:	return Object(d - obj.d);
		default:
			throw Error("Cannot minus objects.");
		}
	}
	throw Error("Only number can be minus.");
}

Object Object::operator*(const Object & obj) const
{
	if (t == obj.t)
	{
		switch (t)
		{
		case NILOBJ:
			throw Error("Cannot multi nil.");
		case BOOLOBJ:
			throw Error("Cannot multi bools.");
		case STRINGOBJ:
			throw Error("Cannot multi strings.");
		case NUMBEROBJ:	return Object(d * obj.d);
		default:
			throw Error("Cannot multi objects.");
		}
	}
	throw Error("Only number can be multi.");
}

Object Object::operator/(const Object & obj) const
{
	if (t == obj.t)
	{
		switch (t)
		{
		case NILOBJ:
			throw Error("Cannot div nil.");
		case BOOLOBJ:
			throw Error("Cannot div bools.");
		case STRINGOBJ:
			throw Error("Cannot div strings.");
		case NUMBEROBJ:	
			if (obj.d == 0.)
				throw Error("Cannot div by zero.");
			return Object(d / obj.d);
		default:
			throw Error("Cannot div objects.");
		}
	}
	throw Error("Only number can be div.");
}

