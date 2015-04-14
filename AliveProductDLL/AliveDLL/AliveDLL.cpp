// AliveDLL.cpp: определяет экспортированные функции для приложения DLL.
//

#include "stdafx.h"
#include "AliveDLL.h"

using namespace std;

enum{EMPTY,NAME,PRICE,COLOR,NUMBER_OF_PROD,FRESHNESS,SMELL};

// Пример экспортированной переменной
ALIVEDLL_API int nAliveDLL=0;

// Пример экспортированной функции.
ALIVEDLL_API int fnAliveDLL(void)
{
	return 42;
}

// Конструктор для экспортированного класса.
// см. определение класса в AliveDLL.h
CAliveDLL::CAliveDLL()
{
	return;
}


extern "C" ALIVEDLL_API BaseFactory* GetFactory()
{
	try
	{
		AliveFactory* AF = new AliveFactory;
		return AF;
	}
	catch (std::bad_alloc& ba)
	{
		return 0;
	}
}

BaseInput* AliveFactory::CreateInput()
{
	try
	{
		AliveInput* AI = new AliveInput;
		return AI;
	}
	catch (std::bad_alloc& ba)
	{
		return 0;
	}
}

BaseOutput* AliveFactory::CreateOutput()
{
	try
	{
		AliveOutput* AO = new AliveOutput;
		return AO;
	}
	catch (std::bad_alloc& ba)
	{
		return 0;
	}
}

BaseLoad* AliveFactory::CreateLoad()
{
	try
	{
		AliveLoad* AL = new AliveLoad;
		return AL;
	}
	catch (std::bad_alloc& ba)
	{
		return 0;
	}
}

BaseSave* AliveFactory::CreateSave()
{
	try
	{
		AliveSave* AS = new AliveSave;
		return AS;
	}
	catch (std::bad_alloc& ba)
	{
		return 0;
	}
}

const char* AliveInput::GetCategoryName()
{
	return "Alive";
}

Product* AliveInput::CreateProduct(BaseInputOutputInfo* BaseInOut)
{
	InputProductData  IPD;
	GetIPD(&IPD,BaseInOut);
	try
	{
		AliveProduct* AP = new AliveProduct(&IPD);
		return (Product*)AP;
	}
	catch (std::bad_alloc& ba)
	{
		return 0;
	}
}

void BufferEnter(char* value, int Size,BaseInputOutputInfo* BaseInOut)
{
	int test=0;
	while(test!=1)
	{
		char *val = BaseInOut->GetValue();
		memcpy_s(value,Size,val,strlen(val)*sizeof(char));
		if(strlen(value)>Size)
		{
			BaseInOut->ShopOutput("Length of string should be less than ");
			char message[10]= {'\0'};
		_itoa_s(Size,message,10,10);
		BaseInOut->ShopOutput(message);    
		BaseInOut->ShopOutput(" ! Try again.",1);
		}
		else if(strlen(value)==0)
			cout<<"You entered nothing! Try again."<<endl; 
		else
			test=1;

	}
}

void BufferEnter(double* value,BaseInputOutputInfo* BaseInOut)
{
	int test=0;
	double count(0);
	
	while(test!=1)
	{
		char* val = BaseInOut->GetValue();
	    count = atof(val);
		if(count==0)
		{
			BaseInOut->ShopOutput("Wrong value, try again.",1);
		}
		else
		{
			if(count>1000000)
				BaseInOut->ShopOutput("Value should be less than 1 000 000! Try again.",1);
			else if(count<=0)
				BaseInOut->ShopOutput("Value should be bigger than 0! Try again.",1); 
			else
				test=1;
		}
	}
	*value = count;
}

void BufferEnter(int* value, int LeftValue, int RightValue,BaseInputOutputInfo* BaseInOut)
{
	int test=0,count(0);
	while(test!=1)
	{
		char* val = BaseInOut->GetValue();
		count = atoi(val);
		if(count==0)
		{
			BaseInOut->ShopOutput("Wrong value, try again.",1);
		}
		else
		{
			if(count>RightValue)
			{
				BaseInOut->ShopOutput("Value should be less than ");
				char message[10]= {'\0'};
				_itoa_s(RightValue,message,10,10);
				BaseInOut->ShopOutput(message);
				BaseInOut->ShopOutput("! Try again.",1);
			}
			else if(count<LeftValue)
			{
				BaseInOut->ShopOutput("Value should be bigger than ");
				char message[10]= {'\0'};
				_itoa_s(LeftValue,message,10,10);
				BaseInOut->ShopOutput(message);
				BaseInOut->ShopOutput("! Try again.",1);
			}
			else
				test=1;
		}
	}
	*value = count;
}


