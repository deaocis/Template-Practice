//#include <iostream>
//#include <time.h>
//using namespace std;
//
//#define random_data 100000    //28->-nan(ind)
//class hypothesis
//{
//public:
//	double a_;
//	double b_;
//	double c_;
//
//	hypothesis()
//		:a_(0.0), b_( 0.0), c_(0.0)
//	{}
//
//	double getY(const double& x_input)
//	{
//		return a_*x_input*x_input + b_*x_input + c_ + rand() / RAND_MAX;
//	}
//};
////ax^2+bx+c+rand()/RAND_MAX
//int main()
//{
//	double* x_data = nullptr;
//	x_data= new double[random_data];
//	double* y_data = nullptr;
//	y_data= new double[random_data];
//
//	hypothesis lh;
//
//	const double learning_rate = 0.00000001;
//
//	for (int i = 0; i < random_data; i++)
//	{
//		x_data[i] = (double)(rand()%3);
//	}
//
//	for (int i = 0; i < random_data; i++)
//	{
//		y_data[i] = 3 * x_data[i]*x_data[i] + 1*x_data[i]+ 1 +(double)(rand()/RAND_MAX);  //y_data={1,4,7,10...}
//	}
//
//	//double error=0;
//
//	for (int tr = 0; tr < 10000; tr++)
//	{
//		for (int i = 0; i < random_data; i++)
//		{
//			const double error = y_data[i] - lh.getY(x_data[i]);	
//			const double square_error = 0.5*error*error;
//		
//			//se=(y_target-ax^2-bx-c)^2
//
//			const double dse_da = 2.0*error*-x_data[i]*x_data[i];
//			const double dse_db = 2.0*error*(-x_data[i]);
//			const double dse_dc = 2.0*error*-1;
//
//			lh.a_ -= dse_da*learning_rate;
//			lh.b_ -= dse_db*learning_rate;
//			lh.c_ -= dse_dc*learning_rate;
//		}
//		if(tr%2000==0)
//			cout << lh.a_ << "   " << lh.b_ << "  " <<lh.c_<<endl;
//	}
//	cout << lh.a_ << "   " << lh.b_ << "  " << lh.c_ << endl;
//
//	delete[] x_data;
//	delete[] y_data;
//	return 0;
//}

//y = 3.0 x + 1 + rand()/RAND_MAX        rand()/RAND_MAX=0~1

//then you will obtain a around 3.0 and b close to 1.



#include <iostream>
using namespace std;

#define random_data 100000 
class hypothesis
{
public:
	double a_;
	double b_;

	hypothesis()
		:a_(0.0), b_(0.0)
	{}

	double getY(const double& x_input)
	{
		return a_*x_input + b_ + rand() / RAND_MAX;
	}
};

int main()
{
	double* x_data = nullptr;
	x_data = new double[random_data];
	double* y_data = nullptr;
	y_data = new double[random_data];

	hypothesis lh;

	const double learning_rate = 0.0000000001;//  rand: 0.00000001;

	for (int i = 0; i < random_data; i++)
	{
		x_data[i] = i;    //rand()%2->a=2.96214, b=1.02348,rand()%3-> a=2.9979 ,b=1.00292
	}										 //rand()%4->a=2.9958, b=1.00091  대략 rand()%15000 부터 이상함
	//x_data[i] = 0.0001*i;  learning_rate = 0.00000001
	for (int i = 0; i < random_data; i++)
	{
		y_data[i] = 3 * x_data[i] + 1 + (double)(rand() / RAND_MAX);  //y_data={1,4,7,10...}
	}

	//double error=0;

	for (int tr = 0; tr < 10000000; tr++)
	{
		for (int i = 0; i < random_data; i++)
		{
			const double error = y_data[i] - lh.getY(x_data[i]);
			const double square_error = 0.5*error*error;

			const double dse_da = 2.0*error*-x_data[i];
			const double dse_db = 2.0*error*(-1);

			lh.a_ -= dse_da*learning_rate;
			lh.b_ -= dse_db*learning_rate;
		}
		if (tr % 20000 == 0)
			cout << lh.a_ << " " << lh.b_ << endl;
	}
	cout << lh.a_ << " " << lh.b_ << endl;

	delete[] x_data;
	delete[] y_data;
	return 0;
}

//y = 3.0 x + 1 + rand()/RAND_MAX        rand()/RAND_MAX=0~1

//then you will obtain a around 3.0 and b close to 1.