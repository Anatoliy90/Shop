#include "stdafx.h"
#include "classes.h" 


using namespace std; 
//using namespace System;

enum{SHOW_ALL,SAVE_IN_FILE};
enum{EMPTY_VALUE,FIND,SORT,ADD,SELL};

/////////////////////////////////////////////////////////////////////////////////
////////////////////////////////Shop/////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////

Shop::Shop():NumberOfCategories(0),SizeOfArray(0),WorkFieldTopY(0),WorkFiledBottomY(0),Balance(0)
{
	try
	{
		hStdout = GetStdHandle(STD_OUTPUT_HANDLE);

		ArrayOfCategories = new Category*[1];

		if(ArrayOfCategories != 0)
			SizeOfArray = 1;

		WIN32_FIND_DATA wf;
		TCHAR Path[_MAX_PATH];
		TCHAR NameDirectory[_MAX_PATH]={'\0'};
		int count(0);

		GetModuleFileName(NULL,Path, sizeof(Path));
		for(int i=wcslen(Path); i>0; --i)
			if(Path[i]=='\\')
			{
				Path[i]=L'\0';
				count=i;
				break;
			}
			wcsncat_s(NameDirectory,Path,count);
			wsprintf(NameDirectory, L"%s\\*.dll", NameDirectory);
			HANDLE hFile = FindFirstFile(NameDirectory, &wf);

			if(hFile != INVALID_HANDLE_VALUE)
			{
				BOOL Cycle = TRUE;
				while(Cycle)
				{
					HMODULE hMod = LoadLibrary(wf.cFileName);

					if(hMod == NULL)
					{
						cout<<"Cannot load library: "<<wf.cFileName<<endl;
						Cycle = FindNextFile(hFile, &wf);
						continue;
					}

					typedef BaseFactory* (*GetFactoryT)();
					GetFactoryT GetFactoryFuction = (GetFactoryT)GetProcAddress(hMod, "GetFactory");
					if(GetFactoryFuction == 0)
					{
						FreeLibrary(hMod);
						continue;
					}
					else			
						if(ArrayOfCategories[NumberOfCategories] != 0)
						{
							try
							{
								if(NumberOfCategories == SizeOfArray)
								{
									try
									{
										Category** tmp = new Category*[SizeOfArray+2];
										memcpy(tmp,ArrayOfCategories,sizeof(Category*));
										delete ArrayOfCategories;
										ArrayOfCategories = tmp;
										SizeOfArray+=2;
									}
									catch(std::bad_alloc& ba)
									{
										throw ba;
									}
								}
								ArrayOfCategories[NumberOfCategories] = new Category(GetFactoryFuction);
								Balance+=ArrayOfCategories[NumberOfCategories]->Balance;
								++NumberOfCategories;
							}
							catch(std::bad_alloc& ba)
							{
								cout<<"Not memory enough for proper shop work"<<endl;
							}
							catch(int er)
							{
								cout<<"Not memory enough for "<<wf.cFileName<<" product group"<<endl;
							}
						}
						Cycle = FindNextFile(hFile, &wf);
				}

			}

			FindClose(hFile);


			if(NumberOfCategories != 0)
			{
				WorkWithShop();
			}
			else
			{
				cout<<endl<<"The Shop is empty! End of work."<<endl;
				Shop::~Shop();
			}

			cout<<"Shop const "<<this<<endl;
	}
	catch (std::bad_alloc& ba)
	{
		cout<<"There's no vacant memory for work! End of work."<<endl;
		Shop::~Shop();
	}

}

Shop::~Shop()
{
	cout<<"Shop dest "<<this<<endl;
	for(int i=0; i<NumberOfCategories; ++i)
		delete ArrayOfCategories[i];
	delete ArrayOfCategories;

}

