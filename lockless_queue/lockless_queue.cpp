#include"lockless_queue.h"
/*
template<typename T>
void lockless_queue<T>::print()
{
	pointer_t<T> head_temp = head.load();
	//for (int i = 0; i < 100; ++i)
	while (head_temp.ptr)
	{
		std::cout << head_temp.ptr->data << "  ";
		head_temp = head_temp.ptr->next.load();
	}
	std::cout << "Done\n" << std::endl;
}
template<typename T>
void lockless_queue<T>::initialize()
{
	pointer_t<T> node(new Node<T>());
	//head.load().ptr = tail.load().ptr = node.ptr;
	head.store(node, std::memory_order_relaxed);
	tail.store(node, std::memory_order_relaxed);
}

template<typename T>
void lockless_queue<T>::enqueue(T val)
{
	Node<T>* node = new Node<T>(val);
	pointer_t<T> tail_temp, next;
	while (true)
	{
		//std::cout << "Enqueuing\n" << std::endl;
		tail_temp = tail.load();
		//tail_temp.ptr = tail.ptr;
		next = tail.load().ptr->next.load();
		if (tail_temp == tail.load())
		{
			if (next.ptr == nullptr)
			{
				if (std::atomic_compare_exchange_weak(&(tail_temp.ptr->next), &(next),
					pointer_t<T>{node, next.count + 1}))
					break;
			}
			else                    // Tail was not pointing to the last node
			{
				std::atomic_compare_exchange_weak(&(tail), &(tail_temp), pointer_t<T>{next.ptr,
					tail_temp.count + 1});
			}
		}
	}
	std::atomic_compare_exchange_weak(&(tail), &(tail_temp),
		pointer_t<T>{node, tail_temp.count + 1});
}


template<typename T>
bool lockless_queue<T>::dequeue(T& data)
{
	pointer_t<T> head_temp, tail_temp, next;
	while (true)
	{
		//std::cout << "Dequeuing\n" << std::endl;
		head_temp = head.load();
		tail_temp = tail.load();
		next = head_temp.ptr->next.load();
		if (head_temp == head.load())
		{
			if (head_temp == tail_temp)
			{
				if (next.ptr == nullptr)
					return false;
				std::atomic_compare_exchange_weak(&(tail), &(tail_temp),
					pointer_t<T>{next.ptr, tail_temp.count + 1});
			}
			//std::cout << "next.ptr: " << next.ptr << std::endl;
			data = next.ptr->data;
			if (std::atomic_compare_exchange_weak(&(head), &(head_temp),
				pointer_t<T>{next.ptr, head_temp.count + 1}))
				break;
		}
	}
	//std::cout << "deleting head_temp.ptr: " << head_temp.ptr << std::endl;
	delete head_temp.ptr;
	return true;
}struct ss{	char ch;	int i;	short s;};int main(){
	//std::cout << sizeof(ss);
	std::cout << "Alignment of 'char': " << alignof(char) << std::endl;
	std::cout << "Alignment of 'short': " << alignof(short) << std::endl;
	std::cout << "Alignment of 'int': " << alignof(int) << std::endl;
	std::cout << "Alignment of 'float': " << alignof(float) << std::endl;
	std::cout << "Alignment of 'int *': " << alignof(int*) << std::endl;
	std::cout << "Alignment of 'double': " << alignof(double) << std::endl;
	std::cout << "Alignment of 'ss': " << alignof(ss) << std::endl;
	//std::cout << "Alignment of 'struct S': " << alignof(S) << std::endl;
	//std::cout << "Alignment of 'struct S64': " << alignof(S64) << std::endl;
	return 0;}*/