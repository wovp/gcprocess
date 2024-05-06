#pragma once
#include <iostream>
#include <string>
class gxStr
{
public:
	gxStr();
	~gxStr();
	int extractID(const std::string& text);
	int extractID(const std::wstring& text);
};