void Shop::WorkWithShop()
{

	COORD cord;
	cord.X = 80;
	cord.Y = 40;
	SMALL_RECT src = {0, 0, cord.X-1, cord.Y-1};
	SetConsoleWindowInfo (hStdout, true, &src);
	SetConsoleScreenBufferSize (hStdout, cord);

	int CurrMenuItemY(0);
	int FirstMenuItemY(0);
	int FooterTextY(0);

	system("cls");
	Header();

	GetConsoleScreenBufferInfo(hStdout, &csbInfo);
	WorkFieldTopY = csbInfo.dwCursorPosition.Y;
	WorkFiledBottomY = csbInfo.srWindow.Bottom-7;
	FooterTextTopY = csbInfo.srWindow.Bottom-5;
	FooterTextBottomY = csbInfo.srWindow.Bottom-3;

	cord.X = 0;
	cord.Y =  FooterTextTopY;
	SetConsoleCursorPosition(hStdout, cord);
	printf("\r%80c", ' ');
	printf("\r%80c", ' ');
	SetConsoleCursorPosition(hStdout, cord);
	cout<<"UP,DOWN - MOVING THROUGH THE MENU"<<endl;
	cout<<"ESC - QUIT, ENTER - RUN MENU ITEM";

	Footer();

	int ChoiceCycle(0), ChoiceStep(0), FirstRoundOfCycle(0);	
	const char* MenuArray[2]={". SHOW ALL PRODUCTS IN STOCK",
		". SAVE SHOP INFORMATION IN FILE"};


	while(ChoiceCycle!=27)
	{
		ClearWorkField();
		cout<<"MENU:"<<endl<<endl;
		cin.clear();
		cin.sync();
		GetConsoleScreenBufferInfo(hStdout, &csbInfo);
		cord.X = 0;
		cord.Y = csbInfo.dwCursorPosition.Y;
		int FirstMenuItemPosition(0);
		for(int i=0; i<sizeof(MenuArray)/sizeof(const char*); ++i)
		{
			if(FirstRoundOfCycle == 0)
			{
				GetConsoleScreenBufferInfo(hStdout, &csbInfo);
				FirstMenuItemY = csbInfo.dwCursorPosition.Y;
			}
			if(FirstMenuItemPosition == 0)
			{
				cord.X = 0;
				cord.Y = FirstMenuItemY;
				SetConsoleCursorPosition(hStdout, cord);
				++FirstMenuItemPosition;
			}
			if(i == ChoiceStep)
			{
				SetConsoleTextAttribute(hStdout,03);
				GetConsoleScreenBufferInfo(hStdout, &csbInfo);
				CurrMenuItemY = csbInfo.dwCursorPosition.Y;
			}
			else
				SetConsoleTextAttribute(hStdout,07);

			printf("\r%80c", ' ');
			GetConsoleScreenBufferInfo(hStdout, &csbInfo);
			cord.Y = --csbInfo.dwCursorPosition.Y;
			SetConsoleCursorPosition(hStdout, cord);
			cout<<i+1<<MenuArray[i]<<endl;

			++FirstRoundOfCycle;
		}

		cord.X = 0;
		cord.Y = CurrMenuItemY;
		SetConsoleCursorPosition(hStdout, cord);

		ChoiceCycle=_getch();
		if(ChoiceCycle==224)
			ChoiceCycle=_getch();

		switch(ChoiceCycle)
		{
		case 80:
			{
				++ChoiceStep;
				if(ChoiceStep == sizeof(MenuArray)/sizeof(const char*))
					ChoiceStep = 0;
				break;
			}  
		case 72:
			{
				if(ChoiceStep == 0)
					ChoiceStep = sizeof(MenuArray)/sizeof(const char*);
				--ChoiceStep;
				break;
			} 
		case 13:
			{
				switch(ChoiceStep)
				{
				case SHOW_ALL:
					{
						Menu1();
						break;
					}             
				case SAVE_IN_FILE:
					{
						Save();
						break;
					}
				}
				break;
			}
		}//end of switch choice cycle
	}//end of while choice!=27

}

void Shop::Menu1()
{
	COORD cord;
	int ChoiceCycle(0), ChoiceStep(0);
	while(ChoiceCycle != 27)
	{
		ClearWorkField();

		cout<<"PRODUCT LIST:"<<endl<<endl;
		cout<<"------+--------------------------------------------------------+---------------"<<endl;
		cout<<"  #   |                       PRODUCT TYPE                     | AVAL. IN STOCK"<<endl;
		cout<<"------+--------------------------------------------------------+---------------"<<endl;

		GetConsoleScreenBufferInfo(hStdout, &csbInfo);
		int FirstMenuItemY_1 =  csbInfo.dwCursorPosition.Y;

		int count(1);
		int x1(6),x2(63);

		for(int i=0; i<NumberOfCategories; ++i)
		{
			cord.X=0;cord.Y=FooterTextTopY;
			SetConsoleCursorPosition(hStdout, cord);
			printf("\r%80c", ' ');
			printf("\r%80c", ' ');
			printf("\r%80c", ' ');
			SetConsoleCursorPosition(hStdout, cord);
			cout<<"UP,DOWN - MOVING THROUGH THE MENU"<<endl;
			cout<<"ESC - QUIT, ENTER - RUN MENU ITEM";

			if(i == 0)  
			{
				cord.X = 0;
				cord.Y = FirstMenuItemY_1;
				SetConsoleCursorPosition(hStdout, cord);
				count = 1;
			}
			cout<<count;
			GetConsoleScreenBufferInfo(hStdout, &csbInfo);
			cord.X = x1;
			cord.Y = csbInfo.dwCursorPosition.Y;
			SetConsoleCursorPosition(hStdout, cord);
			cout<<"|";
			cord.X = x1+2;
			SetConsoleCursorPosition(hStdout, cord);

			if(i == ChoiceStep)
				SetConsoleTextAttribute(hStdout,03);
			else
				SetConsoleTextAttribute(hStdout,07);

			if(strlen(ArrayOfCategories[i]->BI->GetCategoryName()) < x2-x1-6)
				cout<<ArrayOfCategories[i]->BI->GetCategoryName();
			else
			{
				try
				{
					char* tmp = new char[x2-x1-5];
					memcpy(tmp,ArrayOfCategories[i]->BI->GetCategoryName(),x2-x1-6);
					tmp[x2-x1-6] = '\0';
					cout<<tmp<<"...";

					delete tmp;
				}
				catch(std::bad_alloc& ba)
				{
					char* NameCat = (char*)ArrayOfCategories[i]->BI->GetCategoryName();
					for(int i=0; i<x2-x1-3; ++i)
						cout<<NameCat[i];
				}
			}
			SetConsoleTextAttribute(hStdout,07);
			cord.X = x2;
			SetConsoleCursorPosition(hStdout, cord);
			cout<<"|";
			cord.X = x2+2;
			SetConsoleCursorPosition(hStdout, cord);
			cout<<ArrayOfCategories[i]->NumberOfProducts<<endl;
			++count;
		}
		cout<<"------+--------------------------------------------------------+---------------"<<endl;

		ChoiceCycle=_getch();
		if(ChoiceCycle == 224)
			ChoiceCycle=_getch();

		switch(ChoiceCycle)
		{
		case 80:
			{
				++ChoiceStep;
				if(ChoiceStep == NumberOfCategories)
					ChoiceStep = 0;
				break;
			}  
		case 72:
			{
				if(ChoiceStep == 0)
					ChoiceStep = NumberOfCategories;
				--ChoiceStep;
				break;
			} 
		case 13:
			{
				Menu2(ChoiceStep);
				break;
			}
		}
	}
}

