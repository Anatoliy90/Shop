#pragma once
const int SizeOfName = 100;
const int SizeOfColor = 20;

class Product
{
public:
	char Name[SizeOfName];
	double price_sell; 
	double price_buy;
	char Color[SizeOfColor];
	int NumberInStock;
	int ParametersCount;
	friend class Shop;
};