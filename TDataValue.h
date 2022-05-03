#pragma once
#include <string>

class TDataValue;
typedef TDataValue* PTDataValue;

class TDataValue {
public:
	virtual TDataValue* GetCopy() = 0;
	virtual std::string ToString() = 0;
};