void Shop::Menu2(int ChoiceStep,int* OutputList)
{
	COORD cord;

	cout<<ArrayOfCategories[ChoiceStep]->BI->GetCategoryName()<<" CATEGORY PRODUCT LIST. "
		<<ArrayOfCategories[ChoiceStep]->NumberOfProducts<<" PRODUCTS AVALIABLE."<<endl;

	int WorkFieldHight = WorkFiledBottomY - WorkFieldTopY;
	int step(1);
	bool ScrollBorder=false;

	int FirstProdIndexForCycleMooving;
	if(OutputList == 0)
		FirstProdIndexForCycleMooving=0;
	else
		FirstProdIndexForCycleMooving=OutputList[0];

	int FirstProdParamforCycleMooving(0);
	bool StartShowI  = false, StartShowJ = false;

	int ChoiceCycle_2(0),ChoiceStep_2(0);
	int* AvaliableProducts(0);
	try
	{
		AvaliableProducts = new int[WorkFiledBottomY-WorkFieldTopY]; 
	}
	catch(std::bad_alloc& ba)
	{
		cout<<"Not memory enough! Buy,Sell,Delete operations are inaccessible";
	}
	int  AvaliableProductsCount(0);
	while(ChoiceCycle_2!=27)
	{    
		cord.X=0;
		cord.Y = WorkFiledBottomY+2;
		SetConsoleCursorPosition(hStdout, cord);
		printf("\r%80c", ' ');
		printf("\r%80c", ' ');
		SetConsoleCursorPosition(hStdout, cord);
		cout<<"UP,DOWN - moving through the list,ESC - quit, F - find product in list, S - sort";
		cout<<"B - buy product in stock, P - sell operation"<<endl
			<<"D - delete product, A - add product";
		ClearWorkField();

		cout<<ArrayOfCategories[ChoiceStep]->BI->GetCategoryName()<<" CATEGORY PRODUCT LIST. "
			<<ArrayOfCategories[ChoiceStep]->NumberOfProducts<<" PRODUCTS AVALIABLE."<<endl;


		if(AvaliableProducts != 0)
			for(int i=0; i<WorkFiledBottomY-WorkFieldTopY; ++i)
				AvaliableProducts[i]=0;
		AvaliableProductsCount=0;
		int stepaval(0);
		ClearWorkField();
		ScrollBorder = false;
		for(int i=0; i<ArrayOfCategories[ChoiceStep]->NumberOfProducts; ++i)
		{
			if(OutputList != 0)
			{
				bool ContinueOutput = false;
				for(int k=0; k<ArrayOfCategories[ChoiceStep]->NumberOfProducts; ++k)
				{
					if(OutputList[k] == i)
						ContinueOutput = true;
				}
				if(!ContinueOutput)
					continue;
			}

			if(step >= WorkFieldHight)
			{
				cout<<"...";
				break;
			}
			if(i == FirstProdIndexForCycleMooving)
				StartShowI = true;

			cord.X =0;
			cord.Y=WorkFieldTopY+step;
			SetConsoleCursorPosition(hStdout, cord);
			for(int j=0; j<ArrayOfCategories[ChoiceStep]->ArrayOfProducts[0]->ParametersCount; ++j)
			{
				if(StartShowI == true && j == FirstProdParamforCycleMooving)
					StartShowJ = true;
				if(StartShowI == true && StartShowJ == true)
				{
					if(step == WorkFieldHight)
						break;
					if(j == 0)
					{
						cout<<i+1<<". ";
						if(AvaliableProducts != 0)
						{
							AvaliableProducts[stepaval]=i+1;
							++AvaliableProductsCount;
							++stepaval;
						}
					}
					else
						cout<<"     ";
					ArrayOfCategories[ChoiceStep]->BOut->Show(ArrayOfCategories[ChoiceStep]->ArrayOfProducts[i],j);
					++step;
					cord.Y=WorkFieldTopY+step;
					SetConsoleCursorPosition(hStdout, cord);
					if(j == ArrayOfCategories[ChoiceStep]->ArrayOfProducts[0]->ParametersCount-1 && i == ArrayOfCategories[ChoiceStep]->NumberOfProducts-1)
						ScrollBorder = true;
				}
			}
		}
		step=1;
		StartShowI = false; StartShowJ = false;
		ChoiceCycle_2=_getch();
		if(ChoiceCycle_2 == 224)
			ChoiceCycle_2=_getch();

		switch(ChoiceCycle_2)
		{
		case 80:
			{
				if(ScrollBorder == true)
					break;
				if(FirstProdParamforCycleMooving == ArrayOfCategories[ChoiceStep]->ArrayOfProducts[0]->ParametersCount-1)
				{
					++FirstProdIndexForCycleMooving;FirstProdParamforCycleMooving=0;
				}
				else
					++FirstProdParamforCycleMooving;
				break;
			}  
		case 72:
			{
				if(FirstProdIndexForCycleMooving == 0 && FirstProdParamforCycleMooving == 0)
					break;
				if(FirstProdParamforCycleMooving == 0)
				{
					--FirstProdIndexForCycleMooving; FirstProdParamforCycleMooving=ArrayOfCategories[ChoiceStep]->ArrayOfProducts[0]->ParametersCount-1;
				}
				else
					--FirstProdParamforCycleMooving;
				break;
			} 
		case 235:
			{
				Sort(ChoiceStep);
				break;
			}
		case 160:
			{
				Find(ChoiceStep);
				break;
			}
		case 168:
			{
				BuySellDelete(ChoiceStep,AvaliableProducts,AvaliableProductsCount,0);
				Header();
				Footer();
				break;
			}
		case 167:
			{
				BuySellDelete(ChoiceStep,AvaliableProducts,AvaliableProductsCount,1);
				Header();
				Footer();
				break;
			}
		case 162:
			{
				BuySellDelete(ChoiceStep,AvaliableProducts,AvaliableProductsCount,2);
				Header();
				Footer();
				break;
			}
		case 228:
			{
				AddProduct(ChoiceStep);
				Header();
				Footer();
				break;
			}
		}
	}
	delete AvaliableProducts;
}

