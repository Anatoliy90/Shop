#include "stdafx.h"
#include "InputOutputInfoConsole.h"

using namespace std;

enum{SHOW_ALL,SAVE_IN_FILE};

InputOutputInfoConsole::InputOutputInfoConsole():WorkFieldTopY(0),WorkFiledBottomY(0),
	                                             FooterTextTopY(0),FooterTextBottomY(0),
	                                             NumberOfCategories(0),NumberOfProducts(0),MenuIndex(0)
{
	hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
}

bool InputOutputInfoConsole::ShopInput(ShopOperations* SO)
{
	int CurrMenuItemY(0);
	int FirstMenuItemY(0);
	int FooterTextY(0);
	int ChoiceCycle(0);
	int ChoiceStep(0);
	int FirstRoundOfCycle(0);

	NumberOfCategories = SO->shop->NumberOfCategories;
	ArrayOfCategories =  SO->shop->ArrayOfCategories;

	COORD cord;
	cord.X = 80;
	cord.Y = 40;
	SMALL_RECT src = {0, 0, cord.X-1, cord.Y-1};
	SetConsoleWindowInfo (hStdout, true, &src);
	SetConsoleScreenBufferSize (hStdout, cord);

	if(MenuIndex!=0)
		return Menu1(SO);

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

	const char* MenuArray[2]={". SHOW ALL PRODUCTS IN STOCK",
		". SAVE SHOP INFORMATION IN FILE"};


	while(ChoiceCycle!=27)
	{
		int FirstMenuItemPosition(0);
		
		ClearWorkField();
		cout<<"MENU:"<<endl<<endl;
		cin.clear();
		cin.sync();
		GetConsoleScreenBufferInfo(hStdout, &csbInfo);
		cord.X = 0;
		cord.Y = csbInfo.dwCursorPosition.Y;
	
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
						return Menu1(SO);
						break;
					}             
				case SAVE_IN_FILE:
					{
						SO->operation = 6;
						return true;
						break;
					}
				}
				break;
			}
		case 27:
			{
				int check_cycle(0);

				cord.Y = csbInfo.dwCursorPosition.Y+4;
				SetConsoleCursorPosition(hStdout, cord);
				cout<<"Are you sure you want to quit? ENTER - yes, ESC - no."<<endl;
				while (check_cycle!=1)
				{
					int exit =_getch();
					if(exit==13)
					{
						check_cycle=1;
						SO->operation = 27;
						system("cls");
					}
					else if(exit==27)
					{
                        ChoiceCycle=0;
						check_cycle=1;
					}
				}
					break;
			}
		}
	}
	return true;
}

void InputOutputInfoConsole::Header()
{
	system("cls");
	COORD cord={0,0};
	SetConsoleCursorPosition(hStdout, cord);
	char nameShop[12] = "FLOWER SHOP";
	int k(0),NumberOfProdInStock(0);
	for(int i=0; i<NumberOfCategories; ++i)
		NumberOfProdInStock+=ArrayOfCategories[i]->GetNumberOfProducts();

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
				bal+=ArrayOfCategories[i]->GetBalance();
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

void InputOutputInfoConsole::Footer()
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

void InputOutputInfoConsole::ClearWorkField()
{
	COORD cord = {0,WorkFieldTopY};
	SetConsoleCursorPosition(hStdout, cord);
	for(int i=0; i<WorkFiledBottomY-WorkFieldTopY; ++i)
		printf("\r%80c", ' ');
	SetConsoleCursorPosition(hStdout, cord);
	SetConsoleTextAttribute(hStdout,07);
}

bool InputOutputInfoConsole::Menu1(ShopOperations* SO)
{
	if(MenuIndex==2)
		return Menu2(SO->CategoryIndex,SO,SO->IndexesOfProductAvaiableToShow);

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

			if(strlen(ArrayOfCategories[i]->GetBaseInput()->GetCategoryName()) < x2-x1-6)
				cout<<ArrayOfCategories[i]->GetBaseInput()->GetCategoryName();
			else
			{
				try
				{
					char* tmp = new char[x2-x1-5];
					memcpy(tmp,ArrayOfCategories[i]->GetBaseInput()->GetCategoryName(),x2-x1-6);
					tmp[x2-x1-6] = '\0';
					cout<<tmp<<"...";

					delete tmp;
				}
				catch(std::bad_alloc& ba)
				{
					char* NameCat = (char*)ArrayOfCategories[i]->GetBaseInput()->GetCategoryName();
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
			cout<<ArrayOfCategories[i]->GetNumberOfProducts()<<endl;
			++count;
		}
		cout<<"------+--------------------------------------------------------+---------------"<<endl;

		ChoiceCycle=_getch();
		if(ChoiceCycle == 224)
			ChoiceCycle=_getch();

		bool choice_done = false;

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
				choice_done =  Menu2(ChoiceStep,SO);
				break;
			}
		}
		if(choice_done)
			return choice_done;
		else
			MenuIndex = 0;

	}
}

