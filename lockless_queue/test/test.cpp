#include "../includes/lockless_queue.h"
#include<thread>
#include <fstream>
#include<sstream>
#include<math.h>

using namespace std;

//lockless_queue<int> lq{};
lockless_queue<int> lq(1000);


void printQueue()
{
	lq.print();
}
int i = 1;
atomic<int>l = 0;
void produce()
{
	//static int i = 0;
	for (i; i <= 1000000; ++i)
	{
		//++l;
		lq.enqueue(i);
	}
}

int j = 1;
//atomic<int>k = 0;
void consume()
{
	auto myid = this_thread::get_id();
	stringstream ss;
	ss << myid;
	string mystring = ss.str();
	mystring = mystring + ".txt";
	ofstream consumer1(mystring.c_str());
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
		/*if (lq.dequeue(data))
			consumer1 << data << '\n';
		else
			break;*/
		if (!lq.dequeue(data))
			break;

	}
}


int main()
{
	lq.initialize(1000);
	thread tp1(&produce);
	//thread tp2(&produce);
	tp1.join(); cout << "tp1 joined\n";
	//tp2.join(); cout << "tp2 joined\n";
	//exit(0);
	
	/*thread tp3(&produce);
	thread tp4(&produce);
	thread tp5(&produce);
	
	
	tp3.join(); cout << "tp1 joined\n";
	tp4.join(); cout << "tp1 joined\n";
	tp5.join(); cout << "tp1 joined\n";*/
	



	thread tc1(&consume);
	thread tc2(&consume);
	thread tc3(&consume);
	thread tc4(&consume);
	thread tc5(&consume);

	 /*tp2.join(); cout << "tp2 joined\n"; tp3.join(); 
	cout << "tp3 joined\n"; tp4.join(); cout << "tp4 joined\n"; tp5.join();
	cout << "tp5 joined\n";*/
	

	tc1.join(); cout << "tc1 joined\n"; tc2.join(); cout << "tc2 joined\n";tc3.join(); tc4.join(); tc5.join();

	//tc1.join();

	std::cout << "Tc5 joined\n";


	//printQueue();
	//std::cout << "T2 joined\n";


	//std::cout << "l is : " << l.load() << std::endl;
	//std::cout << "k is : " << k.load() << std::endl;
	
	return 0;
}
