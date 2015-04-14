 class Product;
 class BaseInputOutputInfo;

class BaseInput
{
	public:
	virtual const char* GetCategoryName()=0;
	virtual Product* CreateProduct(BaseInputOutputInfo* BaseInOut) = 0;
};