void AliveInput::GetIPD(InputProductData* Info,BaseInputOutputInfo* BaseInOut)
{
	for(int i=0;i<SizeOfName;++i)
		Info->Name[i]='\0';
	for(int i=0;i<SizeOfColor;++i)
		Info->Color[i]='\0';
	BaseInOut->ShopOutput("Please enter name of the product:",1);
	BufferEnter(Info->Name, SizeOfName,BaseInOut);
	BaseInOut->ShopOutput("Please enter color (string) of the product:",1);
	BufferEnter(Info->Color, SizeOfColor,BaseInOut);
	BaseInOut->ShopOutput("Please enter PURCHASE price of the product:",1);
	BufferEnter(&Info->price_buy,BaseInOut);
	BaseInOut->ShopOutput("Please enter SELL price of the product:",1);
	BufferEnter(&Info->price_sell,BaseInOut);
	BaseInOut->ShopOutput("Please enter level of freshness (number) of the product:");
	BaseInOut->ShopOutput("1. new 2. one week old 3. two weeks old 4. elder than two weeks",1);
	BufferEnter(&Info->Freshness,1,4,BaseInOut);
	BaseInOut->ShopOutput("Please enter smell (number) of the product:");
	BaseInOut->ShopOutput("1. Weak 2. Strong",1);
	BufferEnter(&Info->Smell,1,2,BaseInOut);
	BaseInOut->ShopOutput("Please enter number of product items avaliable in stock:",1);
	BufferEnter(&Info->NumberInStock,0,100000,BaseInOut);

}

AliveProduct::AliveProduct(InputProductData* PrData)
{
	ParametersCount = 6;
	price_buy = PrData->price_buy;
	price_sell = PrData->price_sell;
	NumberInStock = PrData->NumberInStock;
	Freshness = PrData->Freshness;
	Smell = PrData->Smell;
	memcpy(&Name,&PrData->Name,SizeOfName);
	memcpy(&Color,&PrData->Color,SizeOfColor);
}

AliveProduct::~AliveProduct()
{
	delete &price_buy;
	delete &price_sell;
	delete &Freshness;
	delete &Smell;
	delete[] &Name;
	delete[] &Color;
	delete &NumberInStock;
}

void AliveOutput::Show(Product* Prod,int ParameterNumber,BaseInputOutputInfo* BaseInOut)
{
	AliveProduct* prod1 = (AliveProduct*)Prod;
	if(ParameterNumber==0)
	{
		BaseInOut->ShopOutput("Name: ");
	    BaseInOut->ShopOutput(prod1->Name);
	}
	else if(ParameterNumber==1)
	{
		BaseInOut->ShopOutput("Color: ");
	    BaseInOut->ShopOutput(prod1->Color);
	}
	else if(ParameterNumber==2)
	{
		BaseInOut->ShopOutput("Price: ");
		char message[10]= {'\0'};
		_itoa_s(prod1->price_sell,message,10,10);
		BaseInOut->ShopOutput(message); 
	}
	else if(ParameterNumber==3)
	{
		BaseInOut->ShopOutput("Number of product avaliable: ");
	    char message[10]= {'\0'};
		_itoa_s(prod1->NumberInStock,message,10,10);
		BaseInOut->ShopOutput(message);
	}
	else if(ParameterNumber==4)
	{
		if(prod1->Freshness==1)
			BaseInOut->ShopOutput("Freshness: new");
		else if(prod1->Freshness==2)
			BaseInOut->ShopOutput("Freshness: one week old");
		else if(prod1->Freshness==3)
			BaseInOut->ShopOutput("Freshness: two weeks old");
		else if(prod1->Freshness==4)
			BaseInOut->ShopOutput("Freshness: elder than two weeks");
	}
	else if(ParameterNumber==5)
	{
		if(prod1->Smell==1)
			BaseInOut->ShopOutput("Smell: Weak");
		else if(prod1->Smell==2)
			BaseInOut->ShopOutput("Smell: Strong");
	}

}