void Shop::ClearWorkField()
{
	COORD cord = {0,WorkFieldTopY};
	SetConsoleCursorPosition(hStdout, cord);
	for(int i=0; i<WorkFiledBottomY-WorkFieldTopY+1; ++i)
		printf("\r%80c", ' ');
	SetConsoleCursorPosition(hStdout, cord);
	SetConsoleTextAttribute(hStdout,07);
}

void Shop::Header()
{
	system("cls");
	COORD cord={0,0};
	SetConsoleCursorPosition(hStdout, cord);
	char nameShop[12] = "FLOWER SHOP";
	int k(0),NumberOfProdInStock(0);

	for(int i=0; i<NumberOfCategories; ++i)
		NumberOfProdInStock+=ArrayOfCategories[i]->NumberOfProducts;

	for(int j=0; j<8; ++j)
	{
		if(j == 5){}
		else if(j == 6)
		{
			for(int l=0; l<40; ++l)
				cout<<" ";
			cout<<"You have "<<NumberOfProdInStock<<" items avaliable in stock.";
		}
		else if(j == 7)
		{
			for(int l=0; l<40; ++l)
				cout<<" ";
			int bal(0);
			for(int i=0; i<NumberOfCategories; ++i)
				bal+=ArrayOfCategories[i]->Balance;
			cout<<"Balance: "<<bal;
		}
		else
		{
			for(int i=0; i<79; ++i)
			{
				if(j == 2 && i >= 33 && i < 44)
				{
					SetConsoleTextAttribute(hStdout,03);
					cout<<nameShop[k];
					++k;
				}
				else
				{
					SetConsoleTextAttribute(hStdout,07);
					cout<<":";
				}
			}
		}
		cout<<endl;
	}
}

void Shop::Footer()
{
	CONSOLE_SCREEN_BUFFER_INFO csbInfo;
	GetConsoleScreenBufferInfo(hStdout, &csbInfo);
	COORD coord = {0,csbInfo.srWindow.Bottom-1};
	SetConsoleCursorPosition(hStdout, coord);	


	for(int j=0; j<2; ++j)
	{

		for(int i=0; i<79; ++i)
		{
			cout<<":";
		}

		if(j!=1)
			cout<<endl;
	}

	coord.Y = csbInfo.srWindow.Bottom-6;
	SetConsoleCursorPosition(hStdout, coord);
	cout<<"============================";
}

void Shop::Sort(int CategoryIndex)
{
	ClearWorkField();

	const char** ArrayOfParameters=0;
	char* ArrEntered=0;

	try
	{
		ArrayOfParameters = new const char*[ArrayOfCategories[CategoryIndex]->ArrayOfProducts[0]->ParametersCount];
		ArrayOfCategories[CategoryIndex]->BOut->GetProductParameters(ArrayOfParameters);
		for(int i=0; i<ArrayOfCategories[CategoryIndex]->ArrayOfProducts[0]->ParametersCount; ++i)
		{

			cout<<i+1<<". "<<ArrayOfParameters[i]<<endl;
		}
		cout<<endl<<"Please enter number of parameters you want to sort by"<<endl;
		cout<<"You can choose several parameters, you need to enter their indexes one by one without space!"<<endl;
		cout<<"For example: 312"<<endl;
		GetConsoleScreenBufferInfo(hStdout, &csbInfo);
		COORD cord = {0,csbInfo.dwCursorPosition.Y};

		int  cycle(0);
		ArrEntered = new char[ArrayOfCategories[CategoryIndex]->ArrayOfProducts[0]->ParametersCount];
		while(cycle != 1)
		{
			cin.clear();
			cin.sync();
			cin>>ArrEntered;
			SetConsoleCursorPosition(hStdout, cord);
			for(int i=0; i<10; ++i)
				printf("\r%80c", ' ');
			SetConsoleCursorPosition(hStdout, cord);
			if(strlen(ArrEntered) > ArrayOfCategories[CategoryIndex]->ArrayOfProducts[0]->ParametersCount)
			{
				cout<<"Number of parameters should be less than "<<ArrayOfCategories[CategoryIndex]->ArrayOfProducts[0]->ParametersCount
					<<" ! Try again."<<endl;
				continue;
			}
			else if(strlen(ArrEntered) == 0)
			{
				cout<<"You entered nothing! Try again."<<endl;
				continue;
			}

			int OneWarning(0), error(0);
			for(int i=0; i<ArrayOfCategories[CategoryIndex]->ArrayOfProducts[0]->ParametersCount; ++i)
			{
				char tmp; tmp = ArrEntered[i];
				if(tmp == '\0')
					break;
				if(atoi(&tmp) == 0)
				{
					if(OneWarning == 0)
					{
						cout<<"Symbols should be all numeric. Try again"<<endl;
						OneWarning=1; error = 1;
					}
					continue;
				}
				else
				{
					if(OneWarning == 0)
					{
						if(atoi(&tmp) > ArrayOfCategories[CategoryIndex]->ArrayOfProducts[0]->ParametersCount)
						{
							cout<<"Wrong number in list! Try again."<<endl;
							OneWarning=1; error = 1;
						}
					}
					continue;
				}
				continue;
			}
			if(error == 0)
				cycle=1;
		}

		int method(0);
		cout<<"Please enter method of sorting"<<endl;
		cout<<"1 - increase"<<endl;
		cout<<"2 - decrease"<<endl;
		cycle=0;
		while(cycle != 1)
		{
			cin>>method;
			if(cin.fail())
			{
				cin.clear();
				cin.sync();
				cout<<"Wrong value! Try again."<<endl;
			}
			else
			{
				if((method == 1) || (method == 2))
					cycle=1;
				else
					cout<<"Wrong value! Try again."<<endl;
			}
		}

		ArrayOfCategories[CategoryIndex]->BOut->Sort(ArrayOfCategories[CategoryIndex]->ArrayOfProducts,
			ArrayOfCategories[CategoryIndex]->NumberOfProducts,
			ArrEntered,method);
	}
	catch(std::bad_alloc& ba)
	{
		cout<<"Memory error, operation can't be proceeded";
	}
	delete ArrayOfParameters;
	delete ArrEntered;
}

