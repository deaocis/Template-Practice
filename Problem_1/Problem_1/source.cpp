#include <iostream>

template <class T_CLASS>


void print(const T_CLASS& input)
{
	std::cout << input << std::endl;
}

int main()
{
	print(1);
	print(2.345f);
	print("Hello World!");
}