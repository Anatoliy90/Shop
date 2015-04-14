#include "stdafx.h"
#include "InputOutputInfoConsole.h"

using namespace std; 

enum{SORT,FIND,BUY,SELL,DELETE_OP,ADD,SAVE};

/////////////////////////////////////////////////////////////////////////////////
////////////////////////////////Shop/////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////

Shop::Shop():NumberOfCategories(0),SizeOfArray(0),Balance(0)
{
	try
	{
		
		ArrayOfCategories = new Category*[1];

		if(ArrayOfCategories != 0)
			SizeOfArray = 1;

		BaseInPFlow = (BaseInputOutputInfo*) new InputOutputInfoConsole;

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
								BaseInPFlow->ShopOutput("Not memory enough for proper shop work");
							}
							catch(int er)
							{
								char* str = new char[wcslen(wf.cFileName+1)];
								BaseInPFlow->ShopOutput("Not memory enough for read ");
								for(int i=0;i<wcslen(wf.cFileName)+1;++i)
								{
									if(str[i]==wcslen(wf.cFileName))
                                       str[i]='\0';
									str[i]=(char)wf.cFileName[i];
								}
								BaseInPFlow->ShopOutput(str);
								delete str;
							}
						}
						Cycle = FindNextFile(hFile, &wf);
				}

			}

			FindClose(hFile);


			if(NumberOfCategories != 0)
			{
				ShopOp.shop = this;

				int max = 0;
				for(int i=0;i<NumberOfCategories;++i)
				{
					if(ArrayOfCategories[i]->NumberOfProducts>max)
						max=ArrayOfCategories[i]->NumberOfProducts;
				}
				int *arr_int = new int[max];
				int *arr_int1 = new int[max];
				for(int i=0;i<max;++i)
				{
					arr_int[i]=-1;
				    arr_int1[i]=-1;
				}
				ShopOp.MaxCountOfProductsInCategories = max;
				ShopOp.IndexesOfProductAvaiableToShow = arr_int;
				ShopOp.ArrayOfProductIndexesForOperation = arr_int1;

				WorkWithShop();
			}
			else
			{
				BaseInPFlow->ShopOutput("The Shop is empty! End of work.");
				Shop::~Shop();
			}
	}
	catch (std::bad_alloc& ba)
	{
		BaseInPFlow->ShopOutput("There's no vacant memory for work! End of work.");
		Shop::~Shop();
	}

}

Shop::~Shop()
{
	for(int i=0; i<NumberOfCategories; ++i)
		delete ArrayOfCategories[i];
	delete ArrayOfCategories;

}