void Shop::Find(int CategoryIndex)
{
	ClearWorkField();
	const char** ArrayOfParameters=0;
	char* value=0;
	int* result=0;

	try
	{
		ArrayOfParameters = new const char*[ArrayOfCategories[CategoryIndex]->ArrayOfProducts[0]->ParametersCount];
		ArrayOfCategories[CategoryIndex]->BOut->GetProductParameters(ArrayOfParameters);
		for(int i=0; i<ArrayOfCategories[CategoryIndex]->ArrayOfProducts[0]->ParametersCount; ++i)
		{

			cout<<i+1<<". "<<ArrayOfParameters[i]<<endl;
		}
		cout<<endl<<"Please enter parameter you want to find"<<endl;
		GetConsoleScreenBufferInfo(hStdout, &csbInfo);
		COORD cord = {0,csbInfo.dwCursorPosition.Y};

		int  cycle(0);
		int param(0);
		while(cycle != 1)
		{
			cin.clear();
			cin.sync();
			cin>>param;
			SetConsoleCursorPosition(hStdout, cord);
			for(int i=0; i<10; ++i)
				printf("\r%80c", ' ');
			SetConsoleCursorPosition(hStdout, cord);

			if(cin.fail())
			{
				cin.clear();
				cin.sync();
				cout<<"Wrong value, try again."<<endl;
			}
			else
			{
				if(param>ArrayOfCategories[CategoryIndex]->ArrayOfProducts[0]->ParametersCount)
					cout<<"Wrong value, try again."<<endl;
				else
					cycle=1;
			}
		}

		value = new char[100];
		cout<<"Please enter value you want to find(less than 100 symbols):"<<endl;
		cin>>value;

		result = new int[ArrayOfCategories[CategoryIndex]->NumberOfProducts];
		for(int i=0; i<ArrayOfCategories[CategoryIndex]->NumberOfProducts; ++i)
			result[i]=-1;

		ArrayOfCategories[CategoryIndex]->BOut->Find(ArrayOfCategories[CategoryIndex]->ArrayOfProducts,
			ArrayOfCategories[CategoryIndex]->NumberOfProducts,
			value,param,result);
		Menu2(CategoryIndex,result);
	}
	catch(std::bad_alloc& ba)
	{
		cout<<"Memory error, operation can't be proceeded";
	}
	delete ArrayOfParameters;
	delete value;
	delete result;
}

