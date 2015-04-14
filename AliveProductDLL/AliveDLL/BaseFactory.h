class BaseInput; 
 class BaseOutput;
 class BaseLoad;
 class BaseSave;

#pragma once
class BaseFactory
{
public:
	 virtual BaseInput* CreateInput()=0;
	 virtual BaseOutput* CreateOutput()=0;
	 virtual BaseLoad* CreateLoad()=0;
	 virtual BaseSave* CreateSave()=0;
};