bool InputOutputInfoConsole::Menu2(int ChoiceStep,ShopOperations* SO, int* OutputList)
{
	Header();
	Footer();

	COORD cord;

	int WorkFieldHight = WorkFiledBottomY - WorkFieldTopY-1;
	int step(1);
	bool ScrollBorder=false;

	int FirstProdIndexForCycleMooving;
	if(OutputList == 0)
		FirstProdIndexForCycleMooving=0;
	else if(OutputList[0] != -1)
		FirstProdIndexForCycleMooving=OutputList[0];
	else
		FirstProdIndexForCycleMooving=0;

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

		NumberOfProducts = ArrayOfCategories[ChoiceStep]->GetNumberOfProducts();

		cout<<ArrayOfCategories[ChoiceStep]->GetBaseInput()->GetCategoryName()<<" CATEGORY PRODUCT LIST. "
			<<NumberOfProducts<<" PRODUCTS AVALIABLE."<<endl;


		if(AvaliableProducts != 0)
			for(int i=0; i<WorkFiledBottomY-WorkFieldTopY; ++i)
				AvaliableProducts[i]=0;
		AvaliableProductsCount=0;
		int stepaval(0);
		
		ScrollBorder = false;
		int LastProd = ArrayOfCategories[ChoiceStep]->GetNumberOfProducts()-1;
		for(int i=0; i<NumberOfProducts; ++i)
		{
			if(OutputList != 0)
			{
				bool ContinueOutput = false;
				for(int k=0; k<NumberOfProducts; ++k)
				{
					if(OutputList[k] == i)
					{
						ContinueOutput = true;
						if(k==NumberOfProducts || OutputList[k+1]==-1)
							LastProd = i;
					}
				}
				if(OutputList[0]==-1)
					ContinueOutput = true;
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
			for(int j=0; j<ArrayOfCategories[ChoiceStep]->GetArrayOfProducts()[0]->ParametersCount; ++j)
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
					ArrayOfCategories[ChoiceStep]->GetBaseOutput()->Show(ArrayOfCategories[ChoiceStep]->GetArrayOfProducts()[i],j,this);
					++step;
					cord.Y=WorkFieldTopY+step;
					SetConsoleCursorPosition(hStdout, cord);
					if(j == ArrayOfCategories[ChoiceStep]->GetArrayOfProducts()[0]->ParametersCount-1 && i == LastProd)
						ScrollBorder = true;
				}
			}
		}
		step=1;
		StartShowI = false; StartShowJ = false;
		ChoiceCycle_2=_getch();
		if(ChoiceCycle_2 == 224)
			ChoiceCycle_2=_getch(); 

		bool choice_done = false;

		switch(ChoiceCycle_2)
		{
		case 80:
			{
				if(ScrollBorder == true)
					break;
				if(FirstProdParamforCycleMooving == ArrayOfCategories[ChoiceStep]->GetArrayOfProducts()[0]->ParametersCount-1)
				{
					++FirstProdIndexForCycleMooving;FirstProdParamforCycleMooving=0;
				}
				else
					++FirstProdParamforCycleMooving;
				break;
			}  
		case 72:
			{
				if((FirstProdIndexForCycleMooving == (((OutputList != 0) && (OutputList[0]!=-1))?OutputList[0]:0)) && (FirstProdParamforCycleMooving == 0))
					break;
				if(FirstProdParamforCycleMooving == 0)
				{
					--FirstProdIndexForCycleMooving; 
					FirstProdParamforCycleMooving=ArrayOfCategories[ChoiceStep]->GetArrayOfProducts()[0]->ParametersCount-1;
				}
				else
					--FirstProdParamforCycleMooving;
				break;
			} 
		case 235:
			{
				choice_done = Sort(SO,ChoiceStep);
				break;
			}
		case 160:
			{
				choice_done = Find(SO,ChoiceStep);
				break;
			}
		case 168:
			{
				choice_done = BuySellDelete(SO,ChoiceStep,AvaliableProducts,AvaliableProductsCount,0);
				break;
			}
		case 167:
			{
				choice_done = BuySellDelete(SO,ChoiceStep,AvaliableProducts,AvaliableProductsCount,1);
				break;
			}
		case 162:
			{
				choice_done = BuySellDelete(SO,ChoiceStep,AvaliableProducts,AvaliableProductsCount,2);
				break;
			}
		case 228:
			{
				choice_done = AddProduct(SO,ChoiceStep);
				if(choice_done)
					system("cls");
				break;
			}
		}
		if(choice_done)
		{
			if(ChoiceCycle_2==27)
            MenuIndex = 1;
			else
			MenuIndex = 2;
			return choice_done;
		}
		else
			MenuIndex = 1;
	}
	delete AvaliableProducts;
	
}

