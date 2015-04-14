// Приведенный ниже блок ifdef - это стандартный метод создания макросов, упрощающий процедуру 
// экспорта из библиотек DLL. Все файлы данной DLL скомпилированы с использованием символа ALIVEDLL_EXPORTS,
// в командной строке. Этот символ не должен быть определен в каком-либо проекте
// использующем данную DLL. Благодаря этому любой другой проект, чьи исходные файлы включают данный файл, видит 
// функции ALIVEDLL_API как импортированные из DLL, тогда как данная DLL видит символы,
// определяемые данным макросом, как экспортированные.
#ifdef ALIVEDLL_EXPORTS
#define ALIVEDLL_API __declspec(dllexport)
#else
#define ALIVEDLL_API __declspec(dllimport)
#endif
#include"BaseFactory.h"
#include"BaseInput.h"
#include"BaseOutput.h"
#include"BaseLoad.h"
#include"BaseSave.h"
#include"Product.h"
#include"BaseInputOutputInfo.h"

// Этот класс экспортирован из AliveDLL.dll
class ALIVEDLL_API CAliveDLL {
public:
	CAliveDLL(void);
	// TODO: Добавьте здесь свои методы.
};
                                                                  
extern ALIVEDLL_API int nAliveDLL;

ALIVEDLL_API int fnAliveDLL(void);

struct InputProductData
{
    double price_sell; 
    double price_buy;
	char Name[SizeOfName];
    char Color[SizeOfColor];
	int Freshness,Smell, NumberInStock;
};

extern "C" ALIVEDLL_API BaseFactory* GetFactory();

class AliveFactory: public BaseFactory
{
	virtual BaseInput* CreateInput();
    virtual BaseOutput* CreateOutput();
	virtual BaseLoad* CreateLoad();
	virtual BaseSave* CreateSave();
};

class AliveInput: public BaseInput
{
   virtual Product* CreateProduct(BaseInputOutputInfo* BaseInOut);
public:
	virtual const char* GetCategoryName();
	void GetIPD(InputProductData* Info,BaseInputOutputInfo* BaseInOut); 
};

class AliveOperations
{
public:
	int SortByName(Product* Left, Product* Right);
	int SortByColor(Product* Left, Product* Right);
	int SortByPrice(Product* Left, Product* Right);
	int SortByStock(Product* Left, Product* Right);
	int SortByFresh(Product* Left, Product* Right);
	int SortBySmell(Product* Left, Product* Right);
	void CheckDoublesName(Product** ArrayOfProducts, int NumberOfProducts, int* CheckArray);
	void CheckDoublesColor(Product** ArrayOfProducts, int NumberOfProducts, int* CheckArray);
	void CheckDoublesPrice(Product** ArrayOfProducts, int NumberOfProducts, int* CheckArray);
	void CheckDoublesStock(Product** ArrayOfProducts, int NumberOfProducts, int* CheckArray);
	void CheckDoublesFresh(Product** ArrayOfProducts, int NumberOfProducts, int* CheckArray);
	void CheckDoublesSmell(Product** ArrayOfProducts, int NumberOfProducts, int* CheckArray);

	bool FindName(Product* Product,char* value);
	bool FindColor(Product* Product,char* value);
	bool FindPrice(Product* Product,char* value);
	bool FindStock(Product* Product,char* value);
	bool FindFreshness(Product* Product,char* value);
	bool FindSmell(Product* Product,char* value);
};

class AliveOutput: public BaseOutput
{
	AliveOperations AlSort;
	void AliveSort(Product** ArrayOfProducts,int Method, int * CheckArray, int(AliveOperations::*SortFunction)(Product*,Product*));
public:
	virtual void Show(Product* Prod,int ParameterNumber,BaseInputOutputInfo* BaseInOut);
	virtual void GetProductParameters(const char** ParametersArray);
	virtual int Sort(Product** ArrayOfProducts,int NumberOfProducts, char* SortArray, int Method);
	void Find(Product** ArrayOfProducts,int NumberOfProducts, char* value,int param, int* result);
};

class AliveProduct: public Product
{
friend class AliveOutput;
friend class AliveOperations;
public:
	int Freshness;
    int Smell;
	AliveProduct(InputProductData* PrData);
	~AliveProduct();
};

class AliveLoad: public BaseLoad
{
	virtual bool FirstLoad();
	virtual int ReturnNumberOfProducts();
	virtual int ReturnBalance();
	bool CreateProduct(Product** ProdArray);
};

class AliveSave: public BaseSave
{
	bool SaveProducts(int Balance, int NumberOfProducts, Product** ArrayOfProducts);
};