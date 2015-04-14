

class BaseLoad
{
public:
	virtual bool FirstLoad()=0;
	virtual int ReturnNumberOfProducts()=0;
	virtual int ReturnBalance()=0;
	virtual bool CreateProduct(Product** ProdArray)=0;

};