void AliveOutput::GetProductParameters(const char** ParametersArray)
{
	const char *pr1 = "Name";
	const char *pr2 = "Price";
	const char *pr3 = "Color";
	const char *pr4 =  "Number of products in stock";
	const char *pr5 = "Freshness";
	const char *pr6 = "Smell";
	ParametersArray[0] = pr1;
	ParametersArray[1] = pr2;
	ParametersArray[2] = pr3;
	ParametersArray[3] = pr4;
	ParametersArray[4] = pr5;
	ParametersArray[5] = pr6;

}

int AliveOutput::Sort(Product** ArrayOfProducts,int NumberOfProducts, char* SortArray, int Method)
{
	if(NumberOfProducts<2)
		return 0;

	int NumberOfParams = strlen(SortArray);
	int ParamForSorting(0);
	if(Method!=1)
		Method=-1;
	if(NumberOfParams==0)
		return 0;

	int size = (NumberOfProducts<2)?2:NumberOfProducts;

	int *CheckArray = new int[size];
	for(int i=0;i<size;++i)
		CheckArray[i]=0;

	for(int i=0;i<NumberOfParams;++i)
	{
		if(i==0)
			CheckArray[1]=NumberOfProducts;


		char tmp; tmp = SortArray[i];
		ParamForSorting = atoi(&tmp);
		int(AliveOperations::*SortFunction)(Product*,Product*);
		void(AliveOperations::*CheckDoublesFunction)(Product**, int, int*);
		switch(ParamForSorting)
		{
		case NAME:
			{
				SortFunction = &AliveOperations::SortByName;
				CheckDoublesFunction = &AliveOperations::CheckDoublesName;  
				break;
			}
		case PRICE:               
			{
				SortFunction = &AliveOperations::SortByPrice;
				CheckDoublesFunction = &AliveOperations::CheckDoublesPrice; 
				break;
			}
		case COLOR:
			{
				SortFunction = &AliveOperations::SortByColor;
				CheckDoublesFunction = &AliveOperations::CheckDoublesColor; 
				break;
			}
		case NUMBER_OF_PROD:
			{
				SortFunction = &AliveOperations::SortByStock;
				CheckDoublesFunction = &AliveOperations::CheckDoublesStock; 
				break;
			}
		case FRESHNESS:
			{
				SortFunction = &AliveOperations::SortByFresh;
				CheckDoublesFunction = &AliveOperations::CheckDoublesFresh; 
				break;
			}
		case SMELL:
			{
				SortFunction = &AliveOperations::SortBySmell;
				CheckDoublesFunction = &AliveOperations::CheckDoublesSmell; 
				break;
			}
		default:
			{
				SortFunction = &AliveOperations::SortByName;
				CheckDoublesFunction = &AliveOperations::CheckDoublesName; 
				break;
			}
		}
		AliveSort(ArrayOfProducts,Method,CheckArray,SortFunction);
		(AlSort.*CheckDoublesFunction)(ArrayOfProducts,NumberOfProducts,CheckArray);
	}
	delete CheckArray;
}

void AliveOutput::AliveSort(Product** ArrayOfProducts,int Method,int * CheckArray,int(AliveOperations::*SortFunction)(Product*,Product*))
{
	Product* temp=0;
	for(int k=0;k<sizeof(*CheckArray)/sizeof(int);k+=2)
	{
		if(CheckArray[k+1]!=0)
		{
			for(int i=CheckArray[k];i<CheckArray[k+1]-1;++i)
			{
				for(int j=CheckArray[k];j<CheckArray[k+1]-1-i;++j)
				{
					int res=(AlSort.*SortFunction)(ArrayOfProducts[j+1],ArrayOfProducts[j]);
					if(res==-1&&Method==1)
					{
						temp=ArrayOfProducts[j];
						ArrayOfProducts[j]=ArrayOfProducts[j+1];
						ArrayOfProducts[j+1]=temp;
					}
					else if(res==1&&Method==-1)
					{
						temp=ArrayOfProducts[j];
						ArrayOfProducts[j]=ArrayOfProducts[j+1];
						ArrayOfProducts[j+1]=temp;
					}
				}
			}
		}
	}
}

