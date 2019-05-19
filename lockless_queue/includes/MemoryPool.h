#ifndef MEMORY_POOL_H
#define MEMORY_POOL_H

#include<iostream>
#define NUM_ITEMS_DEFAULT 100
//int j = 0;



template<typename T>
struct Block
{
	T s_dataBlock;
	Block <T>* next;
	Block() :next(nullptr)
	{
		//s_dataBlock = j;
		//++j;
		memset(&s_dataBlock, 0, sizeof(s_dataBlock));
	}
	T getBlockData() { return s_dataBlock;  }
	T* getBlockDataAddress() { return &s_dataBlock; }
	~Block()
	{
		//next = nullptr;
	}
};

template<typename T>
class Arena
{
	//data members
	Block <T>* s_pMemory;
	Arena <T>* s_nextArena;

	//---------------------------------------------------------------------Methods-------------------------
public:
	Arena(Block <T>** free_list)
	{
		//std::cout << "CReating new ARena\n";
		s_pMemory = new Block<T>[NUM_ITEMS_DEFAULT];
		*free_list = s_pMemory;
	}
	Arena(Block <T>** free_list, int num_items):s_nextArena(nullptr)
	{
		//std::cout << "CReating new ARena\n";
		s_pMemory = new Block<T>[num_items];
		*free_list = s_pMemory;
	}
	Block <T> *getblockChain() { return s_pMemory;  }
	Arena <T>* getNextArena() { return s_nextArena; }
	void setNextArena(Arena<T>* arena/*, Block <T>** free_list*/)
	{
		if (!s_nextArena)
		{
			s_nextArena = arena;
		}
		else
		{
			Arena<T>* curr = s_nextArena;
			Arena<T>* target = curr;
			while (curr)
			{
				target = curr;
				curr = curr->getNextArena();
			}
			target->setNextArena(arena);
		}
	}
	~Arena()
	{
		//std::cout << "Arena's destructor called\n";
		delete[] s_pMemory;
		s_nextArena = nullptr;			// THe next arena if it exists will be lost, may have to readjust this pointer and assign to someone else, in case if it's a valid use-case
	}
	//---------------------------------------------------------------------

};
//#include "MemoryPool.cpp"

template<typename T>
class MemoryPool
{
	Block <T>* freeList;
	//Block <T>* allocList;
	Arena <T>* arena;
	int arenaSize;
public:
	MemoryPool()
	{
		
		arena = new Arena<T>(&freeList);
		arenaSize = NUM_ITEMS_DEFAULT;
		for (int i = 0; i < arenaSize-1; ++i)
		{
			(freeList + i)->next = freeList+(i+1);
		}
	}
	MemoryPool(int num_items)
	{
		arena = new Arena<T>(&freeList, num_items);
		arenaSize = num_items;
		for (int i = 0; i < arenaSize - 1; ++i)
		{
			(freeList + i)->next = freeList + (i + 1);
		}
	}
	T* alloc()
	{
		/*if (freeList)
		{
			Block<T>* curr = freeList;
			freeList = freeList->next;
			return curr->getBlockDataAddress();
		}
		else
		{*/
		if (!freeList)
		{
			arena->setNextArena(new Arena<T>(&freeList, arenaSize));
			for (int i = 0; i < arenaSize - 1; ++i)
				(freeList + i)->next = freeList + (i + 1);
		}
		//}
		Block<T>* curr = freeList;
		freeList = freeList->next;
		return curr->getBlockDataAddress();
	}
	void dealloc(T* mem_block)
	{
		if (mem_block)
		{
			Block<T>* blk = reinterpret_cast<Block<T>*>(mem_block);
			blk->next = freeList;
			freeList = blk;
			mem_block = nullptr;
		}
	}
	~MemoryPool()
	{
		Arena <T>* curr = arena;
		while (curr)
		{
			Arena <T>* temp = curr->getNextArena();
			delete curr;
			curr = temp;
		}
		//delete curr;
		freeList = nullptr;
	}
	void show()
	{
		std::cout << "Displaying freelist contents\n";
		/*for (int i = 0; i < arenaSize; ++i)
		{
			std::cout << (arena->getblockChain() + i)->getBlockData()<<std::endl;
		}*/
		Block <T>* tempList = freeList;
		while (tempList)
		{
			std::cout << tempList->getBlockData() << std::endl;
			tempList = tempList->next;
		}
	}
};

#endif