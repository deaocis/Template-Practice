#include <iostream>
#include <string>
#include <vector>

//template <class TT> 
////class Vector2D
////{
////public:
////	TT x_, y_;
////	
////	Vector2D(const TT& x_input, const TT& y_input)
////		:x_(x_input), y_(y_input)
////	{}
////
////	void print()
////	{
////		std::cout << x_ << " " << y_ << std::endl;
////	}
////};

template <class TT, class TTT>
class Vector2D
{
public:
	TT x_;
	TTT y_;
	
	Vector2D(const TT& x_input, const TTT& y_input)
		:x_(x_input), y_(y_input)
	{}

	void print()
	{
		std::cout << x_ << " " << y_ << std::endl;
	}
};

int main()
{
	//Vector2D<int> int_vec(1, 1);
	//int_vec.print();

	//Vector2D<std::string> str_vector("Hello", "I`m hungry");
	//str_vector.print();

	Vector2D<std::string, int> str_int_vector("Hello", 123123);
	str_int_vector.print();

	
}














//#include <iostream>
//#include <string>
//#include <vector>
//
//class Draw
//{
//public:
//	void doSomething()
//	{
//		std::cout << "Draw" << std::endl;
//	}
//};
//
//class Move
//{
//public:
//	void doSomething()
//	{
//		std::cout << "Move" << std::endl;
//	}
//};
//
//class GeometricObjectInterface
//{
//public:
//	virtual void doSomething() = 0;
//
//};
//
//template<class TT>
//class GeometricObject :public GeometricObjectInterface
//{
//public:
//	void doSomething()
//	{
//		TT operation;
//		operation.doSomething();
//	}
//};
//
//int main()
//{
//	std::vector<GeometricObjectInterface*> obj_list;
//
//	obj_list.push_back(new GeometricObject<Draw>);
//	obj_list.push_back(new GeometricObject<Move>);
//
//	for (auto itr : obj_list)
//		itr->doSomething();
//
//
//	GeometricObject<Draw> draw_object;
//	draw_object.doSomething();
//
//	GeometricObject<Move> move_object;
//	move_object.doSomething();
//}