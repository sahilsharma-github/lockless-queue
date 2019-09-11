#ifndef LOCKLESS_QUEUE_H

#include<iostream>
#include<atomic>
#include "MemoryPool.h"
#include "TaggedPointer.h"

template<typename T>
struct alignas(16) pointer_t;


template<typename T>
struct Node
{
	T data;
	std::atomic<pointer_t<T>> next;
	Node() :data(0), next(nullptr){}
	Node(T val) :data(val), next(nullptr){}
};


template<typename T>
struct alignas(16) pointer_t
{
	Node<T>* ptr;
	unsigned int count;
	pointer_t() noexcept : ptr{ nullptr }, count{ 0 } {}

	pointer_t(Node<T> * ptr) : ptr{ ptr }, count{ 0 } {}

	pointer_t(Node<T> * ptr, unsigned int count) : ptr{ ptr }, count{ count } {}

	bool operator ==(const pointer_t & other) const {
		return ptr == other.ptr && count == other.count;
	}
};


template<typename T>
class lockless_queue
{
	//std::atomic <Node <T>* > head;
	//std::atomic <Node <T>* > tail;
	MemoryPool<Node<T>> mp;
	std::atomic <pointer_t<T>> head;
	std::atomic <pointer_t<T>> tail;

public:
	lockless_queue(int num_items) :mp(num_items) {}
	lockless_queue() {}
	void initialize(int num_items);
	void enqueue(T val);
	bool dequeue(T & data);
	void print();
	//lockless_queue()
};

template<typename T>
void lockless_queue<T>::print()
{
	pointer_t<T> head_temp = head.load();
	//for (int i = 0; i < 100; ++i)
	while(head_temp.ptr)
	{
		std::cout << head_temp.ptr->data << "  ";
		head_temp = head_temp.ptr->next.load();
	}
	std::cout << "Done\n" << std::endl;
}

template<typename T>
void lockless_queue<T>::initialize(int num_items)
{
	//pointer_t<T> node(new Node<T>());
	pointer_t<T> node(mp.alloc());
	//head.load().ptr = tail.load().ptr = node.ptr;
	head.store(node, std::memory_order_relaxed);
	tail.store(node, std::memory_order_relaxed);
}

template<typename T>
void lockless_queue<T>::enqueue(T val)
{
	//Node<T>* node = new Node<T>(val);
	Node<T>* node = mp.alloc();
	node->data = val;
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
bool lockless_queue<T>::dequeue(T & data)
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
				std::atomic_compare_exchange_weak_explicit(&(tail), &(tail_temp),
					pointer_t<T>{next.ptr, tail_temp.count + 1}, std::memory_order_release,
					std::memory_order_relaxed);
			}
			//std::cout << "next.ptr: " << next.p1`1		tr << std::endl;
			data = next.ptr->data;
			if (std::atomic_compare_exchange_weak_explicit(&(head), &(head_temp),
				pointer_t<T>{next.ptr, head_temp.count+1}, std::memory_order_release,
				std::memory_order_relaxed))
				break;
		}
	}
	//std::cout << "deleting head_temp.ptr: " << head_temp.ptr << std::endl;
	//delete head_temp.ptr;
	mp.dealloc(head_temp.ptr);
	return true;
}


#endif // !LOCKLESS_QUEUE_H