void Shop::BuySellDelete(int CategoryIndex, int* ProductsAvaliable, int ProductsAvaliableSize,int BuySellDelete)
{
	ClearWorkField();

	int  cycle(0);
	int* ArrayOfChoosedNumbers=0;
	char* ArrEntered=0;

	try
	{
		if(!ProductsAvaliable)
			throw 0;

		if(BuySellDelete == 0)
			cout<<endl<<"Please enter number of product you want to buy in stock."<<endl;
		else if(BuySellDelete == 1)
			cout<<endl<<"Please enter number of product you want to sell"<<endl;
		else
			cout<<endl<<"Please enter number of product you want to delete"<<endl;

		cout<<"You can choose several numbers, for example: 1,7,12. Enter 'all' for ALL"<<endl<<"products choosing"<<endl;

		GetConsoleScreenBufferInfo(hStdout, &csbInfo);
		COORD cord = {0,csbInfo.dwCursorPosition.Y};

		try
		{
			ArrayOfChoosedNumbers = new int[ArrayOfCategories[CategoryIndex]->NumberOfProducts];
			ArrEntered = new char[ArrayOfCategories[CategoryIndex]->ArrayOfProducts[0]->ParametersCount];
		}
		catch(std::bad_alloc& ba)
		{
			throw ba;
		}
		while(cycle != 27 && cycle != 1)
		{
			SetConsoleCursorPosition(hStdout, cord);
			for(int i=0; i<10; ++i)
				printf("\r%80c", ' ');
			SetConsoleCursorPosition(hStdout, cord);

			for(int i=0; i<ArrayOfCategories[CategoryIndex]->NumberOfProducts; ++i)
				ArrayOfChoosedNumbers[i]=0;

			cin.clear();
			cin.sync();
			cin>>ArrEntered;

			if(strlen(ArrEntered) == 0)
			{
				cout<<"You entered nothing! Try again."<<endl;
				cycle = _getch();
				continue;
			}


			int step(0);
			if(strcmp(ArrEntered,"all") == 0)
			{
				for(int i=0; i<ArrayOfCategories[CategoryIndex]->NumberOfProducts; ++i)
					ArrayOfChoosedNumbers[i] = i;
				cycle=1;
				continue;
			}
			else
			{
				bool error = false;
				bool FirstEnteredNumber = true;
				int numb(0);
				for(int i=0; i<strlen(ArrEntered); ++i)
				{
					bool LastEnteredNumber = true;
					if(ArrEntered[i] == ',')
					{
						if(step == 0)
						{
							char* tmpchar = new char[i+1];
							for(int l=0; l<i+1; ++l)
								tmpchar[l]='0';
							tmpchar[i]='\0';
							for(int l=0; l<i; ++l)
								tmpchar[l]=ArrEntered[l];
							numb = atoi(tmpchar);
							if(numb != 0)
							{
								ArrayOfChoosedNumbers[step]= numb;
								++step;
							}
							else
								error=true;
							FirstEnteredNumber = false;
							delete tmpchar;
						}

						for(int j=1; j<5; ++j)
						{
							if(ArrEntered[i+j] == ',' && (j != 1))
							{
								char* tmpchar = new char[j];
								for(int l=0; l<j; ++l)
									tmpchar[l]='0';
								tmpchar[j-1]='\0';
								for(int l=1; l<j; ++l)
									tmpchar[l-1]=ArrEntered[i+l];
								numb = atoi(tmpchar);
								if(numb != 0)
								{
									ArrayOfChoosedNumbers[step]= numb;
									++step;
								}
								else
									error=true;
								LastEnteredNumber = false;
								FirstEnteredNumber = false;
								delete tmpchar;
							}
						}
						if(LastEnteredNumber)
						{
							char* tmpchar = new char[strlen(ArrEntered)-i];
							for(int l=0; l<strlen(ArrEntered)-i; ++l)
								tmpchar[l]='0';
							tmpchar[strlen(ArrEntered)-i-1]='\0';
							for(int l=1; l<strlen(ArrEntered)-i; ++l)
								tmpchar[l-1]=ArrEntered[i+l];
							numb = atoi(tmpchar);
							if(numb != 0)
							{
								ArrayOfChoosedNumbers[step]= numb;
								++step;
								LastEnteredNumber = false;
							}
							else
								error=true;
							delete tmpchar;
						}
					}
				}

				if(FirstEnteredNumber)
				{
					char* tmpchar = new char[strlen(ArrEntered)+1];
					for(int l=0; l<strlen(ArrEntered)+1; ++l)
						tmpchar[l]='0';
					tmpchar[strlen(ArrEntered)]='\0';
					for(int l=0; l<strlen(ArrEntered); ++l)
						tmpchar[l]=ArrEntered[l];
					numb = atoi(tmpchar);
					if(numb != 0)
					{
						ArrayOfChoosedNumbers[step]= numb;
						++step;
					}
					else
						error=true;
					delete tmpchar;
				}

				bool ProductIsAvaliable = false;
				for(int k=0; k<ProductsAvaliableSize; ++k)
				{
					if(ProductsAvaliable[k]==ArrayOfChoosedNumbers[step-1])
					{
						ProductIsAvaliable = true;
					}
				}
				if(!ProductIsAvaliable)
				{
					cout<<"Wrong value inserted. You should insert values you can see on the screen list."
						<<endl<<"Try again. To see the screen list press ESC, to continue press ENTER"<<endl;
					cycle = _getch();
					if(cycle!=27)
						continue;
					else
						break;
				}
				if(error)
				{
					cout<<"Error in string typed. Try again."<<endl<<"Press ENTER - continue, ESC - quit";
					cycle = _getch();
				}
				else
					cycle=1;
			}
		}
	}
	catch(std::bad_alloc& ba)
	{
		cout<<"Memory error. Your operation will apply to all product list!!!";
		cycle=1;
		ArrayOfChoosedNumbers=0;
	}
	catch(int ba)
	{
		cout<<"Memory error. Your operation will apply to all product list!!!";
		cycle=1;
		ArrayOfChoosedNumbers=0;
	}

	if((cycle == 111) || (cycle == 1))
	{
		int quantity(0);
		int amount(0);
		if(BuySellDelete==0)
		{
			cout<<"Please enter product quantity of products to buy:"<<endl;
			cin>>quantity;
		}
		else if(BuySellDelete == 1)
		{
			cout<<"Please enter product quantity of products to sell:"<<endl;
			cin>>quantity;
		}
		for(int i=0; i<ArrayOfCategories[CategoryIndex]->NumberOfProducts; ++i)
		{
			if(!ArrayOfChoosedNumbers)
				amount+=ArrayOfCategories[CategoryIndex]->ArrayOfProducts[i]->price_buy;
			else
				if(ArrayOfChoosedNumbers[i] != 0)
				{
					amount+=ArrayOfCategories[CategoryIndex]->ArrayOfProducts[ArrayOfChoosedNumbers[i]-1]->price_buy;
				}
		}
		if(BuySellDelete == 0)
		{
			if(amount > Balance)
			{
				cout<<"Amount of products you want to buy is bigger than shop balance ("<<Balance<<"). You need to enter less number of products."<<endl
					<<"Press ENTER - continue, ESC - quit";
				cycle = _getch();
			}
			else
			{
				for(int i=0; i<ArrayOfCategories[CategoryIndex]->NumberOfProducts; ++i)
				{
					if(!ArrayOfChoosedNumbers)
					{
						ArrayOfCategories[CategoryIndex]->ArrayOfProducts[i]->NumberInStock+=quantity;
						ArrayOfCategories[CategoryIndex]->Balance-=ArrayOfCategories[CategoryIndex]->ArrayOfProducts[i]->price_buy;
						Balance-=ArrayOfCategories[CategoryIndex]->ArrayOfProducts[i]->price_buy;
						cycle=27;
					}
					else
						if(ArrayOfChoosedNumbers[i] != 0)
						{
							ArrayOfCategories[CategoryIndex]->ArrayOfProducts[ArrayOfChoosedNumbers[i]-1]->NumberInStock+=quantity;
							ArrayOfCategories[CategoryIndex]->Balance-=ArrayOfCategories[CategoryIndex]->ArrayOfProducts[ArrayOfChoosedNumbers[i]-1]->price_buy;
							Balance-=ArrayOfCategories[CategoryIndex]->ArrayOfProducts[ArrayOfChoosedNumbers[i]-1]->price_buy;
							cycle=27;
						}
				}
			}
		}
		else if(BuySellDelete == 1)
		{
			bool error=false;
			for(int i=0; i<ArrayOfCategories[CategoryIndex]->NumberOfProducts; ++i)
			{
				if(!ArrayOfChoosedNumbers)
				{
					if(ArrayOfCategories[CategoryIndex]->ArrayOfProducts[i]->NumberInStock < quantity)
						error = true;
				}
				else
					if(ArrayOfChoosedNumbers[i] != 0)
					{
						if(ArrayOfCategories[CategoryIndex]->ArrayOfProducts[ArrayOfChoosedNumbers[i]-1]->NumberInStock < quantity)
							error = true;
					}
			}
			if(error)
			{
				cout<<"There is no enough of product in stock, you required, please enter less quantity"<<endl
					<<"Press ENTER - continue, ESC - quit";
				cycle = _getch();
			}
			else
			{
				for(int i=0; i<ArrayOfCategories[CategoryIndex]->NumberOfProducts; ++i)
				{
					if(!ArrayOfChoosedNumbers)
					{
						ArrayOfCategories[CategoryIndex]->ArrayOfProducts[i]->NumberInStock-=quantity;
						ArrayOfCategories[CategoryIndex]->Balance+=ArrayOfCategories[CategoryIndex]->ArrayOfProducts[i]->price_sell;
						Balance+=ArrayOfCategories[CategoryIndex]->ArrayOfProducts[i]->price_sell;
						cycle=27;
					}
					else
						if(ArrayOfChoosedNumbers[i] != 0)
						{
							ArrayOfCategories[CategoryIndex]->ArrayOfProducts[ArrayOfChoosedNumbers[i]-1]->NumberInStock-=quantity;
							ArrayOfCategories[CategoryIndex]->Balance+=ArrayOfCategories[CategoryIndex]->ArrayOfProducts[ArrayOfChoosedNumbers[i]-1]->price_sell;
							Balance+=ArrayOfCategories[CategoryIndex]->ArrayOfProducts[ArrayOfChoosedNumbers[i]-1]->price_sell;
							cycle=27;
						}
				}
			}
		}
		else
		{
			int NumberOfDeleted(0), step(0);    
			for(int i=0; i<ArrayOfCategories[CategoryIndex]->NumberOfProducts; ++i)
			{
				if(!ArrayOfChoosedNumbers)
				{
					delete ArrayOfCategories[CategoryIndex]->ArrayOfProducts[i];
					++NumberOfDeleted;
				}
				else
					if(ArrayOfChoosedNumbers[i] != 0)
					{
						delete ArrayOfCategories[CategoryIndex]->ArrayOfProducts[ArrayOfChoosedNumbers[i]-1];
						++NumberOfDeleted;
					}
			}
			if(ArrayOfCategories[CategoryIndex]->NumberOfProducts-NumberOfDeleted != 0)
			{
				try
				{
					Product** ArrayOfProductsNew = new  Product*[ArrayOfCategories[CategoryIndex]->NumberOfProducts-NumberOfDeleted];
					for(int i=0; i<ArrayOfCategories[CategoryIndex]->NumberOfProducts; ++i)
					{
						try{
							for(int j=0; j<ArrayOfCategories[CategoryIndex]->NumberOfProducts; ++j)
							{
								if(i == ArrayOfChoosedNumbers[j]-1)
									throw 1;
							}
						}
						catch(int a)
						{
							continue;
						}
						ArrayOfProductsNew[step]=ArrayOfCategories[CategoryIndex]->ArrayOfProducts[i];
						++step;
					}
					delete ArrayOfCategories[CategoryIndex]->ArrayOfProducts;
					ArrayOfCategories[CategoryIndex]->ArrayOfProducts = ArrayOfProductsNew;
					ArrayOfCategories[CategoryIndex]->NumberOfProducts-=NumberOfDeleted;
					cycle=27;
				}
				catch(std::bad_alloc& ba)
				{
					cout<<"Memory error! Deleting is impossible."<<endl;
				}
			}
			else
				ArrayOfCategories[CategoryIndex]->NumberOfProducts-=NumberOfDeleted;
		}
	}

	delete ArrEntered;
	delete ArrayOfChoosedNumbers;
}