int AliveOperations::SortByName(Product* Left, Product* Right)
{
	AliveProduct *pLeft = (AliveProduct*)Left;             
	AliveProduct *pRight =(AliveProduct*)Right; 
	return strcmp(pLeft->Name,pRight->Name);

}

int AliveOperations::SortByColor(Product* Left, Product* Right)
{
	AliveProduct *pLeft = (AliveProduct*)Left;             
	AliveProduct *pRight =(AliveProduct*)Right; 
	return strcmp(pLeft->Color,pRight->Color);

}

int AliveOperations::SortByPrice(Product* Left, Product* Right)
{
	AliveProduct *pLeft = (AliveProduct*)Left;             
	AliveProduct *pRight =(AliveProduct*)Right; 
	if(pLeft->price_sell>pRight->price_sell)
		return 1;
	else if(pLeft->price_sell<pRight->price_sell)
		return -1;
	else
		return 0;

}

int AliveOperations::SortByStock(Product* Left, Product* Right)
{
	AliveProduct *pLeft = (AliveProduct*)Left;             
	AliveProduct *pRight =(AliveProduct*)Right; 
	if(pLeft->NumberInStock>pRight->NumberInStock)
		return 1;
	else if(pLeft->NumberInStock<pRight->NumberInStock)
		return -1;
	else
		return 0;

}

int AliveOperations::SortByFresh(Product* Left, Product* Right)
{
	AliveProduct *pLeft = (AliveProduct*)Left;             
	AliveProduct *pRight =(AliveProduct*)Right; 
	if(pLeft->Freshness>pRight->Freshness)
		return 1;
	else if(pLeft->Freshness<pRight->Freshness)
		return -1;
	else
		return 0;

}

int AliveOperations::SortBySmell(Product* Left, Product* Right)
{
	AliveProduct *pLeft = (AliveProduct*)Left;             
	AliveProduct *pRight =(AliveProduct*)Right; 
	if(pLeft->Smell>pRight->Smell)
		return 1;
	else if(pLeft->Smell<pRight->Smell)
		return -1;
	else
		return 0;

}

//void Show(AliveOutput* AOut, Product** ArrayOfProducts,int NumberOfElements)
//{
//	for(int i=0;i<NumberOfElements;++i)
//	{
//		for(int j=0;j<6;++j)
//			AOut->Show(ArrayOfProducts[i],);
//		cout<<endl;
//	}
//
//}

void AliveOperations::CheckDoublesName(Product** ArrayOfProducts, int NumberOfProducts,int* CheckArray)
{
	int* tmp = new int[NumberOfProducts];
	for(int i=0;i<NumberOfProducts;++i)
		tmp[i]=0;

	memcpy(tmp,CheckArray,NumberOfProducts);

	for(int i=0;i<NumberOfProducts;++i)
		CheckArray[i]=0;

	for(int j=0;j<sizeof(*CheckArray)/sizeof(int);j+=2)
	{
		if(tmp[j+1]!=0)
		{
			bool start = false; int step(0);
			for(int i=tmp[j];i<tmp[j]+tmp[j+1];++i)
			{
				if(i!=tmp[j]+tmp[j+1]-1)
				{
					AliveProduct* prod = (AliveProduct*)ArrayOfProducts[i];
					AliveProduct* prodNext = (AliveProduct*)ArrayOfProducts[i+1];
					if(strcmp(prod->Name,prodNext->Name)==0)
					{
						if(start!=true)
						{
							CheckArray[step] = i;
							start = true;
						}
					}
					else
					{
						if(start==true)
						{
							CheckArray[step+1] = i-CheckArray[step]+1;
							start =false; step+=2;
						}
					}
				}
				else
				{
					if(start==true)
						CheckArray[step+1] = i-CheckArray[step]+1;
				}
			}
		}
		else
			break;
	}
	delete tmp;
}

