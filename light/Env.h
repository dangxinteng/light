#pragma once
#include<map>
struct Env:std::enable_shared_from_this<Env>
{
	std::map<std::string, Object> m_varValues;
	std::shared_ptr<Env> m_envNext;
	Env()
	{
		m_envNext = nullptr;
	}
	Env(std::shared_ptr<Env> next)
		:m_envNext(next)
	{
	}
	void define(std::string name, Object value)
	{
		if (m_varValues.count(name))
			throw Error("redefined variable '" + name + "'.");
		m_varValues.insert({ name, value });
	}
	void assign(std::string name, Object value)
	{
		if (m_varValues.count(name))
		{
			m_varValues[name] = value;
			return;
		}
		if (!m_envNext )
			throw Error("Undefined variable '" + name + "'.");

	}
	Object& get(std::string name) 
	{
		try
		{
			if (m_varValues.count(name))
				return m_varValues.at(name);

			if (m_envNext != nullptr)
				return m_envNext->get(name);
			else
				throw Error("Undefined variable '" + name + "'.");
		}
		catch (const std::out_of_range&)
		{
			throw Error("Undefined variable '" + name + "'.");
		}
	}
	std::shared_ptr<Env> getLocalEnv(size_t dist)
	{
		std::shared_ptr<Env> penv = shared_from_this();
		for (size_t i = 0; i < dist; i++)
		{
			penv = penv->m_envNext;
		}
		return penv;
	}
	Object& getLocal(size_t dist, std::string name)
	{
		return getLocalEnv(dist)->m_varValues.at(name);
	}
	void assignLocal(size_t dist, std::string name, Object value)
	{
		getLocalEnv(dist)->m_varValues[name] = value;
	}
};