void InputOutputInfoConsole::ShopOutput(const char*  OutputString, int NewLine)
{
	cout<<OutputString;
	if(NewLine!=0)
		cout<<endl;
}

bool InputOutputInfoConsole::Sort(ShopOperations* SO,int CategoryIndex)
{
	ClearWorkField();

	const char** ArrayOfParameters=0;
	char* ArrEntered=0;

	try
	{
		ArrayOfParameters = new const char*[ArrayOfCategories[CategoryIndex]->GetArrayOfProducts()[0]->ParametersCount];
		ArrayOfCategories[CategoryIndex]->GetBaseOutput()->GetProductParameters(ArrayOfParameters);
		for(int i=0; i<ArrayOfCategories[CategoryIndex]->GetArrayOfProducts()[0]->ParametersCount; ++i)
		{

			cout<<i+1<<". "<<ArrayOfParameters[i]<<endl;
		}
		cout<<endl<<"Please enter number of parameters you want to sort by"<<endl;
		cout<<"You can choose several parameters, you need to enter their indexes one by one without space!"<<endl;
		cout<<"For example: 312"<<endl;
		GetConsoleScreenBufferInfo(hStdout, &csbInfo);
		COORD cord = {0,csbInfo.dwCursorPosition.Y};

		int  cycle(0);
		ArrEntered = new char[ArrayOfCategories[CategoryIndex]->GetArrayOfProducts()[0]->ParametersCount];
		while(cycle != 1)
		{
			cin.clear();
			cin.sync();
			cin>>ArrEntered;
			SetConsoleCursorPosition(hStdout, cord);
			for(int i=0; i<10; ++i)
				printf("\r%80c", ' ');
			SetConsoleCursorPosition(hStdout, cord);
			if(strlen(ArrEntered) > ArrayOfCategories[CategoryIndex]->GetArrayOfProducts()[0]->ParametersCount)
			{
				cout<<"Number of parameters should be less than "<<ArrayOfCategories[CategoryIndex]->GetArrayOfProducts()[0]->ParametersCount
					<<" ! Try again."<<endl;
				continue;
			}
			else if(strlen(ArrEntered) == 0)
			{
				cout<<"You entered nothing! Try again."<<endl;
				continue;
			}

			int OneWarning(0), error(0);
			for(int i=0; i<ArrayOfCategories[CategoryIndex]->GetArrayOfProducts()[0]->ParametersCount; ++i)
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
						if(atoi(&tmp) > ArrayOfCategories[CategoryIndex]->GetArrayOfProducts()[0]->ParametersCount)
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

		SO->operation = 0;
		SO->CategoryIndex = CategoryIndex;
		SO->MetodOfSort = method;

		memcpy(SO->ArrayOfParameters,ArrEntered,strlen(ArrEntered));

	}
	catch(std::bad_alloc& ba)
	{
		cout<<"Memory error, operation can't be proceeded";
		return false;
	}
	delete ArrayOfParameters;
	delete ArrEntered;
	return true;
}

bool InputOutputInfoConsole::Find(ShopOperations* SO,int CategoryIndex)
{
	ClearWorkField();
	const char** ArrayOfParameters=0;
	char* value=0;
	int* result=0;

	try
	{
		ArrayOfParameters = new const char*[ArrayOfCategories[CategoryIndex]->GetArrayOfProducts()[0]->ParametersCount];
		ArrayOfCategories[CategoryIndex]->GetBaseOutput()->GetProductParameters(ArrayOfParameters);
		for(int i=0; i<ArrayOfCategories[CategoryIndex]->GetArrayOfProducts()[0]->ParametersCount; ++i)
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
				if(param>ArrayOfCategories[CategoryIndex]->GetArrayOfProducts()[0]->ParametersCount)
					cout<<"Wrong value, try again."<<endl;
				else
					cycle=1;
			}
		}

		value = new char[100];
		cout<<"Please enter value you want to find(less than 100 symbols):"<<endl;
		cin>>value;

		SO->operation = 1;
		SO->CategoryIndex = CategoryIndex;
		SO->ParamIndex = param;
		memcpy(SO->ArrayOfParameters,value,strlen(value));
	}
	catch(std::bad_alloc& ba)
	{
		cout<<"Memory error, operation can't be proceeded";
		return false;
	}
	delete ArrayOfParameters;
	delete value;
	delete result;
	return true;
}