void AliveOperations::CheckDoublesColor(Product** ArrayOfProducts, int NumberOfProducts,int* CheckArray)
{
	int* tmp = new int[NumberOfProducts];
	for(int i=0;i<NumberOfProducts;++i)
		tmp[i]=0;

	memcpy(tmp,CheckArray,NumberOfProducts);

	for(int i=0;i<NumberOfProducts;++i)
		CheckArray[i]=0;

	for(int j=0;j<sizeof(*CheckArray)/sizeof(int);j+=2)
	{
		if(tmp[j+1]!=0)
		{
			bool start = false; int step(0);
			for(int i=tmp[j];i<tmp[j]+tmp[j+1];++i)
			{
				if(i!=tmp[j]+tmp[j+1]-1)
				{
					AliveProduct* prod = (AliveProduct*)ArrayOfProducts[i];
					AliveProduct* prodNext = (AliveProduct*)ArrayOfProducts[i+1];
					if(strcmp(prod->Color,prodNext->Color)==0)
					{
						if(start!=true)
						{
							CheckArray[step] = i;
							start = true;
						}
					}
					else
					{
						if(start==true)
						{
							CheckArray[step+1] = i-CheckArray[step]+1;
							start =false; step+=2;
						}
					}
				}
				else
				{
					if(start==true)
						CheckArray[step+1] = i-CheckArray[step]+1;
				}
			}
		}
		else
			break;
	}
	delete tmp;
}

void AliveOperations::CheckDoublesPrice(Product** ArrayOfProducts, int NumberOfProducts,int* CheckArray)
{
	int* tmp = new int[NumberOfProducts];
	for(int i=0;i<NumberOfProducts;++i)
		tmp[i]=0;

	memcpy(tmp,CheckArray,NumberOfProducts);

	for(int i=0;i<NumberOfProducts;++i)
		CheckArray[i]=0;

	for(int j=0;j<sizeof(*CheckArray)/sizeof(int);j+=2)
	{
		if(tmp[j+1]!=0)
		{
			bool start = false; int step(0);
			for(int i=tmp[j];i<tmp[j]+tmp[j+1];++i)
			{
				if(i!=tmp[j]+tmp[j+1]-1)
				{
					AliveProduct* prod = (AliveProduct*)ArrayOfProducts[i];
					AliveProduct* prodNext = (AliveProduct*)ArrayOfProducts[i+1];
					if(prod->price_sell==prodNext->price_sell)
					{
						if(start!=true)
						{
							CheckArray[step] = i;
							start = true;
						}
					}
					else
					{
						if(start==true)
						{
							CheckArray[step+1] = i-CheckArray[step]+1;
							start =false; step+=2;
						}
					}
				}
				else
				{
					if(start==true)
						CheckArray[step+1] = i-CheckArray[step]+1;
				}
			}
		}
		else
			break;
	}
	delete tmp;
}

void AliveOperations::CheckDoublesStock(Product** ArrayOfProducts, int NumberOfProducts,int* CheckArray)
{
	int* tmp = new int[NumberOfProducts];
	for(int i=0;i<NumberOfProducts;++i)
		tmp[i]=0;

	memcpy(tmp,CheckArray,NumberOfProducts);

	for(int i=0;i<NumberOfProducts;++i)
		CheckArray[i]=0;

	for(int j=0;j<sizeof(*CheckArray)/sizeof(int);j+=2)
	{
		if(tmp[j+1]!=0)
		{
			bool start = false; int step(0);
			for(int i=tmp[j];i<tmp[j]+tmp[j+1];++i)
			{
				if(i!=tmp[j]+tmp[j+1]-1)
				{
					AliveProduct* prod = (AliveProduct*)ArrayOfProducts[i];
					AliveProduct* prodNext = (AliveProduct*)ArrayOfProducts[i+1];
					if(prod->NumberInStock==prodNext->NumberInStock)
					{
						if(start!=true)
						{
							CheckArray[step] = i;
							start = true;
						}
					}
					else
					{
						if(start==true)
						{
							CheckArray[step+1] = i-CheckArray[step]+1;
							start =false; step+=2;
						}
					}
				}
				else
				{
					if(start==true)
						CheckArray[step+1] = i-CheckArray[step]+1;
				}
			}
		}
		else
			break;
	}
	delete tmp;
}

