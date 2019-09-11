#include<queue>
#include<thread>
#include <fstream>
#include<sstream>
#include<queue>
#include<mutex>
#include<iostream>
#include<math.h>

//using namespace std;

std::queue<int> q;
std::mutex mut;


int i = 0;
void produceSTL()
{
	for (i; i <= 1000000; ++i)
	{
	//	++l;
		q.push(i);
	}
}

void consumeSTL()
{
	auto myid = std::this_thread::get_id();
	std::stringstream ss;
	ss << myid;
	std::string mystring = ss.str();
	mystring = mystring + ".txt";
	std::ofstream consumer1(mystring.c_str());
	int data;
	/*for (j; j <=50; ++j)
	{
		++k;
		if(lq.dequeue(data))
			consumer1 << data<< '\n';
		//cout << data << endl;

	}*/
	while (true)
	{
		mut.lock();
		if (q.size() > 0)
		{
			q.pop();
			mut.unlock();
		}
		else
		{
			mut.unlock();
			return;
		}
	}
}



int main()
{
	std::thread tp1(&produceSTL);
	tp1.join(); std::cout << "tp1 joined\n";
	//exit(0);
	/*thread tp2(&produce);
	thread tp3(&produce);
	thread tp4(&produce);
	thread tp5(&produce);*/
	std::thread tc1(&consumeSTL);
	std::thread tc2(&consumeSTL);
	std::thread tc3(&consumeSTL);
	std::thread tc4(&consumeSTL);
	std::thread tc5(&consumeSTL);
	/*tp2.join(); cout << "tp2 joined\n"; tp3.join();
   cout << "tp3 joined\n"; tp4.join(); cout << "tp4 joined\n"; tp5.join();
   cout << "tp5 joined\n";*/
	tc1.join(); std::cout << "tc1 joined\n"; tc2.join(); std::cout << "tc2 joined\n"; tc3.join(); tc4.join(); tc5.join();
	//tc1.join();
	std::cout << "Tc5 joined\n";
	//printQueue();
	//std::cout << "T2 joined\n";
	//std::cout << "l is : " << l.load() << std::endl;
	//std::cout << "k is : " << k.load() << std::endl;
	return 0;
}