bool InputOutputInfoConsole::BuySellDelete(ShopOperations* SO,int CategoryIndex, int* ProductsAvaliable, int ProductsAvaliableSize,int BuySellDelete)
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
			int max_c(0);
			ArrayOfChoosedNumbers = new int[ArrayOfCategories[CategoryIndex]->GetNumberOfProducts()];
			ArrEntered = new char[(ArrayOfCategories[CategoryIndex]->GetArrayOfProducts()[0]->ParametersCount > ArrayOfCategories[CategoryIndex]->GetNumberOfProducts())
								 ?ArrayOfCategories[CategoryIndex]->GetArrayOfProducts()[0]->ParametersCount*2
			                     :ArrayOfCategories[CategoryIndex]->GetNumberOfProducts()*2];
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

			for(int i=0; i<ArrayOfCategories[CategoryIndex]->GetNumberOfProducts(); ++i)
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
				for(int i=0; i<ArrayOfCategories[CategoryIndex]->GetNumberOfProducts(); ++i)
					ArrayOfChoosedNumbers[i] = i;        ////проверить
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
						else
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

				bool ProductIsAvaliable = true;

				for(int m=0;m<ArrayOfCategories[CategoryIndex]->GetNumberOfProducts();++m)
				{
					if(ArrayOfChoosedNumbers[m]!=0)
					{
						bool cycle_test = false;
						for(int k=0; k<ProductsAvaliableSize; ++k)
							if((ArrayOfChoosedNumbers[m]!=0) && (ProductsAvaliable[k]==ArrayOfChoosedNumbers[m]))
								cycle_test = true;
						if(!cycle_test)
						{
							ProductIsAvaliable = false;
							break;
						}
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
					cout<<"Error in string typed. Try again."<<endl<<"Press ENTER - continue";
					cycle = _getch();
				}
				else
					cycle=1;
			}
		}
	}
	catch(std::bad_alloc& ba)
	{
		cout<<"Memory error. Your operation will apply to all product list!!!Press ENTER - continue";
		cycle = _getch();
		cycle=1;
		ArrayOfChoosedNumbers=0;
	}
	catch(int ba)
	{
		cout<<"Memory error. Your operation will apply to all product list!!!Press ENTER  - continue";
		cycle = _getch();
		cycle=1;
		ArrayOfChoosedNumbers=0;
	}

	if(cycle==27)
		return false;

	bool success = true;

	int Balance = SO->shop->Balance;
	if((cycle == 111) || (cycle == 1))
	{
		int quantity(0);
		int amount(0);
		if(BuySellDelete==0)
		{
			cout<<"Please enter product quantity of products to buy:"<<endl;
		}
		else if(BuySellDelete == 1)
		{
			cout<<"Please enter product quantity of products to sell:"<<endl;
		}
		GetConsoleScreenBufferInfo(hStdout, &csbInfo);
		COORD cord = {0,csbInfo.dwCursorPosition.Y};

		if(BuySellDelete==1 || BuySellDelete==0)
		{
			int test=0;
			while(test!=1)
			{
				SetConsoleCursorPosition(hStdout, cord);
				for(int i=0; i<10; ++i)
					printf("\r%80c", ' ');
				SetConsoleCursorPosition(hStdout, cord);

				cin>>quantity;
				if(cin.fail())
				{
					cin.clear();
					cin.sync();
					cout<<"Wrong value, try again."<<endl;
				}
				else
				{

					for(int i=0; i<ArrayOfCategories[CategoryIndex]->GetNumberOfProducts(); ++i)
					{
						if(!ArrayOfChoosedNumbers)
							amount+=ArrayOfCategories[CategoryIndex]->GetArrayOfProducts()[i]->price_buy;
						else
							if(ArrayOfChoosedNumbers[i] != 0)
							{
								amount+=ArrayOfCategories[CategoryIndex]->GetArrayOfProducts()[ArrayOfChoosedNumbers[i]-1]->price_buy;
							}
					}

					if(BuySellDelete == 0)
					{
						if(amount >ArrayOfCategories[CategoryIndex]->GetBalance())
						{
							cout<<"Amount of products you want to buy is bigger than shop balance ("<<ArrayOfCategories[CategoryIndex]->GetBalance()
								<<"). You need to enter less number of products."<<endl
								<<"Press ENTER - continue, ESC - quit";
							cycle = _getch();
							if(cycle==27)
								test=1;
						}
						else
							test=1;
					}
					else if(BuySellDelete == 1)
					{
						bool error=false;
						for(int i=0; i<ArrayOfCategories[CategoryIndex]->GetNumberOfProducts(); ++i)
						{
							if(!ArrayOfChoosedNumbers)
							{
								if(ArrayOfCategories[CategoryIndex]->GetArrayOfProducts()[i]->NumberInStock < quantity)
									error = true;
							}
							else
								if(ArrayOfChoosedNumbers[i] != 0)
								{
									if(ArrayOfCategories[CategoryIndex]->GetArrayOfProducts()[ArrayOfChoosedNumbers[i]-1]->NumberInStock < quantity)
										error = true;
								}
						}
						if(error)
						{
							cout<<"There is no enough of product in stock, you required, please enter less quantity"<<endl
								<<"Press ENTER - continue, ESC - quit";
							cycle = _getch();
							if(cycle==27)
								test=1;
						}
						else
							test=1;
					}
					else
						test=1;
				}
			}
		}
		
		if(cycle==27)
			return 0;

		if(BuySellDelete == 0)
		{

			SO->operation=2;
			SO->CategoryIndex=CategoryIndex;
			SO->ParamIndex=quantity;
			for(int i=0;i<sizeof(ArrayOfChoosedNumbers);++i)
				if( ArrayOfChoosedNumbers[i]!=0)
					SO->ArrayOfProductIndexesForOperation[i] = ArrayOfChoosedNumbers[i];
			
		}
		else if(BuySellDelete == 1)
		{
			SO->operation=3;
			SO->CategoryIndex=CategoryIndex;
			SO->ParamIndex=quantity;
			for(int i=0;i<sizeof(ArrayOfChoosedNumbers);++i)
				if( ArrayOfChoosedNumbers[i]!=0)
					SO->ArrayOfProductIndexesForOperation[i] = ArrayOfChoosedNumbers[i];
		}
		else
		{
			SO->operation=4;
			SO->CategoryIndex=CategoryIndex;
			for(int i=0;i<sizeof(ArrayOfChoosedNumbers);++i)
				if( ArrayOfChoosedNumbers[i]!=0)
					SO->ArrayOfProductIndexesForOperation[i] = ArrayOfChoosedNumbers[i];
		}
	}
	delete ArrEntered;
	delete ArrayOfChoosedNumbers;
	return success;
}

bool InputOutputInfoConsole::AddProduct(ShopOperations* SO, int CategoryIndex)
{
	int count(0);
	COORD cord = {0,WorkFieldTopY};
	bool success = false;
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
			{
				success = true;
				test=1;
			}
		}

		if(test == 27)
			test=1;
	}

	SO->operation=5;
	SO->CategoryIndex=CategoryIndex;
	SO->ParamIndex=count;
	return success;	
}

char* InputOutputInfoConsole::GetValue()
{
	char val[500];
	cin>>val;
	return val;
}