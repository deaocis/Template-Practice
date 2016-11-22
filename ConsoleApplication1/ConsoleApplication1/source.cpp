#include <iostream>

using namespace std;

#define random_data 100000

class hypothesis
{
public:
	double a_, b_;

	hypothesis()
		:a_(0.0),b_(0.0)
	{}

	double GetY(const double& x_data)
	{
		return a_*x_data + b_+ (double)(rand() / RAND_MAX);
	}
};

int main()
{
	hypothesis lh;

	double* x_data = new double[random_data];
	double* y_data = new double[random_data];

	for (int i = 0; i < random_data; i++)
	{
		x_data[i] = (double)(rand() % 4);
		y_data[i] = 3*x_data[i] +1+(double)(rand()/RAND_MAX);
	}

	const double learning_rate = 0.00001;

	for (int tr = 0; tr < 10000; tr++)
	{	
		for (int i = 0; i < random_data; i++)
		{
			const double error = y_data[i] - lh.GetY(x_data[i]);
			const double square_error = 0.5*error*error;

			//se=(y_target-a*x-b)^2
			const double dse_da = 2 * error*-x_data[i];
			const double dse_db = 2 * error*-1;

			lh.a_ -= dse_da*learning_rate;
			lh.b_ -= dse_db*learning_rate;
		}
		if (tr % 2000 == 0)
			cout << lh.a_ << "   " << lh.b_ << endl << endl;
	}

	cout << y_data[7] << "   " << lh.GetY(x_data[7]) << endl;
	cout << endl << lh.a_ << "    " << lh.b_ << endl;

	delete[] x_data;
	delete[] y_data;
	return 0;
}