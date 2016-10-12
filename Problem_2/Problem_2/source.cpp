#include <iostream>
#include <vector>

class Draw
{
public:
	void doSomething()
	{
		std::cout << "Draw" << std::endl;
	}
};

class Move 
{
public:
	void doSomething()
	{
		std::cout << "Move" << std::endl;
	}
};

class GeometricObjectInterface
{
public:
	virtual void doSomething() = 0;
};

template <class TT>
class GeometricObject :public GeometricObjectInterface
{
public:
	void doSomething()
	{
		TT operation;
		operation.doSomething();
	}
};

int main()
{
	std::vector<GeometricObjectInterface*> obj_list;

	obj_list.push_back(new GeometricObject<Draw>);
	obj_list.push_back(new GeometricObject<Move>);

	for (auto itr : obj_list)
		itr->doSomething();

	GeometricObject<Draw> draw_obj;
	GeometricObject<Move> move_obj;
	
	draw_obj.doSomething();
	move_obj.doSomething();
}
//
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