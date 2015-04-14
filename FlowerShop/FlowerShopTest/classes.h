#include "stdafx.h"
#include "BaseFactory.h"
#include "BaseInput.h"
#include "BaseOutput.h"
#include "Product.h"

using namespace std;


class Category
{
	Product** ArrayOfProducts;
	int NumberOfProducts, SizeOfArray;
	double Balance;
	BaseFactory* BF;
	BaseInput* BI;
	BaseOutput* BOut;
public:
	friend class Shop;
	Category(BaseFactory*(*Getfunction)());
	~Category();
	void Sort();
	int FirstStartOfProgram();
};

class Shop
{
	Category** ArrayOfCategories;
	int NumberOfCategories, SizeOfArray;
	int WorkFieldTopY, WorkFiledBottomY;
	int FooterTextTopY,FooterTextBottomY;
	int Balance;
	HANDLE hStdout;
	CONSOLE_SCREEN_BUFFER_INFO csbInfo;

	void Header();
	void Footer();
	void Menu1();
	void Menu2(int ChoiceStep,int* OutputList=0);
	void Sort(int CategoryIndex);
	void Find(int CategoryIndex);
	void BuySellDelete(int CategoryIndex, int* ProductsAvaliable, int ProductsAvaliableSize, int BuySellDelete);
	void AddProduct(int CategoryIndex);
	void ClearWorkField();
	void Save();
public:
	Shop();
	~Shop();
	void WorkWithShop();
};