void Shop::Save()
{
	for(int i=0; i<NumberOfCategories; ++i)
	{
		FILE *file;
		errno_t err;
		err = fopen_s(&file,ArrayOfCategories[i]->BI->GetCategoryName(),"wb");           
		if(file != 0)
		{
			if(ArrayOfCategories[i]->Balance != 0)
				fwrite(&ArrayOfCategories[i]->Balance,sizeof(double),1,file);
			else
				fseek(file,sizeof(double),SEEK_SET);
			fwrite(&ArrayOfCategories[i]->NumberOfProducts,sizeof(int),1,file);
			for(int j=0; j<ArrayOfCategories[i]->NumberOfProducts; ++j)
			{
				ArrayOfCategories[i]->BOut->SaveProduct(ArrayOfCategories[i]->ArrayOfProducts[j],file);
			}
		}
		fclose(file);
	}
	COORD cord;
	cord.X =0;
	cord.Y = FooterTextTopY+2;
	SetConsoleCursorPosition(hStdout, cord);
	for(int i=0;i<FooterTextBottomY-FooterTextTopY;++i)
		printf("\r%80c", ' ');
	SetConsoleCursorPosition(hStdout, cord);
	cout<<"Saved successfull!";
}

void Shop::AddProduct(int CategoryIndex)
{
	int count(0);
	COORD cord = {0,WorkFieldTopY};
	int test=0;
	while(test != 1)
	{
		ClearWorkField();
		SetConsoleCursorPosition(hStdout, cord);
		cout<<"Please enter number of items you want to add:"<<endl;
		cin>>count;
		if(cin.fail())
		{
			cin.clear();
			cin.sync();
			cout<<"Wrong value, ENTER - Try again, ESC - quit."<<endl;
			test=_getch();
		}
		else
		{
			if(count > 100)
			{
				cout<<"Number should be less than 100!ENTER - Try again, ESC - quit"<<endl;
				test=_getch();
			}
			else if(count <= 0)
			{
				cout<<"Number should be bigger than 0! ENTER - Try again, ESC - quit."<<endl;
				test=_getch();
			}
			else
				test=1;
		}

		if(test == 27)
			test=1;
	}

	try
	{
	Product** ArrayOfProductsNew = new  Product*[ArrayOfCategories[CategoryIndex]->NumberOfProducts+count];
	int i(0);
	for(i; i<ArrayOfCategories[CategoryIndex]->NumberOfProducts; ++i)
	{
		ArrayOfProductsNew[i]=ArrayOfCategories[CategoryIndex]->ArrayOfProducts[i];
	}
	for(int j=0; j<count; ++j)
	{
		ArrayOfProductsNew[i] = ArrayOfCategories[CategoryIndex]->BI->CreateProduct();
		++ArrayOfCategories[CategoryIndex]->NumberOfProducts;
		++i;
	}
	delete ArrayOfCategories[CategoryIndex]->ArrayOfProducts;
	ArrayOfCategories[CategoryIndex]->ArrayOfProducts = ArrayOfProductsNew;
	}
	catch(std::bad_alloc& ba)
	{
		cout<<"Memory error. Product wasn't add.";
	}
}



