#include "stdafx.h"

using namespace std;

class Category;
class BaseInputOutputInfo;

struct ShopOperations
{
	Shop* shop;
	int operation;
	int CategoryIndex;
	int MetodOfSort;
	int ParamIndex;
	int MaxCountOfProductsInCategories;
	int * IndexesOfProductAvaiableToShow;
	int * ArrayOfProductIndexesForOperation;
	char* ArrayOfParameters;
};

class Shop
{
	void Save();
public:
	Category** ArrayOfCategories;
    int NumberOfCategories, SizeOfArray;
	ShopOperations ShopOp;
	BaseInputOutputInfo* BaseInPFlow;
    int Balance;

	Category** GetCategoryArray();
	Shop();
	~Shop();
	void WorkWithShop();

	BaseInputOutputInfo* GetSO();
	void RefreshShopOperations();
};
