#include <iostream>

#define MAX2(a,b)              ((a)>(b) ? (a):(b))
#define MAX3(a,b,c)            MAX2(MAX2(a,b),(c))
#define MAX4(a,b,c,d)          MAX2(MAX3(a,b,c),(d))
using namespace std;

class CellData
{
public:
	double q_[4] = { 0.0, 0.0, 0.0, 0.0 };  //up, down, left, right
	double reward = 0.0;

	CellData()
	{}

	double GetMaxQ()
	{
		return MAX4(q_[0], q_[1], q_[2], q_[3]);
	}
};

class GridWorld    //To Do: make destructor -> it is necessary?
{
public:
	int i_num, j_num;
	CellData *q_arr2d = nullptr;

	GridWorld(const int& i_res, const int& j_res)
		:i_num(i_res), j_num(j_res)
	{
		q_arr2d = new CellData[i_res*j_res];
	}

	CellData& GetCellData(const int& i, const int& j)
	{
		return q_arr2d[i + i_num*j];
	}

	bool IsInsideGrid(const int& i, const int& j)
	{
		if (i >= i_num) return false;
		if (j >= j_num) return false;
		if (i < 0) return false;
		if (j < 0) return false;
		//if (i == 2 && j == 0) return true;	
		//if (i == 2 && j == 1) return true;
		return true;
	}
	
	void printSigned(const float& v)
	{
		if (v > 0.0f) printf("%1.2f",v );   //+%1.f
		else if (v < 0.0f) printf("%1.2f", v);
		else
			printf(" 0.0");
	}

	void print()
	{
		for (int j = j_num - 1; j >= 0; j--)       //i_num=3, j_num=2
		{
			for (int i = 0; i < i_num-1; i++)
			{
				CellData &cell = GetCellData(i, j);

				printf("      "); printSigned(cell.q_[0]); printf("       ");  //up
				printf("      ");
			}

			printf("\n");

			for (int i = 0; i < i_num - 1; i++)
			{
				CellData &cell = GetCellData(i, j);

				printSigned(cell.q_[2]); printf("       "); printSigned(cell.q_[3]);  //left, right
				printf("        ");
			}
			if (j == 1)
				printf("Reward:+1");
			else
				printf("Reward:-1");
			printf("\n");

			for (int i = 0; i < i_num - 1; i++)
			{
				CellData &cell = GetCellData(i, j);

				printf("      "); printSigned(cell.q_[1]); printf("      ");   //down
				printf("       ");
			}
			printf("\n\n");
		}
		printf("\n\n\n");
	}
};

class Agent
{
public:
	int i_, j_;     //position(st)
//	double reward;

	Agent()
		:i_(0), j_(0)
	{ }
};

int main()
{
	cout << "Hello GridWorld!" << endl << endl;
	const int i_res = 3, j_res = 2;
	
	CellData data;
	GridWorld world(i_res, j_res);
	Agent my_agent;

	for (int j = 0; j < j_res; j++)
	{
		for (int i = 0; i < i_res; i++)
		{
			world.GetCellData(i, j).reward = -0.1;   //밑에 2줄까지 합쳐서 reward칸외엔 보상이 -0.1
		}
	}

	world.GetCellData(2, 1).reward = 1.0;
	world.GetCellData(2, 0).reward = -1.0;
	
	

	world.print();

	for (int t = 0; t < 10000; t++)
	{
		const int action = rand() % 4;          //action=0,1,2,3
		int i = my_agent.i_;
		int j = my_agent.j_;   //i=0, my_agent.i_=0
		int i_old = i;
		int j_old = j;               //i_old=0, j_old=0
		
		//Q(s_t,a_t)=Q(s_t,a_t)+ lr*((r_t+1)+df*maxQ_a(s_t+1, a)-Q(s_t,a_t))
		float learning_rate = 0.001f;
		float discount_factor = 0.9f;

		switch (action)       //move agent
		{
		case 0:
			j++;
			break; //up
		case 1:
			j--;
			break;  //down
		case 2:
			i--;
			break;   //left
		case 3:
			i++;
			break;   //right
		}

		if (world.IsInsideGrid(i,j) == true)
		{
			switch (action)       //move agent
			{
			case 0:
				my_agent.j_++;
				break; //up
			case 1:
				my_agent.j_--;
				break;  //down
			case 2:
				my_agent.i_--;     
				break;   //left
			case 3:
				my_agent.i_++;
				break;   //right
			}

			//update reward(r_t) -> ????????

			world.GetCellData(i_old ,j_old).q_[action] += learning_rate*(world.GetCellData(my_agent.i_, my_agent.j_).reward+ discount_factor*MAX4(world.GetCellData(my_agent.i_, my_agent.j_).q_[0], world.GetCellData(my_agent.i_, my_agent.j_).q_[1], world.GetCellData(my_agent.i_, my_agent.j_).q_[2], world.GetCellData(my_agent.i_, my_agent.j_).q_[3]) - world.GetCellData(i_old, j_old).q_[action]);
			//update q values of previous cell (q_t)

			if (my_agent.i_ == 2)
			{
				my_agent.i_ = 0;
				my_agent.j_ = 0;
			}
			//reset if agent is in final cells
		}
		else
		{
			//you may give negative reward(penalty) to agent
		}

		//cout << world.GetCellData(i_old, j_old).q_[action] << endl;
		//cout << "Agent status: " << my_agent.i_ << "   " << my_agent.j_ << endl;
		//cout << "action " << action << endl <<endl ;

		//world.print();
	}

	world.print();

	return 0;
}       