void Shop::WorkWithShop()
{

	int max = 0;
	for(int i=0;i<NumberOfCategories;++i)
	{
		if(ArrayOfCategories[i]->ArrayOfProducts[0]->ParametersCount>max)
			max=ArrayOfCategories[i]->ArrayOfProducts[0]->ParametersCount;
	}

    ShopOp.ArrayOfParameters = new char[max];
	for(int i=0;i<max;++i)
		ShopOp.ArrayOfParameters[i]='\0';
	
	while(ShopOp.operation!=27)
	{
		BaseInPFlow->ShopInput(&ShopOp);
		switch(ShopOp.operation)
		{
		case SORT:
			{
				ArrayOfCategories[ShopOp.CategoryIndex]->BOut->Sort(ArrayOfCategories[ShopOp.CategoryIndex]->ArrayOfProducts,
					ArrayOfCategories[ShopOp.CategoryIndex]->NumberOfProducts,
					ShopOp.ArrayOfParameters,ShopOp.MetodOfSort);
				RefreshShopOperations();
				break;
			}
		case FIND:
			{

				try
				{
					for(int i=0;i<ShopOp.MaxCountOfProductsInCategories;++i)
						ShopOp.IndexesOfProductAvaiableToShow[i] = -1;
		
					ArrayOfCategories[ShopOp.CategoryIndex]->BOut->Find(ArrayOfCategories[ShopOp.CategoryIndex]->ArrayOfProducts,
						ArrayOfCategories[ShopOp.CategoryIndex]->NumberOfProducts,
						ShopOp.ArrayOfParameters,ShopOp.ParamIndex,ShopOp.IndexesOfProductAvaiableToShow);
					if(ShopOp.IndexesOfProductAvaiableToShow[0]==-1)
						ShopOp.IndexesOfProductAvaiableToShow[0]=-2;
				}
				catch(std::bad_alloc& ba)
				{
                 BaseInPFlow->ShopOutput("Not memory enough for completing this operation.");  
				}
				break;
			}
		case BUY:
			{
				for(int i=0; i<ArrayOfCategories[ShopOp.CategoryIndex]->GetNumberOfProducts(); ++i)
				{
					if(!ShopOp.ArrayOfProductIndexesForOperation)
					{
						ArrayOfCategories[ShopOp.CategoryIndex]->GetArrayOfProducts()[i]->NumberInStock+=ShopOp.ParamIndex;
						Balance-=ArrayOfCategories[ShopOp.CategoryIndex]->GetArrayOfProducts()[i]->price_buy;
						ArrayOfCategories[ShopOp.CategoryIndex]->SetBalance(ArrayOfCategories[ShopOp.CategoryIndex]->GetBalance()-ArrayOfCategories[ShopOp.CategoryIndex]->GetArrayOfProducts()[i]->price_buy*ShopOp.ParamIndex);
					}
					else
						if((ShopOp.ArrayOfProductIndexesForOperation[i] != 0) && (ShopOp.ArrayOfProductIndexesForOperation[i]!=-1))
						{
							ArrayOfCategories[ShopOp.CategoryIndex]->GetArrayOfProducts()[ShopOp.ArrayOfProductIndexesForOperation[i]-1]->NumberInStock+=ShopOp.ParamIndex;
							Balance-=ArrayOfCategories[ShopOp.CategoryIndex]->GetArrayOfProducts()[ShopOp.ArrayOfProductIndexesForOperation[i]-1]->price_buy;
							ArrayOfCategories[ShopOp.CategoryIndex]->SetBalance(ArrayOfCategories[ShopOp.CategoryIndex]->GetBalance()-ArrayOfCategories[ShopOp.CategoryIndex]->GetArrayOfProducts()[ShopOp.ArrayOfProductIndexesForOperation[i]-1]->price_buy*ShopOp.ParamIndex);
						}
				}
				RefreshShopOperations();
				break;
			}
		case SELL:               
			{
				for(int i=0; i<ArrayOfCategories[ShopOp.CategoryIndex]->NumberOfProducts; ++i)
				{
					if(!ShopOp.ArrayOfProductIndexesForOperation)
					{
						ArrayOfCategories[ShopOp.CategoryIndex]->GetArrayOfProducts()[i]->NumberInStock-=ShopOp.ParamIndex;
                        ArrayOfCategories[ShopOp.CategoryIndex]->SetBalance(ArrayOfCategories[ShopOp.CategoryIndex]->GetBalance()+ArrayOfCategories[ShopOp.CategoryIndex]->GetArrayOfProducts()[i]->price_sell*ShopOp.ParamIndex);
						Balance+=ArrayOfCategories[ShopOp.CategoryIndex]->GetArrayOfProducts()[i]->price_sell;
					}
					else
						if((ShopOp.ArrayOfProductIndexesForOperation[i] != 0) && (ShopOp.ArrayOfProductIndexesForOperation[i]!=-1))
						{
							ArrayOfCategories[ShopOp.CategoryIndex]->GetArrayOfProducts()[ShopOp.ArrayOfProductIndexesForOperation[i]-1]->NumberInStock-=ShopOp.ParamIndex;
							ArrayOfCategories[ShopOp.CategoryIndex]->SetBalance(ArrayOfCategories[ShopOp.CategoryIndex]->GetBalance()+ArrayOfCategories[ShopOp.CategoryIndex]->GetArrayOfProducts()[ShopOp.ArrayOfProductIndexesForOperation[i]-1]->price_sell*ShopOp.ParamIndex);
							Balance+=ArrayOfCategories[ShopOp.CategoryIndex]->GetArrayOfProducts()[ShopOp.ArrayOfProductIndexesForOperation[i]-1]->price_sell;
						}
				}
				RefreshShopOperations();
				break;
			}
		case DELETE_OP:
			{
				int NumberOfDeleted(0), step(0), res(0);  
				try
				{
					for(int i=0; i<ArrayOfCategories[ShopOp.CategoryIndex]->GetNumberOfProducts(); ++i)
					{
						if(!ShopOp.ArrayOfProductIndexesForOperation)
						{
							++NumberOfDeleted;
						}
						else
							if((ShopOp.ArrayOfProductIndexesForOperation[i] != 0) && (ShopOp.ArrayOfProductIndexesForOperation[i]!=-1))
							{
								++NumberOfDeleted;
							}
					}
					
					Product** ArrayOfProductsNew = new  Product*[ArrayOfCategories[ShopOp.CategoryIndex]->GetNumberOfProducts()-NumberOfDeleted];
					res = ArrayOfCategories[ShopOp.CategoryIndex]->GetNumberOfProducts()-NumberOfDeleted;
					for(int i=0; i<ArrayOfCategories[ShopOp.CategoryIndex]->GetNumberOfProducts(); ++i)
					{
						if(!ShopOp.ArrayOfProductIndexesForOperation)
						{
							delete ArrayOfCategories[ShopOp.CategoryIndex]->GetArrayOfProducts()[i];
							++NumberOfDeleted;
						}
						else
							if((ShopOp.ArrayOfProductIndexesForOperation[i] != 0) && (ShopOp.ArrayOfProductIndexesForOperation[i]!=-1))
							{
								delete ArrayOfCategories[ShopOp.CategoryIndex]->GetArrayOfProducts()[ShopOp.ArrayOfProductIndexesForOperation[i]-1];
								++NumberOfDeleted;
							}
					}
					if(ArrayOfCategories[ShopOp.CategoryIndex]->GetNumberOfProducts()-NumberOfDeleted != 0)
					{
						for(int i=0; i<ArrayOfCategories[ShopOp.CategoryIndex]->GetNumberOfProducts(); ++i)
						{
							try{
								for(int j=0; j<ArrayOfCategories[ShopOp.CategoryIndex]->GetNumberOfProducts(); ++j)
								{
									if(i == ShopOp.ArrayOfProductIndexesForOperation[j]-1)
										throw 1;
								}
							}
							catch(int a)
							{
								continue;
							}
							ArrayOfProductsNew[step]=ArrayOfCategories[ShopOp.CategoryIndex]->GetArrayOfProducts()[i];
							++step;
						}
						delete ArrayOfCategories[ShopOp.CategoryIndex]->GetArrayOfProducts();
						ArrayOfCategories[ShopOp.CategoryIndex]->SetArrayOfProducts(ArrayOfProductsNew);
						ArrayOfCategories[ShopOp.CategoryIndex]->SetNumberOfProducts(res);
					}
					else
					{
						delete ArrayOfProductsNew;
						ArrayOfCategories[ShopOp.CategoryIndex]->SetNumberOfProducts(res);
					}
				}
				catch(std::bad_alloc& ba)
				{
					BaseInPFlow->ShopOutput("Memory error! Deleting is impossible.");
				}
				RefreshShopOperations();
				break;
			}
		case ADD:
			{
				try
				{
					Product** ArrayOfProductsNew = new  Product*[ArrayOfCategories[ShopOp.CategoryIndex]->GetNumberOfProducts()+ShopOp.ParamIndex];
					int i(0);
					for(i; i<ArrayOfCategories[ShopOp.CategoryIndex]->GetNumberOfProducts(); ++i)
					{
						ArrayOfProductsNew[i]=ArrayOfCategories[ShopOp.CategoryIndex]->GetArrayOfProducts()[i];
					}
					for(int j=0; j<ShopOp.ParamIndex; ++j)
					{
						ArrayOfProductsNew[i] = ArrayOfCategories[ShopOp.CategoryIndex]->GetBaseInput()->CreateProduct(BaseInPFlow);
						ArrayOfCategories[ShopOp.CategoryIndex]->SetNumberOfProducts(ArrayOfCategories[ShopOp.CategoryIndex]->GetNumberOfProducts()+1);
						++i;
					}
					delete ArrayOfCategories[ShopOp.CategoryIndex]->GetArrayOfProducts();
					ArrayOfCategories[ShopOp.CategoryIndex]->SetArrayOfProducts(ArrayOfProductsNew);
				}
				catch(std::bad_alloc& ba)
				{
					BaseInPFlow->ShopOutput("Memory error. Product wasn't add.");
				}

				RefreshShopOperations();
				break;
			}
		case SAVE:
			{
				Save();
				RefreshShopOperations();
				break;
			}
		}

	}

}

