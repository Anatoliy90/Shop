extern class Product;
class BaseInputOutputInfo;
//#pragma once
class BaseOutput
{
	public:
	virtual void Show(Product* Prod,int ParameterNumber,BaseInputOutputInfo* BaseInOut) = 0;
	virtual void GetProductParameters(const char** ParametersArray)=0;
	virtual int Sort(Product** ArrayOfProducts,int NumberOfProducts, char* SortArray, int Method)=0;
	virtual void Find(Product** ArrayOfProducts,int NumberOfProducts, char* value,int param, int* result)=0;
};