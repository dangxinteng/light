#pragma once
#include<string>
struct Error
{
	std::string errMsg;
	Error(const std::string &errMsg)
		:errMsg(errMsg){}
	virtual const char* what() const noexcept
	{
		return errMsg.c_str();
	}
};

