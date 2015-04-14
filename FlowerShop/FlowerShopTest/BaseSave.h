//class Product;


class BaseSave
{
public:
	virtual bool SaveProducts(int Balance, int NumberOfProducts, Product** ArrayOfProducts)=0;
};