void Shop::Save()
{
	for(int i=0;i<NumberOfCategories;++i)
	{
    bool err = ArrayOfCategories[i]->BSave->SaveProducts(ArrayOfCategories[i]->Balance,ArrayOfCategories[i]->NumberOfProducts,ArrayOfCategories[i]->ArrayOfProducts);
	if(err)
         BaseInPFlow->ShopOutput("Saved successfull! Category: ");
	else
		 BaseInPFlow->ShopOutput("Error! Data was not saved. Category: ");
	BaseInPFlow->ShopOutput(ArrayOfCategories[i]->BI->GetCategoryName());
	}
}


Category** Shop::GetCategoryArray()
{
	return ArrayOfCategories;
}

BaseInputOutputInfo* Shop::GetSO()
{
	return BaseInPFlow;
}

void Shop::RefreshShopOperations()
{
	for(int i=0;i<ShopOp.MaxCountOfProductsInCategories;++i)
	{
		ShopOp.ArrayOfProductIndexesForOperation[i]=-1;
		ShopOp.IndexesOfProductAvaiableToShow[i] = -1;
	}
	for(int i=0;i<strlen(ShopOp.ArrayOfParameters);++i)
		ShopOp.ArrayOfParameters[i]='\0';
	ShopOp.operation = 0;
	ShopOp.CategoryIndex = 0;
	ShopOp.MetodOfSort = 0;
	ShopOp.ParamIndex = 0;
}