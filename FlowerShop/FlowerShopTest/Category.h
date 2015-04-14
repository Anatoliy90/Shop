#include "stdafx.h"

using namespace std;

class Category
{
	Product** ArrayOfProducts;
	int NumberOfProducts, SizeOfArray;
	double Balance;
	BaseFactory* BF;
	BaseInput* BI;
	BaseOutput* BOut;	
	BaseLoad* BLoad;
	BaseSave* BSave;
	BaseInputOutputInfo* BaseInPFlow;
public:
	friend class Shop;

	int GetNumberOfProducts();
	double GetBalance();
	void SetBalance(double bal);
	void SetArrayOfProducts(Product** ArrayOfProductsNew);
	void SetNumberOfProducts(int NewNumber);
	Product** GetArrayOfProducts();
	BaseInput* GetBaseInput();
	BaseOutput* GetBaseOutput();
	Category(BaseFactory*(*Getfunction)());
	~Category();
	void Sort();
};