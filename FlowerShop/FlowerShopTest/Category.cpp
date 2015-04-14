#include "stdafx.h"
#include "InputOutputInfoConsole.h"

using namespace std; 


Category::Category(BaseFactory*(*Getfunction)()):NumberOfProducts(0),Balance(0),BF(Getfunction())
{
	if(!BF)
		throw 0;
	BI = BF->CreateInput();
	BOut = BF->CreateOutput();
	if(!BI||!BOut)
		throw 0;

	try
	{
		BaseInPFlow = (BaseInputOutputInfo*) new InputOutputInfoConsole;

		BLoad = BF->CreateLoad();
		if(!BLoad)
			throw 0;
		BSave = BF->CreateSave();
		if(!BSave)
			throw 0;
		bool FirstStartOfProg = BLoad->FirstLoad();
		if(FirstStartOfProg)
			throw 0;

		NumberOfProducts = BLoad->ReturnNumberOfProducts();
		if(NumberOfProducts==0)
			throw 0;

		try
		{
			ArrayOfProducts = new  Product*[NumberOfProducts];
		}
		catch(std::bad_alloc& ba)
		{
			throw ba;
		}

		for(int i=0; i<NumberOfProducts; ++i)
			ArrayOfProducts[i]=0;

		bool succsess  = BLoad->CreateProduct(ArrayOfProducts);
		if(!succsess)
		{
			int numb(0);
			for(int i=0; i<NumberOfProducts; ++i)
				if(ArrayOfProducts[i]!=0)
					++numb;
			NumberOfProducts = numb;
		}
	}
	catch(int a)
	{

		BaseInPFlow->ShopOutput("There is no saved files for ");
		BaseInPFlow->ShopOutput(BI->GetCategoryName());
		BaseInPFlow->ShopOutput(" category. Need to create products by yourself. Please enter number of products to create:");
		int count(0);

		int test=0;
		while(test!=1)
		{
			char* val = BaseInPFlow->GetValue();

			count = atoi(val);
			if(count>1000)
				BaseInPFlow->ShopOutput("Value should be less than 1000! Try again.");
			else if(count<0)
				BaseInPFlow->ShopOutput("Value should be bigger than 0! Try again."); 
			else if(count==0)
				BaseInPFlow->ShopOutput("Wrong value, try again.");
			else	
				test=1;

		}

		try 
		{
			ArrayOfProducts = new  Product*[count];
		}
		catch(std::bad_alloc& ba)
		{
			throw ba;
		}
		for(int i=0; i<count; ++i)
		{
			ArrayOfProducts[i]= BI->CreateProduct(BaseInPFlow);
			if(ArrayOfProducts[i] != 0)
				++NumberOfProducts;
		}

	}
	catch(std::bad_alloc& ba)
	{
		throw 0;
	}
}

Category::~Category()
{
	for(int i=0; i<NumberOfProducts; ++i)
		delete ArrayOfProducts[i];
	delete ArrayOfProducts;
	delete BF;
	delete BI;
	delete BOut;
}

int Category::GetNumberOfProducts()
{
	return NumberOfProducts;
}

double Category::GetBalance()
{
	return Balance;
}

BaseInput* Category::GetBaseInput()
{
	return BI;
}

BaseOutput* Category::GetBaseOutput()
{
	return BOut;
}

Product** Category::GetArrayOfProducts()
{
	return ArrayOfProducts;
}

void Category::SetBalance(double bal)
{
	Balance = bal;
}

void Category::SetArrayOfProducts(Product** ArrayOfProductsNew)
{
	ArrayOfProducts = ArrayOfProductsNew;
}

void Category::SetNumberOfProducts(int NewNumber)
{
	NumberOfProducts = NewNumber;
}