void AliveOperations::CheckDoublesFresh(Product** ArrayOfProducts, int NumberOfProducts,int* CheckArray)
{
	int* tmp = new int[NumberOfProducts];
	for(int i=0;i<NumberOfProducts;++i)
		tmp[i]=0;

	memcpy(tmp,CheckArray,NumberOfProducts);

	for(int i=0;i<NumberOfProducts;++i)
		CheckArray[i]=0;

	for(int j=0;j<sizeof(*CheckArray)/sizeof(int);j+=2)
	{
		if(tmp[j+1]!=0)
		{
			bool start = false; int step(0);
			for(int i=tmp[j];i<tmp[j]+tmp[j+1];++i)
			{
				if(i!=tmp[j]+tmp[j+1]-1)
				{
					AliveProduct* prod = (AliveProduct*)ArrayOfProducts[i];
					AliveProduct* prodNext = (AliveProduct*)ArrayOfProducts[i+1];
					if(prod->Freshness==prodNext->Freshness)
					{
						if(start!=true)
						{
							CheckArray[step] = i;
							start = true;
						}
					}
					else
					{
						if(start==true)
						{
							CheckArray[step+1] = i-CheckArray[step]+1;
							start =false; step+=2;
						}
					}
				}
				else
				{
					if(start==true)
						CheckArray[step+1] = i-CheckArray[step]+1;
				}
			}
		}
		else
			break;
	}
	delete tmp;
}

void AliveOperations::CheckDoublesSmell(Product** ArrayOfProducts, int NumberOfProducts,int* CheckArray)
{
	int* tmp = new int[NumberOfProducts];
	for(int i=0;i<NumberOfProducts;++i)
		tmp[i]=0;

	memcpy(tmp,CheckArray,NumberOfProducts);

	for(int i=0;i<NumberOfProducts;++i)
		CheckArray[i]=0;

	for(int j=0;j<sizeof(*CheckArray)/sizeof(int);j+=2)
	{
		if(tmp[j+1]!=0)
		{
			bool start = false; int step(0);
			for(int i=tmp[j];i<tmp[j]+tmp[j+1];++i)
			{
				if(i!=tmp[j]+tmp[j+1]-1)
				{
					AliveProduct* prod = (AliveProduct*)ArrayOfProducts[i];
					AliveProduct* prodNext = (AliveProduct*)ArrayOfProducts[i+1];
					if(prod->Smell==prodNext->Smell)
					{
						if(start!=true)
						{
							CheckArray[step] = i;
							start = true;
						}
					}
					else
					{
						if(start==true)
						{
							CheckArray[step+1] = i-CheckArray[step]+1;
							start =false; step+=2;
						}
					}
				}
				else
				{
					if(start==true)
						CheckArray[step+1] = i-CheckArray[step]+1;
				}
			}
		}
		else
			break;
	}
	delete tmp;
}

void AliveOutput::Find(Product** ArrayOfProducts,int NumberOfProducts, char* value, int param, int* result)
{
	bool(AliveOperations::*FindFunction)(Product*,char*);
	int step(0);
	for(int i=0;i<NumberOfProducts;++i)
	{
		switch(param)
		{
		case NAME:
			{
				FindFunction = &AliveOperations::FindName;
				break;
			}
		case COLOR:
			{
				FindFunction = &AliveOperations::FindColor;
				break;
			}
		case PRICE:
			{
				FindFunction = &AliveOperations::FindPrice;
				break;
			}
		case NUMBER_OF_PROD:
			{
				FindFunction = &AliveOperations::FindStock;
				break;
			}
		case FRESHNESS:
			{
				FindFunction = &AliveOperations::FindFreshness;
				break;
			}
		case SMELL:
			{
				FindFunction = &AliveOperations::FindSmell;
				break;
			}
		default:
			{
				FindFunction = &AliveOperations::FindName;
				break;
			}
		}
		if((AlSort.*FindFunction)(ArrayOfProducts[i],value)==true)
		{
			result[step]=i;
			++step;
		}
	}
}

bool AliveOperations::FindName(Product* Product,char* value)
{
	AliveProduct* prod = (AliveProduct*)Product;
	if(strcmp(prod->Name,value)==0)
		return true;
	else
		return false;
}

bool AliveOperations::FindColor(Product* Product,char* value)
{
	AliveProduct* prod = (AliveProduct*)Product;
	if(strcmp(prod->Color,value)==0)
		return true;
	else
		return false;
}

bool AliveOperations::FindPrice(Product* Product,char* value)
{
	AliveProduct* prod = (AliveProduct*)Product;
	double tmp = atof(value);
	if(tmp==0)
		return false;
	else
	{
		if(prod->price_sell==tmp)
			return true;
		else
			return false;
	}
}

