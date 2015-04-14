#include "BaseInputOutputInfo.h"

class InputOutputInfoConsole: public BaseInputOutputInfo
{
	HANDLE hStdout;
	CONSOLE_SCREEN_BUFFER_INFO csbInfo;
    
	int WorkFieldTopY, WorkFiledBottomY;
	int FooterTextTopY,FooterTextBottomY;
	
	int MenuIndex;
	int NumberOfProducts;
	int NumberOfCategories;
	Category** ArrayOfCategories;
	Product** ArrayOfProducts;
	
	void Header();
	void Footer();
	bool Menu1(ShopOperations* SO);
	bool Menu2(int ChoiceStep,ShopOperations* SO,int* OutputList=0);
	void ClearWorkField();
	bool Sort(ShopOperations* SO,int CategoryIndex);
	bool Find(ShopOperations* SO,int CategoryIndex);
	bool BuySellDelete(ShopOperations* SO,int CategoryIndex, int* ProductsAvaliable, int ProductsAvaliableSize, int BuySellDelete);
	bool AddProduct(ShopOperations* SO, int CategoryIndex);
public:
	virtual bool ShopInput(ShopOperations* SO);
	virtual void ShopOutput(const char*  OutputString,int NewLine=0);
	virtual char* GetValue();
	InputOutputInfoConsole();

};