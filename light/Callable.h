#pragma once
#include"Interpret.h"
class Callable
{
public:
	Callable() = default;
	virtual ~Callable() = default;
	virtual int Arity() = 0;
	virtual std::shared_ptr<Env> GetEnv() const = 0;
	virtual Object Call(Interpret &inter, std::list<Object> &args) = 0;
	virtual Object Get(Token &name) { return Object(); }
	virtual void Set(Token &name, Object &value) {}
};

struct Return
{
	Object value;
	Return(Object value)
		:value(value) {}
};
class Function :public Callable
{
	FunctionStmt* m_funDeclar;
	std::shared_ptr<Env> m_env = nullptr;
public:
	Function(FunctionStmt* funDeclar, std::shared_ptr<Env> env)
		:m_funDeclar(funDeclar), m_env(env){}
	~Function() = default;
	std::string Name() const
	{
		return m_funDeclar->tok.value.s;
	}
	std::shared_ptr<Env> GetEnv() const override
	{
		return m_env;
	}
	int Arity() override
	{
		return m_funDeclar->params.size();
	}
	Object Call(Interpret &inter, std::list<Object> &args) override
	{
		std::shared_ptr<Env> env = inter.GetEnv();
		auto itParam = m_funDeclar->params.begin();
		auto itArg = args.begin();
		while (itParam != m_funDeclar->params.end() && itArg != args.end())
		{
			env->define(*itParam, *itArg);
			itParam++, itArg++;
		}

		try
		{
			 m_funDeclar->body->accept(inter);
		}
		catch (const Return& ret)
		{
			return ret.value;
		}

		return Object(nullptr);
	}
	std::shared_ptr<Function> Bind(std::shared_ptr<Instance> ins)
	{
		std::shared_ptr<Env> env = std::make_shared<Env>(m_env);
		env->define("this", Object(ins));
		return std::make_shared<Function>(m_funDeclar, env);
	}
};
class Class :public Callable
{
	std::string m_name;
	std::shared_ptr<Env> m_env = nullptr;
	std::unique_ptr<std::map<std::string, std::shared_ptr<Function>>> m_methods;
public:
	Class(std::string name, std::unique_ptr<std::map<std::string, std::shared_ptr<Function>>> methods)
		:m_name(name), m_methods(std::move(methods)){}
	~Class() = default;
	std::string Name() const
	{
		return m_name;
	}
	std::shared_ptr<Env> GetEnv() const override
	{
		return nullptr;
	}
	int Arity() override
	{
		return 0;
	}
	Object Call(Interpret &inter, std::list<Object> &args) override
	{
		std::shared_ptr<Instance> ins = std::make_shared<Instance>(this);
		return Object(ins);
	}
	std::shared_ptr<Function> FindMethod(std::string name, std::shared_ptr<Instance> ins)
	{
		auto m = m_methods->find(name);
		if (m != m_methods->end())
			return m->second->Bind(ins);
		return nullptr;
	}
};
class Instance :public std::enable_shared_from_this<Instance>, public Callable
{
	Class* m_cls;
	std::map<std::string, Object> m_fields;
public:
	Instance(Class* cls)
		:m_cls(cls){}
	~Instance() = default;
	std::string Name() const
	{
		return m_cls->Name();
	}
	std::shared_ptr<Env> GetEnv() const override
	{
		return nullptr;
	}
	int Arity() override
	{
		return 0;
	}
	Object Call(Interpret &inter, std::list<Object> &args) override
	{
		
		return Object();
	}
	Object Get(Token &name)  override
	{
		auto var = m_fields.find(name.value);
		if (var != m_fields.end())
			return var->second;
		std::shared_ptr<Function> method = m_cls->FindMethod(name.value, shared_from_this());
		if (method)
			return Object(method);
		else
			throw Error("Undefined property '" + (std::string)name.value + "'.");
	}
	void Set(Token &name, Object &value)
	{
		m_fields[name.value] = value;
	}
};