/////////////////////////////////////////////////////////////////////////////////
////////////////////////////////Category/////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////

Category::Category(BaseFactory*(*Getfunction)()):NumberOfProducts(0),BF(Getfunction())
{
	cout<<"Category const "<<this<<endl;
	if(!BF)
		throw 0;
	BI = BF->CreateInput();
	BOut = BF->CreateOutput();
	if(!BI||!BOut)
		throw 0;


	FILE *file;
	errno_t err;
	err = fopen_s(&file,BI->GetCategoryName(),"rb");           
	if(file != 0)
	{
		fseek(file,0,SEEK_SET); 
		fread(&Balance,sizeof(double),1,file);
		fread(&NumberOfProducts,sizeof(int),1,file);
		ArrayOfProducts = new  Product*[NumberOfProducts];

		for(int i=0; i<NumberOfProducts; ++i)
		{
			ArrayOfProducts[i] = BI->CreateProduct(file);
		}
		fclose(file);
	}
	else
	{
		Balance =0;
		int count = FirstStartOfProgram();
		try
		{
			ArrayOfProducts = new  Product*[count];
		}
		catch(std::bad_alloc& ba)
		{
			throw 0;
		}
		SizeOfArray = count;
		char test;
		cin.getline(&test,1);
		for(int i=0; i<count; ++i)
		{
			ArrayOfProducts[i]= BI->CreateProduct();
			if(ArrayOfProducts[i] != 0)
				++NumberOfProducts;
		}
	}
}

int Category::FirstStartOfProgram()
{
	int number;
	cout<<"There is no any "<<BI->GetCategoryName()<<" products on the shop balance, you need to enter balance by yourself!"<<endl;
	cout<<"Please enter the number of products to create:"<<endl;
	int test=0;
	while(test != 1)
	{
		cin>>number;
		if(cin.fail())
		{
			cin.clear();
			cin.sync();
			cout<<"Wrong value, try again."<<endl;
		}
		else
		{
			if(number > 10000)
				cout<<"Number should be less than 10 000! Try again."<<endl;
			else if(number <= 0)
				cout<<"Number should be bigger than 0! Try again."<<endl; 
			else
				test=1;
		}
	}
	return number;
}

Category::~Category()
{
	for(int i=0; i<NumberOfProducts; ++i)
		delete ArrayOfProducts[i];
	delete ArrayOfProducts;
	delete BF;
	delete BI;
	delete BOut;
	cout<<"Category dest "<<this<<endl;
}

