#include "stdafx.h"

#pragma once

class BaseInputOutputInfo
{
public:
	virtual bool ShopInput(ShopOperations* SO)=0;
	virtual void ShopOutput(const char*  OutputString, int NewLine =0)=0;
	virtual char* GetValue()=0;
};