bool AliveOperations::FindStock(Product* Product,char* value)
{
	AliveProduct* prod = (AliveProduct*)Product;
	int tmp = atoi(value);
	if(tmp==0)
		return false;
	else
	{
		if(prod->NumberInStock==tmp)
			return true;
		else
			return false;
	}
}

bool AliveOperations::FindFreshness(Product* Product,char* value)
{
	AliveProduct* prod = (AliveProduct*)Product;
	int tmp = atoi(value);
	if(tmp==0)
		return false;
	else
	{
		if(prod->Freshness==tmp)
			return true;
		else
			return false;
	}
}

bool AliveOperations::FindSmell(Product* Product,char* value)
{
	AliveProduct* prod = (AliveProduct*)Product;
	int tmp = atoi(value);
	if(tmp==0)
		return false;
	else
	{
		if(prod->Smell==tmp)
			return true;
		else
			return false;
	}
}

bool AliveLoad::FirstLoad()
{
	FILE *file;
	errno_t err;

	AliveInput BI; 
	err = fopen_s(&file,BI.GetCategoryName(),"rb");
	if(file != 0)
	{
		fclose(file);
		return false;
	}
	else
	{
		return true;
	}

}

int AliveLoad::ReturnNumberOfProducts()
{
	FILE *file;
	errno_t err;

	AliveInput BI; 
	err = fopen_s(&file,BI.GetCategoryName(),"rb");
	if(file != 0)
	{
		int numb(0);
		fseek(file,0,SEEK_SET); 
		fseek(file,sizeof(double),SEEK_SET);
		fread(&numb,sizeof(int),1,file);
		fclose(file);
		return numb;
	}
	else
		return 0;

}

int AliveLoad::ReturnBalance()
{
	FILE *file;
	errno_t err;

	AliveInput BI; 
	err = fopen_s(&file,BI.GetCategoryName(),"rb");
	if(file != 0)
	{
		int Balance(0);
		fseek(file,0,SEEK_SET); 
		fread(&Balance,sizeof(double),1,file);
		fclose(file);
		return Balance;
	}
	else
		return 0;

}

bool AliveLoad::CreateProduct(Product** ProdArray)
{
	FILE *file;
	errno_t err;

	try
	{
		AliveInput BI; 
		err = fopen_s(&file,BI.GetCategoryName(),"rb");
		if(file != 0)
		{
			int numb(0);
			InputProductData  IPD;
			fseek(file,0,SEEK_SET); 
			fseek(file,sizeof(double),SEEK_SET);
			fread(&numb,sizeof(int),1,file);
			int step(0);
			for(int i=0;i<numb;++i)
			{
				fread(&IPD,sizeof(InputProductData),1,file);
				try
				{
					AliveProduct* AP = new AliveProduct(&IPD);
					ProdArray[step] = (Product*)AP;
					++step;
				}
				catch (std::bad_alloc& ba)
				{
					return 0;
				}
			}
			fclose(file);
			return true;
		}
	}
	catch(int err)
	{
		return false;
	}
}

bool AliveSave::SaveProducts(int Balance, int NumberOfProducts, Product** ArrayOfProducts)
{
	try
	{
		FILE *file;
		errno_t err;
		AliveInput BI; 
		err = fopen_s(&file,BI.GetCategoryName(),"wb");           
		if(file != 0)
		{
			fwrite(&Balance,sizeof(double),1,file);
			fwrite(&NumberOfProducts,sizeof(int),1,file);
			for(int j=0; j<NumberOfProducts; ++j)
			{
				InputProductData IPD;
				AliveProduct* AlProd = (AliveProduct*)ArrayOfProducts[j];

				memcpy(IPD.Name,AlProd->Name,sizeof(AlProd->Name));
				memcpy(IPD.Color,AlProd->Color,sizeof(AlProd->Color));
				IPD.price_sell=AlProd->price_sell;
				IPD.price_buy=AlProd->price_buy;
				IPD.Freshness=AlProd->Freshness;
				IPD.Smell=AlProd->Smell;
				IPD.NumberInStock=AlProd->NumberInStock;

				fwrite(&IPD,sizeof(InputProductData),1,file);
			}
		}
		fclose(file);
		return true;
	}
	catch(std::bad_alloc& ba)
	{
		return false;
	}
}